#include <QtDebug>
#include <avm-gen/files.h>
#include <avm-gen/logger.h>
#include <include/defaults.h>
#include <include/settings.h>

void Settings::init(QString &logFileName) {
#ifdef LOCALDEBUG
  m_logDirectory = QDir::homePath() + "/.local/share/vdriver/";
  m_configDirectory = QDir::homePath() + "/.config/vdriver/";
#else
  m_logDirectory = "/var/log/";
  m_configDirectory = "/usr/local/etc/vdriver/";
#endif
  QDir dir;
  dir.mkpath(m_logDirectory);
  dir.mkpath(m_configDirectory + "conf.d");
  m_settings = new QSettings(m_configDirectory + Defaults::settingsFileName,
                             QSettings::NativeFormat);
  logFileName = m_logFileName =
      m_settings->value("Logs/logfile", m_logDirectory + Defaults::logFileName)
          .toString();
}

void Settings::readSettings() {
  int logcounter = m_settings->value("Test/counter", "1").toInt();
  m_settings->setValue("Test/counter", ++logcounter);
  m_logLevel = m_settings->value("Logs/loglevel", "Info").toString();
  m_servicePort = m_settings->value("Main/serviceport", "502").toInt();
  m_reconnectPeriodInSec =
      m_settings->value("Client/reconnectperiod", "15").toInt();
  foreachConfFile();
}

void Settings::foreachConfFile() {
  QDir dir(m_configDirectory + "conf.d");

  QStringList sl = dir.entryList(QStringList("*.conf"), QDir::NoFilter,
                                 QDir::SortFlag::Name);
  foreach (QString str, sl) {
    readDevSettings(dir.path() + "/" + str);
  }
}

void Settings::readDevSettings(const QString &confFile) {
  bool ok;
  DeviceStruct device;

  QSettings *settings = new QSettings(confFile, QSettings::NativeFormat);
  device.ip = settings->value("IP", Defaults::ip).toString();
  QStringList sl = device.ip.split(".");
  if (sl.size() < 4) {
    qWarning() << "Device described in file " + confFile +
                      " has wrong IP setting";
    return;
  }
  for (auto it = sl.begin(); it != sl.end(); ++it) {
    (*it).toUInt(&ok);
    if (!ok) {
      qWarning() << "Device described in file " + confFile +
                        " has wrong IP setting";
      return;
    }
  }
  device.startPort = settings->value("Port").toUInt(&ok);
  if (!ok) {
    qWarning() << "Device described in file " + confFile +
                      " has wrong Port setting";
    return;
  }
  for (int i = 1; i < Defaults::maxDevPortsCount; ++i) {
    PortStruct port;
    QString portname = "Port" + QString::number(i);
    port.use =
        (settings->value(portname, "No").toString() == "No") ? false : true;
    port.speed = settings->value(portname + "Speed", "115200").toUInt();
    port.timeout = settings->value(portname + "Timeout", "5").toUInt();
    QString str = settings->value(portname + "Parity", "None").toString();
    if (str == "None")
      port.parity = Parity::NONE;
    else if (str == "Even")
      port.parity = Parity::EVEN;
    else if (str == "Odd")
      port.parity = Parity::ODD;
    else
      port.parity = Parity::ERR;
    port.stopbit = settings->value(portname + "Stop", "1").toUInt();
    device.ports += port;
  }
  m_deviceList += device;
}

void Settings::logSettings() {
  Q_ASSERT(m_settings != nullptr);
  Logger log;
  log.writeRawStart(m_logFileName);
  log.writeLog(log.All, "Reading settings from: " + m_settings->fileName());
  log.writeLog(log.All, "Startup information:");
  log.writeLog(log.All, "=========================");
  log.writeLog(log.All, "LogLevel: " + m_logLevel);
  log.writeLog(log.All, "Service Port: " + QString::number(m_servicePort));
  log.writeLog(log.All, "Client reconnect period: " +
                            QString::number(m_reconnectPeriodInSec));
  log.writeLog(log.All, "=========================");
}

void Settings::writeSettings() {
  Q_ASSERT(m_settings != nullptr);
  m_settings->setValue("Logs/loglevel", m_logLevel);
  m_settings->setValue("Main/serviceport", QString::number(m_servicePort));
  m_settings->setValue("Client/reconnectperiod",
                       QString::number(m_reconnectPeriodInSec));
  m_settings->sync();
}

int Settings::reconnectPeriod() { return m_reconnectPeriodInSec; }

QList<Settings::DeviceStruct> Settings::deviceList() { return m_deviceList; }

int Settings::servicePort() { return m_servicePort; }

QString Settings::loglevel() { return m_logLevel; }
