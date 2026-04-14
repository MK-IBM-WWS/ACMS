#include "hraddstaff.h"
#include "ui_hraddstaff.h"
#include "hrwindow.h"
#include "dbconfig.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

HrAddStaff::HrAddStaff(HrWindow *hrWindow, QString &m_login, QString &m_password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HrAddStaff)
    , login(m_login)
    , password(m_password)
    , depCompleter(nullptr)
    , depModel(nullptr)
    , mainHrWindow(hrWindow)
{
    ui->setupUi(this);

    // Отображаем логин
    ui->llogin->setText(login);

    // Настраиваем ComboBox для статуса работы
    ui->cbiswork->addItem("Работает", true);
    ui->cbiswork->addItem("Уволен", false);

    if (mainHrWindow) {
        db = mainHrWindow->getDatabase();
    }
    if (!ensureDatabaseConnection()) {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }

    // Загружаем список отделов для автодополнения
    loadDepartments();

    // Подключаем сигнал для обработки изменений в поле отдела
    connect(ui->ledepo, &QLineEdit::textChanged, this, &HrAddStaff::on_ledepo_textChanged);
}

HrAddStaff::~HrAddStaff()
{
    delete ui;
}

bool HrAddStaff::ensureDatabaseConnection()
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

void HrAddStaff::loadDepartments()
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
    } else {
        qDebug() << "Error loading departments:" << query.lastError().text();
    }
}

int HrAddStaff::getDepartmentId(const QString &depoText)
{
    if (depoText.isEmpty()) {
        return -1;
    }

    // Разделяем строку на название отдела и адрес
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

void HrAddStaff::on_ledepo_textChanged(const QString &text)
{
    // При изменении текста проверяем, соответствует ли он формату
    // Это просто для визуального контроля, ID будет получен при добавлении
    if (!text.isEmpty() && !text.contains(" | ")) {
        ui->ledepo->setStyleSheet("QLineEdit { background-color: #FFFACD; }");
    } else {
        ui->ledepo->setStyleSheet("");
    }
}

void HrAddStaff::on_btnadd_clicked()
{
    // Проверяем заполнение полей
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

    if (ui->ledepo->text().isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите отдел");
        ui->ledepo->setFocus();
        return;
    }

    // Получаем ID отдела
    int departmentId = getDepartmentId(ui->ledepo->text());
    if (departmentId == -1) {
        QMessageBox::warning(this, "Предупреждение",
                             "Отдел не найден в базе данных.\n"
                             "Используйте формат: Название отдела | Адрес\n"
                             "Выберите отдел из выпадающего списка");
        return;
    }

    // Подготавливаем запрос на добавление
    QSqlQuery query(db);
    query.prepare("INSERT INTO staff (fio, phone, position_name, is_work, department_id) "
                  "VALUES (:fio, :phone, :position, :is_work, :dep_id)");

    query.bindValue(":fio", ui->lefio->text().trimmed());
    query.bindValue(":phone", ui->lephone->text().trimmed());
    query.bindValue(":position", ui->leposition->text().trimmed());
    query.bindValue(":is_work", ui->cbiswork->currentData().toBool());
    query.bindValue(":dep_id", departmentId);

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();

        // Проверяем на уникальность телефона
        if (errorMsg.contains("unique") && errorMsg.contains("phone")) {
            QMessageBox::critical(this, "Ошибка",
                                  "Сотрудник с таким номером телефона уже существует в базе данных");
        } else {
            QMessageBox::critical(this, "Ошибка",
                                  "Не удалось добавить сотрудника:\n" + errorMsg);
        }
        return;
    }

    QMessageBox::information(this, "Успех", "Сотрудник успешно добавлен в базу данных");

    // Очищаем поля после успешного добавления
    clearFields();
}

void HrAddStaff::clearFields()
{
    ui->lefio->clear();
    ui->lephone->clear();
    ui->leposition->clear();
    ui->ledepo->clear();
    ui->cbiswork->setCurrentIndex(0);
    ui->lefio->setFocus();
    ui->ledepo->setStyleSheet("");
}

void HrAddStaff::on_btnlogout_clicked()
{
    // Проверяем, не открыто ли уже окно HrWindow
    QWidget *existingHrWindow = nullptr;
    for (QWidget *widget : QApplication::topLevelWidgets()) {
        if (widget->objectName() == "HrWindow" || widget->inherits("HrWindow")) {
            existingHrWindow = widget;
            break;
        }
    }

    if (!existingHrWindow) {
        // Создаем новое окно HrWindow
        HrWindow *hrWindow = new HrWindow(login, password);
        hrWindow->setAttribute(Qt::WA_DeleteOnClose);
        hrWindow->show();
    } else {
        // Если окно уже открыто, просто активируем его
        existingHrWindow->activateWindow();
        existingHrWindow->raise();
    }

    // Закрываем текущее окно
    this->close();
}
