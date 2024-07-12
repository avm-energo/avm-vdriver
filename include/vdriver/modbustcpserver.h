#ifndef MODBUSTCPSERVER_H
#define MODBUSTCPSERVER_H

#include <QObject>
#include <QTcpServer>

class ModbusTCPServer : public QObject
{
    Q_OBJECT
public:
    ModbusTCPServer(int port, QObject *parent = nullptr);

    bool start();

private:
    QTcpServer *m_server;
    int m_port;
};

#endif // MODBUSTCPSERVER_H
