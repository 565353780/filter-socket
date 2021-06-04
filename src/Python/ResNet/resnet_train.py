import os
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"
import tensorflow as tf
import tensorflow.contrib.slim.nets as nets

class ResNetTrainer:
    def __init__(self, RootImageFolderPath, ModelSavePath, ModelSaveName, batchSize, learningRate, ImageWidth, ImageHeight, numThreads, labelNum):
        super().__init__()

        self.RootImageFolderPath = RootImageFolderPath
        self.ModelSavePath = ModelSavePath
        self.ModelSaveName = ModelSaveName
        self.batchSize = batchSize
        self.learningRate = learningRate
        self.ImageWidth = ImageWidth
        self.ImageHeight = ImageHeight
        self.numThreads = numThreads
        self.labelNum = labelNum

        if self.RootImageFolderPath[-1] != "/":
            self.RootImageFolderPath += "/"
        
        if self.ModelSavePath[-1] != "/":
            self.ModelSavePath += "/"

    def read_and_decode_tfrecord(self, filename):
        filename_deque = tf.train.string_input_producer(filename)
        reader = tf.TFRecordReader()
        _, serialized_example = reader.read(filename_deque)
        features = tf.parse_single_example(serialized_example, features={
            'label': tf.FixedLenFeature([], tf.int64),
            'img_raw': tf.FixedLenFeature([], tf.string)})
        label = tf.cast(features['label'], tf.int64)
        img = tf.decode_raw(features['img_raw'], tf.uint8)
        img = tf.reshape(img, [self.ImageWidth, self.ImageHeight, 3])
        img = tf.cast(img, tf.float32) / 255.0        #将矩阵归一化0-1之间
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!label :", label)
        print(img.shape)
        print(label.shape)
        return img, label

    def train(self):
        save_dir = self.ModelSavePath

        if not os.path.exists(save_dir):
            os.makedirs(save_dir)
        
        save_dir += self.ModelSaveName

        batch_size_ = self.batchSize

        lr = tf.Variable(self.learningRate, dtype=tf.float32)
        x = tf.placeholder(tf.float32, [None, self.ImageWidth, self.ImageHeight, 3])
        y_ = tf.placeholder(tf.float32, [None])

        tfRecordPath = self.RootImageFolderPath.replace("sources", "tfRecord")

        tfRecordName_list = os.listdir(tfRecordPath)

        train_list = []

        for tfRecordName in tfRecordName_list:
            train_list.append(tfRecordPath + tfRecordName)
        # 随机打乱顺序
        img, label = self.read_and_decode_tfrecord(train_list)
        img_batch, label_batch = tf.train.shuffle_batch([img, label], num_threads=self.numThreads, batch_size=batch_size_, capacity=10000,
                                                        min_after_dequeue=9900)

        # 将label值进行onehot编码
        one_hot_labels = tf.one_hot(indices=tf.cast(y_, tf.int32), depth=self.labelNum)
        pred, end_points = nets.resnet_v2.resnet_v2_50(x, num_classes=self.labelNum, is_training=True)
        pred = tf.reshape(pred, shape=[-1, self.labelNum])

        # 定义损失函数和优化器
        loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=pred, labels=one_hot_labels))
        optimizer = tf.train.AdamOptimizer(learning_rate=lr).minimize(loss)

        # 准确度
        a = tf.argmax(pred, 1)
        b = tf.argmax(one_hot_labels, 1)
        correct_pred = tf.equal(a, b)
        accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))

        saver = tf.train.Saver()
        with tf.Session() as sess:
            sess.run(tf.global_variables_initializer())
            # 创建一个协调器，管理线程
            coord = tf.train.Coordinator()
            # 启动QueueRunner,此时文件名队列已经进队
            threads = tf.train.start_queue_runners(sess=sess, coord=coord)
            i = 0
            while True:
                i += 1
                b_image, b_label = sess.run([img_batch, label_batch])
                _, loss_, y_t, y_p, a_, b_ = sess.run([optimizer, loss, one_hot_labels, pred, a, b], feed_dict={x: b_image,
                                                                                                                y_: b_label})
                print('step: {}, train_loss: {}'.format(i, loss_))
                if i % 10 == 0:
                    _loss, acc_train = sess.run([loss, accuracy], feed_dict={x: b_image, y_: b_label})
                    print('--------------------------------------------------------')
                    print('step: {}  train_acc: {}  loss: {}'.format(i, acc_train, _loss))
                    print('--------------------------------------------------------')
                    if i % 100 == 0:
                        saver.save(sess, save_dir, global_step=i)
            coord.request_stop()
            # 其他所有线程关闭之后，这一函数才能返回
            coord.join(threads)
        
if __name__ == "__main__":
    resnet_trainer = ResNetTrainer(RootImageFolderPath=os.getcwd() + "/sources/", \
                                    ModelSavePath=os.getcwd() + "/models/", \
                                    ModelSaveName="train_sandstone.model", \
                                    batchSize=128, \
                                    learningRate=0.0001, \
                                    ImageWidth=224, \
                                    ImageHeight=224, \
                                    numThreads=8, \
                                    labelNum=7)

    resnet_trainer.train()