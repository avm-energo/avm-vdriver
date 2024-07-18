#pragma once

#include <QList>
#include <QMap>
#include <QSettings>

class Settings
{
public:
    enum Parity { NONE, EVEN, ODD };

    struct PortStruct
    {
        bool use;
        int speed;
        Parity parity;
        int stopbit;
        int timeout;
    };

    struct DeviceStruct
    {
        int startPort;
        QString ip;
        QList<PortStruct> ports;
    };

    explicit Settings();

    void init(QString &logFileName);
    void readSettings();
    void foreachConfFile();
    void readDevSettings(const QString &confFile);
    void logSettings();
    void writeSettings(); // write module properties: serialnum, hwversion, etc
    QList<DeviceStruct> deviceList();

    QString logLevel;
    int servicePort; // ModbusTCP service port to configurate VDriver
    int devCount;

private:
    QList<DeviceStruct> m_deviceList;
    QSettings *m_settings;
    QString m_logDirectory, m_configDirectory;
};
