#include "hreditdepo.h"
#include "ui_hreditdepo.h"
#include "hrwindow.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDebug>

HrEditDepo::HrEditDepo(HrWindow *hrWindow, QString m_login, QString m_password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HrEditDepo)
    , login(m_login)
    , password(m_password)
    , mainHrWindow(hrWindow)
    , currentDepartmentIndex(-1)
    , currentDepartmentId(-1)
    , searchCompleter(nullptr)
    , completerModel(nullptr)
{
    ui->setupUi(this);

    // Отображаем логин
    ui->llogin->setText(login);

    // Настройка таблицы сотрудников
    ui->twstaff->setColumnCount(5);
    QStringList headers = {"ID", "ФИО", "Телефон", "Должность", "Статус", "Отдел"};
    ui->twstaff->setHorizontalHeaderLabels(headers);
    ui->twstaff->setColumnHidden(0, true); // Скрываем staff_id
    ui->twstaff->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twstaff->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twstaff->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twstaff->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if (mainHrWindow) {
        db = mainHrWindow->getDatabase();
    }
    if (!ensureDatabaseConnection()) {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }

    // Загружаем список отделов
    loadDepartmentsList();

    // Настраиваем автодополнение для поиска
    setupSearchCompleter();

    // Загружаем первый отдел
    if (!departmentIds.isEmpty()) {
        currentDepartmentIndex = 0;
        loadDepartmentData(departmentIds[currentDepartmentIndex]);
    }
}

HrEditDepo::~HrEditDepo()
{
    delete ui;
}

bool HrEditDepo::ensureDatabaseConnection()
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

void HrEditDepo::loadDepartmentsList()
{
    departmentIds.clear();
    searchSuggestions.clear();

    QSqlQuery query(db);
    query.prepare("SELECT department_id, department_name, address FROM departments ORDER BY department_id");

    if (query.exec()) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            QString address = query.value(2).toString();

            departmentIds.append(id);
            searchSuggestions.append(name + " | " + address);
        }
    } else {
        qDebug() << "Error loading departments list:" << query.lastError().text();
    }
}

bool HrEditDepo::loadDepartmentData(int departmentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT department_id, department_name, address, chief_fio, activity_type "
                  "FROM departments WHERE department_id = :id");
    query.bindValue(":id", departmentId);

    if (!query.exec() || !query.next()) {
        qDebug() << "Error loading department data:" << query.lastError().text();
        return false;
    }

    currentDepartmentId = query.value(0).toInt();
    ui->ledeponame->setText(query.value(1).toString());
    ui->leaddress->setText(query.value(2).toString());
    ui->lefio->setText(query.value(3).toString());
    ui->lework->setText(query.value(4).toString());

    // Загружаем сотрудников отдела
    loadStaffData(currentDepartmentId);

    return true;
}

void HrEditDepo::loadStaffData(int departmentId)
{
    ui->twstaff->setRowCount(0);

    QSqlQuery query(db);
    query.prepare("SELECT s.staff_id, s.fio, s.phone, s.position_name, s.is_work, "
                  "d.department_name "
                  "FROM staff s "
                  "LEFT JOIN departments d ON s.department_id = d.department_id "
                  "WHERE s.department_id = :dept_id OR (s.department_id IS NULL) "
                  "ORDER BY s.fio");
    query.bindValue(":dept_id", departmentId);

    if (!query.exec()) {
        qDebug() << "Error loading staff data:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->twstaff->insertRow(row);

        // staff_id (скрытый)
        ui->twstaff->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));

        // ФИО
        ui->twstaff->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));

        // Телефон
        ui->twstaff->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));

        // Должность
        ui->twstaff->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));

        // Статус
        QString status = query.value(4).toBool() ? "Работает" : "Уволен";
        ui->twstaff->setItem(row, 4, new QTableWidgetItem(status));

        // Отдел (название вместо ID)
        ui->twstaff->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));

        row++;
    }
}

void HrEditDepo::updateDepartmentFields()
{
    // Метод для обновления UI после изменений
    if (currentDepartmentId != -1) {
        loadDepartmentData(currentDepartmentId);
    }
}

void HrEditDepo::clearDepartmentFields()
{
    ui->ledeponame->clear();
    ui->leaddress->clear();
    ui->lefio->clear();
    ui->lework->clear();
    ui->twstaff->setRowCount(0);
}

void HrEditDepo::setupSearchCompleter()
{
    completerModel = new QStringListModel(searchSuggestions, this);
    searchCompleter = new QCompleter(completerModel, this);
    searchCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    searchCompleter->setCompletionMode(QCompleter::PopupCompletion);
    ui->lesearch->setCompleter(searchCompleter);
}

bool HrEditDepo::moveToDepartment(int index)
{
    if (index >= 0 && index < departmentIds.size()) {
        currentDepartmentIndex = index;
        return loadDepartmentData(departmentIds[index]);
    }
    return false;
}

bool HrEditDepo::saveDepartmentChanges()
{
    if (currentDepartmentId == -1) {
        QMessageBox::warning(this, "Ошибка", "Нет выбранного отдела для сохранения");
        return false;
    }

    QString name = ui->ledeponame->text().trimmed();
    QString address = ui->leaddress->text().trimmed();
    QString fio = ui->lefio->text().trimmed();
    QString activity = ui->lework->text().trimmed();

    if (name.isEmpty() || address.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Название и адрес отдела не могут быть пустыми");
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE departments SET "
                  "department_name = :name, "
                  "address = :address, "
                  "chief_fio = :fio, "
                  "activity_type = :activity "
                  "WHERE department_id = :id");

    query.bindValue(":name", name);
    query.bindValue(":address", address);
    query.bindValue(":fio", fio);
    query.bindValue(":activity", activity);
    query.bindValue(":id", currentDepartmentId);

    if (!query.exec()) {
        QString errorText = query.lastError().text();

        if (query.lastError().nativeErrorCode() == "23505") {
            QMessageBox::warning(this, "Дубликат отдела",
                                 QString("Отдел с названием '%1' и адресом '%2' уже существует в базе данных.")
                                     .arg(name, address));
        } else {
            QMessageBox::critical(this, "Ошибка сохранения",
                                  "Не удалось сохранить изменения:\n" + errorText);
        }
        return false;
    }

    // Обновляем список отделов
    loadDepartmentsList();
    setupSearchCompleter();

    QMessageBox::information(this, "Успех", "Изменения сохранены");
    return true;
}

bool HrEditDepo::deleteDepartment()
{
    if (currentDepartmentId == -1) {
        QMessageBox::warning(this, "Ошибка", "Нет выбранного отдела для удаления");
        return false;
    }

    QString deptName = ui->ledeponame->text();

    // Запрос подтверждения
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение удаления",
                                                              QString("Вы действительно хотите удалить отдел '%1'?\n"
                                                                      "Все сотрудники этого отдела останутся в базе данных, но будут откреплены от отдела.")
                                                                  .arg(deptName),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return false;
    }

    // Начинаем транзакцию
    if (!db.transaction()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось начать транзакцию");
        return false;
    }

    // Обновляем сотрудников - устанавливаем department_id в NULL
    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE staff SET department_id = NULL WHERE department_id = :id");
    updateQuery.bindValue(":id", currentDepartmentId);

    if (!updateQuery.exec()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось обновить сотрудников:\n" + updateQuery.lastError().text());
        return false;
    }

    // Удаляем отдел
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM departments WHERE department_id = :id");
    deleteQuery.bindValue(":id", currentDepartmentId);

    if (!deleteQuery.exec()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось удалить отдел:\n" + deleteQuery.lastError().text());
        return false;
    }

    // Подтверждаем транзакцию
    if (!db.commit()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подтвердить транзакцию");
        return false;
    }

    // Обновляем список отделов
    loadDepartmentsList();
    setupSearchCompleter();

    QMessageBox::information(this, "Успех", "Отдел успешно удален");

    // Переходим к следующему отделу или очищаем поля
    if (!departmentIds.isEmpty()) {
        if (currentDepartmentIndex >= departmentIds.size()) {
            currentDepartmentIndex = departmentIds.size() - 1;
        }
        if (currentDepartmentIndex >= 0) {
            loadDepartmentData(departmentIds[currentDepartmentIndex]);
        }
    } else {
        currentDepartmentIndex = -1;
        currentDepartmentId = -1;
        clearDepartmentFields();
    }

    return true;
}

// Слоты
void HrEditDepo::on_btnlogout_clicked()
{
    QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
    HrWindow *existingHrWindow = nullptr;

    for (QWidget *widget : topLevelWidgets) {
        existingHrWindow = qobject_cast<HrWindow*>(widget);
        if (existingHrWindow) {
            break;
        }
    }

    if (!existingHrWindow) {
        existingHrWindow = new HrWindow(login, password);
        existingHrWindow->show();
    } else {
        existingHrWindow->raise();
        existingHrWindow->activateWindow();
    }

    this->close();
}

void HrEditDepo::on_btnback_clicked()
{
    if (currentDepartmentIndex > 0) {
        moveToDepartment(currentDepartmentIndex - 1);
    }
}

void HrEditDepo::on_btnforward_clicked()
{
    if (currentDepartmentIndex < departmentIds.size() - 1) {
        moveToDepartment(currentDepartmentIndex + 1);
    }
}

void HrEditDepo::on_btnupdate_clicked()
{
    if (currentDepartmentId != -1) {
        loadDepartmentData(currentDepartmentId);
    }
}

void HrEditDepo::on_btnsearch_clicked()
{
    QString searchText = ui->lesearch->text().trimmed();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите название отдела и адрес для поиска");
        return;
    }

    int foundIndex = searchSuggestions.indexOf(searchText);

    if (foundIndex != -1) {
        moveToDepartment(foundIndex);
    } else {
        QMessageBox::warning(this, "Не найдено",
                             QString("Отдел '%1' не найден в базе данных").arg(searchText));
    }
}

void HrEditDepo::on_btnsave_clicked()
{
    saveDepartmentChanges();
}

void HrEditDepo::on_btndelete_clicked()
{
    deleteDepartment();
}

void HrEditDepo::on_btnabout_clicked()
{
    int currentRow = ui->twstaff->currentRow();

    if (currentRow == -1) {
        QMessageBox::warning(this, "Предупреждение", "Выберите сотрудника для просмотра информации");
        return;
    }

    QString staffId = ui->twstaff->item(currentRow, 0)->text();

    HrEditStaff *editStaffWindow = new HrEditStaff(mainHrWindow, login, password, staffId.toInt());
    editStaffWindow->setAttribute(Qt::WA_DeleteOnClose);
    editStaffWindow->show();
}

QString HrEditDepo::generateDepartmentReportHtml()
{
    // Проверяем, что есть данные для отчета
    if (currentDepartmentId == -1) {
        return QString();
    }

    // Собираем данные о сотрудниках для HTML таблицы
    QStringList staffRows;
    int workingCount = 0;
    int firedCount = 0;

    for (int row = 0; row < ui->twstaff->rowCount(); ++row) {
        QString fio = ui->twstaff->item(row, 1)->text();
        QString phone = ui->twstaff->item(row, 2)->text();
        QString position = ui->twstaff->item(row, 3)->text();
        QString status = ui->twstaff->item(row, 4)->text();

        // Считаем статистику
        if (status == "Работает") {
            workingCount++;
        } else {
            firedCount++;
        }

        // Экранируем специальные символы HTML
        fio = fio.toHtmlEscaped();
        phone = phone.toHtmlEscaped();
        position = position.toHtmlEscaped();
        status = status.toHtmlEscaped();

        staffRows << QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td></tr>")
                         .arg(fio, phone, position, status);
    }

    // Получаем текущую дату и время
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeStr = currentDateTime.toString("dd.MM.yyyy HH:mm:ss");

    // Экранируем данные отдела
    QString deptName = ui->ledeponame->text().toHtmlEscaped();
    QString address = ui->leaddress->text().toHtmlEscaped();
    QString chiefFio = ui->lefio->text().toHtmlEscaped();
    QString activity = ui->lework->text().toHtmlEscaped();
    QString userLogin = mainHrWindow ? mainHrWindow->getLogin().toHtmlEscaped() : "Неизвестно";

    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Отчет по отделу</title>
</head>
<body>

<!-- Заголовок -->
<h1 align="center">ОТЧЕТ ПО ОТДЕЛУ ОРГАНИЗАЦИИ</h1>
<hr width="100%" size="2" noshade>

<!-- Информация об отчете -->
<p align="right"><b>Дата и время формирования:</b> %1<br>
<b>Отчет создал:</b> %2</p>

<!-- Информация об отделе -->
<h2>Информация об отделе</h2>
<table width="100%" border="0" cellpadding="5">
    <tr>
        <td width="200"><b>Название отдела:</b></td>
        <td>%3</td>
    </tr>
    <tr>
        <td><b>Адрес:</b></td>
        <td>%4</td>
    </tr>
    <tr>
        <td><b>Начальник отдела:</b></td>
        <td>%5</td>
    </tr>
    <tr>
        <td><b>Род деятельности:</b></td>
        <td>%6</td>
    </tr>
</table>

<br>

<!-- Статистика -->
<h2>Статистика по отделу</h2>
<table width="100%" border="1" cellpadding="10">
    <tr>
        <td align="center" width="33%">
            <font size="5"><b>%7</b></font><br>
            <font size="3">Всего сотрудников</font>
        </td>
        <td align="center" width="33%">
            <font size="5"><b>%8</b></font><br>
            <font size="3">Работают</font>
        </td>
        <td align="center" width="33%">
            <font size="5"><b>%9</b></font><br>
            <font size="3">Уволены</font>
        </td>
    </tr>
</table>

<br>

<!-- Список сотрудников -->
<h2>Список сотрудников отдела</h2>
<table width="100%" border="1" cellpadding="8" cellspacing="0">
    <thead>
        <tr bgcolor="#CCCCCC">
            <th align="left"><b>№</b></th>
            <th align="left"><b>ФИО сотрудника</b></th>
            <th align="left"><b>Контактный телефон</b></th>
            <th align="left"><b>Должность</b></th>
            <th align="left"><b>Статус</b></th>
        </tr>
    </thead>
    <tbody>
        %10
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
    </tr>
</table>

<br>
<br>

<!-- Места для подписей -->
<table width="100%" border="0">
    <tr>
        <td width="50%">
            Начальник отдела _______________ %11
        </td>
        <td width="50%" align="right">
            "___" ____________ 20___ г.
        </td>
    </tr>
</table>

</body>
</html>
    )";

    // Создаем строки таблицы с нумерацией
    QString numberedStaffRows;
    for (int i = 0; i < staffRows.size(); ++i) {
        QString row = staffRows[i];
        row.replace("<tr><td>", QString("<tr><td>%1</td><td>").arg(i + 1));
        numberedStaffRows += row + "\n";
    }

    // Если нет сотрудников, вставляем сообщение
    if (ui->twstaff->rowCount() == 0) {
        numberedStaffRows = "<tr><td colspan='5' align='center'><i>Нет сотрудников в отделе</i></td></tr>";
    }

    // Заполняем шаблон данными
    html = html.arg(dateTimeStr)                         // %1
               .arg(userLogin)                           // %2
               .arg(deptName)                            // %3
               .arg(address)                             // %4
               .arg(chiefFio)                            // %5
               .arg(activity)                            // %6
               .arg(ui->twstaff->rowCount())             // %7
               .arg(workingCount)                        // %8
               .arg(firedCount)                          // %9
               .arg(numberedStaffRows)                   // %10
               .arg(chiefFio.split(" ").value(0, ""));   // %11 (фамилия начальника)

    return html;
}

void HrEditDepo::on_btnsavereport_clicked()
{
    if (currentDepartmentId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет открытого отдела для создания отчета");
        return;
    }

    // Генерируем HTML для отчета
    QString html = generateDepartmentReportHtml();

    if (html.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сформировать отчет");
        return;
    }

    // Открываем окно предварительного просмотра
    PrintWindow *printWindow = new PrintWindow(html);
    printWindow->setAttribute(Qt::WA_DeleteOnClose);
    printWindow->show();

    qDebug() << "Открыто окно предварительного просмотра отчета";
}
