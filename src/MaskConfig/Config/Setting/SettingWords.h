#ifndef SETTINGWORDS_H
#define SETTINGWORDS_H
#include <QString>
namespace MaskConfig{
namespace SettingWords{
    const QString MinValue = "min_value";
    const QString MaxValue = "max_value";
    const QString NumValue = "value";
    const QString Value = "value";

    const QString Type = "type";
    const QString UI_Name = "ui_name";
    const QString Category = "category";
    const QString LabelDict = "labelDict";

    const QString Polygon = "polygon";
    const QString Color = "color";
    const QString SelectedColor = "color1";

    const QString R = "r";
    const QString G = "g";
    const QString B = "b";


    const QString Polygons = "polygons";
    const QString Name = "name";
    const QString Options = "options";
    const QString Labels = "labels";
    const QString Default = "default";
    const QString Values = "values";
    const QString CheckBox = "checkbox";
    const QString BindTargets = "bingTargets";
    const QString Bindings = "bindings";

    const QString Shape = "shape";

}

namespace SettingInfo {
    const QString PolygonError = "Load Polygon Error";
    const QString JsonError = "Json Error";
    const QString OpenFileError = "Open File Error";
    const QString EmptyPolygonError = "EmptyPolygonError";
    const QString EmptyBindingError = "EmptyBindingError";
    const QString EmptyLabelWarning = "EmptyLabelWarning";

    const QString UnknownKey = "Unknown Key";
}
}
#endif // SETTINGWORDS_H
