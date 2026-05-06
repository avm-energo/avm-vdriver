#pragma once

#include <include/settings.h>

#include <QObject>

class CommandLineParser : public QObject
{
    Q_OBJECT
public:
    CommandLineParser(QObject *parent = nullptr);

    bool parseCommandLine();

private:

};
