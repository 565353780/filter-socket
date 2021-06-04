import os
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"
import tensorflow as tf
import tensorflow.contrib.slim.nets as nets
from PIL import Image

class ResNetDetector:
    def __init__(self, RootImageFolderPath, ModelLoadName, ImageWidth, ImageHeight, LabelNum):
        super().__init__()

        self.RootImageFolderPath = RootImageFolderPath
        self.ModelLoadName = ModelLoadName
        self.ImageWidth = ImageWidth
        self.ImageHeight = ImageHeight
        self.LabelNum = LabelNum

        if self.RootImageFolderPath[-1] != "/":
            self.RootImageFolderPath += "/"
        
        self.ImageFormart = ["jpg"]

    def detect(self, ImagePath):
        x = tf.placeholder(tf.float32, [None, self.ImageWidth, self.ImageHeight, 3])
        classes = [str(i) for i in range(self.LabelNum)] # 标签顺序

        classes = os.listdir(self.RootImageFolderPath)

        pred, end_points = nets.resnet_v2.resnet_v2_50(x, num_classes=self.LabelNum, is_training=True)
        pred = tf.reshape(pred, shape=[-1, self.LabelNum])
        a = tf.argmax(pred, 1)
        saver = tf.train.Saver()
        with tf.Session() as sess:
            sess.run(tf.global_variables_initializer())
            saver.restore(sess, self.ModelLoadName)

            img = Image.open(img_path)
            img = img.resize((self.ImageWidth, self.ImageHeight))
            img = tf.reshape(img, [1, self.ImageWidth, self.ImageHeight, 3])
            img1 = tf.reshape(img, [1, self.ImageWidth, self.ImageHeight, 3])
            img = tf.cast(img, tf.float32) / 255.0
            b_image, b_image_raw = sess.run([img, img1])
            t_label = sess.run(a, feed_dict={x: b_image})
            index_ = t_label[0]
            predict = classes[index_]
            print(predict)
    
    def detectOnFolder(self):
        sourceDatasetPath = self.RootImageFolderPath + "resize_dataset/"

        x = tf.placeholder(tf.float32, [None, self.ImageWidth, self.ImageHeight, 3])
        classes = [str(i) for i in range(self.LabelNum)] # 标签顺序

        classes = os.listdir(self.RootImageFolderPath + "sources/")

        pred, end_points = nets.resnet_v2.resnet_v2_50(x, num_classes=self.LabelNum, is_training=True)
        pred = tf.reshape(pred, shape=[-1, self.LabelNum])
        a = tf.argmax(pred, 1)
        saver = tf.train.Saver()
        with tf.Session() as sess:
            sess.run(tf.global_variables_initializer())
            saver.restore(sess, self.ModelLoadName)

            ImageFolderPath_list = os.listdir(sourceDatasetPath)

            currentImageFolderIndex = 0
            totalImageFolderNum = len(ImageFolderPath_list)
            for ImageFolderPath in ImageFolderPath_list:
                currentImageFolderIndex += 1
                currentImageFolderPath = sourceDatasetPath + ImageFolderPath + "/"
                if os.path.isdir(currentImageFolderPath):
                    ImagePath_list = os.listdir(currentImageFolderPath)
                    currentImageIndex = 0
                    totalImageNum = len(ImagePath_list)
                    print("====Folder of : " + classes[currentImageFolderIndex - 1] + "====")
                    for ImagePath in ImagePath_list:
                        currentImageIndex += 1
                        if not os.path.isdir(currentImageFolderPath + ImagePath):
                            if ImagePath.split(".")[1] in self.ImageFormart:
                                currentImagePath = currentImageFolderPath + ImagePath
                                img = Image.open(currentImagePath)
                                img = img.resize((self.ImageWidth, self.ImageHeight))
                                img = tf.reshape(img, [1, self.ImageWidth, self.ImageHeight, 3])
                                img1 = tf.reshape(img, [1, self.ImageWidth, self.ImageHeight, 3])
                                img = tf.cast(img, tf.float32) / 255.0
                                b_image, b_image_raw = sess.run([img, img1])
                                t_label = sess.run(a, feed_dict={x: b_image})
                                index_ = t_label[0]
                                predict = classes[index_]
                                print(predict)
            
if __name__ == "__main__":
    label_list = os.listdir(os.getcwd() + "/sources/")

    resnet_detector = ResNetDetector(RootImageFolderPath=os.getcwd(), \
                                        ModelLoadName=os.getcwd() + "/models/train_sandstone.model-600", \
                                        ImageWidth=224, \
                                        ImageHeight=224, \
                                        LabelNum=7)
    
    resnet_detector.detectOnFolder()

    print("label : ")
    for i in range(len(label_list)):
        print(i, label_list[i])