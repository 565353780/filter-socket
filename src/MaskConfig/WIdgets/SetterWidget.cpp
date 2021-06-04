#include "SetterWidget.h"

namespace MaskConfig{


CommandButton::CommandButton(const QString &command, QWidget *parent):QPushButton(parent), command_(command)
{
    this->setText(Dict::t(command));
    connect(this,SIGNAL(clicked(bool)),this,SLOT(slotCommand()));
}

void CommandButton::slotCommand()
{
    emit this->signalCommand(command_);
}



SetterWidget::SetterWidget(const QString &key, Setter *setter, QWidget *parent):QWidget(parent),setter_(setter),key_(key)
{

    tracking_widget_ = NULL;

    QScrollArea* scrollarea = new QScrollArea(this);
    scrollarea->setWidget(new InfoWidget(setter_,scrollarea));
    info_widget_ = scrollarea;

    QList<Setter::Command> command_lists = setter->command_list();
//    QGridLayout* grid_layout = new QGridLayout(this);
    for(int i=0; i < command_lists.size(); i++)
    {
        CommandButton* button = new CommandButton(command_lists.at(i).command_,this);
        connect(button,SIGNAL(signalCommand(QString)),this,SLOT(slotCommand(QString)));
        QShortcut *shortcut= new QShortcut(QKeySequence("Ctrl+"+QString::number(i+1)),this);
        connect(shortcut,&QShortcut::activated,button,&CommandButton::click);
        button->setToolTip("Ctrl+"+QString::number(i+1));

        buttons_.push_back(button);
//        grid_layout->addWidget(button,i / 3, i % 3);
    }

//    this->setLayout(grid_layout);
    PolygonListSetting* setting = dynamic_cast<PolygonListSetting* >(setter->setting());
    if(setting)
    {
        for(int i=0; i < setting->label_dict_list_.size(); i++)
        {
            this->addLabelDict(setting->label_dict_list_[i].get());
        }
        PolygonsSetter* polys_setter =  dynamic_cast<PolygonsSetter* >(setter);
        if(polys_setter && polys_setter->bindingSize() > 0)
        {
            for(int i=0; i < polys_setter->bindingSize(); i++)
            {
                binding_widgets_.push_back(new BindingWidget(polys_setter->getBinding(i),this));
            }
        }

        BBoxSetter* bbox_setter =  dynamic_cast<BBoxSetter* >(setter);
        if(bbox_setter && bbox_setter->is_tracking())
        {
            tracking_widget_ = new TrackIdWidget(bbox_setter,this);
        }

    }

    btnNextImage = new CommandButton(tr("Next"), this);
    btnPrevImage = new CommandButton(tr("Prev"), this);

    buttons_.push_back(btnNextImage);
    buttons_.push_back(btnPrevImage);
}

void SetterWidget::addLabelDict(LabelDict *dict)
{
    LabelDictWidget* w = new LabelDictWidget(dict,this);
    label_dict_widgets_.push_back(w);
    connect(w,SIGNAL(signalChanged()),this,SLOT(slotUpdate()));
    connect(setter_->setting(),SIGNAL(signalUpdate()),w,SLOT(slotUpdateValue()));

//    resetGeometry();
}

void SetterWidget::slotCommand(const QString &command)
{
    setter_->runCommand(command);
    emit this->signalUpdate();

}

void SetterWidget::slotUpdate()
{
    setter_->update();
    emit signalUpdate();
    //qDebug()<<"find change in setter widget";
}

void SetterWidget::resizeEvent(QResizeEvent *event)
{
    this->resetGeometry();
}

void SetterWidget::resetGeometry()
{
    int buttonWidth = 90;
    int buttonHeight = 25;
    int y = 0;
    for(int i=0; i < buttons_.size(); i++)
    {
        buttons_[i]->setGeometry(i % 3 * (buttonWidth+10)+5, i / 3 * (buttonHeight+5)+20,   buttonWidth,buttonHeight);
        y = buttons_[i]->y()+buttons_[i]->height();
    }
    y += 25;


    for(int i=0; i < binding_widgets_.size(); i++)
    {
        binding_widgets_[i]->setGeometry(10,y+10,this->width()-20,30);
        y = binding_widgets_[i]->y() + binding_widgets_[i]->height();
    }

    for(int i=0; i < label_dict_widgets_.size(); i++)
    {
        label_dict_widgets_[i]->setGeometry(10,y+10,this->width()-20,25);
        y = label_dict_widgets_[i]->y() + label_dict_widgets_[i]->height();
    }
    if(tracking_widget_)
    {
        tracking_widget_->setGeometry(10,y+10,this->width()-20,60);
        y = tracking_widget_->y() + tracking_widget_->height();
    }
    y += 30;
    if(info_widget_)
    {
         info_widget_->setGeometry(5,y + 30,this->width()-15,this->height() -y);
    }

}

void SetterWidget::keyPressEvent(QKeyEvent *event)
{
//    if(event->key() == Qt::Key_Backspace)
//    {
//        label_dict_widgets_[0]->keyInput(-1);
//    }
//    else
//    {
//        for(int i = 0; i < 10; ++i)
//        {
//            if(event->key() == 0x30 + i)
//            {
//                label_dict_widgets_[0]->keyInput(i);
//                break;
//            }
//        }
//    }
}

void SetterWidget::Press0()
{
    label_dict_widgets_[0]->keyInput(0);
}
void SetterWidget::Press1()
{
    label_dict_widgets_[0]->keyInput(1);
}
void SetterWidget::Press2()
{
    label_dict_widgets_[0]->keyInput(2);
}
void SetterWidget::Press3()
{
    label_dict_widgets_[0]->keyInput(3);
}
void SetterWidget::Press4()
{
    label_dict_widgets_[0]->keyInput(4);
}
void SetterWidget::Press5()
{
    label_dict_widgets_[0]->keyInput(5);
}
void SetterWidget::Press6()
{
    label_dict_widgets_[0]->keyInput(6);
}
void SetterWidget::Press7()
{
    label_dict_widgets_[0]->keyInput(7);
}
void SetterWidget::Press8()
{
    label_dict_widgets_[0]->keyInput(8);
}
void SetterWidget::Press9()
{
    label_dict_widgets_[0]->keyInput(9);
}
void SetterWidget::PressBS()
{
    label_dict_widgets_[0]->keyInput(-1);
}
}
