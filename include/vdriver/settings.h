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
    };

    struct DeviceStruct
    {
        int startPort;
        QString ip;
        QList<PortStruct> ports;
    };

    Settings();

    void init(QString &logFileName);
    void readSettings();
    void foreachConfFile();
    void readDevSettings(const QString &confFile);
    void logSettings();
    void writeSettings(); // write module properties: serialnum, hwversion, etc

    QString logLevel;
    QString logFilename;
    int servicePort; // ModbusTCP service port to configurate VDriver
    int devCount;
    QList<DeviceStruct> m_deviceList;

private:
    QSettings *m_settings;
};
