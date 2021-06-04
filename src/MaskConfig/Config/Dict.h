#ifndef DICT_H
#define DICT_H

#include <QObject>
#include <QMap>
namespace MaskConfig {

class Dict : public QObject
{
    Q_OBJECT
public:
    explicit Dict(QObject *parent = 0);
signals:

public slots:

public:
    QString translate(const QString& key) const;


    static QString t(const QString& key);
private:
    void init();
    static Dict* instance_;
    QMap<QString,QString> map_;
};
}
#endif // DICT_H
