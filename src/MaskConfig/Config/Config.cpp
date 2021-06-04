#include "Config.h"
#include <QJsonDocument>
#include "Util/LogTool.h"

#include "Setting/SettingWords.h"
#include "Setting/Setting.h"
#include "Setting/NumSetting.h"
#include "Setting/SelectSetting.h"
#include "Setting/PolygonSetting.h"
#include "Setting/PolygonListSetting.h"
#include "Setting/BBoxSetting.h"
#include "Setting/TextSetting.h"

#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QJsonArray>

#include <iostream>
#include <fstream>

#ifdef WIN32
#include <io.h>
#include <direct.h>
#endif

#ifdef Linux
#include <sys/io.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace MaskConfig {
Config::Config()
{

}

void Config::init(const QString &filename)
{
    settings_.clear();
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        LogTool::debug(SettingInfo::OpenFileError, filename);
        return;
    }
    QByteArray b = file.readAll();
    file.close();

    QJsonDocument json_doc;
    QJsonParseError parse_error;
    json_doc = QJsonDocument::fromJson(b,&parse_error);

    if(parse_error.error == QJsonParseError::NoError)
    {
        if(json_doc.isObject())
        {

            QJsonObject object = json_doc.object();
            QStringList keys = object.keys();
            foreach (const QString& key, keys) {
                addFromJsonObject(key,object[key].toObject());
            }

        }
    }
    else
    {
        LogTool::warn(SettingInfo::JsonError);
    }
}

void Config::load(const QString& filename)
{
    QString single_file_name = filename;
    QString json_file_name = filename;

    single_file_name = single_file_name.split("/")[single_file_name.split("/").size() - 1];

    json_file_name = json_file_name.replace(single_file_name, "json/")  + single_file_name;

    QFile file(json_file_name);
    if(!file.open(QIODevice::ReadOnly))
    {
        LogTool::debug(SettingInfo::OpenFileError, json_file_name);
        return;
    }
    QByteArray b = file.readAll();
    file.close();

    QJsonDocument json_doc;
    QJsonParseError parse_error;
    json_doc = QJsonDocument::fromJson(b,&parse_error);

    if(parse_error.error == QJsonParseError::NoError)
    {
        if(json_doc.isObject())
        {

            QJsonObject object = json_doc.object();
            QStringList keys = object.keys();
            foreach (const QString& key, keys) {
                if(settings_.contains(key))
                {
                    settings_[key]->loadFromJsonObject(object[key].toObject());
                }
            }

        }
    }
    else
    {
        LogTool::warn(SettingInfo::JsonError);
    }
}

void Config::json2xml(const QString& filename, bool need_to_change_image_size, bool use_my_labels, int image_width, int image_height)
{
    QString my_labels = "person";

    QString single_file_name = filename;
    QString json_file_name = filename;
    QString xml_file_name = filename;

    single_file_name = single_file_name.split("/")[single_file_name.split("/").size() - 1];

    json_file_name = json_file_name.replace(single_file_name, "json/")  + single_file_name;

    xml_file_name = xml_file_name.replace(single_file_name, "Annotations/")  + single_file_name;

    QDir dir_(xml_file_name.split("/" + single_file_name)[0]);
    if(!dir_.exists())
    {
        dir_.mkpath(xml_file_name.split("/" + single_file_name)[0]);
    }

    /*
    if (access((xml_file_name.split("Annotations/" + single_file_name)[0] + "ImageSets").toStdString().c_str(), 00) != 0)
    {
        mkdir((xml_file_name.split("Annotations/" + single_file_name)[0] + "ImageSets").toStdString().c_str());
    }
    if (access((xml_file_name.split("Annotations/" + single_file_name)[0] + "ImageSets/Main").toStdString().c_str(), 00) != 0)
    {
        mkdir((xml_file_name.split("Annotations/" + single_file_name)[0] + "ImageSets/Main").toStdString().c_str());
    }
    if (access((xml_file_name.split("Annotations/" + single_file_name)[0] + "JPEGImages").toStdString().c_str(), 00) != 0)
    {
        mkdir((xml_file_name.split("Annotations/" + single_file_name)[0] + "JPEGImages").toStdString().c_str());
    }
    */

    xml_file_name = xml_file_name.replace(".json", ".xml");

    QFile loadFile(json_file_name);

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "could't open projects json";
        return;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    QJsonObject subObj = rootObj.value("Area").toObject();
    QJsonArray sub_label_Array = subObj.value("labels").toArray();
    QJsonArray sub_polygon_Array = subObj.value("polygons").toArray();
    QJsonArray sub_shape_Array = subObj.value("shape").toArray();

    //qDebug() << "QString : " << xml_file_name << endl;
    //std::cout << "std::string : " << xml_file_name.toStdString() << std::endl;
    //std::cout << "new std::string : " << std::string((const char *)xml_file_name.toLocal8Bit()) << std::endl;
    //std::ofstream out(xml_file_name.toStdString());
    std::ofstream out(std::string((const char *)xml_file_name.toLocal8Bit()));

    out <<"<annotation>\n";
    out <<"\t<folder>VOC2007</folder>\n";
    out <<"\t<filename>" << single_file_name.split(".")[0].toStdString() << "</filename>\n";
    out <<"\t<size>\n";
    if (need_to_change_image_size)
    {
        out <<"\t\t<width>" << image_width << "</width>\n";
        out <<"\t\t<height>" << image_height << "</height>\n";
    }
    else
    {
        out <<"\t\t<width>" << sub_shape_Array.at(0).toInt() << "</width>\n";
        out <<"\t\t<height>" << sub_shape_Array.at(1).toInt() << "</height>\n";
    }
    out <<"\t\t<depth>3</depth>\n";
    out <<"\t</size>\n";
    out <<"\t<segmented>0</segmented>\n";
    if (sub_label_Array.size() > 0)
    {
        for (int i = 0; i < sub_label_Array.size(); ++i)
        {
            out <<"\t<object>\n";
            if (use_my_labels)
            {
                out <<"\t\t<name>" << my_labels.toStdString() << "</name>\n";
            }
            else
            {
                out <<"\t\t<name>person</name>\n";
            }
            out <<"\t\t<pose>Unspecified</pose>\n";
            out <<"\t\t<truncated>0</truncated>\n";
            out <<"\t\t<difficult>0</difficult>\n";
            out <<"\t\t<bndbox>\n";
            if (need_to_change_image_size)
            {
                out << "\t\t\t<xmin>" << int(sub_polygon_Array.at(i).toArray().at(0).toArray().at(0).toInt() * image_width / sub_shape_Array.at(0).toInt()) << "</xmin>\n";
                out << "\t\t\t<ymin>" << int(sub_polygon_Array.at(i).toArray().at(0).toArray().at(1).toInt() * image_height / sub_shape_Array.at(1).toInt()) << "</ymin>\n";
                out << "\t\t\t<xmax>" << int(sub_polygon_Array.at(i).toArray().at(2).toArray().at(0).toInt() * image_width / sub_shape_Array.at(0).toInt()) << "</xmax>\n";
                out << "\t\t\t<ymax>" << int(sub_polygon_Array.at(i).toArray().at(2).toArray().at(1).toInt() * image_height / sub_shape_Array.at(1).toInt()) << "</ymax>\n";
            }
            else
            {
                out << "\t\t\t<xmin>" << sub_polygon_Array.at(i).toArray().at(0).toArray().at(0).toInt() << "</xmin>\n";
                out << "\t\t\t<ymin>" << sub_polygon_Array.at(i).toArray().at(0).toArray().at(1).toInt() << "</ymin>\n";
                out << "\t\t\t<xmax>" << sub_polygon_Array.at(i).toArray().at(2).toArray().at(0).toInt() << "</xmax>\n";
                out << "\t\t\t<ymax>" << sub_polygon_Array.at(i).toArray().at(2).toArray().at(1).toInt() << "</ymax>\n";
            }

            out << "\t\t</bndbox>\n";
            out << "\t</object>\n";
        }
    }
    out << "</annotation>";

    out.close();
}

void Config::save(const QString &filename)
{
    QString single_file_name = filename;
    QString json_file_name = filename;

    single_file_name = single_file_name.split("/")[single_file_name.split("/").size() - 1];

    json_file_name = json_file_name.replace(single_file_name, "json/")  + single_file_name;

    QDir dir_(json_file_name.split("/" + single_file_name)[0]);
    if(!dir_.exists())
    {
        dir_.mkpath(json_file_name.split("/" + single_file_name)[0]);
    }

    QJsonObject object;
    foreach(const QString& key, settings_.keys())
    {
         Setting* setting = settings_[key].get();
         if(!setting->isReadyToSave())
         {
//             return;
         }
         setting->saveAsJsonObject();
         object[key] = setting->object();
    }
    QJsonDocument json_doc;
    json_doc.setObject(object);

    QFile file(json_file_name);
    if(!file.open(QIODevice::WriteOnly))
    {
        LogTool::debug(SettingInfo::OpenFileError, json_file_name);
        return;
    }

    file.write(json_doc.toJson());

    file.close();

    json2xml(filename);
}

void Config::add(const QString &name, std::shared_ptr<Setting> setting)
{
    if(!setting.get()) return;
    settings_[name] = setting;
}

void Config::clear()
{
    QList<QString> keys = settings_.keys();
    for(int i=0; i < keys.size(); i++)
    {
        settings_[keys[i]]->clear();
    }
}

QList<QString> Config::keys() const
{
    return settings_.keys();
}

Setting *Config::getSetting(const QString &key)
{
    if(settings_.contains(key))
    {
        return settings_[key].get();
    }
    else {
        return NULL;
    }
}

QString Config::getName(const QString &key)
{
    Setting* setting = this->getSetting(key);
    if(setting)
    {
        if(!setting->name().isEmpty())
        {
            return setting->name();
        }
    }
    return key;
}

void Config::addFromJsonObject(const QString &name, const QJsonObject &object)
{
    QString type;
    std::shared_ptr<Setting> setting;
    if(object.contains(SettingWords::Type))
    {
        type = object[SettingWords::Type].toString();
    }
    if( SettingTypes::Int == type||SettingTypes::Float==type)
    {
        setting = std::shared_ptr<Setting>(new NumSetting());

    }
    else if(SettingTypes::Text == type)
    {
        setting = std::shared_ptr<Setting>(new TextSetting());
    }
    else if(SettingTypes::Polygon == type)
    {
         setting =std::shared_ptr<Setting>(new PolygonSetting());

    }
    else if(SettingTypes::Polygons == type)
    {
         setting =std::shared_ptr<Setting>(new PolygonListSetting());

    }
    else if(SettingTypes::BBox == type)
    {
        setting =std::shared_ptr<Setting>(new BBoxSetting());
    }
    else if(SettingTypes::Select == type)
    {
        setting = std::shared_ptr<Setting>(new SelectSetting());
    }
    if(setting.get())
    {
        setting->initFromJsonObject(object);
        settings_[name] = setting;
    }
    else
    {
         LogTool::debug(SettingInfo::UnknownKey, name);
    }

}
}
