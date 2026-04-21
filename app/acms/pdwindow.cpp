#include "pdwindow.h"
#include "ui_pdwindow.h"
#include "loginwindow.h"
#include "dbconfig.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlDriver>

PdWindow::PdWindow(QString &login, QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PdWindow)
    , m_login(login)
    , m_password(password)
    , m_dbConfig(nullptr)
{
    ui->setupUi(this);

    m_dbConfig = new DbConfig();
    m_dbConfig->loadConfig();

    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных");
    }

    ui->llogin->setText(login);

    initDateFields();
}

PdWindow::~PdWindow()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    delete m_dbConfig;
    delete ui;
}

bool PdWindow::connectToDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }

    m_db = QSqlDatabase::addDatabase("QPSQL", "PdWindowConnection");
    m_db.setHostName(m_dbConfig->host);
    m_db.setPort(m_dbConfig->port);
    m_db.setDatabaseName("acms");
    m_db.setUserName(m_login);
    m_db.setPassword(m_password);

    if (!m_db.open()) {
        QMessageBox::critical(this, "Ошибка подключения",
                              "Не удалось подключиться к базе данных:\n" + m_db.lastError().text());
        return false;
    }

    return true;
}

bool PdWindow::reconnectToDatabase()
{
    if (!isDatabaseConnected()) {
        return connectToDatabase();
    }

    if (m_db.driver()->hasFeature(QSqlDriver::QuerySize)) {
        QSqlQuery testQuery(m_db);
        if (!testQuery.exec("SELECT 1")) {
            return connectToDatabase();
        }
    }

    return true;
}

void PdWindow::initDateFields()
{
    QDate today = QDate::currentDate();
    QDate startDate = today.addDays(-30);

    ui->lestart->setText(startDate.toString("yyyy-MM-dd"));
    ui->leend->setText(today.toString("yyyy-MM-dd"));
}

void PdWindow::on_btnlogout_clicked()
{
    this->close();
    LoginWindow *loginWindow = new LoginWindow(nullptr);
    loginWindow->show();
}

void PdWindow::on_btnnewpass_clicked()
{
    if (!reconnectToDatabase()) {
        QMessageBox::critical(this, "Ошибка", "Нет подключения к базе данных");
        return;
    }
    PdAddPass *addPassWindow = new PdAddPass(this, m_login, m_password, nullptr);
    addPassWindow->show();
}

void PdWindow::on_btneditpass_clicked()
{
    if (!reconnectToDatabase()) {
        QMessageBox::critical(this, "Ошибка", "Нет подключения к базе данных");
        return;
    }

    PdEditPass *editPassWindow = new PdEditPass(this, m_login, m_password, nullptr);
    editPassWindow->show();
}

void PdWindow::on_btnaddgroup_clicked()
{
    PdAddGroup *addGroupWindow = new PdAddGroup(this, m_login, m_password, nullptr);
    addGroupWindow->show();
}

void PdWindow::on_btneditgroup_clicked()
{
    if (!reconnectToDatabase()) {
        QMessageBox::critical(this, "Ошибка", "Нет подключения к базе данных");
        return;
    }

    PdEditGroup *editGroupWindow = new PdEditGroup(this, m_login, m_password, nullptr);
    editGroupWindow->show();
}

void PdWindow::on_btnobjectsreport_clicked()
{
    if (!reconnectToDatabase()) {
        QMessageBox::critical(this, "Ошибка", "Нет подключения к базе данных");
        return;
    }

    QString html = generateObjectsReport();
    if (!html.isEmpty()) {
        PrintWindow *printWindow = new PrintWindow(html);
        printWindow->setAttribute(Qt::WA_DeleteOnClose);
        printWindow->show();
    }
}

void PdWindow::on_btnpassreport_clicked()
{
    if (!reconnectToDatabase()) {
        QMessageBox::critical(this, "Ошибка", "Нет подключения к базе данных");
        return;
    }

    QDate startDate = QDate::fromString(ui->lestart->text(), "yyyy-MM-dd");
    QDate endDate = QDate::fromString(ui->leend->text(), "yyyy-MM-dd");

    if (!startDate.isValid() || !endDate.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Некорректный формат даты");
        return;
    }

    if (startDate > endDate) {
        QMessageBox::warning(this, "Ошибка", "Дата начала не может быть больше даты конца");
        return;
    }

    QString html = generatePassReport(startDate, endDate);
    if (!html.isEmpty()) {
        PrintWindow *printWindow = new PrintWindow(html);
        printWindow->setAttribute(Qt::WA_DeleteOnClose);
        printWindow->show();
    }
}

QString PdWindow::generateObjectsReport()
{
    if (!isDatabaseConnected()) {
        QMessageBox::critical(this, "Ошибка", "Нет подключения к базе данных");
        return QString();
    }

    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Отчет об объектах доступа</title>
</head>
<body>

<!-- Заголовок -->
<h1 align="center">ОТЧЕТ ОБ ОБЪЕКТАХ ДОСТУПА</h1>
<hr width="100%" size="2" noshade>

<!-- Информация об отчете -->
<p align="right"><b>Дата и время формирования:</b> %1<br>
<b>Отчет создал:</b> %2</p>
)";

    html = html.arg(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"))
               .arg(m_login);

    QSqlQuery groupQuery(m_db);
    groupQuery.prepare("SELECT group_id, group_name, change_date FROM access_groups ORDER BY group_name");

    if (!groupQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось получить группы доступа: " + groupQuery.lastError().text());
        return QString();
    }

    int groupNumber = 0;

    while (groupQuery.next()) {
        groupNumber++;
        int groupId = groupQuery.value("group_id").toInt();
        QString groupName = groupQuery.value("group_name").toString();
        QDate changeDate = groupQuery.value("change_date").toDate();

        html += QString(R"(
<!-- Информация о группе доступа -->
<h2>Группа доступа №%1</h2>
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td width="200"><b>Название группы:</b></td>
        <td>%2</td>
    </tr>
    <tr>
        <td><b>Дата изменения:</b></td>
        <td>%3</td>
    </tr>
</table>

<br>
)")
                    .arg(groupNumber)
                    .arg(groupName)
                    .arg(changeDate.toString("dd.MM.yyyy"));

        QSqlQuery controllerQuery(m_db);
        controllerQuery.prepare(
            "SELECT ac.object_name, ac.address, ac.work_shedule "
            "FROM access_controller ac "
            "JOIN group_consists gc ON ac.controller_id = gc.controller_id "
            "WHERE gc.group_id = :groupId "
            "ORDER BY ac.object_name"
            );
        controllerQuery.bindValue(":groupId", groupId);

        if (!controllerQuery.exec()) {
            html += "<p>Ошибка получения данных контроллеров</p>";
            continue;
        }

        if (controllerQuery.size() == 0) {
            html += "<p>Нет объектов доступа в этой группе</p><br>";
        } else {
            html += R"(
<!-- Список объектов доступа -->
<h2>Список объектов доступа группы</h2>
<table width="100%" border="1" cellpadding="8" cellspacing="0">
    <thead>
        <tr bgcolor="#CCCCCC">
            <th align="left"><b>№</b></th>
            <th align="left"><b>Имя объекта</b></th>
            <th align="left"><b>Адрес</b></th>
            <th align="left"><b>График работы</b></th>
        </tr>
    </thead>
    <tbody>
)";

            int objectNumber = 0;
            while (controllerQuery.next()) {
                objectNumber++;
                QString objectName = controllerQuery.value("object_name").toString();
                QString address = controllerQuery.value("address").toString();
                QString workSchedule = controllerQuery.value("work_shedule").toString();

                if (address.isEmpty()) address = "-";
                if (workSchedule.isEmpty()) workSchedule = "-";

                html += QString(R"(
        <tr>
            <td>%1</td>
            <td>%2</td>
            <td>%3</td>
            <td>%4</td>
        </tr>
)")
                            .arg(objectNumber)
                            .arg(objectName)
                            .arg(address)
                            .arg(workSchedule);
            }

            html += R"(
    </tbody>
</table>

<br>
)";
        }
    }

    html += R"(
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
    </tr>
</table>

<br>
<br>

<!-- Места для подписей -->
<table width="100%" border="0">
    <tr>
        <td width="50%">
            Начальник отдела пропусков _______________
        </td>
        <td width="50%" align="right">
            "___" ____________ 20___ г.
        </td>
    </tr>
</table>

</body>
</html>
)";

    return html;
}

QString PdWindow::generatePassReport(const QDate &startDate, const QDate &endDate)
{
    if (!isDatabaseConnected()) {
        QMessageBox::critical(this, "Ошибка", "Нет подключения к базе данных");
        return QString();
    }

    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Отчет о выданных пропусках</title>
</head>
<body>

<!-- Заголовок -->
<h1 align="center">ОТЧЕТ О ВЫДАННЫХ ПРОПУСКАХ</h1>
<hr width="100%" size="2" noshade>

<!-- Информация об отчете -->
<p align="right"><b>Дата и время формирования:</b> %1<br>
<b>Отчет создал:</b> %2</p>

<!-- Информация о периоде -->
<h2>Информация о периоде отчета</h2>
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td width="200"><b>Дата начала:</b></td>
        <td>%3</td>
    </tr>
    <tr>
        <td><b>Дата окончания:</b></td>
        <td>%4</td>
    </tr>
</table>

<br>
)";

    html = html.arg(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"))
               .arg(m_login)
               .arg(startDate.toString("dd.MM.yyyy"))
               .arg(endDate.toString("dd.MM.yyyy"));

    QSqlQuery deptQuery(m_db);
    deptQuery.prepare("SELECT department_id, department_name FROM departments ORDER BY department_name");

    if (!deptQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось получить список отделов: " + deptQuery.lastError().text());
        return QString();
    }

    bool hasAnyPass = false;
    int deptNumber = 0;

    while (deptQuery.next()) {
        int deptId = deptQuery.value("department_id").toInt();
        QString deptName = deptQuery.value("department_name").toString();
        QSqlQuery passQuery(m_db);
        passQuery.prepare(
            "SELECT p.pass_id, p.issue_date, p.expiration_date, s.fio "
            "FROM pass p "
            "JOIN staff s ON p.staff_id = s.staff_id "
            "WHERE p.pass_department_id = :deptId "
            "AND p.issue_date BETWEEN :startDate AND :endDate "
            "ORDER BY p.issue_date DESC"
            );
        passQuery.bindValue(":deptId", deptId);
        passQuery.bindValue(":startDate", startDate);
        passQuery.bindValue(":endDate", endDate);

        if (!passQuery.exec()) {
            html += QString("<p>Ошибка получения пропусков для отдела %1</p>").arg(deptName);
            continue;
        }

        if (passQuery.size() > 0) {
            hasAnyPass = true;
            deptNumber++;

            html += QString(R"(
<!-- Информация об отделе -->
<h2>Отдел №%1</h2>
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td width="200"><b>Название отдела:</b></td>
        <td>%2</td>
    </tr>
</table>

<br>

<!-- Список выданных пропусков -->
<h2>Список выданных пропусков отдела</h2>
<table width="100%" border="1" cellpadding="8" cellspacing="0">
    <thead>
        <tr bgcolor="#CCCCCC">
            <th align="left"><b>№</b></th>
            <th align="left"><b>Номер пропуска</b></th>
            <th align="left"><b>Дата выдачи</b></th>
            <th align="left"><b>Дата истечения</b></th>
            <th align="left"><b>ФИО сотрудника</b></th>
        </tr>
    </thead>
    <tbody>
)")
                        .arg(deptNumber)
                        .arg(deptName);

            int passNumber = 0;
            while (passQuery.next()) {
                passNumber++;
                int passId = passQuery.value("pass_id").toInt();
                QDate issueDate = passQuery.value("issue_date").toDate();
                QDate expDate = passQuery.value("expiration_date").toDate();
                QString fio = passQuery.value("fio").toString();

                html += QString(R"(
        <tr>
            <td>%1</td>
            <td>%2</td>
            <td>%3</td>
            <td>%4</td>
            <td>%5</td>
        </tr>
)")
                            .arg(passNumber)
                            .arg(passId)
                            .arg(issueDate.toString("dd.MM.yyyy"))
                            .arg(expDate.toString("dd.MM.yyyy"))
                            .arg(fio);
            }

            html += R"(
    </tbody>
</table>

<br>
)";
        }
    }

    if (!hasAnyPass) {
        html += R"(
<h2>Информация</h2>
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td>За указанный период пропуска не выдавались.</td>
    </tr>
</table>

<br>
)";
    }

    html += R"(
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
    </tr>
</table>

<br>
<br>

<!-- Места для подписей -->
<table width="100%" border="0">
    <tr>
        <td width="50%">
            Начальник отдела пропусков _______________
        </td>
        <td width="50%" align="right">
            "___" ____________ 20___ г.
        </td>
    </tr>
</table>

</body>
</html>
)";

    return html;
}
