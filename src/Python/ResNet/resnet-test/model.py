#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jan  3 16:44:07 2020

@author: admin
"""

import torch.nn as nn
import torch.nn.functional as F


class ResNet34(nn.Module):
    def __init__(self,model,num_classes):
        super(ResNet34,self).__init__()
        self.conv1= model.conv1
        self.bn1 = model.bn1
        self.relu = model.relu
        self.maxpool= model.maxpool
        self.layer1 = model.layer1
        self.layer2 = model.layer2
        self.layer3 = model.layer3
        self.layer4 = model.layer4
        self.fc = nn.Linear(512,num_classes)

    def forward(self,x):
        x = self.conv1(x)
        x = self.bn1(x)
        x = self.relu(x)
        x = self.maxpool(x)
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)
        x = F.avg_pool2d(x, kernel_size=x.shape[2])
        x = x.view(x.size(0),-1)
        x = self.fc(x)
        return x