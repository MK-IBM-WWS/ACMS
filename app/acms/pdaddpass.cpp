#include "pdaddpass.h"
#include "ui_pdaddpass.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QStringListModel>
#include <QCompleter>

PdAddPass::PdAddPass(PdWindow *pdWindow, const QString &login, const QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PdAddPass)
    , m_pdWindow(pdWindow)
    , m_login(login)
    , m_password(password)
{
    ui->setupUi(this);
    ui->llogin->setText(m_login);
    ui->lestartdate->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    ui->leenddate->setText(QDate::currentDate().addYears(1).toString("yyyy-MM-dd"));

    setupStaffCompleter();
    setupDepartmentCompleter();
}

PdAddPass::~PdAddPass()
{
    delete ui;
}

bool PdAddPass::validateDate(const QString &date, const QString &fieldName)
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

bool PdAddPass::ensureDatabaseConnection()
{
    if (!m_pdWindow) {
        showErrorMessage("Ошибка: отсутствует связь с главным окном!");
        return false;
    }

    QSqlDatabase db = m_pdWindow->getDatabase();

    if (!db.isOpen()) {
        if (!m_pdWindow->reconnectToDatabase()) {
            showErrorMessage("Не удалось подключиться к базе данных!");
            return false;
        }
    }

    return true;
}

void PdAddPass::setupStaffCompleter()
{
    if (!ensureDatabaseConnection()) return;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);
    query.exec("SELECT staff_id, fio, phone FROM staff ORDER BY fio");

    QStringList staffList;
    m_staffMap.clear();

    while (query.next()) {
        int staffId = query.value("staff_id").toInt();
        QString fio = query.value("fio").toString();
        QString phone = query.value("phone").toString();
        QString displayText = fio + " | " + phone;

        staffList << displayText;
        m_staffMap[displayText] = staffId;
    }

    QCompleter *completer = new QCompleter(staffList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->lefio->setCompleter(completer);
}

void PdAddPass::setupDepartmentCompleter()
{
    if (!ensureDatabaseConnection()) return;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);
    query.exec("SELECT department_id, department_name, address FROM departments ORDER BY department_name");

    QStringList departmentList;
    m_departmentMap.clear();

    while (query.next()) {
        int departmentId = query.value("department_id").toInt();
        QString departmentName = query.value("department_name").toString();
        QString address = query.value("address").toString();
        QString displayText = departmentName + " | " + address;

        departmentList << displayText;
        m_departmentMap[displayText] = departmentId;
    }

    QCompleter *completer = new QCompleter(departmentList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->lepassdeponame->setCompleter(completer);
}

bool PdAddPass::addPass(int staffId, const QDate &issueDate, const QDate &expirationDate, int departmentId)
{
    if (!ensureDatabaseConnection()) return false;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);

    query.prepare("INSERT INTO pass (issue_date, expiration_date, staff_id, pass_department_id) "
                  "VALUES (:issue_date, :expiration_date, :staff_id, :pass_department_id)");
    query.bindValue(":issue_date", issueDate);
    query.bindValue(":expiration_date", expirationDate);
    query.bindValue(":staff_id", staffId);
    query.bindValue(":pass_department_id", departmentId);

    if (!query.exec()) {
        showErrorMessage("Ошибка при выдаче пропуска: " + query.lastError().text());
        return false;
    }

    return true;
}

void PdAddPass::clearFields()
{
    ui->lefio->clear();
    ui->lestartdate->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    ui->leenddate->setText(QDate::currentDate().addYears(1).toString("yyyy-MM-dd"));
    ui->lepassdeponame->clear();
}

void PdAddPass::on_btnadd_clicked()
{
    QString staffText = ui->lefio->text().trimmed();
    QString issueDateStr = ui->lestartdate->text().trimmed();
    QString expirationDateStr = ui->leenddate->text().trimmed();
    QString departmentText = ui->lepassdeponame->text().trimmed();

    if (staffText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите сотрудника!");
        return;
    }

    if (departmentText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите отдел, выдавший пропуск!");
        return;
    }

    if (!validateDate(issueDateStr, "Дата выдачи")) {
        return;
    }

    if (!validateDate(expirationDateStr, "Дата истечения")) {
        return;
    }

    QDate issueDate = QDate::fromString(issueDateStr, "yyyy-MM-dd");
    QDate expirationDate = QDate::fromString(expirationDateStr, "yyyy-MM-dd");

    if (expirationDate <= issueDate) {
        QMessageBox::warning(this, "Предупреждение",
                             "Дата истечения должна быть позже даты выдачи!");
        return;
    }

    if (!m_staffMap.contains(staffText)) {
        QMessageBox::warning(this, "Предупреждение",
                             "Сотрудник не найден в базе данных!\n"
                             "Проверьте правильность ввода или выберите из списка.");
        return;
    }

    if (!m_departmentMap.contains(departmentText)) {
        QMessageBox::warning(this, "Предупреждение",
                             "Отдел не найден в базе данных!\n"
                             "Проверьте правильность ввода или выберите из списка.");
        return;
    }

    int staffId = m_staffMap[staffText];
    int departmentId = m_departmentMap[departmentText];

    if (!ensureDatabaseConnection()) return;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT pass_id FROM pass WHERE staff_id = :staff_id AND expiration_date >= CURRENT_DATE");
    checkQuery.bindValue(":staff_id", staffId);

    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Предупреждение",
                                                                  "У этого сотрудника уже есть действующий пропуск.\n"
                                                                  "Всё равно выдать новый пропуск?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    if (addPass(staffId, issueDate, expirationDate, departmentId)) {
        showInfoMessage("Пропуск успешно выдан!");
        clearFields();
        setupStaffCompleter();
        setupDepartmentCompleter();
    }
}

void PdAddPass::on_btnlogout_clicked()
{
    this->close();

    if (!m_pdWindow || !m_pdWindow->isVisible()) {
        PdWindow *newPdWindow = new PdWindow(m_login, m_password);
        newPdWindow->show();
    }
}

void PdAddPass::showErrorMessage(const QString &message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void PdAddPass::showInfoMessage(const QString &message)
{
    QMessageBox::information(this, "Информация", message);
}
