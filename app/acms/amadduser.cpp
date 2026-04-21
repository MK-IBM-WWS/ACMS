#include "amadduser.h"
#include "ui_amadduser.h"
#include <QSqlError>

AmAddUser::AmAddUser(AdminWindow *amWindow, QString login, QString password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AmAddUser)
    , m_adminWindow(amWindow)
    , m_currentLogin(login)
    , m_currentPassword(password)
{
    ui->setupUi(this);
    ui->llogin->setText(login);
    setupComboBox();

    ui->lenewpassword->setEchoMode(QLineEdit::Password);
}

AmAddUser::~AmAddUser()
{
    delete ui;
}

void AmAddUser::setupComboBox()
{
    ui->cbusertype->clear();
    ui->cbusertype->addItem("Администратор", "Admin");
    ui->cbusertype->addItem("Контроллер доступа", "Controller");
    ui->cbusertype->addItem("Отдел кадров", "HR");
    ui->cbusertype->addItem("Отдел пропусков", "PD");
}

void AmAddUser::on_btnadd_clicked()
{
    QString newLogin = ui->lenewlogin->text().trimmed();
    QString newPassword = ui->lenewpassword->text().trimmed();
    QString userRoleDb = ui->cbusertype->currentData().toString();

    if (newLogin.isEmpty() || newPassword.isEmpty()) {
        showErrorMessage("Ошибка", "Логин и пароль не могут быть пустыми!");
        return;
    }

    if (newLogin.length() < 3) {
        showErrorMessage("Ошибка", "Логин должен содержать минимум 3 символа!");
        return;
    }

    if (newPassword.length() < 6) {
        showErrorMessage("Ошибка", "Пароль должен содержать минимум 6 символов!");
        return;
    }

    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.isOpen()) {
        if (!m_adminWindow->reconnectToDatabase()) {
            showErrorMessage("Ошибка подключения", "Не удалось подключиться к базе данных!");
            return;
        }
    }

    if (!db.transaction()) {
        showErrorMessage("Ошибка транзакции", "Не удалось начать транзакцию: " + db.lastError().text());
        return;
    }

    if (!addUserToTable(newLogin, newPassword, userRoleDb)) {
        db.rollback();
        return;
    }

    if (!createDatabaseUser(newLogin, newPassword, userRoleDb)) {
        db.rollback();
        return;
    }

    if (!db.commit()) {
        showErrorMessage("Ошибка транзакции", "Не удалось зафиксировать транзакцию: " + db.lastError().text());
        return;
    }

    showSuccessMessage("Успех", "Пользователь успешно добавлен!");

    ui->lenewlogin->clear();
    ui->lenewpassword->clear();
}

bool AmAddUser::addUserToTable(const QString &login, const QString &password, const QString &roleDb)
{
    QSqlDatabase db = m_adminWindow->getDatabase();
    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM users WHERE login = :login");
    query.bindValue(":login", login);

    if (!query.exec()) {
        showErrorMessage("Ошибка БД", "Не удалось проверить уникальность логина: " + query.lastError().text());
        return false;
    }

    query.next();
    if (query.value(0).toInt() > 0) {
        showErrorMessage("Ошибка", "Пользователь с таким логином уже существует!");
        return false;
    }

    query.prepare("INSERT INTO users (login, passphrase, user_role) VALUES (:login, :passphrase, :user_role)");
    query.bindValue(":login", login);
    query.bindValue(":passphrase", password);
    query.bindValue(":user_role", roleDb);

    if (!query.exec()) {
        showErrorMessage("Ошибка БД", "Не удалось добавить пользователя в таблицу users: " + query.lastError().text());
        return false;
    }

    return true;
}

bool AmAddUser::createDatabaseUser(const QString &login, const QString &password, const QString &roleDb)
{
    bool need_admin = true;
    QSqlDatabase db = m_adminWindow->getDatabase();

    {
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT 1 FROM pg_roles WHERE rolname = :login");
        checkQuery.bindValue(":login", login);
        if (!checkQuery.exec()) {
            showErrorMessage("Ошибка проверки", checkQuery.lastError().text());
            return false;
        }
        if (checkQuery.next()) {
            showErrorMessage("Ошибка", "Пользователь уже существует");
            return false;
        }
    }

    {
        QSqlQuery createQuery(db);
        QString createSql;

        if (roleDb == "Admin") {
            createSql = QString("CREATE USER \"%1\" WITH PASSWORD '%2' CREATEROLE CREATEDB").arg(login, password);
        } else {
            createSql = QString("CREATE USER \"%1\" WITH PASSWORD '%2'").arg(login, password);
        }

        if (!createQuery.exec(createSql)) {
            showErrorMessage("Ошибка создания пользователя", createQuery.lastError().text());
            return false;
        }
    }

    {
        QSqlQuery grantQuery(db);
        QString grantSql;

        if (roleDb == "Controller") {
            grantSql = QString("GRANT group_controller TO \"%1\"").arg(login);
        } else if (roleDb == "HR") {
            grantSql = QString("GRANT group_hr TO \"%1\"").arg(login);
        } else if (roleDb == "PD") {
            grantSql = QString("GRANT group_pd TO \"%1\"").arg(login);
        } else if (roleDb == "Admin") {
            grantSql = QString("GRANT group_admin TO \"%1\" WITH ADMIN OPTION").arg(login);
            need_admin = false;
        } else {
            showErrorMessage("Ошибка", "Неизвестная роль");
            return false;
        }

        if (!grantQuery.exec(grantSql)) {
            showErrorMessage("Ошибка назначения прав", grantQuery.lastError().text());
            return false;
        }
    }

    if (need_admin){
        QSqlQuery adminGrantQuery(db);
        QString adminGrantSql = QString("GRANT \"%1\" TO group_admin WITH ADMIN OPTION").arg(login);
        if (!adminGrantQuery.exec(adminGrantSql)) {
            showErrorMessage("Ошибка передачи прав группе администраторов", adminGrantQuery.lastError().text());
            return false;
        }
    }

    return true;
}

void AmAddUser::on_btnlogout_clicked()
{
    if (m_adminWindow && m_adminWindow->isVisible()) {
        m_adminWindow->show();
    } else {
        AdminWindow *adminWindow = new AdminWindow(m_currentLogin, m_currentPassword, nullptr);
        adminWindow->setAttribute(Qt::WA_DeleteOnClose);
        adminWindow->show();

        if (m_adminWindow) {
            m_adminWindow->deleteLater();
        }
    }

    this->close();
}

void AmAddUser::showErrorMessage(const QString &title, const QString &message)
{
    QMessageBox::critical(this, title, message);
}

void AmAddUser::showSuccessMessage(const QString &title, const QString &message)
{
    QMessageBox::information(this, title, message);
}
