#pragma once

#include <QObject>
#include <include/vdriver/modbustcpserver.h>
#include <include/vdriver/settings.h>
#include <include/vdriver/tcpclient.h>

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);
    ~Engine();

    void init(Settings &settings);

signals:

private:
    ModbusTCPServer *m_server;
};
