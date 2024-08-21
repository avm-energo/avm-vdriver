#pragma once

#include <QObject>
#include "settings.h"
#include "tcpclient.h"

class DeviceServiceParser : public QObject
{
    Q_OBJECT
public:
    explicit DeviceServiceParser(QObject *parent = nullptr);

signals:

private:
    Settings::DeviceStruct m_deviceSettings;
    TCPClient *m_client;
};

