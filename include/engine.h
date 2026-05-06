#pragma once

#include <QObject>
#include <include/settings.h>
#include <include/tcpclient.h>
#include <include/tcpserver.h>

class Engine : public QObject {
  Q_OBJECT
public:
  explicit Engine(QObject *parent = nullptr);
  ~Engine();

  void init();

signals:

private:
  TCPServer *m_server;
  TCPClient *m_client;
};
