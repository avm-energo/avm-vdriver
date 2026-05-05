#include <include/vdriver/device.h>
#include <include/vdriver/deviceparser.h>
#include <include/vdriver/deviceserviceparser.h>

Device::Device(QObject *parent) : QObject{parent} {}

Device::~Device() {
  foreach (auto server, m_serverList) {
    server->disconnect();
    delete server;
  }
  foreach (auto client, m_clientList) {
    client->disconnectSocket();
    delete client;
  }
}

bool Device::init(Settings::DeviceStruct &device) {
  int currentPort = device.startPort;
  int startRTUPort = 10001;

  const QString nameserv = device.ip + " service";

  TCPClient *clientserv = new TCPClient;
  clientserv->setName(nameserv);

  DeviceServiceParser *dparser = new DeviceServiceParser(clientserv, device, this);
  connect(clientserv, &TCPClient::newDataReady, dparser,
          &DeviceServiceParser::newDataReceived);
  connect(dparser, &DeviceServiceParser::writeToService, clientserv,
          &TCPClient::writeData);

  clientserv->init(device.ip, 10000);
  dparser->start();

  foreach (auto port, device.ports) {
    if (port.use) {
      const QString name = device.ip + QString::number(currentPort);
      TCPServer *server = new TCPServer(currentPort);
      server->setName(name);
      TCPClient *client = new TCPClient;
      client->setName(name);
      DeviceParser *parser = new DeviceParser(client, this);
      parser->setName(name);
      connect(server, &TCPServer::newDataReady, parser,
              &DeviceParser::newDataReceivedFromTCP);
      connect(parser, &DeviceParser::writeToTCP, server, &TCPServer::writeData);
      server->start();
      m_serverList.append(server);
      connect(client, &TCPClient::newDataReady, parser,
              &DeviceParser::newDataReceivedFromRTU);
      connect(parser, &DeviceParser::writeToRTU, client, &TCPClient::writeData);
      client->init(device.ip, startRTUPort);
      m_clientList.append(client);
    }
    ++startRTUPort;
    ++currentPort;
  }
  return true;
}
