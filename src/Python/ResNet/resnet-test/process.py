#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jan  3 19:56:19 2020

@author: admin
"""

import torch
from torch.autograd import Variable
import torchvision.transforms as transforms
from PIL import Image
import cv2

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

def dataprocess(img, x1, x2, y1, y2):
    # input a opencv image
    transform = transforms.Compose([
            transforms.Resize((448,448)),
            transforms.ToTensor(),
            transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])])
    img = crop_image(img,x1,x2,y1,y2,0.3)
    img = resize_img_keep_ratio(img)
    img = transform(img)
    img = torch.unsqueeze(img,0)
    #if torch.cuda.is_available():
    #    img = img.cuda()
    img = Variable(img)
    return img
