#ifndef SERVICEPARSER_H
#define SERVICEPARSER_H

#include <QObject>

class ServiceParser : public QObject
{
    Q_OBJECT
public:
    explicit ServiceParser(QObject *parent = nullptr);

signals:
};

#endif // SERVICEPARSER_H
