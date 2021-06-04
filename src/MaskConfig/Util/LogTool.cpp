#include "LogTool.h"
#include <iostream>
#include <QDebug>
namespace MaskConfig{
LogTool *LogTool::instance_ = NULL;
LogTool::LogTool()
{
    instance_ = this;
}

void LogTool::debugLog(const QString &info, const QString& detail)
{
    qDebug()<<info<<" "<<detail;
}

void LogTool::statusLog(const QString &info, const QString& detail)
{
    qDebug()<<"status:"<<info<<" "<<detail;
}

void LogTool::warnLog(const QString &info, const QString& detail)
{
    qDebug()<<"warn:"<<info<<" "<<detail;
}

void LogTool::debug(const QString &info, const QString &detail)
{
    if(instance_)
    {
        instance_->debugLog(info,detail);
    }
}

void LogTool::status(const QString &info, const QString& detail)
{
    if(instance_)
    {
        instance_->statusLog(info,detail);
    }
}

void LogTool::warn(const QString &info, const QString& detail)
{
    if(instance_)
    {
        instance_->warnLog(info,detail);
    }
}
}
