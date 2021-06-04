#include "QPalletizerWidget.h"
#include "Palletizer.h"
#include "Meshes/QPrimitiveMesh.h"
#include "Core/Q3DScene.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "Meshes/QQuadMesh.h"
#include <random>

namespace GCL {

QPalletizerWidget::QPalletizerWidget(QWidget *parent) : QRenderWidget(parent), palletizer_(new Palletizer())
{
    scene_->setRotationSpeed(2);
    scene_->setPanSpeed(40);
    scene_->setScaleSpeed(5);
    scene_->setPanSpeed(20);
    scene_->setProjectionParameters(0.1,10000.0,45);

    palletizer_->setOnlineNum(5);
    palletizer_->setMaxTestNumAtOnce(2);


}

void QPalletizerWidget::initQuery(int w, int h)
{
    palletizer_->init(w,h);
    palletizer_->initOnline();
    query_box_list_.clear();
    online_box_list_.clear();
    query_count_ = 0;
    target_id_ = -1;
}


void QPalletizerWidget::slotTryOnce()
{
    if(query_box_list_.empty() && online_box_list_.empty()) return;

    if(target_id_ >= 0)
    {
        for(int i=0; i < online_box_list_.size(); i++)
        {
            if(online_box_list_[i].id_ == target_id_)
            {
                online_box_list_.erase(online_box_list_.begin() + i);
                break;
            }
        }
    }
    if(!query_box_list_.empty())
    {
        palletizer_->addQueryBox(query_box_list_.back());
        online_box_list_.push_back(query_box_list_.back());
        query_box_list_.pop_back();
    }

    target_id_ = palletizer_->queryBoxOnline();





    qDebug()<<palletizer_->n_ordered_list_size();
    update();
}

void QPalletizerWidget::slotLoadTestQueue(QString filename)
{
    if(filename.isEmpty())
    {
        filename = QFileDialog::getOpenFileName(0,tr("Get Test Queue"),"","*.p.json");
    }

    if(filename.isEmpty()) return;

    QByteArray message;

    QFile file(filename);

    if(!file.open(QFile::ReadOnly)) return;

    message = file.readAll();

    QJsonDocument doc;
    QJsonParseError error{};
    doc = QJsonDocument::fromJson(message,&error);
    QJsonObject obj = doc.object();
    int flag = 0;
    if(obj.contains("platform"))
    {
        flag = 1;
        QJsonArray platform = obj.value("platform").toArray();
        if(platform.size() < 2) {
            flag = 0;
        }
        else {
            this->initQuery(platform.at(0).toInt(1000),platform.at(1).toInt(1000));
        }
    }
    if(flag == 1 && obj.contains("list"))
    {
       std::vector<QueryBox> qblist;
       QJsonObject list = obj.value("list").toObject();
       for(auto iter = list.begin(); iter != list.end(); iter++)
       {
            QJsonObject bbox = iter.value().toObject();
            int num = bbox.value("num").toInt(0);
            for(int i=0; i < num; i++)
            {
                int w = bbox.value("w").toInt(100);
                int h = bbox.value("h").toInt(100);
                int d = bbox.value("d").toInt(100);
                qblist.emplace_back(w,h,d);
            }
       }
       query_box_list_ = qblist;

    }

    randomQueryOrder();

    update();

}

void QPalletizerWidget::initializeGL()
{
    QRenderWidget::initializeGL();

    QMatrix4x4 matrix;
    matrix.lookAt(QVector3D(0,-3000,3000),QVector3D(0,0,0),QVector3D(0,1,0));

    scene_->setDefaultModelMatrix(matrix);
    scene_->setDefaultView();
    cube_ = new QPrimitiveMesh(QPrimitiveMesh::Box,getScene());
    cube_->init();


    quad_ = new QQuadMesh(getScene());
    quad_->init();

    quad_->setElementType(GL_LINES);
    quad_->updateRect(-0.9,-0.9,1.8,1.8);
    quad_->setColor(QVector4D(1.0,1.0,0.0,1.0));
}

void QPalletizerWidget::paintGL()
{
    QList<QColor> colors;
//    srand(time(nullptr));
    for(int i=0; i < 50; i++)
    {
        QColor c;
        c = QColor::fromHsl(30 * i % 360,90 + i * 30 % 150,100);
        colors.push_back(c.toRgb());
    }
   glClearColor(0,0,0,0);
   glViewport(0,0,this->width(),this->height());
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   QMatrix4x4 project_matrix = scene_->getProjectionMatrix();
   QMatrix4x4 model_matrix = scene_->getModelMatrix();
   int state = 0;

   if( palletizer_->n_ordered_list_size() >1 && target_id_>=0)
   {
       state = 1;
   }

   for(int i=0; i < palletizer_->n_ordered_list_size() - state; i++)
   {
       QueryBox box = palletizer_->getOrderedBoxAt(i);
       QColor color = colors[(box.id_+50)%50];
       QVector4D cc(color.red(),color.green(),color.blue(),color.alpha());
       cc /= 255.0;

       if(i == 0)
       {
           box.size_[2] = 20;
           box.pos_[2] = -20;
           cc = QVector4D(0.6,0.6,0.6,1.0);
       }

       cube_->setScale(QVector3D(box.w(),box.h(),box.d()) / 10.0);

       Vec3i center = box.center();

       cube_->setOffset(QVector3D(center[0],center[1],center[2]));

       cube_->setColor(cc);
       cube_->render(project_matrix,model_matrix);
   }
   int th = 100;
   float sc = this->width()/float(this->height());
   for(int i=0; i < online_box_list_.size(); i++)
   {
       glViewport(0,(online_box_list_.size()-1-i) *th,th * sc,th);

       int ii = i;
       if(ii >= this->online_box_list_.size()) break;
       QueryBox box = this->online_box_list_[ii];

       cube_->setScale(QVector3D(box.w(),box.h(),box.d()) / 4.0);

       cube_->setOffset(QVector3D(0,0,0));

       QColor color = colors[(box.id_+50)%50];
       QVector4D cc(color.red(),color.green(),color.blue(),color.alpha());
       cc /= 255.0;

       cube_->setColor(cc);

       cube_->render(project_matrix,model_matrix);

        if(box.id_ == target_id_)
        {
            quad_->render();
        }

   }



}

void QPalletizerWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        scene_->setDefaultView();
        update();
    }
    else if(event->key() == Qt::Key_R)
    {
        this->slotTryOnce();
        update();
    }
    else if(event->key() == Qt::Key_O)
    {
        this->slotLoadTestQueue();
    }
    else if(event->key() == Qt::Key_X)
    {
        auto orderlist = palletizer_->getOrderedList();
        orderlist.erase(orderlist.begin());
        for(auto qbox : query_box_list_)
        {
            orderlist.push_back(qbox);
        }
        for(auto qbox : online_box_list_)
        {
            if(qbox.id_ == target_id_) continue;
            orderlist.push_back(qbox);
        }
        this->initQuery(this->palletizer_->getPlatformWidth(),this->palletizer_->getPlatformHeight());
        query_box_list_ = orderlist;

        randomQueryOrder();

        update();

    }
}

void QPalletizerWidget::randomQueryOrder()
{
    std::random_device rd;  // 将用于为随机数引擎获得种子
    std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
    std::uniform_int_distribution<> dis(1, query_box_list_.size());
    for(int i=0; i < query_box_list_.size(); i++)
    {
        int j =  (i + dis(gen)) % query_box_list_.size();
        if(i == j) continue;
        std::swap(query_box_list_[i],query_box_list_[j]);
    }

    for(int i=0; i < query_box_list_.size(); i++)
    {
        query_box_list_[i].id_ = query_box_list_.size() - 1 - i;
    }
}
}
