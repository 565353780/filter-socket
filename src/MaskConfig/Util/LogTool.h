#ifndef LOGTOOL_H
#define LOGTOOL_H
#include <QString>
#include <QDebug>
namespace MaskConfig{

class LogTool
{
public:
    LogTool();
    virtual ~LogTool() {}

    virtual void debugLog(const QString& info, const QString& detail);
    virtual void statusLog(const QString& info, const QString& detail);
    virtual void warnLog(const QString& info, const QString& detail);
public:
    static void debug(const QString& info, const QString& detail = "");
    static void status(const QString& info, const QString& detail = "");
    static void warn(const QString& info, const QString& detail = "");
private:
    static LogTool* instance_;
};
}
#endif // LOGTOOL_H
