#include "pdeditpass.h"
#include "ui_pdeditpass.h"
#include "pdwindow.h"
#include "printwindow.h"
#include <QInputDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>

PdEditPass::PdEditPass(PdWindow *pdWindow, const QString &login, const QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PdEditPass)
    , m_pdWindow(pdWindow)
    , m_login(login)
    , m_password(password)
    , m_currentPassId(-1)
    , m_currentIndex(-1)
{
    ui->setupUi(this);
    ui->llogin->setText(m_login);
    m_db = m_pdWindow->getDatabase();

    ui->twcontrollers->setColumnCount(4);
    ui->twcontrollers->setHorizontalHeaderLabels({"ID", "Дата выдачи", "Дата истечения", "Группа доступа"});
    ui->twcontrollers->setColumnHidden(0, true);
    ui->twcontrollers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twcontrollers->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_completerModel = new QStringListModel(this);
    m_searchCompleter = new QCompleter(m_completerModel, this);
    m_searchCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lesearch->setCompleter(m_searchCompleter);

    QSqlQuery query(m_db);
    query.exec("SELECT pass_id FROM pass ORDER BY pass_id");
    while (query.next()) {
        m_passIds.append(query.value(0).toInt());
    }

    if (!m_passIds.isEmpty()) {
        m_currentIndex = 0;
        m_currentPassId = m_passIds[0];
        loadPassData(m_currentPassId);
    }

    updateNavigationButtons();
    updateSearchCompleter();
}

PdEditPass::~PdEditPass()
{
    delete ui;
}

void PdEditPass::on_btnlogout_clicked()
{
    if (!m_pdWindow->isVisible()) {
        PdWindow *pdWindow = new PdWindow(m_login, m_password);
        pdWindow->show();
    }
    close();
}

void PdEditPass::on_btnsearch_clicked()
{
    QString searchText = ui->lesearch->text().trimmed();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите данные для поиска");
        return;
    }

    QStringList parts = searchText.split('|');
    if (parts.size() != 2) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат поиска. Используйте: ФИО | номер_пропуска");
        return;
    }

    int passId = parts[1].trimmed().toInt();
    if (passId <= 0) {
        QMessageBox::warning(this, "Ошибка", "Неверный номер пропуска");
        return;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT p.pass_id FROM pass p "
                  "JOIN staff s ON p.staff_id = s.staff_id "
                  "WHERE p.pass_id = :pass_id AND s.fio ILIKE :fio");
    query.bindValue(":pass_id", passId);
    query.bindValue(":fio", "%" + parts[0].trimmed() + "%");

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Ошибка", "Пропуск не найден");
        return;
    }

    int index = m_passIds.indexOf(passId);
    if (index != -1) {
        m_currentIndex = index;
        m_currentPassId = passId;
        loadPassData(passId);
        updateNavigationButtons();
    }
}

void PdEditPass::on_btnforward_clicked()
{
    if (m_currentIndex < m_passIds.size() - 1) {
        m_currentIndex++;
        m_currentPassId = m_passIds[m_currentIndex];
        loadPassData(m_currentPassId);
        updateNavigationButtons();
    }
}

void PdEditPass::on_btnback_clicked()
{
    if (m_currentIndex > 0) {
        m_currentIndex--;
        m_currentPassId = m_passIds[m_currentIndex];
        loadPassData(m_currentPassId);
        updateNavigationButtons();
    }
}

void PdEditPass::on_btnupdate_clicked()
{
    if (m_currentPassId != -1) {
        loadPassData(m_currentPassId);
    }
}

void PdEditPass::on_btnsetend_clicked()
{
    if (m_currentPassId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет открытого пропуска");
        return;
    }

    QString newDate = ui->lenewend->text().trimmed();
    if (!validateDate(newDate)) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат даты. Используйте YYYY-MM-DD");
        return;
    }

    QSqlQuery query(m_db);
    query.prepare("UPDATE pass SET expiration_date = :exp_date WHERE pass_id = :pass_id");
    query.bindValue(":exp_date", newDate);
    query.bindValue(":pass_id", m_currentPassId);

    if (!executeQuery(query)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось обновить дату истечения");
        return;
    }

    QMessageBox::information(this, "Успех", "Дата истечения обновлена");
    loadPassData(m_currentPassId);
    ui->lenewend->clear();
}

void PdEditPass::on_btndelete_clicked()
{
    if (m_currentPassId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет открытого пропуска");
        return;
    }

    int ret = QMessageBox::question(this, "Подтверждение",
                                    "Вы действительно хотите удалить этот пропуск?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query(m_db);

    query.prepare("DELETE FROM purpose WHERE pass_id = :pass_id");
    query.bindValue(":pass_id", m_currentPassId);
    if (!executeQuery(query)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить связанные группы доступа");
        return;
    }

    query.prepare("DELETE FROM pass WHERE pass_id = :pass_id");
    query.bindValue(":pass_id", m_currentPassId);
    if (!executeQuery(query)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить пропуск");
        return;
    }

    QMessageBox::information(this, "Успех", "Пропуск удален");

    m_passIds.removeAt(m_currentIndex);
    query.exec("SELECT pass_id FROM pass ORDER BY pass_id");
    m_passIds.clear();
    while (query.next()) {
        m_passIds.append(query.value(0).toInt());
    }

    if (m_passIds.isEmpty()) {
        m_currentPassId = -1;
        m_currentIndex = -1;
        clearForm();
    } else {
        if (m_currentIndex >= m_passIds.size()) {
            m_currentIndex = m_passIds.size() - 1;
        }
        m_currentPassId = m_passIds[m_currentIndex];
        loadPassData(m_currentPassId);
    }

    updateNavigationButtons();
    updateSearchCompleter();
}

void PdEditPass::on_btnaddgroup_clicked()
{
    if (m_currentPassId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет открытого пропуска");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Выдать группу доступа");
    dialog.setMinimumSize(400, 200);

    QFormLayout form(&dialog);

    QLineEdit *groupNameEdit = new QLineEdit(&dialog);
    groupNameEdit->setPlaceholderText("Введите название группы");

    QCompleter *groupCompleter = new QCompleter(&dialog);
    QStringListModel *groupModel = new QStringListModel(&dialog);
    QSqlQuery query(m_db);
    query.exec("SELECT group_name FROM access_groups ORDER BY group_name");
    QStringList groups;
    while (query.next()) {
        groups.append(query.value(0).toString());
    }
    groupModel->setStringList(groups);
    groupCompleter->setModel(groupModel);
    groupCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    groupNameEdit->setCompleter(groupCompleter);

    QLineEdit *startDateEdit = new QLineEdit(&dialog);
    startDateEdit->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    startDateEdit->setPlaceholderText("YYYY-MM-DD");

    QLineEdit *endDateEdit = new QLineEdit(&dialog);
    endDateEdit->setText(QDate::currentDate().addYears(10).toString("yyyy-MM-dd"));
    endDateEdit->setPlaceholderText("YYYY-MM-DD");

    form.addRow("Группа доступа:", groupNameEdit);
    form.addRow("Дата начала:", startDateEdit);
    form.addRow("Дата окончания:", endDateEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString groupName = groupNameEdit->text().trimmed();
        QString startDate = startDateEdit->text().trimmed();
        QString endDate = endDateEdit->text().trimmed();

        if (groupName.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите название группы доступа");
            return;
        }

        if (!validateDate(startDate) || !validateDate(endDate)) {
            QMessageBox::warning(this, "Ошибка", "Неверный формат даты. Используйте YYYY-MM-DD");
            return;
        }

        QSqlQuery query(m_db);
        query.prepare("SELECT group_id FROM access_groups WHERE group_name = :group_name");
        query.bindValue(":group_name", groupName);

        if (!query.exec() || !query.next()) {
            QMessageBox::warning(this, "Ошибка", "Группа доступа не найдена");
            return;
        }

        int groupId = query.value(0).toInt();

        query.prepare("INSERT INTO purpose (issue_date, expiration_date, pass_id, group_id) "
                      "VALUES (:issue_date, :exp_date, :pass_id, :group_id)");
        query.bindValue(":issue_date", startDate);
        query.bindValue(":exp_date", endDate);
        query.bindValue(":pass_id", m_currentPassId);
        query.bindValue(":group_id", groupId);

        if (!executeQuery(query)) {
            QMessageBox::critical(this, "Ошибка", "Не удалось выдать группу доступа");
            return;
        }

        QMessageBox::information(this, "Успех", "Группа доступа выдана");
        loadAccessGroups(m_currentPassId);
    }
}

void PdEditPass::on_btnremovegroup_clicked()
{
    int currentRow = ui->twcontrollers->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Предупреждение", "Выберите группу доступа для отзыва");
        return;
    }

    int ret = QMessageBox::question(this, "Подтверждение",
                                    "Вы действительно хотите отозвать эту группу доступа?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    int purposeId = ui->twcontrollers->item(currentRow, 0)->text().toInt();

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM purpose WHERE purpose_id = :purpose_id");
    query.bindValue(":purpose_id", purposeId);

    if (!executeQuery(query)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось отозвать группу доступа");
        return;
    }

    QMessageBox::information(this, "Успех", "Группа доступа отозвана");
    loadAccessGroups(m_currentPassId);
}

void PdEditPass::on_btnprintpass_clicked()
{
    if (m_currentPassId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет открытого пропуска");
        return;
    }

    QString html = generatePassHtml();
    PrintWindow *printWindow = new PrintWindow(html);
    printWindow->setAttribute(Qt::WA_DeleteOnClose);
    printWindow->show();
}

void PdEditPass::loadPassData(int passId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT p.pass_id, s.fio, p.issue_date, p.expiration_date, d.department_name "
                  "FROM pass p "
                  "JOIN staff s ON p.staff_id = s.staff_id "
                  "JOIN departments d ON p.pass_department_id = d.department_id "
                  "WHERE p.pass_id = :pass_id");
    query.bindValue(":pass_id", passId);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить данные пропуска");
        return;
    }

    ui->lepassid->setText(query.value(0).toString());
    ui->lefio->setText(query.value(1).toString());
    ui->lestartdate->setText(query.value(2).toDate().toString("yyyy-MM-dd"));
    ui->leenddate->setText(query.value(3).toDate().toString("yyyy-MM-dd"));
    ui->ledepartment->setText(query.value(4).toString());

    loadAccessGroups(passId);
}

void PdEditPass::loadAccessGroups(int passId)
{
    ui->twcontrollers->setRowCount(0);

    QSqlQuery query(m_db);
    query.prepare("SELECT pur.purpose_id, pur.issue_date, pur.expiration_date, ag.group_name "
                  "FROM purpose pur "
                  "JOIN access_groups ag ON pur.group_id = ag.group_id "
                  "WHERE pur.pass_id = :pass_id "
                  "ORDER BY pur.issue_date DESC");
    query.bindValue(":pass_id", passId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить группы доступа");
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->twcontrollers->insertRow(row);
        ui->twcontrollers->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->twcontrollers->setItem(row, 1, new QTableWidgetItem(query.value(1).toDate().toString("yyyy-MM-dd")));
        ui->twcontrollers->setItem(row, 2, new QTableWidgetItem(query.value(2).toDate().toString("yyyy-MM-dd")));
        ui->twcontrollers->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        row++;
    }

    ui->twcontrollers->resizeColumnsToContents();
}

void PdEditPass::updateNavigationButtons()
{
    ui->btnback->setEnabled(m_currentIndex > 0);
    ui->btnforward->setEnabled(m_currentIndex < m_passIds.size() - 1);
}

void PdEditPass::updateSearchCompleter()
{
    QSqlQuery query(m_db);
    query.exec("SELECT s.fio || ' | ' || p.pass_id "
               "FROM pass p "
               "JOIN staff s ON p.staff_id = s.staff_id "
               "ORDER BY s.fio");

    QStringList items;
    while (query.next()) {
        items.append(query.value(0).toString());
    }

    m_completerModel->setStringList(items);
}

bool PdEditPass::validateDate(const QString &dateStr)
{
    QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
    return date.isValid() && dateStr.length() == 10;
}

QString PdEditPass::generatePassHtml()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT p.pass_id, s.fio, d.department_name, p.issue_date, p.expiration_date, "
                  "pd.department_name as pass_dep_name "
                  "FROM pass p "
                  "JOIN staff s ON p.staff_id = s.staff_id "
                  "JOIN departments d ON s.department_id = d.department_id "
                  "JOIN departments pd ON p.pass_department_id = pd.department_id "
                  "WHERE p.pass_id = :pass_id");
    query.bindValue(":pass_id", m_currentPassId);

    if (!query.exec() || !query.next()) {
        return "<html><body><h1><center>Ошибка генерации пропуска</center></h1></body></html>";
    }

    QString passId = query.value(0).toString();
    QString fio = query.value(1).toString();
    QString department = query.value(2).toString();
    QString issueDate = query.value(3).toDate().toString("dd.MM.yyyy");
    QString expDate = query.value(4).toDate().toString("dd.MM.yyyy");
    QString passDepartment = query.value(5).toString();

    QString html = R"(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Пропуск</title>
</head>
<body bgcolor="#E8E8E8" leftmargin="0" topmargin="0" marginwidth="0" marginheight="0">
<table width="100%" height="100%" cellpadding="0" cellspacing="0" border="0">
<tr>
<td align="center" valign="middle">
<table width="450" cellpadding="20" cellspacing="0" border="0" bgcolor="#2C3E50" style="border-radius: 10px; border: 2px solid #1A252F;">
<tr>
<td>
<!-- Заголовок -->
<table width="100%" cellpadding="0" cellspacing="0" border="0">
<tr>
<td align="center">
<font face="Arial, sans-serif" size="6" color="#FFFFFF"><b>ПРОПУСК</b></font>
</td>
</tr>
</table>

<!-- Номер пропуска -->
<table width="100%" cellpadding="10" cellspacing="0" border="0" bgcolor="#2C3E50">
<tr>
<td align="center">
<font face="Arial, sans-serif" size="5" color="#FFFFFF"><b>№ )" + passId + R"(</b></font>
</td>
</tr>
</table>

<table height="15"><tr><td></td></tr></table>

<!-- Информация о владельце -->
<table width="100%" cellpadding="8" cellspacing="0" border="0" bgcolor="#ECF0F1">
<tr>
<td width="40%">
<font face="Arial, sans-serif" size="3" color="#7F8C8D"><b>Владелец:</b></font>
</td>
<td width="60%">
<font face="Arial, sans-serif" size="4" color="#2C3E50"><b>)" + fio + R"(</b></font>
</td>
</tr>
</table>

<table height="5"><tr><td></td></tr></table>

<table width="100%" cellpadding="8" cellspacing="0" border="0" bgcolor="#F8F9FA">
<tr>
<td width="40%">
<font face="Arial, sans-serif" size="3" color="#7F8C8D"><b>Отдел:</b></font>
</td>
<td width="60%">
<font face="Arial, sans-serif" size="4" color="#2C3E50"><b>)" + department + R"(</b></font>
</td>
</tr>
</table>

<table height="15"><tr><td></td></tr></table>

<!-- Даты -->
<table width="100%" cellpadding="8" cellspacing="0" border="0" bgcolor="#ECF0F1">
<tr>
<td width="40%">
<font face="Arial, sans-serif" size="3" color="#7F8C8D"><b>Выдан:</b></font>
</td>
<td width="60%">
<font face="Arial, sans-serif" size="4" color="#2C3E50"><b>)" + issueDate + R"(</b></font>
</td>
</tr>
</table>

<table height="5"><tr><td></td></tr></table>

<table width="100%" cellpadding="8" cellspacing="0" border="0" bgcolor="#F8F9FA">
<tr>
<td width="40%">
<font face="Arial, sans-serif" size="3" color="#7F8C8D"><b>Действителен до:</b></font>
</td>
<td width="60%">
<font face="Arial, sans-serif" size="4" color="#E74C3C"><b>)" + expDate + R"(</b></font>
</td>
</tr>
</table>

<table height="15"><tr><td></td></tr></table>

<!-- Отдел, выдавший пропуск -->
<table width="100%" cellpadding="8" cellspacing="0" border="0" bgcolor="#ECF0F1">
<tr>
<td width="40%">
<font face="Arial, sans-serif" size="3" color="#7F8C8D"><b>Кем выдан:</b></font>
</td>
<td width="60%">
<font face="Arial, sans-serif" size="4" color="#2C3E50"><b>)" + passDepartment + R"(</b></font>
</td>
</tr>
</table>

<table height="20"><tr><td></td></tr></table>

<!-- Подвал -->
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td align="center" bgcolor="#2C3E50" height="1"></td>
</tr>
<tr>
<td height="10"></td>
</tr>
<tr>
<td align="center">
<font face="Arial, sans-serif" size="2" color="#BDC3C7">
Автоматизированная система контроля и управления доступом
</font>
</td>
</tr>
</table>

</td>
</tr>
</table>
</td>
</tr>
</table>
</body>
</html>
)";

    return html;
}

void PdEditPass::clearForm()
{
    ui->lepassid->setText("pass_id");
    ui->lefio->setText("staff_fio");
    ui->lestartdate->setText("start_date");
    ui->leenddate->setText("end_date");
    ui->ledepartment->setText("pass_department");
    ui->twcontrollers->setRowCount(0);
}

bool PdEditPass::executeQuery(QSqlQuery &query)
{
    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return false;
    }
    return true;
}
