#ifndef SetterCOMMANDWIDGET_H
#define SetterCOMMANDWIDGET_H
#include "Config/Setter/Setter.h"
#include "LabelDictWidget.h"
#include "Config/Setting/PolygonListSetting.h"
#include <QGridLayout>
#include "Config/Dict.h"
#include "InfoWidget.h"
#include "Config/Setter/PolygonsSetter.h"
#include "Config/Setter/BBoxSetter.h"
#include "TrackIdWidget.h"
#include "BindingWidget.h"
#include <QScrollArea>
#include <QShortcut>
#include <QKeySequence>
#include <QWidget>
#include <QPushButton>
#include <QKeyEvent>
#include <QAction>

namespace MaskConfig {
class Setter;
class LabelDict;
class LabelDictWidget;
class BindingWidget;
/**
 * @brief 对标定框的命令按钮
 */
class CommandButton : public QPushButton
{
    Q_OBJECT
public:
     explicit CommandButton(const QString& command, QWidget* parent);
signals:
    void signalCommand(const QString& command);

public slots:
    void slotCommand();
private:
    QString command_;
};
/**
 * @brief 对标定框进行操作命令的widget
 */
class SetterWidget : public QWidget
{
    Q_OBJECT
public:
   explicit SetterWidget(const QString& key, Setter* setter, QWidget* parent);

    /**
     * @brief 添加"LabelDict"（下拉列表）
     * @param dict
     */
    void addLabelDict(LabelDict* dict);
signals:
    void signalUpdate();
public slots:
    /**
     * @brief 运行command
     * @param command
     */
    void slotCommand(const QString& command);
    void slotUpdate();
public:
    QString key() const {return key_;}

protected:
    void resizeEvent(QResizeEvent *event);
    void resetGeometry();

    void keyPressEvent(QKeyEvent *event);
public slots:
    void Press0();
    void Press1();
    void Press2();
    void Press3();
    void Press4();
    void Press5();
    void Press6();
    void Press7();
    void Press8();
    void Press9();
    void PressBS();

private:
    QString key_;
    Setter* setter_;

    QList<CommandButton* > buttons_;
    QList<LabelDictWidget* > label_dict_widgets_;
    QList<BindingWidget* > binding_widgets_;
    QWidget* tracking_widget_;
    QWidget* info_widget_;

public:
    CommandButton* btnNextImage;
    CommandButton* btnPrevImage;
};
}
#endif // COMMANDWIDGET_H
