#include "hreditstaff.h"
#include "ui_hreditstaff.h"
#include "hrwindow.h"
#include "dbconfig.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDateEdit>
#include <QMessageBox>

HrEditStaff::HrEditStaff(HrWindow *hrWindow, QString &m_login, QString &m_password, int staff_id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HrEditStaff)
    , login(m_login)
    , password(m_password)
    , currentStaffId(-1)
    , mainHrWindow(hrWindow)
    , currentIndex(-1)
    , depCompleter(nullptr)
    , depModel(nullptr)
    , searchCompleter(nullptr)
    , searchModel(nullptr)
{
    ui->setupUi(this);
    ui->llogin->setText(login);
    ui->cbiswork->addItem("Работает", true);
    ui->cbiswork->addItem("Уволен", false);

    if (mainHrWindow) {
        db = mainHrWindow->getDatabase();
    }
    if (!ensureDatabaseConnection()) {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }

    loadDepartments();
    loadStaffList();
    loadSearchSuggestions();

    connect(ui->ledepo, &QLineEdit::textChanged, this, &HrEditStaff::on_ledepo_textChanged);
    connect(ui->lesearch, &QLineEdit::textChanged, this, &HrEditStaff::on_lesearch_textChanged);

    if (staff_id != -1) {
        loadStaffData(staff_id);
        currentIndex = staffIds.indexOf(staff_id);
    } else {
        if (!staffIds.isEmpty()) {
            currentIndex = 0;
            loadStaffData(staffIds[currentIndex]);
        }
    }
}

HrEditStaff::~HrEditStaff()
{
    delete ui;
}

bool HrEditStaff::ensureDatabaseConnection()
{
    if (!db.isOpen()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Нет подключения",
                                                                  "Отсутствует подключение к базе данных.\n"
                                                                  "Попробовать переподключиться?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes && mainHrWindow) {
            return mainHrWindow->reconnectToDatabase();
        }
        return false;
    }
    return true;
}

void HrEditStaff::loadDepartments()
{
    QSqlQuery query(db);
    QStringList departments;

    QString sql = "SELECT department_name, address FROM departments ORDER BY department_name";

    if (query.exec(sql)) {
        while (query.next()) {
            QString depName = query.value(0).toString();
            QString address = query.value(1).toString();
            departments << depName + " | " + address;
        }

        depModel = new QStringListModel(departments, this);
        depCompleter = new QCompleter(depModel, this);
        depCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        depCompleter->setFilterMode(Qt::MatchContains);
        ui->ledepo->setCompleter(depCompleter);
    }
}

void HrEditStaff::loadStaffList()
{
    QSqlQuery query(db);
    QString sql = "SELECT staff_id FROM staff ORDER BY staff_id";

    staffIds.clear();

    if (query.exec(sql)) {
        while (query.next()) {
            staffIds.append(query.value(0).toInt());
        }
    }
}

void HrEditStaff::loadSearchSuggestions()
{
    QSqlQuery query(db);
    QStringList suggestions;

    QString sql = "SELECT fio, phone FROM staff ORDER BY fio";

    if (query.exec(sql)) {
        while (query.next()) {
            QString fio = query.value(0).toString();
            QString phone = query.value(1).toString();
            suggestions << fio + " | " + phone;
        }

        searchModel = new QStringListModel(suggestions, this);
        searchCompleter = new QCompleter(searchModel, this);
        searchCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        searchCompleter->setFilterMode(Qt::MatchContains);
        ui->lesearch->setCompleter(searchCompleter);
    }
}

void HrEditStaff::loadStaffData(int staffId)
{
    QSqlQuery query(db);
    query.prepare("SELECT s.fio, s.phone, s.position_name, s.is_work, "
                  "d.department_name, d.address, s.department_id "
                  "FROM staff s "
                  "LEFT JOIN departments d ON s.department_id = d.department_id "
                  "WHERE s.staff_id = :id");
    query.bindValue(":id", staffId);

    if (query.exec() && query.next()) {
        currentStaffId = staffId;

        ui->lefio->setText(query.value(0).toString());
        ui->lephone->setText(query.value(1).toString());
        ui->leposition->setText(query.value(2).toString());

        bool isWork = query.value(3).toBool();
        ui->cbiswork->setCurrentIndex(isWork ? 0 : 1);

        if (query.value(6).isNull()) {
            ui->ledepo->clear();
        } else {
            QString depString = query.value(4).toString() + " | " + query.value(5).toString();
            ui->ledepo->setText(depString);
        }

        ui->btnback->setEnabled(currentIndex > 0);
        ui->btforward->setEnabled(currentIndex < staffIds.size() - 1);

        ui->ledepo->setStyleSheet("");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить данные сотрудника");
    }
}

int HrEditStaff::getDepartmentId(const QString &depoText)
{
    if (depoText.isEmpty()) {
        return -1;
    }

    QStringList parts = depoText.split(" | ");
    if (parts.size() != 2) {
        return -1;
    }

    QString depName = parts[0].trimmed();
    QString address = parts[1].trimmed();

    QSqlQuery query(db);
    query.prepare("SELECT department_id FROM departments WHERE department_name = :name AND address = :address");
    query.bindValue(":name", depName);
    query.bindValue(":address", address);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return -1;
}

QString HrEditStaff::getDepartmentString(int departmentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT department_name, address FROM departments WHERE department_id = :id");
    query.bindValue(":id", departmentId);

    if (query.exec() && query.next()) {
        return query.value(0).toString() + " | " + query.value(1).toString();
    }

    return QString();
}

void HrEditStaff::navigateToStaff(int direction)
{
    int newIndex = currentIndex + direction;

    if (newIndex >= 0 && newIndex < staffIds.size()) {
        currentIndex = newIndex;
        loadStaffData(staffIds[currentIndex]);
    }
}

void HrEditStaff::on_ledepo_textChanged(const QString &text)
{
    if (!text.isEmpty() && !text.contains(" | ")) {
        ui->ledepo->setStyleSheet("QLineEdit { background-color: #FFFACD; }");
    } else {
        ui->ledepo->setStyleSheet("");
    }
}

void HrEditStaff::on_lesearch_textChanged(const QString &text)
{
    if (!text.isEmpty() && !text.contains(" | ")) {
        ui->lesearch->setStyleSheet("QLineEdit { background-color: #FFFACD; }");
    } else {
        ui->lesearch->setStyleSheet("");
    }
}

void HrEditStaff::on_btnback_clicked()
{
    navigateToStaff(-1);
}

void HrEditStaff::on_btforward_clicked()
{
    navigateToStaff(1);
}

void HrEditStaff::on_btnupdate_clicked()
{
    if (currentStaffId != -1) {
        loadStaffData(currentStaffId);
        QMessageBox::information(this, "Обновление", "Данные обновлены");
    }
}

void HrEditStaff::on_btnsave_clicked()
{
    if (currentStaffId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет загруженных данных для сохранения");
        return;
    }

    if (ui->lefio->text().isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите ФИО сотрудника");
        ui->lefio->setFocus();
        return;
    }

    if (ui->lephone->text().isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите номер телефона");
        ui->lephone->setFocus();
        return;
    }

    int departmentId = -1;
    if (!ui->ledepo->text().isEmpty()) {
        departmentId = getDepartmentId(ui->ledepo->text());
        if (departmentId == -1) {
            QMessageBox::warning(this, "Предупреждение",
                                 "Отдел не найден в базе данных.\n"
                                 "Используйте формат: Название отдела | Адрес\n"
                                 "Выберите отдел из выпадающего списка или оставьте поле пустым");
            return;
        }
    }

    QSqlQuery query(db);
    query.prepare("UPDATE staff SET fio = :fio, phone = :phone, "
                  "position_name = :position, is_work = :is_work, "
                  "department_id = :dep_id "
                  "WHERE staff_id = :id");

    query.bindValue(":fio", ui->lefio->text().trimmed());
    query.bindValue(":phone", ui->lephone->text().trimmed());
    query.bindValue(":position", ui->leposition->text().trimmed());
    query.bindValue(":is_work", ui->cbiswork->currentData().toBool());

    if (departmentId == -1) {
        query.bindValue(":dep_id", QVariant(QVariant::Int));
    } else {
        query.bindValue(":dep_id", departmentId);
    }

    query.bindValue(":id", currentStaffId);

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();

        if (errorMsg.contains("unique") && errorMsg.contains("phone")) {
            QMessageBox::critical(this, "Ошибка",
                                  "Сотрудник с таким номером телефона уже существует");
        } else {
            QMessageBox::critical(this, "Ошибка",
                                  "Не удалось обновить данные:\n" + errorMsg);
        }
        return;
    }

    QMessageBox::information(this, "Успех", "Данные сотрудника обновлены");

    loadSearchSuggestions();
    loadStaffData(currentStaffId);
}

void HrEditStaff::on_btndelete_clicked()
{
    if (currentStaffId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет загруженных данных для удаления");
        return;
    }

    QString fio = ui->lefio->text();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение удаления",
                                                              QString("Вы уверены, что хотите удалить сотрудника '%1'?").arg(fio),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM staff WHERE staff_id = :id");
        query.bindValue(":id", currentStaffId);

        if (query.exec()) {
            QMessageBox::information(this, "Успех", "Сотрудник удален");

            loadStaffList();
            loadSearchSuggestions();

            if (!staffIds.isEmpty()) {
                if (currentIndex < staffIds.size()) {

                    loadStaffData(staffIds[currentIndex]);
                } else if (currentIndex > 0) {

                    currentIndex--;
                    loadStaffData(staffIds[currentIndex]);
                }
            } else {
                currentStaffId = -1;
                currentIndex = -1;
                ui->lefio->clear();
                ui->lephone->clear();
                ui->leposition->clear();
                ui->ledepo->clear();
                ui->cbiswork->setCurrentIndex(0);
                ui->btnback->setEnabled(false);
                ui->btforward->setEnabled(false);
            }
        } else {
            QString errorMsg = query.lastError().text();
            if (errorMsg.contains("foreign key") || errorMsg.contains("constraint")) {
                QMessageBox::critical(this, "Ошибка",
                                      "Невозможно удалить сотрудника, так как существуют связанные записи\n"
                                      "(пропуска, назначения и т.д.)");
            } else {
                QMessageBox::critical(this, "Ошибка",
                                      "Не удалось удалить сотрудника:\n" + errorMsg);
            }
        }
    }
}

void HrEditStaff::on_btnsearch_clicked()
{
    QString searchText = ui->lesearch->text().trimmed();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите данные для поиска");
        return;
    }

    QStringList parts = searchText.split(" | ");
    if (parts.size() != 2) {
        QMessageBox::warning(this, "Предупреждение",
                             "Используйте формат: ФИО | Телефон\n"
                             "Выберите сотрудника из выпадающего списка");
        return;
    }

    QString fio = parts[0].trimmed();
    QString phone = parts[1].trimmed();

    QSqlQuery query(db);
    query.prepare("SELECT staff_id FROM staff WHERE fio = :fio AND phone = :phone");
    query.bindValue(":fio", fio);
    query.bindValue(":phone", phone);

    if (query.exec() && query.next()) {
        int foundId = query.value(0).toInt();
        int foundIndex = staffIds.indexOf(foundId);

        if (foundIndex != -1) {
            currentIndex = foundIndex;
            loadStaffData(foundId);
            ui->lesearch->clear();
            ui->lesearch->setStyleSheet("");
        } else {
            QMessageBox::warning(this, "Ошибка", "Сотрудник найден, но не в списке навигации");
        }
    } else {
        QMessageBox::warning(this, "Не найдено",
                             QString("Сотрудник с ФИО '%1' и телефоном '%2' не найден").arg(fio, phone));
    }
}

void HrEditStaff::on_btnaccessrepo_clicked()
{
    if (currentStaffId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет загруженных данных сотрудника");
        return;
    }

    QDialog *dateDialog = new QDialog(this);
    dateDialog->setWindowTitle("Период отчета по фактам доступа");
    dateDialog->setMinimumSize(350, 150);

    QFormLayout *formLayout = new QFormLayout(dateDialog);

    QDateEdit *startDateEdit = new QDateEdit(QDate::currentDate().addDays(-7));
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("yyyy-MM-dd");

    QDateEdit *endDateEdit = new QDateEdit(QDate::currentDate());
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("yyyy-MM-dd");

    formLayout->addRow("Дата начала:", startDateEdit);
    formLayout->addRow("Дата конца:", endDateEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dateDialog);
    formLayout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, dateDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, dateDialog, &QDialog::reject);

    if (dateDialog->exec() == QDialog::Accepted) {
        QDate startDate = startDateEdit->date();
        QDate endDate = endDateEdit->date();

        if (startDate > endDate) {
            QMessageBox::warning(this, "Предупреждение",
                                 "Дата начала не может быть позже даты окончания периода");
            return;
        }

        if (endDate > QDate::currentDate()) {
            QMessageBox::warning(this, "Предупреждение",
                                 "Дата окончания не может быть позже текущей даты");
            return;
        }

        generateAccessReport(startDate, endDate);
    }

    delete dateDialog;
}

void HrEditStaff::generateAccessReport(const QDate &startDate, const QDate &endDate)
{
    QString fio = ui->lefio->text();

    QDateTime now = QDateTime::currentDateTime();
    QString dateTimeStr = now.toString("dd.MM.yyyy HH:mm:ss");

    QSqlQuery passQuery(db);
    passQuery.prepare("SELECT pass_id FROM pass WHERE staff_id = :staff_id");
    passQuery.bindValue(":staff_id", currentStaffId);

    QList<int> passIds;
    if (passQuery.exec()) {
        while (passQuery.next()) {
            passIds.append(passQuery.value(0).toInt());
        }
    }

    if (passIds.isEmpty()) {
        QMessageBox::information(this, "Информация", "У сотрудника нет пропусков");
        return;
    }

    QStringList passIdList;
    for (int id : passIds) {
        passIdList.append(QString::number(id));
    }
    QString passIdsStr = passIdList.join(", ");

    QSqlQuery controllersQuery(db);
    QString controllersSql = QString(
                                 "SELECT DISTINCT ac.controller_id, ac.object_name, ac.address, ac.work_shedule "
                                 "FROM access_facts af "
                                 "JOIN access_controller ac ON af.controller_id = ac.controller_id "
                                 "WHERE af.pass_id IN (%1) "
                                 "AND af.access_time > '%2'::timestamp "
                                 "AND af.access_time < '%3'::timestamp "
                                 "ORDER BY ac.object_name")
                                 .arg(passIdsStr)
                                 .arg(startDate.toString("yyyy-MM-dd") + " 00:00:00")
                                 .arg(endDate.toString("yyyy-MM-dd") + " 23:59:59");

    QString reportGroups;
    int totalFacts = 0;

    if (controllersQuery.exec(controllersSql)) {
        while (controllersQuery.next()) {
            int controllerId = controllersQuery.value(0).toInt();
            QString objectName = controllersQuery.value(1).toString();
            QString address = controllersQuery.value(2).toString();
            QString workSchedule = controllersQuery.value(3).toString();

            QSqlQuery factsQuery(db);
            factsQuery.prepare(QString(
                                   "SELECT af.access_time, af.access_type, af.solution "
                                   "FROM access_facts af "
                                   "WHERE af.controller_id = :controller_id "
                                   "AND af.pass_id IN (%1) "
                                   "AND af.access_time > :start_time::timestamp "
                                   "AND af.access_time < :end_time::timestamp "
                                   "ORDER BY af.access_time")
                                   .arg(passIdsStr));

            factsQuery.bindValue(":controller_id", controllerId);
            factsQuery.bindValue(":start_time", startDate.toString("yyyy-MM-dd") + " 00:00:00");
            factsQuery.bindValue(":end_time", endDate.toString("yyyy-MM-dd") + " 23:59:59");

            QString factsRows;
            int controllerFacts = 0;

            if (factsQuery.exec()) {
                while (factsQuery.next()) {
                    QDateTime accessTime = factsQuery.value(0).toDateTime();
                    QString accessType = factsQuery.value(1).toString();
                    QString solution = factsQuery.value(2).toString();

                    QString accessTypeRu;
                    if (accessType == "IN") accessTypeRu = "Вход";
                    else if (accessType == "OUT") accessTypeRu = "Выход";
                    else accessTypeRu = accessType;

                    QString solutionRu;
                    if (solution == "Accepted") solutionRu = "Разрешен";
                    else if (solution == "Rejected") solutionRu = "Отклонен";
                    else if (solution == "Pending") solutionRu = "Ожидание";
                    else solutionRu = solution;

                    factsRows += QString(
                                     "<tr>"
                                     "<td align=\"center\">%1</td>"
                                     "<td>%2</td>"
                                     "<td align=\"center\">%3</td>"
                                     "</tr>\n")
                                     .arg(accessTime.toString("dd.MM.yyyy HH:mm:ss"))
                                     .arg(accessTypeRu)
                                     .arg(solutionRu);

                    controllerFacts++;
                    totalFacts++;
                }
            }

            reportGroups += QString(
                                "<br>\n"
                                "<h3>Контроллер: %1</h3>\n"
                                "<table width=\"100%\" border=\"0\" cellpadding=\"5\">\n"
                                "    <tr>\n"
                                "        <td width=\"200\"><b>Объект:</b></td>\n"
                                "        <td>%1</td>\n"
                                "    </tr>\n"
                                "    <tr>\n"
                                "        <td><b>Адрес:</b></td>\n"
                                "        <td>%2</td>\n"
                                "    </tr>\n"
                                "    <tr>\n"
                                "        <td><b>График работы:</b></td>\n"
                                "        <td>%3</td>\n"
                                "    </tr>\n"
                                "    <tr>\n"
                                "        <td><b>Количество фактов:</b></td>\n"
                                "        <td>%4</td>\n"
                                "    </tr>\n"
                                "</table>\n"
                                "<br>\n")
                                .arg(objectName)
                                .arg(address.isEmpty() ? "Не указан" : address)
                                .arg(workSchedule.isEmpty() ? "Не указан" : workSchedule)
                                .arg(controllerFacts);

            if (controllerFacts > 0) {
                reportGroups += QString(
                                    "<table width=\"100%\" border=\"1\" cellpadding=\"8\" cellspacing=\"0\">\n"
                                    "    <thead>\n"
                                    "        <tr bgcolor=\"#CCCCCC\">\n"
                                    "            <th align=\"center\" width=\"200\"><b>Время доступа</b></th>\n"
                                    "            <th align=\"center\" width=\"150\"><b>Тип доступа</b></th>\n"
                                    "            <th align=\"center\" width=\"150\"><b>Решение</b></th>\n"
                                    "        </tr>\n"
                                    "    </thead>\n"
                                    "    <tbody>\n"
                                    "        %1\n"
                                    "    </tbody>\n"
                                    "</table>\n"
                                    "<br>\n")
                                    .arg(factsRows);
            } else {
                reportGroups += "<p>Нет фактов доступа за указанный период</p>\n";
            }
        }
    }

    QString html = QString(R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Отчет по фактам доступа сотрудника</title>
</head>
<body>

<!-- Заголовок -->
<h1 align="center">ОТЧЕТ ПО ФАКТАМ ДОСТУПА СОТРУДНИКА</h1>
<hr width="100%" size="2" noshade>

<!-- Информация об отчете -->
<p align="right"><b>Дата и время формирования:</b> %1<br>
<b>Отчет создал:</b> %2</p>

<!-- Информация о сотруднике и фильтре -->
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td width="200"><b>ФИО сотрудника:</b></td>
        <td>%3</td>
    </tr>
    <tr>
        <td><b>Период отчета:</b></td>
        <td>с %4 по %5</td>
    </tr>
    <tr>
        <td><b>Общее количество фактов:</b></td>
        <td>%6</td>
    </tr>
</table>

<br>

<!-- Группы по контроллерам -->
%7

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
    )")
                       .arg(dateTimeStr)
                       .arg(login)
                       .arg(fio)
                       .arg(startDate.toString("dd.MM.yyyy"))
                       .arg(endDate.toString("dd.MM.yyyy"))
                       .arg(totalFacts)
                       .arg(reportGroups.isEmpty() ? "<p>Нет данных за указанный период</p>" : reportGroups);

    PrintWindow *printWindow = new PrintWindow(html);
    printWindow->setAttribute(Qt::WA_DeleteOnClose);
    printWindow->show();
}

void HrEditStaff::on_btnlogout_clicked()
{
    QWidget *existingHrWindow = nullptr;
    for (QWidget *widget : QApplication::topLevelWidgets()) {
        if (widget->objectName() == "HrWindow" || widget->inherits("HrWindow")) {
            existingHrWindow = widget;
            break;
        }
    }

    if (!existingHrWindow) {
        HrWindow *hrWindow = new HrWindow(login, password);
        hrWindow->setAttribute(Qt::WA_DeleteOnClose);
        hrWindow->show();
    } else {
        existingHrWindow->activateWindow();
        existingHrWindow->raise();
    }

    this->close();
}
