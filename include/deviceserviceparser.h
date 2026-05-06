#pragma once

#include "settings.h"
#include "tcpclient.h"
#include <QObject>

class DeviceServiceParser : public QObject {
  Q_OBJECT
public:
  explicit DeviceServiceParser(TCPClient *client,
                               Settings::DeviceStruct devSettings,
                               QObject *parent = nullptr);

  void start(void);
  void newDataReceived(QByteArray &ba);

signals:
  void writeToService(const QByteArray &ba);

private:
  QTimer *m_ConfTimer;
  void getportstatus(int &np);
  bool checkintport(int &np);
  void setbaud(int &np);
  void setdataformat(int &np);
  Settings::DeviceStruct m_deviceSettings;
  TCPClient *m_client;
  bool isinitDev;
  bool noerr = true;
  bool isdataformat = false;
  char ninitport;
  int initcurport;
  char stepinitport;
  char Cpar[3] = {'N', 'O', 'E'};

private slots:
  void ConfTimerTimeout();
};
