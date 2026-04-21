#include "pdeditgroup.h"
#include "ui_pdeditgroup.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QStringListModel>
#include <QCompleter>

PdEditGroup::PdEditGroup(PdWindow *pdWindow, const QString &login, const QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PdEditGroup)
    , m_pdWindow(pdWindow)
    , m_login(login)
    , m_password(password)
    , m_currentGroupId(0)
    , m_currentIndex(-1)
{
    ui->setupUi(this);
    ui->llogin->setText(m_login);

    ui->twcontrollers->setColumnCount(4);
    ui->twcontrollers->setHorizontalHeaderLabels({"position_id", "Объект", "Адрес", "Расписание"});
    ui->twcontrollers->setColumnHidden(0, true);
    ui->twcontrollers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twcontrollers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twcontrollers->setSelectionMode(QAbstractItemView::SingleSelection);

    loadGroupIds();
    setupSearchCompleter();

    if (!m_groupIds.isEmpty()) {
        m_currentIndex = 0;
        m_currentGroupId = m_groupIds[m_currentIndex];
        loadGroupData(m_currentGroupId);
    }
}

PdEditGroup::~PdEditGroup()
{
    delete ui;
}

bool PdEditGroup::validateDate(const QString &date, const QString &fieldName)
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

bool PdEditGroup::ensureDatabaseConnection()
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

void PdEditGroup::setupSearchCompleter()
{
    QSqlDatabase db = m_pdWindow->getDatabase();
    if (!db.isOpen()) return;

    QSqlQuery query(db);
    query.exec("SELECT group_name FROM access_groups ORDER BY group_name");

    QStringList groupNames;
    while (query.next()) {
        groupNames << query.value(0).toString();
    }

    QCompleter *completer = new QCompleter(groupNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lesearch->setCompleter(completer);
}

void PdEditGroup::loadGroupIds()
{
    if (!ensureDatabaseConnection()) return;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);
    query.exec("SELECT group_id FROM access_groups ORDER BY group_id");

    m_groupIds.clear();
    while (query.next()) {
        m_groupIds.append(query.value(0).toInt());
    }
}

void PdEditGroup::loadGroupData(int groupId)
{
    if (!ensureDatabaseConnection()) return;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT group_name, change_date FROM access_groups WHERE group_id = :group_id");
    query.bindValue(":group_id", groupId);

    if (query.exec() && query.next()) {
        ui->legroupname->setText(query.value("group_name").toString());
        ui->lechangedate->setText(query.value("change_date").toDate().toString("yyyy-MM-dd"));
        m_currentGroupId = groupId;

        loadControllersForGroup(groupId);
    }
}

void PdEditGroup::loadControllersForGroup(int groupId)
{
    if (!ensureDatabaseConnection()) return;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);
    query.prepare(
        "SELECT gc.position_id, ac.object_name, ac.address, ac.work_shedule "
        "FROM group_consists gc "
        "JOIN access_controller ac ON gc.controller_id = ac.controller_id "
        "WHERE gc.group_id = :group_id "
        "ORDER BY ac.object_name"
        );
    query.bindValue(":group_id", groupId);

    if (query.exec()) {
        ui->twcontrollers->setRowCount(0);
        int row = 0;

        while (query.next()) {
            ui->twcontrollers->insertRow(row);
            ui->twcontrollers->setItem(row, 0, new QTableWidgetItem(query.value("position_id").toString()));
            ui->twcontrollers->setItem(row, 1, new QTableWidgetItem(query.value("object_name").toString()));
            ui->twcontrollers->setItem(row, 2, new QTableWidgetItem(query.value("address").toString()));
            ui->twcontrollers->setItem(row, 3, new QTableWidgetItem(query.value("work_shedule").toString()));

            row++;
        }
    }
}

void PdEditGroup::navigateToGroup(int direction)
{
    if (m_groupIds.isEmpty()) return;

    int newIndex = m_currentIndex + direction;

    if (newIndex >= 0 && newIndex < m_groupIds.size()) {
        m_currentIndex = newIndex;
        m_currentGroupId = m_groupIds[m_currentIndex];
        loadGroupData(m_currentGroupId);
    }
}

bool PdEditGroup::saveGroupData(int groupId, const QString &groupName)
{
    if (!ensureDatabaseConnection()) return false;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);

    QDate currentDate = QDate::currentDate();

    query.prepare("UPDATE access_groups SET group_name = :group_name, change_date = :change_date WHERE group_id = :group_id");
    query.bindValue(":group_name", groupName);
    query.bindValue(":change_date", currentDate);
    query.bindValue(":group_id", groupId);

    if (!query.exec()) {
        QString errorText = query.lastError().text();

        if (errorText.contains("unique") || errorText.contains("duplicate key", Qt::CaseInsensitive)) {
            showErrorMessage("Группа доступа с таким названием уже существует!");
        } else {
            showErrorMessage("Ошибка при сохранении записи: " + errorText);
        }
        return false;
    }

    return true;
}

bool PdEditGroup::deleteGroup(int groupId)
{
    if (!ensureDatabaseConnection()) return false;

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение",
                                                              "Вы действительно хотите удалить эту группу доступа?\n"
                                                              "Все связанные назначения также будут удалены!",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return false;
    }

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);

    if (!db.transaction()) {
        showErrorMessage("Не удалось начать транзакцию: " + db.lastError().text());
        return false;
    }

    query.prepare("DELETE FROM access_groups WHERE group_id = :group_id");
    query.bindValue(":group_id", groupId);
    if (!query.exec()) {
        db.rollback();
        showErrorMessage("Ошибка при удалении группы: " + query.lastError().text());
        return false;
    }

    if (!db.commit()) {
        showErrorMessage("Не удалось зафиксировать транзакцию: " + db.lastError().text());
        return false;
    }

    showInfoMessage("Группа доступа успешно удалена!");
    loadGroupIds();

    if (!m_groupIds.isEmpty()) {
        if (m_currentIndex >= m_groupIds.size()) {
            m_currentIndex = m_groupIds.size() - 1;
        }
        if (m_currentIndex >= 0) {
            m_currentGroupId = m_groupIds[m_currentIndex];
            loadGroupData(m_currentGroupId);
        }
    } else {
        ui->legroupname->clear();
        ui->lechangedate->clear();
        ui->twcontrollers->setRowCount(0);
        m_currentGroupId = 0;
        m_currentIndex = -1;
    }

    return true;
}

void PdEditGroup::updateGroupChangeDate(int groupId)
{
    if (!ensureDatabaseConnection()) return;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);

    query.prepare("UPDATE access_groups SET change_date = :change_date WHERE group_id = :group_id");
    query.bindValue(":change_date", QDate::currentDate());
    query.bindValue(":group_id", groupId);

    query.exec();
}

bool PdEditGroup::removeControllerFromGroup(int positionId)
{
    if (!ensureDatabaseConnection()) return false;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);

    query.prepare("DELETE FROM group_consists WHERE position_id = :position_id");
    query.bindValue(":position_id", positionId);

    if (!query.exec()) {
        showErrorMessage("Ошибка при удалении контроллера из группы: " + query.lastError().text());
        return false;
    }

    updateGroupChangeDate(m_currentGroupId);

    return true;
}

bool PdEditGroup::addControllerToGroup(int groupId, int controllerId)
{
    if (!ensureDatabaseConnection()) return false;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);

    query.prepare("SELECT COUNT(*) FROM group_consists WHERE group_id = :group_id AND controller_id = :controller_id");
    query.bindValue(":group_id", groupId);
    query.bindValue(":controller_id", controllerId);

    if (query.exec() && query.next() && query.value(0).toInt() > 0) {
        showErrorMessage("Этот контроллер уже добавлен в группу!");
        return false;
    }

    query.prepare("INSERT INTO group_consists (group_id, controller_id) VALUES (:group_id, :controller_id)");
    query.bindValue(":group_id", groupId);
    query.bindValue(":controller_id", controllerId);

    if (!query.exec()) {
        showErrorMessage("Ошибка при добавлении контроллера в группу: " + query.lastError().text());
        return false;
    }

    updateGroupChangeDate(m_currentGroupId);

    return true;
}

void PdEditGroup::on_btnlogout_clicked()
{
    if (!m_pdWindow || !m_pdWindow->isVisible()) {
        PdWindow *newPdWindow = new PdWindow(m_login, m_password);
        newPdWindow->show();
    }
    this->close();
}

void PdEditGroup::on_btnsearch_clicked()
{
    QString searchName = ui->lesearch->text().trimmed();

    if (searchName.isEmpty()) {
        showErrorMessage("Введите название группы для поиска!");
        return;
    }

    if (!ensureDatabaseConnection()) return;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT group_id FROM access_groups WHERE group_name = :group_name");
    query.bindValue(":group_name", searchName);

    if (query.exec() && query.next()) {
        int groupId = query.value(0).toInt();
        int index = m_groupIds.indexOf(groupId);

        if (index != -1) {
            m_currentIndex = index;
            m_currentGroupId = groupId;
            loadGroupData(groupId);
        }
    } else {
        showErrorMessage("Группа с таким названием не найдена!");
    }
}

void PdEditGroup::on_btnback_clicked()
{
    navigateToGroup(-1);
}

void PdEditGroup::on_btnforward_clicked()
{
    navigateToGroup(1);
}

void PdEditGroup::on_btnupdate_clicked()
{
    if (m_currentGroupId > 0) {
        loadGroupData(m_currentGroupId);
        showInfoMessage("Данные обновлены!");
    }
}

void PdEditGroup::on_btnsave_clicked()
{
    if (m_currentGroupId == 0) {
        showErrorMessage("Нет открытой группы для сохранения!");
        return;
    }

    QString groupName = ui->legroupname->text().trimmed();
    QString changeDateStr = ui->lechangedate->text().trimmed();

    if (groupName.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Название группы доступа не может быть пустым!");
        return;
    }

    if (!validateDate(changeDateStr, "Дата изменения")) {
        return;
    }

    if (saveGroupData(m_currentGroupId, groupName)) {
        showInfoMessage("Изменения сохранены!");
        loadGroupData(m_currentGroupId);
        loadGroupIds();
        setupSearchCompleter();
    }
}

void PdEditGroup::on_btndelete_clicked()
{
    if (m_currentGroupId == 0) {
        showErrorMessage("Нет открытой группы для удаления!");
        return;
    }

    deleteGroup(m_currentGroupId);
}

void PdEditGroup::on_btnaddcontrol_clicked()
{
    if (m_currentGroupId == 0) {
        showErrorMessage("Сначала откройте группу доступа!");
        return;
    }

    if (!ensureDatabaseConnection()) return;

    QSqlDatabase db = m_pdWindow->getDatabase();
    QSqlQuery query(db);
    query.exec("SELECT object_name, address FROM access_controller ORDER BY object_name");

    QStringList controllerList;
    QMap<QString, int> controllerMap;

    while (query.next()) {
        QString objectName = query.value("object_name").toString();
        QString address = query.value("address").toString();
        QString displayText = objectName + " | " + address;
        controllerList << displayText;

        QSqlQuery idQuery(db);
        idQuery.prepare("SELECT controller_id FROM access_controller WHERE object_name = :object_name AND address = :address");
        idQuery.bindValue(":object_name", objectName);
        idQuery.bindValue(":address", address);
        if (idQuery.exec() && idQuery.next()) {
            controllerMap[displayText] = idQuery.value(0).toInt();
        }
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Добавить контроллер");
    dialog.setMinimumSize(400, 150);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Введите название объекта и адрес (формат: объект | адрес):", &dialog);
    layout->addWidget(label);

    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QCompleter *completer = new QCompleter(controllerList, &dialog);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    lineEdit->setCompleter(completer);
    layout->addWidget(lineEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString selectedText = lineEdit->text().trimmed();

        if (selectedText.isEmpty()) {
            showErrorMessage("Не выбран контроллер!");
            return;
        }

        if (!controllerMap.contains(selectedText)) {
            showErrorMessage("Контроллер не найден!");
            return;
        }

        int controllerId = controllerMap[selectedText];

        if (addControllerToGroup(m_currentGroupId, controllerId)) {
            showInfoMessage("Контроллер успешно добавлен в группу!");
            loadControllersForGroup(m_currentGroupId);
        }
    }
}

void PdEditGroup::on_btnremovecontrol_clicked()
{
    if (m_currentGroupId == 0) {
        showErrorMessage("Сначала откройте группу доступа!");
        return;
    }

    int currentRow = ui->twcontrollers->currentRow();

    if (currentRow < 0) {
        showErrorMessage("Выберите контроллер для удаления!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение",
                                                              "Вы действительно хотите удалить этот контроллер из группы?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    int positionId = ui->twcontrollers->item(currentRow, 0)->text().toInt();

    if (removeControllerFromGroup(positionId)) {
        showInfoMessage("Контроллер удален из группы!");
        loadControllersForGroup(m_currentGroupId);
        loadGroupData(m_currentGroupId);
    }
}

void PdEditGroup::showErrorMessage(const QString &message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void PdEditGroup::showInfoMessage(const QString &message)
{
    QMessageBox::information(this, "Информация", message);
}
