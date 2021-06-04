#include "QLogTool.h"
#include <QMessageBox>
#include <QDebug>
namespace MaskConfig {
QLogTool::QLogTool(QObject *parent):QObject(parent)
{
}

void QLogTool::warnLog(const QString &info, const QString &detail)
{
    QMessageBox::warning(0,"",info+detail);
}
}
