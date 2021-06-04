#include "Dict.h"
#include "Config/Setter/SetterCommands.h"
namespace MaskConfig {
Dict* Dict::instance_ = NULL;
Dict::Dict(QObject *parent) : QObject(parent)
{
    if(!instance_)
    {
        instance_ = this;
    }
    this->init();
}

QString Dict::translate(const QString &key) const
{
    if(map_.contains(key))
    {
        return map_[key];
    }
    else
    {
        return key;
    }
}

QString Dict::t(const QString &key)
{
    if(instance_)
    {
        return instance_->translate(key);
    }
    else
    {
        return key;
    }
}

void Dict::init()
{
    map_[ SetterCommands::Clear ] = tr("clear");
    map_[ SetterCommands::PopPoint ] = tr("PopBack");
    map_[ SetterCommands::SetClosed] = tr("SetClosed");
    map_[ SetterCommands::PopPoint] = tr("PopPoint");
    map_[ SetterCommands::PopPoly] = tr("PopPoly");
    map_[ SetterCommands::Add] = tr("Add");
    map_[ SetterCommands::Prev] = tr("Prev");
    map_[ SetterCommands::Next] = tr("Next");
    map_[ SetterCommands::ShowMode] = tr("ShowMode");
    map_[ SetterCommands::Remove] = tr("Remove");
    map_[ SetterCommands::SetNextId] = tr("SetNextId");

    map_[ SetterWords::PointsNum] = tr("Num");
    map_[ SetterWords::None] = tr("None");


}
}
