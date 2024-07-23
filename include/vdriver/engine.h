#pragma once

#include <QObject>
#include <include/vdriver/settings.h>
#include <include/vdriver/tcpclient.h>
#include <include/vdriver/tcpserver.h>

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);
    ~Engine();

    void init();

signals:

private:
    TCPServer *m_server;
};
