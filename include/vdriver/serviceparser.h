#pragma once

#include <QObject>

class ServiceParser : public QObject
{
    Q_OBJECT
public:
    explicit ServiceParser(QObject *parent = nullptr);

signals:
    void writeToSocket(const QByteArray &ba);

public slots:
    void newDataReceivedFromSocket(QByteArray &ba);
};
