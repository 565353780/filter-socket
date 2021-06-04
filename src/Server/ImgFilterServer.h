#ifndef IMGFILTERSERVER_H
#define IMGFILTERSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <memory>
namespace FilterSocket {
class DeeplearningSolver;
class ImgFilterServer : public QTcpServer
{
    Q_OBJECT
public:

    ImgFilterServer(quint16 port, QObject *parent = nullptr);

    void init();
signals:
    void signalState(const QString &);

public slots:
    void slotDisconnected();
protected:
   void incomingConnection(qintptr handle);

   std::shared_ptr<DeeplearningSolver> solver_;

};
}
#endif // IMGFILTERSERVER_H
