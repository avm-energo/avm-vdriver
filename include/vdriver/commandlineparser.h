#pragma once

#include <include/vdriver/settings.h>

#include <QObject>

class CommandLineParser : public QObject
{
    Q_OBJECT
public:
    CommandLineParser(QObject *parent = nullptr);

    bool parseCommandLine();

private:

};
