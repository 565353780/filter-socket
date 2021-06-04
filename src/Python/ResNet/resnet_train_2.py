import os
import torch.utils.data as data
import torch
import torch.optim as optim
import torch.nn as nn
from torch.optim import lr_scheduler
from torchvision import datasets, models, transforms
from PIL import Image
import time
import copy
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def default_loader(path):
    with open(path, 'rb') as f:
        with Image.open(f) as img:
            return img.convert('RGB')

class CustomImageLoader(data.Dataset): # 定义自己的数据类
    ##自定义类型数据输入
    def __init__(self, RootImageFolderPath, dataset = '', data_transforms=None, loader = default_loader):
        im_list = []
        im_labels = []

        if RootImageFolderPath[-1] != "/":
            RootImageFolderPath += "/"
        
        ImageFolderPath_list = os.listdir(RootImageFolderPath)

        for ImageFolderPath in ImageFolderPath_list:
            currentImageFolderPath = RootImageFolderPath + ImageFolderPath
            if os.path.isdir(currentImageFolderPath):
                ImagePath_list = os.listdir(currentImageFolderPath)
                for ImagePath in ImagePath_list:
                    if "train" in ImagePath:
                        continue
                    if ImagePath.split(".")[1] == "jpg":
                        currentImagePath = currentImageFolderPath + "/" + ImagePath
                        im_list.append(currentImagePath)
                        im_labels.append(int(ImageFolderPath))

        self.imgs = im_list
        self.labels = im_labels
        self.data_tranforms = data_transforms
        self.loader = loader
        self.dataset = dataset
 
    def __len__(self):
        return len(self.imgs)
 
    def __getitem__(self, item):
        img_name = self.imgs[item]
        label = self.labels[item]
        img = self.loader(img_name)
 
        if self.data_tranforms is not None:
            try:
                img = self.data_tranforms[self.dataset](img)
            except:
                print("Cannot transform image: {}".format(img_name))
        return img, label
    
data_tranforms={
    'Train':transforms.Compose([
        # transforms.RandomResizedCrop(224), # 随机裁剪为不同的大小和宽高比,缩放所为制定的大小
        transforms.RandomHorizontalFlip(0.5),
        transforms.RandomVerticalFlip(0.5),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406],[0.229, 0.224, 0.225]) # 各通道颜色的均值和方差,用于归一化
    ]),
    'Test':transforms.Compose([
        # transforms.Resize(256), # 变换大小
        # transforms.CenterCrop(224), # 中心裁剪
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406],[0.229, 0.224, 0.225])
    ])
}

def train_model(model, crtiation, optimizer, schedular, num_epochs, batch_size, device):
    image_datasets = {x : CustomImageLoader(os.getcwd() + "/resnet-test/sand-pic/", # 默认目录为根目录，配搭文件中使用全路径
                                            data_transforms=data_tranforms,
                                            dataset=x) for x in ['Train', 'Test']
                    }
 
    dataloders = {x: torch.utils.data.DataLoader(image_datasets[x],
                                                    batch_size=batch_size,
                                                    shuffle=True) for x in ['Train', 'Test']}

    dataset_sizes = {x: len(image_datasets[x]) for x in ['Train', 'Test']} # 数据大小

    begin_time = time.time()
    best_weights = copy.deepcopy(model.state_dict())#copy the weights from the model
    best_acc = 0.0
    arr_acc = [] # 用于作图
 
    for epoch in range(num_epochs):
        print("-*-" * 20)
        item_acc = []
        for phase in ['Train', 'Test']:
            if phase=='Train':
                schedular.step()
                model.train()
            else:
                model.eval()
            running_loss = 0.0
            running_acc = 0.0
 
            for images, labels in dataloders[phase]:
                images.to(device)
                labels.to(device)
                optimizer.zero_grad()
 
                with torch.set_grad_enabled(phase=='Train'):
                    opt = model(images.cuda())
                    # opt = model(images)
                    _,pred = torch.max(opt,1)
                    labels = labels.cuda()
                    loss = crtiation(opt, labels)
                    if phase=='Train':
                        loss.backward()
                        optimizer.step()
 
                running_loss += loss.item()*images.size(0)
                running_acc += torch.sum(pred==labels)
            epoch_loss = running_loss/dataset_sizes[phase]
            epoch_acc = running_acc.double()/dataset_sizes[phase]
            print('epoch={}, Phase={}, Loss={:.4f}, ACC:{:.4f}'.format(epoch, phase, 
                                                                       epoch_loss, epoch_acc))
            item_acc.append(epoch_acc)
 
            if phase == 'Test' and epoch_acc>best_acc:
                # Upgrade the weights
                best_acc=epoch_acc
                best_weights = copy.deepcopy(model.state_dict())
                torch.save(model.state_dict(), os.getcwd() + "/resnet-test/models/chLi-ResNet-best-model.pkl")
        arr_acc.append(item_acc)
        time_elapes = time.time() - begin_time
        time_elapes // 60, time_elapes % 60

    print('Best Val ACC: {:}'.format(best_acc))
 
    model.load_state_dict(best_weights) # 保存最好的参数
    return model,arr_acc

if __name__ == '__main__':
    device = torch.device('cuda:0') # 默认使用 GPU
    NUMCLASS = 7 # 类别数

    model_ft = models.resnet50(pretrained=False)
    num_fits = model_ft.fc.in_features
    model_ft.fc = nn.Linear(num_fits, NUMCLASS) # 替换最后一个全连接层
    model_ft = model_ft.to(device)
    model_ft.cuda()
    # try:
    #     model_ft.load_state_dict(torch.load(os.getcwd() + "/resnet-test/models/chLi-ResNet-best-model.pkl"))
    # except:
    #     print("load model failed.")
    criterion = nn.CrossEntropyLoss()
    optimizer_ft = optim.SGD(model_ft.parameters(), lr=0.001, momentum=0.9)
    exp_lr_scheduler = lr_scheduler.StepLR(optimizer_ft, step_size=10, gamma=0.1)

    model_ft,arr_acc = train_model(model=model_ft, \
                                    crtiation=criterion, \
                                    optimizer=optimizer_ft, \
                                    schedular=exp_lr_scheduler, \
                                    num_epochs=1000, \
                                    batch_size=1, \
                                    device=device)

    # ll = np.array(arr_acc)
    # plt.plot(ll[:,0])
    # plt.plot(ll[:,1])
    
    # plt.show()