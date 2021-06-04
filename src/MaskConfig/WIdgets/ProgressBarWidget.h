#ifndef PROGRESSBARWIDGET_H
#define PROGRESSBARWIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QDebug>

namespace Ui {
class ProgressBarWidget;
}

class ProgressBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressBarWidget(QWidget *parent = 0);
    ~ProgressBarWidget();

    void setMaxValue(int max_value);

    void setValue(int value);

private slots:
    void on_Btn_ChangeTo_clicked();

signals:
    void progressChangeTo(int index);

private:
    Ui::ProgressBarWidget *ui;

    int max_value_;
};

#endif // PROGRESSBARWIDGET_H
