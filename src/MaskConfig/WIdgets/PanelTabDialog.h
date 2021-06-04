#ifndef SETTERDIALOG_H
#define SETTERDIALOG_H

#include <QDialog>
#include "Util/LogTool.h"
class QTabWidget;
namespace MaskConfig {
class ConfigWidget;
class SetterWidget;
/**
 * @brief 右侧菜单栏的背景填充
 */
class PanelTabDialog : public QWidget
{
    Q_OBJECT
public:
    explicit PanelTabDialog(ConfigWidget *parent);

signals:

public slots:
    void slotTabChanged(int index);
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *e);
private:
    ConfigWidget *config_widget_;
    QTabWidget* tab_widget_;

public:
    SetterWidget* setter_widget_;
};
}
#endif // SETTERDIALOG_H
