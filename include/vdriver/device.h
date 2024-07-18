#pragma once

#include <QObject>
#include <QTcpServer>
#include <include/vdriver/settings.h>
#include <include/vdriver/tcpclient.h>
#include <include/vdriver/tcpserver.h>

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    ~Device();
    bool init(Settings::DeviceStruct &device);

signals:
    void dataReceived(QByteArray &ba);
    void writeData(const QByteArray &ba);

private:
    QList<TCPServer *> m_serverList;
    QList<TCPClient *> m_clientList;
};
