#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Jan 11 22:32:03 2020

@author: admin
"""
import cv2
import os
import numpy as np
import random
import torch
import torch.nn as nn
from torch.autograd import Variable
import torch.optim as optim
from torch.utils.data import DataLoader, Dataset
import torchvision.transforms as transforms
import torchvision.models as models
import torch.nn.functional as F
from PIL import Image
from model import ResNet34
from utils import AverageLoss, AverageAcc, ConfusionMatrix
import pandas as pd
import time
np.set_printoptions(suppress=True) 


def set_seed(x):
    np.random.seed(x)
    random.seed(x)
    torch.manual_seed(x)
    torch.cuda.manual_seed_all(x)
    torch.backends.cudnn.deterministic = True

def crop_image(img,x1,x2,y1,y2,margin):
    h,w = img.shape[:2]
    minx1 = int(x1)
    miny1 = int(y1)
    maxx2 = int(x2)
    maxy2 = int(y2)
    x1 = int(max(minx1 - (maxx2-minx1)*margin, 0))
    x2 = int(min(maxx2 + (maxx2-minx1)*margin, w))
    y1 = int(max(miny1 - (maxy2-miny1)*margin, 0))
    y2 = int(min(maxy2 + (maxy2-miny1)*margin, h))
    if max(h,w)>=550:
        if (maxx2-minx1)<(w/3) or (maxy2-miny1)<(h/3):
            x1,x2,y1,y2 = 0,w,0,h
    else:
        if (maxx2-minx1)<50 or (maxy2-miny1)<50:
            x1,x2,y1,y2 = 0,w,0,h
    img_new = img[y1:y2,x1:x2]
    return img_new

def resize_img_keep_ratio(img,target_size=(448,448)):
    old_size= img.shape[0:2]
    ratio = min(float(target_size[i]/old_size[i]) for i in range(len(old_size)))
    new_size = tuple([int(i*ratio) for i in old_size])
    img = cv2.resize(img,(new_size[1], new_size[0]))
    pad_w = target_size[1] - new_size[1]
    pad_h = target_size[0] - new_size[0]
    top,bottom = pad_h//2, pad_h-(pad_h//2)
    left,right = pad_w//2, pad_w -(pad_w//2)
    image_cv = cv2.copyMakeBorder(img,top,bottom,left,right,cv2.BORDER_CONSTANT,None,(0,0,0))
    image_cv = cv2.cvtColor(image_cv, cv2.COLOR_BGR2RGB)
    image_pil = Image.fromarray(image_cv)
    return image_pil

class Mydataset(Dataset):
    def __init__(self,annotation,img_transform):
        self.img_list = [i["image_file"] for i in annotation if os.path.exists(i["image_file"])]
        self.id_list = [i["id"] for i in annotation if os.path.exists(i["image_file"])]
        self.x1_list = [i["x1"] for i in annotation if os.path.exists(i["image_file"])]
        self.x2_list = [i["x2"] for i in annotation if os.path.exists(i["image_file"])]
        self.y1_list = [i["y1"] for i in annotation if os.path.exists(i["image_file"])]
        self.y2_list = [i["y2"] for i in annotation if os.path.exists(i["image_file"])]
        self.label_list = [int(i["label"]) for i in annotation if os.path.exists(i["image_file"])]
        self.img_transform=img_transform

    def __getitem__(self,index):
        img_path = self.img_list[index]
        aoi_id = self.id_list[index]
        x1 = self.x1_list[index]
        x2 = self.x2_list[index]
        y1 = self.y1_list[index]
        y2 = self.y2_list[index]
        img = cv2.imread(img_path)
        img = crop_image(img,x1,x2,y1,y2,0.3)
        img = resize_img_keep_ratio(img)
        img = self.img_transform(img)
        label = self.label_list[index]
        return aoi_id, img, label

    def __len__(self):
        return len(self.label_list) 


class Data(object):
    def __init__(self, annotation_file, configs, use_gpu):
        transform_train = transforms.Compose([
                transforms.Resize([448,448]),
                transforms.RandomHorizontalFlip(0.5),
                transforms.RandomVerticalFlip(0.5),
                transforms.ToTensor(),
                transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])])
        
        transform_test = transforms.Compose([
                transforms.Resize([448,448]),
                transforms.ToTensor(),
                transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])])

        pin_memory = True if use_gpu else False
        
        train_data = Mydataset(annotation_file["train"],transform_train)
        valid_data = Mydataset(annotation_file["validation"].transform_test)
        test_data = Mydataset(annotation_file["test"],transform_test)
        
        self.trainloader = DataLoader(train_data, batch_size=int(configs["train_batch_size"]),
                                      shuffle=True, num_workers=int(configs["num_workers"]),
                                      pin_memory=pin_memory)
        self.validloader = DataLoader(valid_data, batch_size=int(configs["test_batch_size"]),
                                     shuffle=False, num_workers=int(configs["num_workers"]),
                                     pin_memory=pin_memory)
        self.testloader = DataLoader(test_data, batch_size=int(configs["test_batch_size"]),
                                     shuffle=False, num_workers=int(configs["num_workers"]),
                                     pin_memory=pin_memory)
        
        self.num_classes = int(configs["num_classes"])
        print("train,valid,test size:",len(self.trainloader),len(self.validloader),len(self.testloader))


class CE_weighted(nn.Module):
    def __init__(self, class_num, alpha=None):
        super(CE_weighted, self).__init__()
        self.class_num = class_num
        if alpha is None:
            self.alpha = Variable(torch.ones(class_num, 1))
        else:
            if isinstance(alpha, Variable):
                self.alpha = alpha
            else:
                self.alpha = Variable(alpha)
    
    def forward(self, inputs, targets):
        N = inputs.size(0)
        C = inputs.size(1)
        log_P = F.log_softmax(inputs,dim=1)
        
        class_mask = inputs.data.new(N, C).fill_(0)
        class_mask = Variable(class_mask)
        ids = targets.view(-1, 1)
        class_mask.scatter_(1, ids.data, 1.)
        
        batch_CE_loss = - (log_P*class_mask).sum(1).view(-1,1)
        
        batch_loss = batch_CE_loss
        
        if inputs.is_cuda and not self.alpha.is_cuda:
            self.alpha = self.alpha.cuda()
        alpha = self.alpha[ids.data.view(-1)]
        batch_loss = alpha*batch_loss
        
        loss = batch_loss.mean()
        
        return loss


def load_model(num_classes, use_gpu):
    model = ResNet34(models.resnet34(pretrained=False),num_classes)
    if use_gpu:
        model = nn.DataParallel(model).cuda()
        print("use gpu train")
    else:
        print("use cpu train")
    return model


class Learning(object):
    def __init__(self,configs, model_name, annotation_file):
        set_seed(int(configs["seed"]))
        
        self.configs = configs
        self.model_name = model_name
        self.use_gpu = torch.cuda.is_available()
        dataset = Data(annotation_file, self.configs, use_gpu=self.use_gpu)
        self.trainloader, self.validloader, self.testloader = dataset.trainloader, dataset.validloader, dataset.testloader
        self.num_classes = dataset.num_classes
        self.model = load_model(self.num_classes, self.use_gpu)
        
        weights=[float(i) for i in configs["class_weights"].split(',')]
        self.criterion = CE_weighted(self.num_classes, alpha=torch.tensor(weights))
        self.optimizer = optim.SGD(self.model.parameters(), lr=float(configs["learning_rate"]), 
                                   weight_decay=float(configs["weight_decay"]), momentum=float(configs["momentum"]))
        self.scheduler = optim.lr_scheduler.StepLR(self.optimizer,step_size=float(configs["scheduler_step_size"]),gamma=0.1)
        
    def train(self):
        self.model.train()
        losses = AverageLoss()
        accs = AverageAcc()
        print("finish count avg in train")
        for aoi_id, data, labels in self.trainloader:
            #print("finish  get train data,label",labels)
            if self.use_gpu:
                data, labels = data.cuda(), labels.cuda()
            outputs = self.model(data)
            loss = self.criterion(outputs, labels)
            self.optimizer.zero_grad()
            loss.backward()
            self.optimizer.step()
            # update metrics
            predictions = outputs.data.max(1)[1]
            losses.update(loss.item(), labels.size(0))
            accs.update((predictions == labels.data).sum().item(), labels.size(0))
        print("train_set_loaded")
        self.scheduler.step()
        return losses.avg, accs.avg 
        
    def valid(self):
        self.model.eval()
        total_label = []
        total_prediction = []
        total_id = []
        losses = AverageLoss()
        accs = AverageAcc()
        conf = ConfusionMatrix(self.num_classes)
        print("finish count avg in valid")
        
        for aoi_id, data, labels in self.testloader:
            #print("finish  get test id,data,label",aoi_id)
            if self.use_gpu:
                data, labels = data.cuda(), labels.cuda()
            outputs = self.model(data)
            #print("finish self.model(data)")
            loss = self.criterion(outputs, labels)
            #print("finish self.criterion(outputs, labels)")
            # update metrics
            predictions = outputs.data.max(1)[1]
            #print("finish predictions = outputs.data.max(1)[1]")
            losses.update(loss.item(), labels.size(0))
            #print("fiish predictions = outputs.data.max(1)[1]")
            accs.update((predictions == labels.data).sum().item(), labels.size(0))
            #print("finish accs.update((predictions == labels.data).sum().item(), labels.size(0))")
            conf.update(labels.data.cpu().numpy(), predictions.cpu().numpy())
            #print("finish conf.update(labels.data.cpu().numpy(), predictions.cpu().numpy())")
            total_label.extend(labels.data.cpu().numpy())
            total_prediction.extend(predictions.cpu().numpy())
            total_id.extend(aoi_id)
            #print("finish extent total,aoi_id",aoi_id)
        conf_maxtrix_detail = []
        print("finish data predict in valid")
        for i in range(len(total_id)):
            conf_maxtrix_detail.append([int(total_label[i]), int(total_prediction[i]), total_id[i]])
        total_label_se = pd.Series(total_label)
        total_dict=dict(total_label_se.value_counts())
        detail={}
        for key in total_dict:
            detail[str(key)]=str(total_dict[key]);
        set_info = {
                "id":"0@12@" + str(len(total_id)) +"@"+ time.strftime("%Y%m%d%H%M%S"),
                "label_info":{
                        "type":"0",
                        "classn_num":12
                        },
                "data_count":{
                        "total":str(len(total_id)),
                        "detail":detail
                        }
                }
        model_wts = self.model.state_dict()
        print("finish get state dict in valid")
        torch.save(model_wts, os.path.join('./models/',self.model_name+'.pkl'))
        print("val_set_loaded")
        
        return losses.avg, accs.avg, conf.conf_matrix, conf_maxtrix_detail, set_info
    
    def test(self):
        self.model.eval()
        total_label = []
        total_prediction = []
        total_id = []
        losses = AverageLoss()
        accs = AverageAcc()
        conf = ConfusionMatrix(self.num_classes)
        for aoi_id, data, labels in self.testloader:
            if self.use_gpu:
                data, labels = data.cuda(), labels.cuda()
            outputs = self.model(data)
            loss = self.criterion(outputs, labels)
            # update metrics
            predictions = outputs.data.max(1)[1]
            losses.update(loss.item(), labels.size(0))
            accs.update((predictions == labels.data).sum().item(), labels.size(0))
            conf.update(labels.data.cpu().numpy(), predictions.cpu().numpy())
            total_label.extend(labels.data.cpu().numpy())
            total_prediction.extend(predictions.cpu().numpy())
            total_id.extend(aoi_id)
        conf_maxtrix_detail = []
        for i in range(len(total_id)):
            conf_maxtrix_detail.append([int(total_label[i]), int(total_prediction[i]), total_id[i]])
        total_label_se = pd.Series(total_label)
        total_dict=dict(total_label_se.value_counts())
        detail={}
        for key in total_dict:
            detail[str(key)]=str(total_dict[key]);
        set_info = {
                "id":"0@12@" + str(len(total_id)) +"@"+ time.strftime("%Y%m%d%H%M%S"),
                "label_info":{
                        "type":"0",
                        "classn_num":12
                        },
                "data_count":{
                        "total":str(len(total_id)),
                        "detail":detail
                        }
                }
        model_wts = self.model.state_dict()
        torch.save(model_wts, os.path.join('./models/',self.model_name+'.pkl'))
        print("test_set_loaded")
        return losses.avg, accs.avg, conf.conf_matrix, conf_maxtrix_detail, set_info
