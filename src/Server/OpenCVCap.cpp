#include "OpenCVCap.h"

OpencvCap::OpencvCap(QObject* parent, QString url)
{
    m_mutex = new QMutex();
    url_ = url;
    m_cap.open(url_.toStdString());
}

OpencvCap::~OpencvCap()
{
    m_cap.release();
}

void OpencvCap::run()
{
    if(!m_cap.isOpened())
    {
        m_cap.open(url_.toStdString());
        if(!m_cap.isOpened())
        {
            qDebug("cannot open the videocapture\n");
            return ;
        }
    }
    Mat current_frame;
    while(true)
    {
        m_cap >> current_frame;
        if(current_frame.empty())
        {
            qDebug("frame empty\n");
            return;
        }
        put_frame(current_frame);
    }
}


void OpencvCap::put_frame(Mat frame)
{
    if(m_queue_frame_.size() > 5)
    {
        m_queue_frame_.dequeue();
    }
    //存入容器
    m_queue_frame_.enqueue(frame);
    return;
}

bool OpencvCap::get_frame(Mat &frame)
{
    if(m_queue_frame_.size() < 1)
    {
        qDebug() << "OpencvCap::get_frame -> m_vec_frame is empty";
        return false;
    }
    else
    {
         //从容器中取图像
        frame = m_queue_frame_.dequeue();
    }
    return true;
}
