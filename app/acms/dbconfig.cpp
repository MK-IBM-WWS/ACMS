#include "dbconfig.h"

DbConfig::DbConfig()
{
    host = "localhost";
    port = 228;
}

QString DbConfig::getConfigPath()
{
    return QCoreApplication::applicationDirPath() + "/config.json";
}

void DbConfig::loadConfig()
{
    QString configPath = getConfigPath();
    QFile file(configPath);

    if (!file.exists()) {
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        return;
    }

    QJsonObject obj = doc.object();

    this->host = obj["host"].toString("localhost");
    this->port = obj["port"].toInt(228);
}

bool DbConfig::saveConfig()
{
    QString configPath = getConfigPath();

    QJsonObject obj;
    obj["host"] = this->host;
    obj["port"] = this->port;

    QJsonDocument doc(obj);

    QFile file(configPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

bool DbConfig::configExists()
{
    return QFile::exists(getConfigPath());
}
