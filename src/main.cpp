#include <QCoreApplication>
#include <QTcpSocket>
#include <avm-gen/messagehandler.h>
#include <gitversion/gitversion.h>
#include <include/commandlineparser.h>
#include <include/engine.h>
#include <include/settings.h>
#include <iostream>

int main(int argc, char *argv[]) {
  GitVersion gitVersion;
  QString logFileName;
  CommandLineParser parser;

  QCoreApplication::setApplicationName("VDriver");
  QCoreApplication::setOrganizationName("AVM-Energo");
  QCoreApplication a(argc, argv);

  std::cout << "VDriver version: "
            << gitVersion.getConfigVersion().toStdString() << "-"
            << gitVersion.getGitCounter() << "-"
            << gitVersion.getGitHash().toStdString() << std::endl;

  Settings::init(logFileName);
  Settings::readSettings();
  Settings::writeSettings(); // to set those settings which was not set
                             // (settings.ini is absent or is old enough)
  MessageHandler::setMessageHandlerFilename(logFileName);
  MessageHandler::setLogLevel(Logger::s_logLevelsMap[Settings::loglevel()]);
  qInstallMessageHandler(&MessageHandler::messageHandler);

  Settings::logSettings();

  Engine *engine = new Engine;
  engine->init();

  return a.exec();
}
