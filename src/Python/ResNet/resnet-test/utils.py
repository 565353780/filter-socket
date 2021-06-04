#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jan 13 16:42:32 2020

@author: admin
"""

import numpy as np

class AverageLoss(object):
    def __init__(self):
        self.reset()

    def reset(self):
        self.val = 0
        self.avg = 0
        self.sum = 0
        self.count = 0

    def update(self, val, n=1):
        self.val = val
        self.sum += val * n
        self.count += n
        self.avg = self.sum / self.count


class AverageAcc(object):
    def __init__(self):
        self.reset()

    def reset(self):
        self.val = 0
        self.avg = 0
        self.sum = 0
        self.count = 0

    def update(self, val, n=1):
        self.val = val
        self.sum += val
        self.count += n
        self.avg = self.sum / self.count


def confusion_matrix(true, pred, num=None):
    assert(len(true) == len(pred))
    if num is None:
        num = len(set(pred))
    conf_mat = [[0 for i in range(num)] for j in range(num)]
    for a, b in zip(true, pred):
        conf_mat[a][b] += 1
    return np.array(conf_mat)


class ConfusionMatrix(object):
    def __init__(self,num_classes):
        self.num_classes = num_classes
        self.conf_matrix = np.zeros((num_classes,num_classes))
        
    def update(self, true, pred):
        conf = confusion_matrix(true, pred, num=self.num_classes)
        self.conf_matrix += conf
