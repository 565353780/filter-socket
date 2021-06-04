#include "ConfigWidget.h"
#include "Config/Config.h"
#include "Config/Setter/PolygonSetter.h"
#include "Config/Setting/PolygonSetting.h"
#include "Config/Setting/BBoxSetting.h"

#include "PaintWidget.h"
#include <QToolBar>
#include <QFileDialog>
#include <QPalette>

#include "Util/LogTool.h"
#include "Config/Dict.h"
#include "PanelTabDialog.h"
#include "Config/Setter/PolygonsSetter.h"
#include "Config/Setter/BBoxSetter.h"
#include "WIdgets/SetterWidget.h"

#include "QLogTool.h"
#include <QKeyEvent>
#include <QPainter>
#include "Config/Setting/PolygonListSetting.h"
#include <QDir>
#include <QDirIterator>
#include <QInputDialog>
#include <ctime>
#include <QDebug>

namespace MaskConfig{

ConfigWidget::ConfigWidget(QWidget *parent) : QWidget(parent),
    config_(new Config()),current_setter_(NULL),paint_widget_(new PaintWidget(this)),marked_id_(-1)
{
    new QLogTool(this);
    new Dict(this);

    toolbar_ = new QToolBar("Main",this);

    toolbar_->addAction(tr("OpenDir"),this,SLOT(slotOpenDir()));
    QAction* prevAction=new QAction(tr("Prev"),this);
    prevAction->setShortcut(Qt::Key_Up);
    connect(prevAction,&QAction::triggered,this,&ConfigWidget::slotPrev);
    QAction* nextAction=new QAction(tr("Next"),this);
    nextAction->setShortcut(Qt::Key_Down);
    connect(nextAction,&QAction::triggered,this,&ConfigWidget::slotNext);
    toolbar_->addAction(prevAction);
    toolbar_->addAction(nextAction);
//    toolbar_->addAction(tr("LoadPrevResult"),this,SLOT(slotLoadPrev()));
//    toolbar_->addAction(tr("Interpolation"),this,SLOT(slotInterpolation()));

    this->setAutoFillBackground(true);
    QPalette pal;
    pal.setColor(QPalette::Background, Qt::darkGray);
    this->setPalette(pal);

    toolbar_->setAutoFillBackground(true);
    pal.setColor(QPalette::Background, Qt::gray);

    toolbar_->setPalette(pal);

    this->initConfig("Config/config.json");

    QList<QString> keys = config()->keys();
    foreach(QString key, keys)
    {
        Setting* setting = config()->getSetting(key);
        connect(setting,SIGNAL(signalUpdate()),paint_widget_,SLOT(update()));
    }

    dialog_ = new PanelTabDialog(this);
//    dialog_->setGeometry(100, 200,320,650);
//    dialog_->show();

    progress_bar_widget_ = new ProgressBarWidget(this);

    connect(progress_bar_widget_, SIGNAL(progressChangeTo(int)), this, SLOT(slotChangeToImageIndex(int)));

    actPress0 = new QAction(tr("Press0"),this);
    actPress0->setShortcut(Qt::Key_0);
    actPress1 = new QAction(tr("Press1"),this);
    actPress1->setShortcut(Qt::Key_1);
    actPress2 = new QAction(tr("Press2"),this);
    actPress2->setShortcut(Qt::Key_2);
    actPress3 = new QAction(tr("Press3"),this);
    actPress3->setShortcut(Qt::Key_3);
    actPress4 = new QAction(tr("Press4"),this);
    actPress4->setShortcut(Qt::Key_4);
    actPress5 = new QAction(tr("Press5"),this);
    actPress5->setShortcut(Qt::Key_5);
    actPress6 = new QAction(tr("Press6"),this);
    actPress6->setShortcut(Qt::Key_6);
    actPress7 = new QAction(tr("Press7"),this);
    actPress7->setShortcut(Qt::Key_7);
    actPress8 = new QAction(tr("Press8"),this);
    actPress8->setShortcut(Qt::Key_8);
    actPress9 = new QAction(tr("Press9"),this);
    actPress9->setShortcut(Qt::Key_9);
    actPressBS = new QAction(tr("PressBS"),this);
    actPressBS->setShortcut(Qt::Key_Backspace);

    this->addAction(actPress0);
    this->addAction(actPress1);
    this->addAction(actPress2);
    this->addAction(actPress3);
    this->addAction(actPress4);
    this->addAction(actPress5);
    this->addAction(actPress6);
    this->addAction(actPress7);
    this->addAction(actPress8);
    this->addAction(actPress9);
    this->addAction(actPressBS);

    connect(actPress0, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press0);
    connect(actPress1, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press1);
    connect(actPress2, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press2);
    connect(actPress3, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press3);
    connect(actPress4, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press4);
    connect(actPress5, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press5);
    connect(actPress6, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press6);
    connect(actPress7, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press7);
    connect(actPress8, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press8);
    connect(actPress9, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::Press9);
    connect(actPressBS, &QAction::triggered, dialog_->setter_widget_, &SetterWidget::PressBS);
    connect(dialog_->setter_widget_->btnNextImage, &QPushButton::clicked, this, &ConfigWidget::slotNext);
    connect(dialog_->setter_widget_->btnPrevImage, &QPushButton::clicked, this, &ConfigWidget::slotPrev);
}

Setter *ConfigWidget::current_setter()
{
    return current_setter_;
}

void ConfigWidget::initConfig(const QString &filename)
{
    config_->init(filename);
    QString current_key;
    QList<QString> keys = config_->keys();
//    LogTool::debug(QString::number(keys.size()));

    foreach (const QString& key, keys) {
        Setting* setting = config_->getSetting(key);
        if(!setting) continue;
        if(setting->type() == SettingTypes::Polygon)
        {
            setters_[key] = std::shared_ptr<Setter>(new PolygonSetter(setting));
            if(current_key.isEmpty()) current_key = key;
        }
        else if(setting->type() == SettingTypes::Polygons)
        {
            setters_[key] =  std::shared_ptr<Setter>(new PolygonsSetter(setting));
            if(current_key.isEmpty()) current_key = key;
        }
        else if(setting->type() == SettingTypes::BBox)
        {
            setters_[key] =  std::shared_ptr<Setter>(new BBoxSetter(setting));
            if(current_key.isEmpty()) current_key = key;
        }
    }
    foreach (const QString& key, keys) {
        Setting* setting = config_->getSetting(key);
        if(setting->type() == SettingTypes::Polygons ||setting->type() == SettingTypes::BBox )
        {
            PolygonListSetting* poly_setting = dynamic_cast<PolygonListSetting * >(setting);
            if(poly_setting)
            {
                QStringList targets = poly_setting->targetKeys();
                foreach(const QString& target_key,targets)
                {
                    PolygonListSetting* target_setting = dynamic_cast<PolygonListSetting * >(config_->getSetting(target_key));
                    if(target_setting)
                    {
                        PolygonsSetter* setter = dynamic_cast<PolygonsSetter * >(setters_[key].get());
                        if(setter)
                        {
                            setter->addBinding(target_key,target_setting);
                        }
                    }
                }
            }
        }
    }

    if(!current_key.isEmpty())
    {
        current_setter_ = setters_[current_key].get();
        current_setter_->init(0);
    }
    else
    {
        current_setter_ = NULL;
    }
    setterChanged();
}

void ConfigWidget::loadConfig(const QString &filename)
{
    config_->load(filename);
    if(current_setter_)
    {
        current_setter_->updateInfo();
    }


}

void ConfigWidget::saveConfig(const QString &filename)
{
    config_->save(filename);
}

void ConfigWidget::loadImage(const QString &filename)
{
    image_.load(filename);
    setterChanged();
    resizeWidgets();

}

void ConfigWidget::resizeEvent(QResizeEvent *event)
{
    toolbar_->setGeometry(0,0,this->width(),30);
    resizeWidgets();

    progress_bar_widget_->setGeometry(QRect(paint_widget_->width() + 15, 285, 300, 70));//Todo : 放大缩小后位置会被改变
}

void ConfigWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(dialog_->geometry(),Qt::white);
}

void ConfigWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers()==Qt::ControlModifier && event->key() == Qt::Key_S)
    {
        saveCurrentFile();
    }
    if(event->key() == Qt::Key_Space)
    {

    }
    else if(current_setter_)
    {
        current_setter_->keyDown(event->key(),event->modifiers());
        update();
    }
    event->accept();
}

void ConfigWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
         this->slotNext();
         this->slotLoadPrev();
    }
//    else if(event->key() == Qt::Key_Down || event->key()==Qt::Key_W)
//    {
//        this->slotNext();
//    }
//    else if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Q)
//    {
//        this->slotPrev();
//    }
//    else if(event->key() == Qt::Key_E)
//    {
//        this->slotLoadPrev();
//    }
//    else if(event->key() == Qt::Key_M)
//    {
//        this->marked_id_ = current_file_id_;
//        this->updateWindowsTitle();
//    }
    else if(current_setter_)
    {
        current_setter_->keyUp(event->key(),event->modifiers());
        update();
    }
    event->accept();
}

void ConfigWidget::resizeWidgets()
{

    int dx = 320;
    int x = 4;
    int w = this->width() - dx - 2;
    int h = 0;
    int ow = w;
    int oh = this->height() - 30;

    dialog_->setGeometry(w+10,30,dx,this->height()-30);


    if(image_.width() > 0)
    {
        h = ((double)image_.height() / (double)image_.width()) * w;

        if(h > oh)
        {
            h = this->height();
            w = ((double)image_.width() / (double)image_.height()) * oh;
            x = 4 + (ow - w) / 2;
        }


    }
    int y = 30 + (oh - h) / 2;

    if(paint_widget_)
    {
        paint_widget_->setGeometry(x,y,w,h);
    }

}

void ConfigWidget::setterChanged()
{
    if(current_setter_)
    {
        current_setter_->setVideoSize(image_.width(), image_.height());
        current_setter_->resize(paint_widget_->width(), paint_widget_->height());
        current_setter_->setImageShowSize(QPointF(0,0), image_.size());
    }
}

const QImage &ConfigWidget::image() const
{
    return image_;
}

Setter *ConfigWidget::getSetter(const QString &key)
{
    if(setters_.contains(key))
    {
        return setters_[key].get();
    }
    else
    {
        return NULL;
    }
}

void ConfigWidget::changeSetter(const QString &key)
{
    if(setters_.contains(key))
    {
        LogTool::debug(key);
        current_setter_ = setters_[key].get();
        current_setter_->init();
        setterChanged();
        this->update();
    }
}

void ConfigWidget::loadCurrentFile()
{
    if(dirname_.isEmpty()) return;
    if(current_file_id_ < 0 || current_file_id_>=filelist_.size()) return;
    loadImage(filelist_[current_file_id_]);

    if(current_setter())
    {
        current_setter()->finished();
    }

    QString config_file = filelist_[current_file_id_];
//    this->setWindowTitle(config_file);
    this->updateWindowsTitle();

    config_file = config_file.replace(".png",".json");
    config_file = config_file.replace(".jpg",".json");
    config_->clear();
    loadConfig(config_file);

    if(current_setter_)
    {
        current_setter()->init(0);
    }


}

void ConfigWidget::saveCurrentFile()
{
    if(dirname_.isEmpty()) return;
    if(current_file_id_ < 0 || current_file_id_>=filelist_.size()) return;
    QString config_file = filelist_[current_file_id_];
    config_file = config_file.replace(".png",".json");
    config_file = config_file.replace(".jpg",".json");
    saveConfig(config_file);

}

void ConfigWidget::updateWindowsTitle()
{
    if(current_file_id_ < 0 || current_file_id_>=filelist_.size()) return;
    QString config_file = filelist_[current_file_id_];
    QString title = "Id "+QString::number(current_file_id_)+": "+config_file;
    if(marked_id_>=0)
    {
        title += "   Marked Id: "+QString::number(marked_id_);
    }
    this->setWindowTitle(title);

}

void ConfigWidget::slotLoadImage()
{
    QString filename = QFileDialog::getOpenFileName(0,tr("Load Image..."),"./","*.jpg *.png");
    if(filename.isEmpty())
    {
        return;
    }
    dirname_ = "";
    filelist_.clear();
    current_file_id_ = -1;
    loadImage(filename);
}

void ConfigWidget::slotLoadConfig()
{
    QString filename = QFileDialog::getOpenFileName(0,tr("Load Config..."),"./","*.json");
    if(filename.isEmpty())
    {
        return;
    }
    loadConfig(filename);
}

void ConfigWidget::slotSaveConfig()
{
    QString filename = QFileDialog::getSaveFileName(0,tr("Save Config..."),"./","*.json");
    if(filename.isEmpty())
    {
        return;
    }
    saveConfig(filename);
}

void ConfigWidget::slotLoadPrev()
{
    if(current_file_id_>0)
    {
        QString config_file = filelist_[current_file_id_-1];
        config_file = config_file.replace(".png",".json");
        config_file = config_file.replace(".jpg",".json");
        config_->clear();
        loadConfig(config_file);

        if(current_setter_)
        {
            current_setter()->init(0);
        }
    }
}

void ConfigWidget::slotOpenDir()
{
    QString dirname = QFileDialog::getExistingDirectory(0,tr("Open Dir..."),"./");
    if(dirname.isEmpty()) return;
    QDir dir(dirname);

    QFile file(dirname + ".txt");
    if(file.open(QFile::ReadOnly))
    {
        qDebug() << "start quick read mode." << endl;
        filelist_.clear();
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);

            filelist_.push_back(dirname + "/" + str.split("|chLi|")[0] + ".jpg");
        }

        file.close();
    }
    else
    {
        QStringList filters;
        filters<<QString("*.jpg")<<QString("*.png");
        auto list = dir.entryList(filters, QDir::Files | QDir::NoSymLinks);

        filelist_.clear();
        for(auto filename : list)
        {
            QString absolute_file_path = dir.filePath(filename);
            filelist_.push_back(absolute_file_path);
        }
    }

//    QStringList filters;
//    filters<<QString("*.jpg")<<QString("*.png");
//    auto list = dir.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

    if(filelist_.size() > 0)
    {
        dirname_ = dirname;
        current_file_id_ = 0;
        loadCurrentFile();

        progress_bar_widget_->setMaxValue(filelist_.size());
    }

}

void ConfigWidget::slotNext()
{
    if(dirname_.isEmpty()) return;
    if(current_file_id_ < 0 || current_file_id_>=filelist_.size()) return;

    saveCurrentFile();
    if(current_file_id_ == filelist_.size() - 1)
    {
        if(this->windowTitle().contains("  ||    ")) return;
        this->setWindowTitle(this->windowTitle()+"  ||    "+tr("It's the last image!"));
        return;
    }

    current_file_id_ += 1;
    if(current_file_id_>=filelist_.size()) current_file_id_ = filelist_.size() - 1;

    loadCurrentFile();

    progress_bar_widget_->setValue(current_file_id_);
}

void ConfigWidget::slotPrev()
{
    if(dirname_.isEmpty()) return;
    if(current_file_id_ < 0 || current_file_id_>=filelist_.size()) return;


    saveCurrentFile();
    if(current_file_id_ == 0)
    {
        if(this->windowTitle().contains("  ||    ")) return;
        this->setWindowTitle(this->windowTitle()+"  ||    "+tr("It's the first image!"));
        return;
    }
    current_file_id_ -=1;
    if(current_file_id_ < 0)
    {
        current_file_id_ = 0;
    }

    loadCurrentFile();

    progress_bar_widget_->setValue(current_file_id_);
}

void ConfigWidget::slotInterpolation()
{
    if(current_file_id_ < 0) return;
    if(current_file_id_ >= filelist_.size()) return;
    int minId = current_file_id_ - 20;
    int default_id = minId + 10;
    if(minId < 0) minId = 0;
    if(default_id<0) default_id = 0;

    if(marked_id_>=0) default_id = marked_id_;
    int  prev_id = QInputDialog::getInt(this,tr("Get prev id"),tr("Get prev id"),minId,minId,current_file_id_);

    Config config;
    config.init("Config/config.json");
    QString config_file = filelist_[prev_id];
    config_file = config_file.replace(".png",".json");
    config_file = config_file.replace(".jpg",".json");
    config.load(config_file);
    Setting *setting = config.getSetting("Area");
    if(!setting) return;
    if(setting->type()!=SettingTypes::BBox) return;
    BBoxSetting *bbox_setting = dynamic_cast<BBoxSetting * >(setting);
    if(!bbox_setting) return;

    QMap<int, QRectF> prev_map;
    QMap<int, QRectF> current_map;
    QMap<int, int> idMap;
    for(int i=0; i < bbox_setting->poly_size(); i++)
    {
        int id;
        QRectF rect = bbox_setting->getBBox(i,id);
        prev_map[id] = rect;
        idMap[id] = i;
    }
    qDebug()<<"Stage 1";

    Setting *current_setting = config_->getSetting("Area");
    if(!current_setting || current_setting->type() != SettingTypes::BBox) return;
    BBoxSetting *current_bbox_setting = dynamic_cast<BBoxSetting * >(current_setting);
    if(!current_bbox_setting) return;

    for(int i=0; i < current_bbox_setting->poly_size(); i++)
    {
        int id;
        QRectF rect = current_bbox_setting->getBBox(i,id);
        current_map[id] = rect;
    }
    qDebug()<<"Stage 2";

    double tlen = current_file_id_ - prev_id;
    QList<int> keys = idMap.keys();
    for(int file_id=prev_id+1; file_id < current_file_id_; file_id++)
    {
        double alpha = (file_id - prev_id) / tlen;
        for(int ki=0; ki < keys.size(); ki++)
        {
             int id = keys[ki];
             int i = idMap[id];
             if(!current_map.contains(id)) continue;

             QRectF prev_rect = prev_map[id];
             QRectF current_rect = current_map[id];

             QRectF inter_rect;
             inter_rect.setLeft(prev_rect.left()*(1-alpha)+current_rect.left()*alpha);
             inter_rect.setTop(prev_rect.top()*(1-alpha)+current_rect.top()*alpha);
             inter_rect.setBottom(prev_rect.bottom()*(1-alpha)+current_rect.bottom()*alpha);
             inter_rect.setRight(prev_rect.right()*(1-alpha)+current_rect.right()*alpha);


             bbox_setting->setBBox(i,inter_rect);
        }
        QString save_file = filelist_[file_id];
        save_file = save_file.replace(".png",".json");
        save_file = save_file.replace(".jpg",".json");
        config.save(save_file);


    }

    marked_id_ = -1;
    this->updateWindowsTitle();
}

void ConfigWidget::slotUpdatePaint()
{
    if(paint_widget_)
    {
        paint_widget_->update();
    }
    if(current_setter_)
    {
        current_setter_->updateInfo();
    }
}

void ConfigWidget::slotChangeToImageIndex(int index)
{
    int change_to_file_id = index - 1;

    if(change_to_file_id < 0 || change_to_file_id >= filelist_.size())
    {
        qDebug() << "ConfigWidget::slotChangeToImageIndex : index out of range.";
        return;
    }

    saveCurrentFile();

    current_file_id_ = change_to_file_id;

    if(current_file_id_ == filelist_.size() - 1)
    {
        this->setWindowTitle(this->windowTitle()+"  ||    "+tr("It's the last image!"));
    }
    else if(current_file_id_ == 0)
    {
        this->setWindowTitle(this->windowTitle()+"  ||    "+tr("It's the first image!"));
    }

    loadCurrentFile();

    progress_bar_widget_->setValue(current_file_id_);
}

}
