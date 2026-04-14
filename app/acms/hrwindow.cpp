#include "hrwindow.h"
#include "ui_hrwindow.h"
#include "loginwindow.h"
#include "dbconfig.h"
#include <QMessageBox>
#include <QDate>
#include <QDebug>
#include <QApplication>
#include <QTimer>

HrWindow::HrWindow(QString &login, QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HrWindow)
    , m_login(login), m_password(password)
{
    ui->setupUi(this);
    ui->llogin->setText(login);
    setDefaultDates();

    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Ошибка подключения",
                              "Не удалось подключиться к базе данных.\n"
                              "Проверьте настройки подключения и повторите попытку.\n\n"
                              "Приложение будет закрыто.");
        QTimer::singleShot(0, this, &QWidget::close);
        return;
    }
}

HrWindow::~HrWindow()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "Соединение с БД закрыто";
    }

    QString connectionName = db.connectionName();
    if (!connectionName.isEmpty()) {
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(connectionName);
        qDebug() << "Соединение с БД удалено:" << connectionName;
    }

    delete ui;
    qDebug() << "HrWindow уничтожено";
}

bool HrWindow::connectToDatabase()
{
    DbConfig config;
    config.loadConfig();

    QString connectionName = QString("HR_Connection_%1_%2").arg(m_login);

    qDebug() << "Создание нового соединения с БД:" << connectionName;

    db = QSqlDatabase::addDatabase("QPSQL", connectionName);

    db.setHostName(config.host);
    db.setPort(config.port);
    db.setDatabaseName("acms");
    db.setUserName("acms");
    db.setPassword("CV6Enterprise");

    db.setConnectOptions("connect_timeout=10");

    // Пытаемся открыть соединение
    if (!db.open()) {
        qDebug() << "Ошибка подключения к БД:" << db.lastError().text();
        qDebug() << "Детали ошибки:" << db.lastError().driverText();
        qDebug() << "Код ошибки:" << db.lastError().nativeErrorCode();
        return false;
    }

    qDebug() << "Успешное подключение к БД. Хост:" << config.host
             << "Порт:" << config.port
             << "БД: acms";

    return true;
}

bool HrWindow::reconnectToDatabase()
{
    qDebug() << "Попытка переподключения к БД...";

    // Закрываем текущее соединение если оно открыто
    if (db.isOpen()) {
        db.close();
    }

    // Пытаемся открыть заново
    bool result = db.open();

    if (result) {
        qDebug() << "Переподключение успешно";
        QMessageBox::information(this, "Подключение восстановлено",
                                 "Соединение с базой данных успешно восстановлено.");
    } else {
        qDebug() << "Ошибка переподключения:" << db.lastError().text();
        QMessageBox::critical(this, "Ошибка подключения",
                              "Не удалось восстановить соединение с базой данных.\n"
                              "Попробуйте перезапустить приложение.");
    }

    return result;
}

void HrWindow::on_btnlogout_clicked()
{
    LoginWindow *loginWindow = new LoginWindow(nullptr);
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();

    this->close();
}

void HrWindow::on_btnaddstaff_clicked()
{
    HrAddStaff *addStaffWindow = new HrAddStaff(this, m_login,m_password);
    addStaffWindow->setAttribute(Qt::WA_DeleteOnClose);
    addStaffWindow->show();
}

void HrWindow::on_btneditstaff_clicked()
{
    HrEditStaff *editStaffWindow = new HrEditStaff(this, m_login, m_password);
    editStaffWindow->setAttribute(Qt::WA_DeleteOnClose);
    editStaffWindow->show();
}

void HrWindow::on_btnadddepo_clicked()
{
    HrAddDepo *addDepoWindow = new HrAddDepo(this, m_login,m_password);
    addDepoWindow->setAttribute(Qt::WA_DeleteOnClose);
    addDepoWindow->show();
}

void HrWindow::on_btneditdepo_clicked()
{
    HrEditDepo *editDepoWindow = new HrEditDepo(this, m_login,m_password);
    editDepoWindow->setAttribute(Qt::WA_DeleteOnClose);
    editDepoWindow->show();
}

QString HrWindow::translateAccessType(const QString &type)
{
    if (type == "IN") return "Вход";
    if (type == "OUT") return "Выход";
    return type;
}

QString HrWindow::generatePositionReportHtml(const QString &position)
{
    if (!db.isOpen()) {
        qDebug() << "Ошибка: нет подключения к БД";
        return QString();
    }

    // Получаем данные о сотрудниках с указанной должностью
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT s.fio, s.phone, d.department_name
        FROM staff s
        LEFT JOIN departments d ON s.department_id = d.department_id
        WHERE s.position_name = :position AND s.is_work = true
        ORDER BY s.fio
    )");
    query.bindValue(":position", position);

    if (!query.exec()) {
        qDebug() << "Ошибка запроса сотрудников по должности:" << query.lastError().text();
        return QString();
    }

    // Формируем строки таблицы
    QStringList staffRows;
    int rowNum = 1;

    while (query.next()) {
        QString fio = query.value(0).toString().toHtmlEscaped();
        QString phone = query.value(1).toString().toHtmlEscaped();
        QString department = query.value(2).toString().toHtmlEscaped();

        staffRows << QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td></tr>")
                         .arg(rowNum++)
                         .arg(fio)
                         .arg(phone)
                         .arg(department);
    }

    int totalStaff = rowNum - 1;

    if (totalStaff == 0) {
        staffRows << "<tr><td colspan='4' align='center'><i>Нет работающих сотрудников с данной должностью</i></td></tr>";
    }

    // Текущая дата и время
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeStr = currentDateTime.toString("dd.MM.yyyy HH:mm:ss");
    QString userLogin = m_login.toHtmlEscaped();
    QString positionEscaped = position.toHtmlEscaped();

    // Формируем HTML
    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Отчет по должности</title>
</head>
<body>

<!-- Заголовок -->
<h1 align="center">ОТЧЕТ ПО ДОЛЖНОСТИ</h1>
<hr width="100%" size="2" noshade>

<!-- Информация об отчете -->
<p align="right"><b>Дата и время формирования:</b> %1<br>
<b>Отчет создал:</b> %2</p>

<!-- Информация о фильтре -->
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td width="200"><b>Должность:</b></td>
        <td>%3</td>
    </tr>
    <tr>
        <td><b>Количество сотрудников:</b></td>
        <td>%4</td>
    </tr>
</table>

<br>

<!-- Список сотрудников -->
<h2>Список работающих сотрудников</h2>
<table width="100%" border="1" cellpadding="8" cellspacing="0">
    <thead>
        <tr bgcolor="#CCCCCC">
            <th align="left"><b>№</b></th>
            <th align="left"><b>ФИО сотрудника</b></th>
            <th align="left"><b>Контактный телефон</b></th>
            <th align="left"><b>Отдел</b></th>
        </tr>
    </thead>
    <tbody>
        %5
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
            Руководитель _______________
        </td>
        <td width="50%" align="right">
            "___" ____________ 20___ г.
        </td>
    </tr>
</table>

</body>
</html>
    )";

    html = html.arg(dateTimeStr)
               .arg(userLogin)
               .arg(positionEscaped)
               .arg(totalStaff)
               .arg(staffRows.join("\n"));

    return html;
}

QString HrWindow::generateAccessReportHtml(const QDate &startDate, const QDate &endDate)
{
    if (!db.isOpen()) {
        qDebug() << "Ошибка: нет подключения к БД";
        return QString();
    }

    // Получаем список контроллеров
    QSqlQuery controllersQuery(db);
    controllersQuery.prepare(R"(
        SELECT DISTINCT ac.controller_id, ac.object_name, ac.address
        FROM access_controller ac
        WHERE EXISTS (
            SELECT 1 FROM access_facts af
            WHERE af.controller_id = ac.controller_id
            AND DATE(af.access_time) BETWEEN :start_date AND :end_date
        )
        ORDER BY ac.object_name
    )");
    controllersQuery.bindValue(":start_date", startDate);
    controllersQuery.bindValue(":end_date", endDate);

    if (!controllersQuery.exec()) {
        qDebug() << "Ошибка запроса контроллеров:" << controllersQuery.lastError().text();
        return QString();
    }

    // Собираем данные по каждому контроллеру
    QStringList controllersHtml;
    int totalAccessFacts = 0;

    while (controllersQuery.next()) {
        int controllerId = controllersQuery.value(0).toInt();
        QString objectName = controllersQuery.value(1).toString().toHtmlEscaped();
        QString address = controllersQuery.value(2).toString().toHtmlEscaped();

        // Получаем факты доступа для данного контроллера
        QSqlQuery factsQuery(db);
        factsQuery.prepare(R"(
            SELECT s.fio, af.pass_id, af.access_time, af.access_type
            FROM access_facts af
            JOIN pass p ON af.pass_id = p.pass_id
            JOIN staff s ON p.staff_id = s.staff_id
            WHERE af.controller_id = :controller_id
            AND DATE(af.access_time) BETWEEN :start_date AND :end_date
            ORDER BY af.access_time
        )");
        factsQuery.bindValue(":controller_id", controllerId);
        factsQuery.bindValue(":start_date", startDate);
        factsQuery.bindValue(":end_date", endDate);

        if (!factsQuery.exec()) {
            qDebug() << "Ошибка запроса фактов доступа:" << factsQuery.lastError().text();
            continue;
        }

        // Формируем строки таблицы для контроллера
        QStringList factRows;
        int localCount = 0;
        int rowNum = 1;

        while (factsQuery.next()) {
            QString fio = factsQuery.value(0).toString().toHtmlEscaped();
            QString passId = factsQuery.value(1).toString();
            QDateTime accessTime = factsQuery.value(2).toDateTime();
            QString accessType = translateAccessType(factsQuery.value(3).toString());

            factRows << QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td></tr>")
                            .arg(rowNum++)
                            .arg(fio)
                            .arg(passId)
                            .arg(accessTime.toString("dd.MM.yyyy HH:mm:ss"))
                            .arg(accessType);

            localCount++;
            totalAccessFacts++;
        }

        if (localCount == 0) {
            factRows << "<tr><td colspan='5' align='center'><i>Нет данных о доступах за указанный период</i></td></tr>";
        }

        // Формируем блок для контроллера
        QString controllerBlock = R"(
<!-- Контроллер -->
<h2>%1</h2>
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td width="200"><b>Объект:</b></td>
        <td>%2</td>
    </tr>
    <tr>
        <td><b>Адрес:</b></td>
        <td>%3</td>
    </tr>
    <tr>
        <td><b>Количество проходов:</b></td>
        <td>%4</td>
    </tr>
</table>

<br>

<table width="100%" border="1" cellpadding="8" cellspacing="0">
    <thead>
        <tr bgcolor="#CCCCCC">
            <th align="left"><b>№</b></th>
            <th align="left"><b>ФИО сотрудника</b></th>
            <th align="left"><b>Номер пропуска</b></th>
            <th align="left"><b>Время прохода</b></th>
            <th align="left"><b>Тип прохода</b></th>
        </tr>
    </thead>
    <tbody>
        %5
    </tbody>
</table>

<br>
<br>
        )";

        controllerBlock = controllerBlock.arg(objectName)
                              .arg(objectName)
                              .arg(address)
                              .arg(localCount)
                              .arg(factRows.join("\n"));

        controllersHtml << controllerBlock;
    }

    // Если нет данных ни по одному контроллеру
    if (controllersHtml.isEmpty()) {
        controllersHtml << "<p align='center'><i>Нет данных о доступах за указанный период</i></p>";
    }

    // Текущая дата и время
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeStr = currentDateTime.toString("dd.MM.yyyy HH:mm:ss");
    QString userLogin = m_login.toHtmlEscaped();

    // Формируем итоговый HTML
    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Отчет по доступам</title>
</head>
<body>

<!-- Заголовок -->
<h1 align="center">ОТЧЕТ ПО ДОСТУПАМ</h1>
<hr width="100%" size="2" noshade>

<!-- Информация об отчете -->
<p align="right"><b>Дата и время формирования:</b> %1<br>
<b>Отчет создал:</b> %2</p>

<!-- Параметры отчета -->
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td width="200"><b>Период:</b></td>
        <td>с %3 по %4</td>
    </tr>
    <tr>
        <td><b>Всего проходов:</b></td>
        <td>%5</td>
    </tr>
    <tr>
        <td><b>Количество контроллеров:</b></td>
        <td>%6</td>
    </tr>
</table>

<br>

<!-- Данные по контроллерам -->
%7

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
            Начальник службы безопасности _______________
        </td>
        <td width="50%" align="right">
            "___" ____________ 20___ г.
        </td>
    </tr>
</table>

</body>
</html>
    )";

    // Получаем количество контроллеров
    int controllersCount = 0;
    QSqlQuery countQuery(db);
    countQuery.prepare(R"(
        SELECT COUNT(DISTINCT ac.controller_id)
        FROM access_controller ac
        WHERE EXISTS (
            SELECT 1 FROM access_facts af
            WHERE af.controller_id = ac.controller_id
            AND DATE(af.access_time) BETWEEN :start_date AND :end_date
        )
    )");
    countQuery.bindValue(":start_date", startDate);
    countQuery.bindValue(":end_date", endDate);
    if (countQuery.exec() && countQuery.next()) {
        controllersCount = countQuery.value(0).toInt();
    }

    html = html.arg(dateTimeStr)
               .arg(userLogin)
               .arg(startDate.toString("dd.MM.yyyy"))
               .arg(endDate.toString("dd.MM.yyyy"))
               .arg(totalAccessFacts)
               .arg(controllersCount)
               .arg(controllersHtml.join("\n"));

    return html;
}

void HrWindow::on_btnpositionreport_clicked()
{
    QString position = ui->leposition->text().trimmed();

    if (position.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите должность!");
        return;
    }

    // Проверяем соединение с БД
    if (!db.isOpen()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Нет подключения",
                                                                  "Отсутствует подключение к базе данных.\n"
                                                                  "Попробовать переподключиться?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (!reconnectToDatabase()) {
                return;
            }
        } else {
            return;
        }
    }

    // Генерируем HTML отчет
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString html = generatePositionReportHtml(position);
    QApplication::restoreOverrideCursor();

    if (html.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сформировать отчет");
        return;
    }

    // Открываем окно предварительного просмотра
    PrintWindow *printWindow = new PrintWindow(html);
    printWindow->setAttribute(Qt::WA_DeleteOnClose);
    printWindow->show();
}

void HrWindow::on_btnaccessreport_clicked()
{
    QString startDate = ui->lestart->text();
    QString endDate = ui->leend->text();

    if (!validateDate(startDate, "Дата начала")) return;
    if (!validateDate(endDate, "Дата конца")) return;

    QDate start = QDate::fromString(startDate, "yyyy-MM-dd");
    QDate end = QDate::fromString(endDate, "yyyy-MM-dd");

    if (start > end) {
        QMessageBox::warning(this, "Предупреждение",
                             "Дата начала не может быть позже даты конца!");
        return;
    }

    // Проверяем соединение с БД
    if (!db.isOpen()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Нет подключения",
                                                                  "Отсутствует подключение к базе данных.\n"
                                                                  "Попробовать переподключиться?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (!reconnectToDatabase()) {
                return;
            }
        } else {
            return;
        }
    }

    // Генерируем HTML отчет
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString html = generateAccessReportHtml(start, end);
    QApplication::restoreOverrideCursor();

    if (html.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сформировать отчет");
        return;
    }

    // Открываем окно предварительного просмотра
    PrintWindow *printWindow = new PrintWindow(html);
    printWindow->setAttribute(Qt::WA_DeleteOnClose);
    printWindow->show();
}

void HrWindow::setDefaultDates()
{
    QDate currentDate = QDate::currentDate();
    QDate startDate = currentDate.addDays(-7);

    ui->lestart->setText(startDate.toString("yyyy-MM-dd"));
    ui->leend->setText(currentDate.toString("yyyy-MM-dd"));
}

bool HrWindow::validateDate(const QString &date, const QString &fieldName)
{
    if (date.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение",
                             QString("Поле '%1' не может быть пустым!").arg(fieldName));
        return false;
    }

    QDate parsedDate = QDate::fromString(date, "yyyy-MM-dd");
    if (!parsedDate.isValid()) {
        QMessageBox::warning(this, "Предупреждение",
                             QString("Поле '%1' содержит некорректную дату!\n"
                                     "Используйте формат: ГГГГ-ММ-ДД (например, 2024-12-31)").arg(fieldName));
        return false;
    }

    return true;
}
