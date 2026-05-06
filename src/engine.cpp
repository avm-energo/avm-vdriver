#include <include/device.h>
#include <include/deviceparser.h>
#include <include/engine.h>
#include <include/serviceparser.h>

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

  // создаём
  foreach (auto deviceDescr, Settings::deviceList()) {
    Device *dev = new Device();
    if (!dev->init(deviceDescr)) {
      qCritical() << "Cannot initialize device " << deviceDescr.ip << ":"
                  << deviceDescr.startPort;
    }
  }
}
