#ifndef BINDINGWIDGET_H
#define BINDINGWIDGET_H

#include <QWidget>
class QPushButton;
class QLabel;
namespace MaskConfig {
class PolygonsBinding;
/**
 * @brief The BindingWidget class
 */
class BindingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BindingWidget(PolygonsBinding* binding, QWidget *parent = 0);

signals:

public slots:
    /**
     * @brief 转入下一个选框
     */
    void slotNext();
    /**
     * @brief 转入前一个选框
     */
    void slotPrev();

//    void resizeEvent(QResizeEvent *event);
    void updateLabel();
private:


    PolygonsBinding *binding_;
    QLabel*     index_label_;
    QPushButton* prev_button_;
    QPushButton* next_button_;
};
}
#endif // BINDINGWIDGET_H
