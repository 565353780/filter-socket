#include "PolygonListSetting.h"
#include "SettingWords.h"
#include "SettingFunctions.h"
#include "Util/LogTool.h"
#include <QJsonArray>
#include "LabelDict.h"
#include <QDebug>
namespace MaskConfig{

PolygonListSetting::PolygonListSetting()
{
    has_setter_ = true;
    has_labels_ = false;
    color_ = QColor(Qt::yellow);
    type_ = SettingTypes::Polygons;
}

void PolygonListSetting::initFromJsonObject(const QJsonObject &object)
{
    Setting::loadFromJsonObject(object);
    has_labels_ = false;
    if(object_.contains(SettingWords::LabelDict))
    {
        QJsonArray array = object_[SettingWords::LabelDict].toArray();
        label_dict_list_.clear();
        for(int i=0; i < array.size(); i++)
        {
            std::shared_ptr<LabelDict> label_dict(new LabelDict());
            label_dict->loadFromJsonObject(array[i].toObject());
            label_dict_list_.push_back(label_dict);
        }
        has_labels_ = true;
    }
    if(object_.contains(SettingWords::BindTargets))
    {
        QJsonArray  array = object_[SettingWords::BindTargets].toArray();
        targetKeys_.clear();
        for(int i=0; i < array.size(); i++)
        {
            QString name = array[i].toString();
            if(!name.isEmpty())
                targetKeys_.push_back(name);
        }
    }

    bool polygons_error = true;

    if(object.contains(SettingWords::Polygons))
    {
        polygons_.clear();
        labels_.clear();
        polygons_error = false;
        QJsonValue val = object[SettingWords::Polygons];
        if(val.isArray())
        {
            QJsonArray array = val.toArray();
            for(int i=0; i < array.size(); i++)
            {
                QPolygonF p;
                if(Functions::getPolygonFromJsonValue(p,array.at(i)))
                {
                    this->add(p);

                }
                else
                {
                    polygons_error = true;
                }
            }

        }
        else
        {
            polygons_error = true;
        }
    }
    if(object.contains(SettingWords::Labels))
    {
        QJsonArray array = object[SettingWords::Labels].toArray();
        for(int i=0; i < array.size() && i < labels_.size(); i++)
        {
            QJsonArray l_array = array[i].toArray();
            for(int j=0; j < l_array.size(); j++)
            {
                if(j < labels_[i].size())
                {
                    labels_[i][j] = (l_array[j].toString());
                }
                else
                {
                    labels_[i].push_back(l_array[j].toString());
                }
            }
        }

    }
    if(polygons_error)
    {
        LogTool::warn(SettingInfo::PolygonError);
    }

    if(object_.contains(SettingWords::Color))
    {
        QJsonValue val = object_[SettingWords::Color];
        Functions::getColorFromJsonObject(color_,val.toObject());
    }

    if(object_.contains(SettingWords::SelectedColor))
    {
        QJsonValue val = object_[SettingWords::SelectedColor];
        Functions::getColorFromJsonObject(selectedColor_,val.toObject());
    }


    this->update();

}

void PolygonListSetting::loadFromJsonObject(const QJsonObject &object)
{
    Setting::loadFromJsonObject(object);

    bool polygons_error = true;

    if(object.contains(SettingWords::Polygons))
    {
        polygons_.clear();
        labels_.clear();
        polygons_error = false;
        QJsonValue val = object[SettingWords::Polygons];
        if(val.isArray())
        {
            QJsonArray array = val.toArray();
            for(int i=0; i < array.size(); i++)
            {
                QPolygonF p;
                if(Functions::getPolygonFromJsonValue(p,array.at(i)))
                {
                    this->add(p);

                }
                else
                {
                    polygons_error = true;
                }
            }

        }
        else
        {
            polygons_error = true;
        }
    }
    if(has_labels_ && object.contains(SettingWords::Labels))
    {
        QJsonArray array = object[SettingWords::Labels].toArray();
        for(int i=0; i < array.size() && i < labels_.size(); i++)
        {
            QJsonArray l_array = array[i].toArray();
            for(int j=0; j < l_array.size(); j++)
            {
                if(j < labels_[i].size())
                {
                    labels_[i][j] = (l_array[j].toString());
                }
                else
                {
                    labels_[i].push_back(l_array[j].toString());
                }
            }
        }

    }
    if(targetKeys_.size() > 0 && object.contains(SettingWords::Bindings))
    {
        QJsonArray array = object[SettingWords::Bindings].toArray();
        for(int i=0; i < array.size() && i < bindings_.size(); i++)
        {
            QJsonArray l_array = array[i].toArray();
            for(int j=0; j < l_array.size() && j < targetKeys_.size(); j++)
            {
                bindings_[i][targetKeys_[j]] = l_array[j].toInt();
            }
        }
    }

    if(polygons_error)
    {
        LogTool::warn(SettingInfo::PolygonError);
    }


    this->update();

}

void PolygonListSetting::saveAsJsonObject()
{
    object_ = QJsonObject();
    Setting::saveAsJsonObject();
    QJsonArray array;
    for(int i=0; i < polygons_.size(); i++)
    {
        QJsonArray p_array;
        if(Functions::getJsonArrayFromPolygon(p_array,polygons_[i]))
        {
            array.push_back(p_array);
        }
        else
        {
            LogTool::debug(SettingInfo::PolygonError);

        }
    }
    object_[SettingWords::Polygons] = array;

//    QJsonObject color_object;
//    color_object[SettingWords::R] = color_.red();
//    color_object[SettingWords::G] = color_.green();
//    color_object[SettingWords::B] = color_.blue();

//    object_[SettingWords::Color] = color_object;

    QJsonArray label_array;
    for(int i=0; i < labels_.size(); i++)
    {
        QJsonArray l_array;
        foreach (QString label, labels_[i]) {
            l_array.push_back(QJsonValue(label));
        }
        label_array.push_back(l_array);
    }
    object_[SettingWords::Labels] = label_array;
    if(targetKeys_.size() > 0)
    {
        QJsonArray binding_array;
        for(int i=0; i < bindings_.size(); i++)
        {
            QJsonArray l_array;
            foreach(QString key, targetKeys_)
            {
                l_array.push_back( QJsonValue(bindings_[i][key]));
            }
            binding_array.push_back(l_array);
        }
        object_[SettingWords::Bindings] = binding_array;

    }
    QJsonArray shape_array;
    shape_array.push_back(QJsonValue(v_size_.width()));
    shape_array.push_back(QJsonValue(v_size_.height()));
    object_[SettingWords::Shape] = shape_array;
}

bool PolygonListSetting::isReadyToSave()
{
    for(int i=0; i < polygons_.size(); i++)
    {
        if(polygons_[i].size() < 3)
        {
            LogTool::warn(SettingInfo::EmptyPolygonError);
            return false;
        }
    }

//    if(targetKeys_.size() > 0)
//    {
//        for(int i=0; i < bindings_.size(); i++)
//        {
//            for(int j=0; j < targetKeys_.size(); j++)
//            {
//                if(bindings_[i][targetKeys_[j]] < 0)
//                {
//                    LogTool::warn(SettingInfo::EmptyBindingError);
//                    return false;
//                }
//            }
//        }
//    }
    return true;
}

void PolygonListSetting::add(const QPolygonF &poly)
{
    polygons_.push_back(poly);
    labels_.push_back(QStringList());
    bindings_.push_back(QMap<QString,int>());
    for(int i=0; i < label_dict_list_.size(); i++)
    {
        labels_.back().push_back(label_dict_list_[i]->key_);
    }
//    qDebug()<<labels_.back().size();
    for(int i=0; i < targetKeys_.size(); i++)
    {
        bindings_.back()[targetKeys_[i]] = 0;
    }


}

void PolygonListSetting::remove(int index)
{
    if(index < 0 ||index>=this->polygons_.size()) return;
    this->polygons_.removeAt(index);
    this->labels_.removeAt(index);
    this->bindings_.removeAt(index);
}

void PolygonListSetting::clear()
{
    this->polygons_.clear();
    this->labels_.clear();
    this->bindings_.clear();
}

void PolygonListSetting::pop_back()
{
    if(this->polygons_.size() > 0)
    {
        this->polygons_.pop_back();
        this->labels_.pop_back();
        this->bindings_.pop_back();
    }
}



QColor PolygonListSetting::color() const
{
    return color_;
}

QStringList PolygonListSetting::targetKeys() const
{
    return targetKeys_;
}



QColor PolygonListSetting::selectedColor() const
{
    return selectedColor_;
}



}
