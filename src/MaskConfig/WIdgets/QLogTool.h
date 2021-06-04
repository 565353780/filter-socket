#ifndef QLOGTOOL_H
#define QLOGTOOL_H
#include <QObject>
#include "Util/LogTool.h"
namespace MaskConfig {
class QLogTool : public QObject, public LogTool
{
    Q_OBJECT
public:
    QLogTool(QObject* parent);



public:
//    void debugLog(const QString& info, const QString& detail);
//    void statusLog(const QString& info, const QString& detail);
    void warnLog(const QString& info, const QString& detail);
};
}
#endif // QLOGTOOL_H
