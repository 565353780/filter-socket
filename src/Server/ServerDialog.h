#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include <QFile>
#include <QDateTime>

namespace Ui {
class ServerDialog;
}

class ServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerDialog(int port, QWidget *parent = 0);
    ~ServerDialog();

    void createActions();
    void createMenu();
public slots:
    void slotState(const QString &text);

    void on_HideButton_clicked();

    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

    void on_showMainAction();
    void on_exitAppAction();

private:
    Ui::ServerDialog *ui;

    QSystemTrayIcon *mSysTrayIcon;
    QMenu *mMenu;
    QAction *mShowMainAction;
    QAction *mExitAppAction;
};

#endif // SERVERDIALOG_H
