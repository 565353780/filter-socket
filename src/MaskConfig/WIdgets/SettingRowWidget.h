#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>

class QLabel;
class QCheckBox;
namespace MaskConfig {
class Setting;
class SelectSetting;
class NumSetting;
class TextSetting;
/**
 * @brief 数字输入框
 */
class NumSettingWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit NumSettingWidget(NumSetting* setting, QWidget* parent);


public slots:
    void slotTextChanged(QString text);
    void updateState();

private:
    NumSetting* setting_;

};
/**
 * @brief 文字输入框
 */
class TextSettingWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit TextSettingWidget(TextSetting* setting, QWidget* parent);


public slots:
    void slotTextChanged(QString text);
    void updateState();

private:
    TextSetting* setting_;
};
/**
 * @brief 单选框
 */
class SingleSelectSettingWidget : public QComboBox
{
    Q_OBJECT
public:
    SingleSelectSettingWidget(SelectSetting* setting, QWidget* parent);
public slots:
    void updateState(int state);
    void slotChanged(int index);
private:
    SelectSetting* setting_;
};
/**
 * @brief 复选框
 */
class MultiSelectSettingWidget : public QWidget
{
    Q_OBJECT
public:
    MultiSelectSettingWidget(SelectSetting* setting, QWidget* parent);
public slots:
    void updateState();
    void slotChangeState();
private:
    SelectSetting* setting_;
    QMap<QString,QCheckBox* > checkbox_map_;

};
/**
 * @brief 标定内容相关的widget
 */
class SettingRowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingRowWidget(Setting* setting, QWidget* parent);

protected:
    void init();
private:
    Setting* setting_;
    QLabel* label_;
    QWidget* val_widget_;

};




}
#endif // SETTINGWIDGET_H
