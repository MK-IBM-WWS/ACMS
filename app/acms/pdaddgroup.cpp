#include "pdaddgroup.h"
#include "ui_pdaddgroup.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QCloseEvent>

PdAddGroup::PdAddGroup(PdWindow *pdWindow, const QString &login, const QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PdAddGroup)
    , m_pdWindow(pdWindow)
    , m_login(login)
    , m_password(password)
{
    ui->setupUi(this);
    ui->llogin->setText(m_login);
    ui->lechangedate->setText(QDate::currentDate().toString("yyyy-MM-dd"));
}

PdAddGroup::~PdAddGroup()
{
    delete ui;
}

bool PdAddGroup::validateDate(const QString &date, const QString &fieldName)
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

void PdAddGroup::on_btnadd_clicked()
{
    QString groupName = ui->legroupname->text().trimmed();
    QString changeDateStr = ui->lechangedate->text().trimmed();

    if (groupName.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Название группы доступа не может быть пустым!");
        return;
    }

    if (!validateDate(changeDateStr, "Дата изменения")) {
        return;
    }

    QDate changeDate = QDate::fromString(changeDateStr, "yyyy-MM-dd");

    if (addAccessGroup(groupName, changeDate)) {
        QMessageBox::information(this, "Информация", "Группа доступа успешно добавлена!");

        ui->legroupname->clear();
        ui->lechangedate->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    }
}

void PdAddGroup::on_btnlogout_clicked()
{
    this->close();

    if (!m_pdWindow || !m_pdWindow->isVisible()) {
        PdWindow *newPdWindow = new PdWindow(m_login, m_password);
        newPdWindow->show();
    }
}

bool PdAddGroup::addAccessGroup(const QString &groupName, const QDate &changeDate)
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
        db = m_pdWindow->getDatabase();
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO access_groups (group_name, change_date) VALUES (:group_name, :change_date)");
    query.bindValue(":group_name", groupName);
    query.bindValue(":change_date", changeDate);

    if (!query.exec()) {
        QString errorText = query.lastError().text();

        if (errorText.contains("unique") || errorText.contains("duplicate key", Qt::CaseInsensitive)) {
            showErrorMessage("Группа доступа с таким названием уже существует!");
        } else {
            showErrorMessage("Ошибка при добавлении записи: " + errorText);
        }
        return false;
    }

    return true;
}

void PdAddGroup::showErrorMessage(const QString &message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void PdAddGroup::showInfoMessage(const QString &message)
{
    QMessageBox::information(this, "Информация", message);
}
