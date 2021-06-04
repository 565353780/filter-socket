#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Jan 11 22:32:01 2020

@author: admin
"""
import os
from flask import Flask, request, Response
import requests
import numpy as np
import os
import base64
import cv2
import json
import configparser
import torch
from testimage import load_model
from process import dataprocess


os.environ['CUDA_VISIBLE_DEVICES'] = "1"

# app = Flask(__name__)

def cost_sensitive(inputs,a=[1.,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2]):
    N = inputs.size(0)
    C = inputs.size(1)
    mask = torch.Tensor(a).expand(N,C)
    mask = mask.type_as(inputs)
    new_inputs = torch.mul(inputs,mask)
    return new_inputs

# @app.route("/getModelInfo", methods=["POST"])
def getModelInfo():
    model_list = os.listdir("./models/")
    for i in range(len(model_list)) :
        model_list[i] = model_list[i].split('.')[0]
    result_json=json.dumps({
    	"model_type":"Mixed",
    	"model_list":model_list,
        "port":"2000"
    })
    return result_json

# @app.route("/loadModel", methods=["POST"])
def loadModel(data_json):
    # data = request.get_data()
    # data_json = json.loads(data)
    model_name = data_json["model_name"]
    config = configparser.ConfigParser()
    config.read(os.path.join('./configs',model_name+'.ini'))
    configs = config["default"]
    num_classes = int(configs["num_classes"])
    global model
    model = load_model(model_name,num_classes)
    # response
    result_json=json.dumps({
    	"success":True,
    	"info":model_name
    })
    try:
        r = requests.post("http://172.17.0.1:1919/finishLoad",data=result_json,timeout=0.1)
    except IOError:
        pass
    else:
        pass
    return {}


# @app.route("/predict", methods=["POST"])
def predict(data_json):
    # data = request.get_data()
    # data_json = json.loads(data)
    # img_b64encode = bytes(data_json["image"][2:-1],encoding="utf-8")
    # img_b64decode = base64.b64decode(img_b64encode)
    img_name=data_json["image_file"]
    img_array = cv2.imread(img_name)
    img = cv2.cvtColor(img_array,cv2.COLOR_BGR2RGB)
    x1 = data_json["x1"]
    x2 = data_json["x2"]
    y1 = data_json["y1"]
    y2 = data_json["y2"]
    inputs = dataprocess(img,x1,x2,y1,y2)
    outputs = model(inputs)
    inputs = inputs.cpu()
    torch.cuda.empty_cache()
    #pros = outputs.data.cpu().numpy()[0]
    outputs = cost_sensitive(outputs)    
    _,pred = torch.max(outputs.data,1)
    pred = pred.cpu().numpy()[0]
    #pred = base64.b64encode(pred)
    pred = str(pred)
    # 数组、字典不能直接返回，需要json包装
    input_id = data_json["id"]
    result_json={
            "result":pred,
            "id":input_id
    }
    print(result_json)
    return result_json

if __name__ == "__main__":
    # app.run(host="0.0.0.0", port="2000",debug=True)
    loadModel "SandResNet"
