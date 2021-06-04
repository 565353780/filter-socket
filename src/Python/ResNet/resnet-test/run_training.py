import requests
import json
from trainmodel import Learning
import configparser
import datetime

def update_config(param, model_name):
    config = configparser.ConfigParser()
    config.read('./configs/default.ini')
    configs = config["default"]
    new_configs = configparser.ConfigParser()
    new_configs["default"] = configs
    new_configs["appointed"] = param
    with open('./configs/{}.ini'.format(model_name), 'w') as f:
        new_configs.write(f)
    
    for key, value in param.items():
        #print("key,value",key,value)
        configs[key] = value
        
    return configs

def training(configs, model_name, annotation_file):
    aoi = Learning(configs, model_name, annotation_file)
    num_epochs = int(configs["num_epochs"])
    
    best_val_acc = 0.0
    for epoch in range(num_epochs):
        train_loss, train_acc = aoi.train()
        valid_loss, valid_acc, valid_conf_maxtrix, conf_maxtrix_detail, set_info = aoi.valid()
        # response
        result_json={
        	"time":datetime.datetime.now().strftime('%Y/%m/%d-%H:%M:%S'),
        	"train_loss":{"epoch":epoch, "loss":train_loss},  
        	"train_accuracy":{"epoch":epoch, "accuracy":train_acc},
        	"validation_loss":{"epoch":epoch, "loss":valid_loss},
        	"validation_accuracy":{"epoch":epoch, "accuracy":valid_acc}
            }
        
        print(result_json)
        if valid_acc >= best_val_acc:
            best_val_acc = valid_acc
            best_valid_loss, best_valid_acc, best_valid_conf_maxtrix, best_valid_conf_maxtrix_detail,  valid_set_info = valid_loss, valid_acc, valid_conf_maxtrix, conf_maxtrix_detail, set_info
            best_test_loss, best_test_acc, best_test_conf_maxtrix, best_test_conf_maxtrix_detail,  test_set_info= aoi.test()
    # response
    result={
    	"time":datetime.datetime.now().strftime('%Y/%m/%d-%H:%M:%S'),
    	"success":True,
    	"confusion_matrix_validation":best_valid_conf_maxtrix.tolist(),
    	"confusion_matrix_test":best_test_conf_maxtrix.tolist(),
        "confusion_matrix_validation_detail":best_valid_conf_maxtrix_detail,
        "confusion_matrix_yesy_detail":best_test_conf_maxtrix_detail,
        "validation_set_info":valid_set_info,
        "test_set_info":test_set_info
        }
    print(result)
    #result_json=json.dumps(result)
    

class NpEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, np.integer):
            return int(obj)
        elif isinstance(obj, np.floating):
            return float(obj)
        elif isinstance(obj, np.ndarray):
            return obj.tolist()
        elif isinstabce(obj,'int64'):
            return str(obj)
        else:
            return super(NpEncoder, self).default(obj)


if __name__ == "__main__":
    model_name="SandResNet"
    param={}
    # param["num_epochs"]=100
    f=open("./train.json",'r')
    annotation_file = json.loads(f.read())
    print(annotation_file)
    # configs = update_config(param,model_name)
    # training(configs, model_name, annotation_file)