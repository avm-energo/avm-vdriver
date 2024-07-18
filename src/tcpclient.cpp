#include <include/vdriver/tcpclient.h>

TCPClient::TCPClient(QObject *parent)
    : QObject{parent}
{
    m_socket = new QTcpSocket(this);
    m_in.setDevice(m_socket);
}

TCPClient::~TCPClient()
{
    m_socket->disconnect();
    delete m_socket;
}

bool TCPClient::init(const QString &ip, int port)
{
    setName(ip + ":" + QString::number(port));
    m_socket->connectToHost(ip, port);
    connect(m_socket, &QAbstractSocket::readyRead, this, &TCPClient::newDataReceived);
    connect(m_socket, &QAbstractSocket::errorOccurred, this, &TCPClient::errorOccured);
    return true;
}

void TCPClient::setName(const QString &name)
{
    m_name = "[RTU][" + name + "] ";
}

void TCPClient::writeData(const QByteArray &ba)
{
    qint64 writtenDataSize = m_socket->write(ba);
    if (writtenDataSize != ba.size()) {
        qWarning() << m_name << "Write data size is not equal to data size actually written";
    }
}

void TCPClient::errorOccured(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qCritical() << m_name << "The host was not found";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qCritical() << m_name << "The connection was refused by the peer";
        break;
    default:
        qCritical() << m_name << "The following error occurred: " << m_socket->errorString();
    }
}

void TCPClient::newDataReceived()
{
    if (m_socket == nullptr) {
        qCritical() << m_name << "Socket is null";
        return;
    }
    m_in.startTransaction();
    QByteArray ba;
    m_in >> ba;
    if (!m_in.commitTransaction())
        return;
    emit newDataReady(ba);
}
