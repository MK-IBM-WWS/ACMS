#include "amedituser.h"
#include "ui_amedituser.h"

AmEditUser::AmEditUser(AdminWindow *amWindow, const QString &login, const QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AmEditUser)
    , m_adminWindow(amWindow)
    , m_currentLogin(login)
    , m_currentPassword(password)
    , m_currentIndex(-1)
    , m_passwordVisible(false)
{
    ui->setupUi(this);
    ui->llogin->setText(login);
    ui->lenewpassword->setEchoMode(QLineEdit::Password);

    setupCompleter();
    loadUserList();

    if (!m_userIds.isEmpty()) {
        m_currentIndex = 0;
        loadUserData(m_userIds[m_currentIndex]);
    }

    updateNavigationButtons();
}

AmEditUser::~AmEditUser()
{
    delete ui;
}

void AmEditUser::setupCompleter()
{
    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.isOpen()) {
        m_adminWindow->reconnectToDatabase();
    }

    QSqlQuery query(db);
    QStringList logins;

    if (query.exec("SELECT DISTINCT login FROM users ORDER BY login")) {
        while (query.next()) {
            logins << query.value(0).toString();
        }
    }

    QCompleter *completer = new QCompleter(logins, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->lesearch->setCompleter(completer);
}

void AmEditUser::loadUserList()
{
    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.isOpen()) {
        if (!m_adminWindow->reconnectToDatabase()) {
            showErrorMessage("Ошибка подключения", "Не удалось подключиться к базе данных!");
            return;
        }
    }

    QSqlQuery query(db);
    m_userIds.clear();

    if (query.exec("SELECT user_id FROM users ORDER BY user_id")) {
        while (query.next()) {
            m_userIds.append(query.value(0).toInt());
        }
    } else {
        showErrorMessage("Ошибка БД", "Не удалось загрузить список пользователей: " + query.lastError().text());
    }
}

void AmEditUser::loadUserData(int userId)
{
    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.isOpen()) {
        if (!m_adminWindow->reconnectToDatabase()) {
            showErrorMessage("Ошибка подключения", "Не удалось подключиться к базе данных!");
            return;
        }
    }

    QSqlQuery query(db);
    query.prepare("SELECT login, passphrase, user_role FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (query.exec() && query.next()) {
        QString login = query.value("login").toString();
        QString password = query.value("passphrase").toString();
        QString role = query.value("user_role").toString();

        ui->lenewlogin->setText(login);
        ui->lenewpassword->setText(password);
        ui->cbusertype->setText(getRoleDisplayName(role));

        updateNavigationButtons();
    } else {
        showErrorMessage("Ошибка БД", "Не удалось загрузить данные пользователя: " + query.lastError().text());
    }
}

QString AmEditUser::getRoleDisplayName(const QString &roleDb)
{
    if (roleDb == "Admin") return "Администратор";
    if (roleDb == "Controller") return "Контроллер доступа";
    if (roleDb == "HR") return "Отдел кадров";
    if (roleDb == "PD") return "Отдел пропусков";
    return roleDb;
}

void AmEditUser::updateNavigationButtons()
{
    ui->btnback->setEnabled(m_currentIndex > 0);
    ui->btforward->setEnabled(m_currentIndex < m_userIds.size() - 1);
}

void AmEditUser::on_btnback_clicked()
{
    if (m_currentIndex > 0) {
        m_currentIndex--;
        loadUserData(m_userIds[m_currentIndex]);
    }
}

void AmEditUser::on_btforward_clicked()
{
    if (m_currentIndex < m_userIds.size() - 1) {
        m_currentIndex++;
        loadUserData(m_userIds[m_currentIndex]);
    }
}

void AmEditUser::on_btnupdate_clicked()
{
    if (m_currentIndex >= 0 && m_currentIndex < m_userIds.size()) {
        loadUserData(m_userIds[m_currentIndex]);
        showSuccessMessage("Обновление", "Данные пользователя обновлены");
    }
}

void AmEditUser::on_btnsearch_clicked()
{
    QString searchLogin = ui->lesearch->text().trimmed();

    if (searchLogin.isEmpty()) {
        showErrorMessage("Ошибка", "Введите логин для поиска!");
        return;
    }

    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.isOpen()) {
        if (!m_adminWindow->reconnectToDatabase()) {
            showErrorMessage("Ошибка подключения", "Не удалось подключиться к базе данных!");
            return;
        }
    }

    QSqlQuery query(db);
    query.prepare("SELECT user_id FROM users WHERE login = :login");
    query.bindValue(":login", searchLogin);

    if (query.exec() && query.next()) {
        int userId = query.value(0).toInt();
        int index = m_userIds.indexOf(userId);
        if (index != -1) {
            m_currentIndex = index;
            loadUserData(userId);
        }
    } else {
        showErrorMessage("Ошибка", "Пользователь с таким логином не найден!");
    }
}

void AmEditUser::on_btnsave_clicked()
{
    if (m_currentIndex < 0 || m_currentIndex >= m_userIds.size()) {
        showErrorMessage("Ошибка", "Нет выбранного пользователя для сохранения!");
        return;
    }

    QString newPassword = ui->lenewpassword->text().trimmed();

    if (newPassword.isEmpty()) {
        showErrorMessage("Ошибка", "Пароль не может быть пустым!");
        return;
    }

    if (newPassword.length() < 6) {
        showErrorMessage("Ошибка", "Пароль должен содержать минимум 6 символов!");
        return;
    }

    int userId = m_userIds[m_currentIndex];
    QString login = ui->lenewlogin->text();
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

    if (!updateUserPassword(userId, newPassword)) {
        db.rollback();
        return;
    }

    QSqlQuery query(db);
    QString alterUserQuery = QString("ALTER USER \"%1\" WITH PASSWORD '%2'").arg(login, newPassword);

    if (!query.exec(alterUserQuery)) {
        showErrorMessage("Ошибка БД", "Не удалось изменить пароль пользователя базы данных: " + query.lastError().text());
        db.rollback();
        return;
    }

    if (!db.commit()) {
        showErrorMessage("Ошибка транзакции", "Не удалось зафиксировать транзакцию: " + db.lastError().text());
        return;
    }

    showSuccessMessage("Успех", "Пароль пользователя успешно изменен!");
}

bool AmEditUser::updateUserPassword(int userId, const QString &newPassword)
{
    QSqlDatabase db = m_adminWindow->getDatabase();
    QSqlQuery query(db);

    query.prepare("UPDATE users SET passphrase = :passphrase WHERE user_id = :user_id");
    query.bindValue(":passphrase", newPassword);
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        showErrorMessage("Ошибка БД", "Не удалось обновить пароль в таблице users: " + query.lastError().text());
        return false;
    }

    return true;
}

void AmEditUser::on_btndelete_clicked()
{
    if (m_currentIndex < 0 || m_currentIndex >= m_userIds.size()) {
        showErrorMessage("Ошибка", "Нет выбранного пользователя для удаления!");
        return;
    }

    int userId = m_userIds[m_currentIndex];
    QString login = ui->lenewlogin->text();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение удаления",
                                                              QString("Вы действительно хотите удалить пользователя '%1'?\nЭто действие нельзя отменить!").arg(login),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
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

    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        showErrorMessage("Ошибка БД", "Не удалось удалить пользователя из таблицы users: " + query.lastError().text());
        db.rollback();
        return;
    }

    if (!deleteDatabaseUser(login)) {
        db.rollback();
        return;
    }

    if (!db.commit()) {
        showErrorMessage("Ошибка транзакции", "Не удалось зафиксировать транзакцию: " + db.lastError().text());
        return;
    }

    showSuccessMessage("Успех", "Пользователь успешно удален!");
    loadUserList();

    if (!m_userIds.isEmpty()) {
        if (m_currentIndex >= m_userIds.size()) {
            m_currentIndex = m_userIds.size() - 1;
        }
        loadUserData(m_userIds[m_currentIndex]);
    } else {
        m_currentIndex = -1;
        ui->lenewlogin->clear();
        ui->lenewpassword->clear();
        ui->cbusertype->clear();
        updateNavigationButtons();
    }
}

bool AmEditUser::deleteDatabaseUser(const QString &login)
{
    QSqlDatabase db = m_adminWindow->getDatabase();
    QSqlQuery query(db);

    query.exec(QString("SELECT 1 FROM pg_roles WHERE rolname = '%1'").arg(login));

    if (query.next()) {
        QString dropUserQuery = QString("DROP USER IF EXISTS \"%1\"").arg(login);

        if (!query.exec(dropUserQuery)) {
            showErrorMessage("Ошибка БД", "Не удалось удалить пользователя базы данных: " + query.lastError().text());
            return false;
        }
    }

    return true;
}

void AmEditUser::on_btnshowpass_clicked()
{
    m_passwordVisible = !m_passwordVisible;

    if (m_passwordVisible) {
        ui->lenewpassword->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lenewpassword->setEchoMode(QLineEdit::Password);
    }
}

void AmEditUser::on_btnlogout_clicked()
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

void AmEditUser::showErrorMessage(const QString &title, const QString &message)
{
    QMessageBox::critical(this, title, message);
}

void AmEditUser::showSuccessMessage(const QString &title, const QString &message)
{
    QMessageBox::information(this, title, message);
}
