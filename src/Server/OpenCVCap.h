#ifndef OPENCVCAP_H
#define OPENCVCAP_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QMutex>
#include <QString>
#include <QDebug>
#include <QQueue>

using namespace cv;
using namespace std;

class OpencvCap :public QThread
{
    Q_OBJECT

public:
    OpencvCap(QObject* parent, QString url);
    ~OpencvCap();

    void put_frame(Mat frame);
    bool get_frame(Mat &frame);

private:
    void run() override;
    //缓存容器
    QQueue<Mat> m_queue_frame_;
    //互斥锁
    QMutex *m_mutex;

public:
    //videocapture
    VideoCapture m_cap;

    QString url_;
};

#endif // OPENCVCAP_H
