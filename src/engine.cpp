#include <include/vdriver/engine.h>

Engine::Engine(QObject *parent)
    : QObject{parent}
{}

Engine::~Engine()
{
    delete m_server;
}

void Engine::init(Settings &settings)
{
    // создаём сервер
    m_server = new ModbusTCPServer(settings.servicePort);
    // создаём клиенты
    settings.deviceList();
}
