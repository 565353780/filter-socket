#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>

namespace Ui {
class ClientDialog;
}

class ClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientDialog(QWidget *parent = 0);
    ~ClientDialog();

private:
    Ui::ClientDialog *ui;
};

#endif // CLIENTDIALOG_H
