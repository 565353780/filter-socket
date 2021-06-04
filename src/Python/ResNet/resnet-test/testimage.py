#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Jan 11 23:12:47 2020

@author: admin
"""

import os
import torch
import torch.nn as nn
import torchvision.models as models
from model import ResNet34

def load_model(model_name,num_classes):
    model_root = os.path.join('./models/',model_name+'.pkl')
    model = ResNet34(models.resnet34(pretrained=False),num_classes)
    if torch.cuda.is_available():
        model = nn.DataParallel(model)
        model = model.cuda()
        model.load_state_dict(torch.load(model_root))
        model.eval()
        print("using gpu test")
    else:
        model = nn.DataParallel(model)
        model.load_state_dict(torch.load(model_root,map_location=torch.device("cpu")))
        print("using cpu")
    return model
