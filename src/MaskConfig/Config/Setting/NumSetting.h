#ifndef NUMSETTING_H
#define NUMSETTING_H
#include "Setting.h"
namespace MaskConfig{
/**
 * @brief 包含"type"为"float"的JsonObject的类
 */
class NumSetting : public Setting
{
public:
    NumSetting();
    virtual ~NumSetting() {}


public:
    virtual void loadFromJsonObject(const QJsonObject& object);
    virtual void saveAsJsonObject();


public:
    float min_value() const;

    float max_value() const;

    float default_value() const; //缺省值

    float value() const;

    void setValue(float v);

    void setDefault();  //设置缺省值
private:
    float value_;
    float min_value_;
    float max_value_;
    float default_value_;
};
}
#endif // NUMSETTING_H
