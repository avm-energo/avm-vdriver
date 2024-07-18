#pragma once

#include <QObject>

class DeviceParser : public QObject
{
    Q_OBJECT
public:
    explicit DeviceParser(QObject *parent = nullptr);

    void setName(const QString &name);

signals:
    void writeToTCP(const QByteArray &ba);
    void writeToRTU(const QByteArray &ba);

public slots:
    void newDataReceivedFromTCP(QByteArray &ba);
    void newDataReceivedFromRTU(QByteArray &ba);

private:
    QString m_name;
};
