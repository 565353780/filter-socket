#include "QModelMakerWindow.h"
#include <QTabWidget>
#include <QToolBar>
#include <QDebug>
#include "QModelMakerWidget.h"

namespace GCL
{
const int EditModelMode = 1;
const int PickPointMode = 2;
QModelMakerWindow::QModelMakerWindow(QWidget *parent) : QMainWindow(parent), maker_widget_(new QModelMakerWidget(this))
{
    QToolBar *toolbar = this->addToolBar("main");

    toolbar->addAction(tr("Load"), maker_widget_, SLOT(slotLoadModel()));
    toolbar->addAction(tr("Save"), maker_widget_, SLOT(slotSaveModel()));

    mode_action_ =  toolbar->addAction(tr("PickPoint"));
    maker_widget_->slotChangeMode(true);
    mode_action_->setCheckable(true);
    mode_action_->setChecked(true);


    show_action_ = toolbar->addAction(tr("ShowAll"), toolbar, [&](bool t)
    {
        if(t)
        {
            show_action_->setText(tr("ShowOnlySelected"));
            maker_widget_->slotShowOnlySeclected();
        }
        else
        {
            show_action_->setText(tr("ShowAll"));
            maker_widget_->slotShowAll();
        }
    });
    show_action_->setCheckable(true);


    toolbar->addAction(tr("RecordPose"), maker_widget_, SLOT(slotRecordPlaceTransform()))->setData(int(PickPointMode));
    toolbar->addAction(tr("ShowPose"), maker_widget_, SLOT(slotShowPlaceTransform()))->setData(int(PickPointMode));
    toolbar->addAction(tr("OriginPose"), maker_widget_, SLOT(slotOriginTransform()))->setData(int(PickPointMode));
    toolbar->addAction(tr("SaveResults"), maker_widget_, SLOT(slotSaveResults()))->setData(int(PickPointMode));
    toolbar->addAction(tr("SaveFeatures"), maker_widget_, SLOT(saveFeaturePoints()))->setData(int(PickPointMode));


    toolbar->addSeparator()->setData(int(EditModelMode));

    toolbar->addAction(tr("RotateBySelection"), maker_widget_, SLOT(slotRotateBySelectedFaces()))->setData(int(EditModelMode));
    toolbar->addAction(tr("CutByPlane"), maker_widget_, SLOT(slotCutByZval()))->setData(int(EditModelMode));
    toolbar->addAction(tr("DelUnselection"), maker_widget_, SLOT(deleteUnselection()))->setData(int(EditModelMode));
    toolbar->addAction(tr("MoveCenter"), maker_widget_, SLOT(slotMoveModelCenter()))->setData(int(EditModelMode));
    toolbar->addAction(tr("UpdateVertices"), maker_widget_, SLOT(slotUpdateVertices()))->setData(int(EditModelMode));

    slotSetMode(true);
    connect(mode_action_, SIGNAL(toggled(bool)), this, SLOT(slotSetMode(bool)));

    connect(maker_widget_, SIGNAL(windowTitleChanged(QString)), this, SLOT(setWindowTitle(QString)));

//    QTabWidget *tab_widget = new QTabWidget(this);


    this->setCentralWidget(maker_widget_);

}

void QModelMakerWindow::slotSetMode(bool t)
{
    QList<QAction *> actions = this->findChildren<QAction *>();
    if(!t)
    {
        mode_action_->setText(tr("EditMesh"));
        for(QAction *action :  actions)
        {
            if(action->data().toInt() == 2)
            {
                action->setVisible(false);
            }
            else if(action->data().toInt() == 1)
            {
                action->setVisible(true);
            }
        }

    }
    else
    {
        mode_action_->setText(tr("PickPoint"));
        for(QAction *action :  actions)
        {
            if(action->data().toInt() == 1)
            {
                action->setVisible(false);
            }
            else if(action->data().toInt() == 2)
            {
                action->setVisible(true);
            }
        }
    }
    maker_widget_->slotChangeMode(t);
}

void QModelMakerWindow::keyPressEvent(QKeyEvent *event)
{
    maker_widget_->keyPressEvent(event);
}


}
