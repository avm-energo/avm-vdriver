#pragma once

#include <QObject>
#include <include/deviceparser.h>
#include <include/settings.h>

class ServiceParser : public QObject {
  Q_OBJECT
public:
  explicit ServiceParser(QObject *parent = nullptr);
  QList<TCPClient *> ClientList;
  int nunclientdocket;
  bool InitCom(TCPClient *client, Settings::DeviceStruct dev);

signals:
  void writeToSocket(const QByteArray &ba);
  void writeToService(const QByteArray &ba);

public slots:
  void newDataReceivedFromSocket(QByteArray &ba);
  void newDataServiceReceived(QByteArray &ba);
};
