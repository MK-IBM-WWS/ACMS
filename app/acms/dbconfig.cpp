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
        qDebug() << "Config file not found, using default values";
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open config file, using default values";
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qDebug() << "Invalid JSON format, using default values";
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
        qDebug() << "Cannot save config file";
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
