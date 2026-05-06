#include <QtDebug>
#include <include/tcpserver.h>

TCPServer::TCPServer(int port, QObject *parent)
    : m_port(port), QObject(parent), m_clientConnection(nullptr) {
  m_server = new QTcpServer(this);
}

bool TCPServer::start(QHostAddress address) {
  if (!m_server->listen(address, m_port)) {
    //   if (!m_server->listen(QHostAddress::AnyIPv4, m_port)) {

    qDebug() << m_name
             << "Unable to start the server: " << m_server->errorString();
    return false;
  }
  connect(m_server, &QTcpServer::newConnection, this,
          &TCPServer::clientTryingToConnect);
  return true;
}

void TCPServer::setName(const QString &name) {
  m_name = "[TCP][" + name + "] ";
}

void TCPServer::writeData(const QByteArray &ba) {
  if (m_clientConnection == nullptr) {
    qWarning() << m_name << "No client connected, dropping write";
    return;
  }
  qint64 writtenDataSize = m_clientConnection->write(ba);
  if (writtenDataSize != ba.size()) {
    qWarning() << m_name
               << "Write data size is not equal to data size actually written";
  }
}

void TCPServer::clientTryingToConnect() {
  m_clientConnection = m_server->nextPendingConnection();
  qDebug() << "TCP Client trying to connect: "
           << m_clientConnection->peerAddress().toString() << ":"
           << m_clientConnection->localPort();
  connect(m_clientConnection, &QAbstractSocket::disconnected,
          m_clientConnection, &QObject::deleteLater);
  connect(m_clientConnection, &QObject::destroyed, this,
          [this]() { m_clientConnection = nullptr; });
  connect(m_clientConnection, &QIODevice::readyRead, this,
          &TCPServer::newDataReceived);
}

void TCPServer::newDataReceived() {
  if (m_clientConnection == nullptr) {
    qCritical() << m_name << "Tcp socket is null";
    return;
  }
  QByteArray ba = m_clientConnection->readAll();
  emit newDataReady(ba);
}
