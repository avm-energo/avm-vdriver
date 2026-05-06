#pragma once

#include <QList>
#include <QMap>
#include <QSettings>

namespace Settings {

enum Parity { NONE, ODD, EVEN, ERR };

struct PortStruct {
  bool use;
  int speed;
  Parity parity;
  int stopbit;
  int timeout;
};

struct DeviceStruct {
  int startPort;
  QString ip;
  QList<PortStruct> ports;
};

void init(QString &logFileName);
void readSettings();
void foreachConfFile();
void readDevSettings(const QString &confFile);
void logSettings();
void writeSettings(); // write module properties: serialnum, hwversion, etc
int reconnectPeriod();
int servicePort();
QString loglevel();

QList<DeviceStruct> deviceList();

namespace {
QList<DeviceStruct> m_deviceList;
QSettings *m_settings;
QString m_logDirectory, m_configDirectory;
QString m_logLevel;
QString m_logFileName;
int devCount;
int m_servicePort; // ModbusTCP service port to configurate VDriver
int m_reconnectPeriodInSec;
}; // namespace
}; // namespace Settings
