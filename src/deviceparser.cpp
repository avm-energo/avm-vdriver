#include <include/vdriver/deviceparser.h>

DeviceParser::DeviceParser(QObject *parent)
    : QObject{parent}
{}

void DeviceParser::setName(const QString &name)
{
    m_name = "[Parser][" + name + "]";
}

void DeviceParser::newDataReceivedFromTCP(QByteArray &ba)
{
    // the code of modbus TCP parser would be here
    emit writeToRTU(ba);
}

void DeviceParser::newDataReceivedFromRTU(QByteArray &ba)
{
    // the code of modbus RTU parser would be here
    emit writeToTCP(ba);
}
