#include <include/vdriver/device.h>
#include <include/vdriver/deviceparser.h>
#include <include/vdriver/engine.h>
#include <include/vdriver/serviceparser.h>

Engine::Engine(QObject *parent) : QObject{parent} {}

Engine::~Engine() { delete m_server; }

void Engine::init() {
  ServiceParser *parser = new ServiceParser;
  // создаём сервер
  m_server = new TCPServer(Settings::servicePort());
  connect(m_server, &TCPServer::newDataReady, parser,
          &ServiceParser::newDataReceivedFromSocket);
  connect(parser, &ServiceParser::writeToSocket, m_server,
          &TCPServer::writeData);
  m_server->start();

  // DeviceParser *dparser = new DeviceParser();

  // ServiceParser *service = new ServiceParser(client);
  /*
    foreach (auto deviceDescr, Settings::deviceList()) {
      m_client = new TCPClient;
      connect(m_client, &TCPClient::newDataReady, parser,
              &ServiceParser::newDataServiceReceived);
      connect(parser, &ServiceParser::writeToService, m_client,
              &TCPClient::writeData);
      m_client->init(deviceDescr.ip, 10000);
      //    parser->ClientList += m_client;
      parser->ClientList.append(m_client);
      parser->nunclientdocket++;

      if (!parser->InitCom(m_client, deviceDescr)) {
        qCritical() << "Cannot initialize comports " << deviceDescr.ip;
      }
    }
  */
  // создаём
  foreach (auto deviceDescr, Settings::deviceList()) {
    Device *dev = new Device();
    if (!dev->init(deviceDescr)) {
      qCritical() << "Cannot initialize device " << deviceDescr.ip << ":"
                  << deviceDescr.startPort;
    }
  }
}
