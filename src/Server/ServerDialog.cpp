#include "ServerDialog.h"
#include "ui_serverdialog.h"

ServerDialog::ServerDialog(int port, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle(tr("ServerDialog"));
    this->ui->SeverLabel->setText(tr("Img Filter Server Listen Port:") + QString::number(port));

    mSysTrayIcon = new QSystemTrayIcon(this);

    QIcon icon = QIcon("Icon/Icon.jpg");
    mSysTrayIcon->setIcon(icon);

    mSysTrayIcon->setToolTip(tr("Deep Learning Server"));

    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    createActions();
    createMenu();

    mSysTrayIcon->show();
}

ServerDialog::~ServerDialog()
{
    delete ui;
}

void ServerDialog::slotState(const QString &text)
{
    this->ui->StateLabel->setText(text);
}

void ServerDialog::on_HideButton_clicked()
{
    this->hide();
}

void ServerDialog::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;
//    case QSystemTrayIcon::Trigger:
//        mSysTrayIcon->showMessage(QObject::trUtf8("Message Title"), QObject::trUtf8("Welcome to use this Inference Engine"), QSystemTrayIcon::Information, 1000);
//        break;
    default:
        break;
    }
}

void ServerDialog::createActions()
{
    mShowMainAction = new QAction(tr("Show Main Window"),this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));

    mExitAppAction = new QAction(tr("Exit"),this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));

}

void ServerDialog::createMenu()
{
    mMenu = new QMenu(this);
    mMenu->addAction(mShowMainAction);

    mMenu->addSeparator();

    mMenu->addAction(mExitAppAction);

    mSysTrayIcon->setContextMenu(mMenu);
}

void ServerDialog::on_showMainAction()
{
    this->show();
}

void ServerDialog::on_exitAppAction()
{
    exit(0);
}
