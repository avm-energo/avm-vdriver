#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class TCPClient : public QObject {
  Q_OBJECT
public:
  explicit TCPClient(QObject *parent = nullptr);
  ~TCPClient();

  bool init(const QString &ip, int port);
  void setName(const QString &name);
  void disconnectSocket();

  bool status;
  bool isinitcom;

public slots:
  void writeData(const QByteArray &ba);

signals:
  void newDataReady(QByteArray &ba);

private slots:
  void errorOccured(QAbstractSocket::SocketError error);
  void newDataReceived();
  void socketConnected();
  void socketDisconnected();
  void itsTimeToReconnect();

private:
  QTcpSocket *m_socket;
  QString m_name, m_ip;
  int m_port;
  QTimer *m_reconnectTimer;
};
