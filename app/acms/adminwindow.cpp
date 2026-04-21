#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "loginwindow.h"
#include "dbconfig.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QApplication>
#include <QTimer>

AdminWindow::AdminWindow(QString &login, QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminWindow)
    , m_login(login)
    , m_password(password)
{
    ui->setupUi(this);
    ui->llogin->setText(m_login);
    setDefaultDate();

    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Ошибка подключения",
                              "Не удалось подключиться к базе данных.\n"
                              "Проверьте настройки подключения и повторите попытку.\n\n"
                              "Приложение будет закрыто.");
        QTimer::singleShot(0, this, &QWidget::close);
        return;
    }
}

AdminWindow::~AdminWindow()
{
    if (db.isOpen()) {
        db.close();
    }

    QString connectionName = db.connectionName();
    if (!connectionName.isEmpty()) {
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(connectionName);
    }

    delete ui;
}

bool AdminWindow::connectToDatabase()
{
    DbConfig config;
    config.loadConfig();

    QString connectionName = QString("Admin_Connection_%1").arg(m_login);

    db = QSqlDatabase::addDatabase("QPSQL", connectionName);

    db.setHostName(config.host);
    db.setPort(config.port);
    db.setDatabaseName("acms");
    db.setUserName(m_login);
    db.setPassword(m_password);

    db.setConnectOptions("connect_timeout=10");

    if (!db.open()) {
        return false;
    }

    return true;
}

bool AdminWindow::reconnectToDatabase()
{
    if (db.isOpen()) {
        db.close();
    }

    bool result = db.open();

    if (result) {
        QMessageBox::information(this, "Подключение восстановлено",
                                 "Соединение с базой данных успешно восстановлено.");
    } else {
        QMessageBox::critical(this, "Ошибка подключения",
                              "Не удалось восстановить соединение с базой данных.\n"
                              "Попробуйте перезапустить приложение.");
    }

    return result;
}

void AdminWindow::setDefaultDate()
{
    QDate defaultDate = QDate::currentDate().addYears(-2);
    ui->leenddate->setText(defaultDate.toString("yyyy-MM-dd"));
}

void AdminWindow::on_btnlogout_clicked()
{
    LoginWindow *loginWindow = new LoginWindow(nullptr);
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();

    this->close();
}

void AdminWindow::on_btnaddcontrol_clicked()
{
    AmAddControl *addControl = new AmAddControl(this, m_login, m_password, nullptr);
    addControl->setAttribute(Qt::WA_DeleteOnClose);
    addControl->show();
}

void AdminWindow::on_btneditcontrol_clicked()
{
    AmEditControl *editControl = new AmEditControl(this, m_login, m_password, nullptr);
    editControl->setAttribute(Qt::WA_DeleteOnClose);
    editControl->show();
}

void AdminWindow::on_btnadduser_clicked()
{
    AmAddUser *addUser = new AmAddUser(this, m_login, m_password, nullptr);
    addUser->setAttribute(Qt::WA_DeleteOnClose);
    addUser->show();
}

void AdminWindow::on_btnedituser_clicked()
{
    AmEditUser *editUser = new AmEditUser(this, m_login, m_password, nullptr);
    editUser->setAttribute(Qt::WA_DeleteOnClose);
    editUser->show();
}

QString AdminWindow::generateControllersReport()
{
    if (!db.isOpen()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Нет подключения",
                                                                  "Отсутствует подключение к базе данных.\n"
                                                                  "Попробовать переподключиться?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (!reconnectToDatabase()) {
                return QString();
            }
        } else {
            return QString();
        }
    }

    QSqlQuery query(db);
    QStringList rows;
    int rowNum = 1;

    query.prepare("SELECT object_name, address, work_shedule, network_address, model, status "
                  "FROM access_controller ORDER BY object_name");

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось получить данные о контроллерах:\n" + query.lastError().text());
        return QString();
    }

    while (query.next()) {
        QString objectName = query.value(0).toString();
        QString address = query.value(1).toString();
        QString workShedule = query.value(2).toString();
        QString networkAddress = query.value(3).toString();
        QString model = query.value(4).toString();
        QString status = query.value(5).toString();
        QString statusColor;
        if (status == "Online") {
            statusColor = "green";
        } else if (status == "Offline") {
            statusColor = "red";
        } else {
            statusColor = "orange";
        }

        QString row = QString("<tr>"
                              "<td align=\"center\">%1</td>"
                              "<td>%2</td>"
                              "<td>%3</td>"
                              "<td>%4</td>"
                              "<td>%5</td>"
                              "<td>%6</td>"
                              "<td><font color=\"%7\"><b>%8</b></font></td>"
                              "</tr>")
                          .arg(rowNum++)
                          .arg(objectName.isEmpty() ? "—" : objectName.toHtmlEscaped())
                          .arg(address.isEmpty() ? "—" : address.toHtmlEscaped())
                          .arg(workShedule.isEmpty() ? "—" : workShedule.toHtmlEscaped())
                          .arg(networkAddress.isEmpty() ? "—" : networkAddress.toHtmlEscaped())
                          .arg(model.isEmpty() ? "—" : model.toHtmlEscaped())
                          .arg(statusColor)
                          .arg(status.toHtmlEscaped());

        rows.append(row);
    }

    if (rows.isEmpty()) {
        rows.append("<tr><td colspan=\"7\" align=\"center\">Нет данных о контроллерах</td></tr>");
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeStr = currentDateTime.toString("dd.MM.yyyy HH:mm:ss");
    QString userLogin = m_login.toHtmlEscaped();
    int totalControllers = rowNum - 1;

    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Отчет о состоянии контроллеров доступа</title>
    <style>
        body { font-family: Carlito, Arial, sans-serif; }
        table { border-collapse: collapse; }
        th { background-color: #CCCCCC; }
    </style>
</head>
<body>

<h1 align="center">ОТЧЕТ О СОСТОЯНИИ КОНТРОЛЛЕРОВ ДОСТУПА</h1>
<hr width="100%" size="2" noshade>

<!-- Информация об отчете -->
<p align="right"><b>Дата и время формирования:</b> %1<br>
<b>Отчет создал:</b> %2</p>

<!-- Общая информация -->
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td width="200"><b>Всего контроллеров:</b></td>
        <td>%3</td>
    </tr>
</table>

<br>

<!-- Таблица контроллеров -->
<table width="100%" border="1" cellpadding="8" cellspacing="0">
    <thead>
        <tr bgcolor="#CCCCCC">
            <th align="center"><b>№</b></th>
            <th align="left"><b>Объект</b></th>
            <th align="left"><b>Адрес</b></th>
            <th align="left"><b>График работы</b></th>
            <th align="left"><b>Сетевой адрес</b></th>
            <th align="left"><b>Модель</b></th>
            <th align="left"><b>Статус</b></th>
        </tr>
    </thead>
    <tbody>
        %4
    </tbody>
</table>

<br>
<br>

<!-- Подвал -->
<hr width="100%" size="1">
<table width="100%" border="0">
    <tr>
        <td width="50%">
            <b>АИС СКУД</b><br>
            <font size="2">Автоматизированная информационная система<br>
            контроля и управления доступом</font>
        </td>
        <td width="50%" align="right">
            <font size="2">Документ сгенерирован автоматически<br>
            Электронная подпись не требуется</font>
        </td>
    </tr>
</table>

<br>
<br>

<!-- Места для подписей -->
<table width="100%" border="0">
    <tr>
        <td width="50%">
            Администратор системы _______________
        </td>
        <td width="50%" align="right">
            "___" ____________ 20___ г.
        </td>
    </tr>
</table>

</body>
</html>
    )";

    return html.arg(dateTimeStr)
        .arg(userLogin)
        .arg(totalControllers)
        .arg(rows.join("\n"));
}

void AdminWindow::on_btncontrolrepo_clicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString html = generateControllersReport();
    QApplication::restoreOverrideCursor();

    if (html.isEmpty()) {
        return;
    }

    PrintWindow *printWindow = new PrintWindow(html);
    printWindow->setAttribute(Qt::WA_DeleteOnClose);
    printWindow->show();
}

bool AdminWindow::deleteOldAccessFacts(const QDate &endDate)
{
    if (!db.isOpen()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Нет подключения",
                                                                  "Отсутствует подключение к базе данных.\n"
                                                                  "Попробовать переподключиться?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (!reconnectToDatabase()) {
                return false;
            }
        } else {
            return false;
        }
    }

    QSqlQuery query(db);

    query.prepare("DELETE FROM access_facts WHERE access_time::date < :end_date");
    query.bindValue(":end_date", endDate);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось удалить старые записи о доступах:\n" + query.lastError().text());
        return false;
    }

    return true;
}

void AdminWindow::on_btndelete_clicked()
{
    QString dateStr = ui->leenddate->text();
    QDate endDate = QDate::fromString(dateStr, "yyyy-MM-dd");

    if (!endDate.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Некорректный формат даты. Используйте YYYY-MM-DD");
        return;
    }

    if (endDate > QDate::currentDate()) {
        QMessageBox::warning(this, "Ошибка", "Дата не может быть в будущем");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Подтверждение удаления",
                                                              QString("Вы уверены, что хотите удалить все записи о доступах ранее %1?\n"
                                                                      "Это действие нельзя отменить.")
                                                                  .arg(endDate.toString("dd.MM.yyyy")),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (!db.transaction()) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "Ошибка", "Не удалось начать транзакцию");
        return;
    }

    if (deleteOldAccessFacts(endDate)) {
        if (!db.commit()) {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(this, "Ошибка", "Не удалось зафиксировать транзакцию");
            db.rollback();
            return;
        }

        QApplication::restoreOverrideCursor();
        QMessageBox::information(this, "Успех",
                                 QString("Старые записи о доступах (ранее %1) успешно удалены")
                                     .arg(endDate.toString("dd.MM.yyyy")));
    } else {
        QApplication::restoreOverrideCursor();
        db.rollback();
    }
}
