import os 
import shutil
import json

dirlist=os.listdir("/home/abaci/wjh/resnet-test/sand-pic")
dataset=[]
test_dataset=[]
validate_dataset=[]
classes=[]
i=0
for folder in dirlist:
    print(folder)
    for sand_file in os.listdir(os.path.join("./sand-pic",folder)):
        if sand_file.find("train")>-1 :
            continue
        classes.append(folder)
        dataset.append({"label":str(i),"x1":"0","x2":"1","y1":"0","y2":"1","image_file":os.path.join("./sand-pic",folder,sand_file), "id":sand_file})
    test_dataset.append(dataset.pop())
    validate_dataset.append(dataset.pop())
    i+=1

final={}
final["train"]=dataset
final["validation"]=validate_dataset
final["test"]=test_dataset

with open("train.json",'w') as f:
    f.write(json.dumps(final))
    f.close()