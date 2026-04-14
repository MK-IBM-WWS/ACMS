#ifndef DBCONFIG_H
#define DBCONFIG_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QCoreApplication>

class DbConfig
{
public:
    DbConfig();
    QString host;
    int port;

    void loadConfig();
    bool saveConfig();
    bool configExists();

private:
    static QString getConfigPath();
};

#endif // DBCONFIG_H
