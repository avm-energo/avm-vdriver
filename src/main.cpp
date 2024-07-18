#include <QCoreApplication>
#include <QTcpSocket>
#include <gen/logger.h>
#include <gen/stdfunc.h>
#include <gitversion/gitversion.h>
#include <include/vdriver/commandlineparser.h>
#include <include/vdriver/engine.h>
#include <include/vdriver/settings.h>
#include <iostream>

int main(int argc, char *argv[])
{
    GitVersion gitVersion;
    Settings settings;
    QString logFileName;
    CommandLineParser parser;

    QCoreApplication::setApplicationName("VDriver");
    QCoreApplication::setOrganizationName("AVM-Energo");
    QCoreApplication a(argc, argv);

    std::cout << "VDriver version: " << gitVersion.getConfigVersion().toStdString() << "-"
              << gitVersion.getGitCounter() << "-" << gitVersion.getGitHash().toStdString()
              << std::endl;

    settings.init(logFileName);
    settings.readSettings();
    settings
        .writeSettings(); // to set those settings which was not set (settings.ini is absent or is old enough)
    Logger::setLogLevel(settings.logLevel);
    Logger::writeStart(logFileName);
    qInstallMessageHandler(Logger::messageHandler);
    settings.logSettings();

    Engine *engine = new Engine;
    engine->init(settings);

    return a.exec();
}
