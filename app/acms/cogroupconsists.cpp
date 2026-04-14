#include "cogroupconsists.h"
#include "ui_cogroupconsists.h"

CoGroupConsists::CoGroupConsists(int groupId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CoGroupConsists)
    , m_groupId(groupId)
{
    ui->setupUi(this);

    // Устанавливаем заголовок окна
    setWindowTitle("Состав группы доступа");

    // Настраиваем таблицу
    ui->twgroupconsists->setColumnCount(3);
    ui->twgroupconsists->setHorizontalHeaderLabels(QStringList() << "Название объекта" << "Адрес" << "График работы");
    ui->twgroupconsists->horizontalHeader()->setStretchLastSection(true);
    ui->twgroupconsists->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twgroupconsists->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Загружаем информацию о группе
    loadGroupInfo();

    // Загружаем состав группы
    loadGroupConsists();
}

CoGroupConsists::~CoGroupConsists()
{
    delete ui;
}

void CoGroupConsists::loadGroupInfo()
{
    QSqlQuery query;
    query.prepare("SELECT group_name, change_date FROM access_groups WHERE group_id = :group_id");
    query.bindValue(":group_id", m_groupId);

    if (query.exec() && query.next()) {
        QString groupName = query.value(0).toString();
        QDate changeDate = query.value(1).toDate();

        ui->lagroup->setText(groupName);
        ui->ladate->setText(changeDate.toString("dd.MM.yyyy"));
    } else {
        ui->lagroup->setText("Группа не найдена");
        ui->ladate->setText("Дата неизвестна");
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить информацию о группе доступа!");
    }
}

void CoGroupConsists::loadGroupConsists()
{
    QSqlQuery query;
    query.prepare("SELECT ac.object_name, ac.address, ac.work_shedule "
                  "FROM group_consists gc "
                  "JOIN access_controller ac ON gc.controller_id = ac.controller_id "
                  "WHERE gc.group_id = :group_id "
                  "ORDER BY ac.object_name");
    query.bindValue(":group_id", m_groupId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить состав группы: " + query.lastError().text());
        return;
    }

    ui->twgroupconsists->clearContents();
    ui->twgroupconsists->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->twgroupconsists->insertRow(row);
        QString objectName = query.value(0).toString();
        QString address = query.value(1).toString();
        QString workSchedule = query.value(2).toString();

        if (address.isNull()) address = "";
        if (workSchedule.isNull()) workSchedule = "";
        ui->twgroupconsists->setItem(row, 0, new QTableWidgetItem(objectName));
        ui->twgroupconsists->setItem(row, 1, new QTableWidgetItem(address));
        ui->twgroupconsists->setItem(row, 2, new QTableWidgetItem(workSchedule));

        row++;
    }

    if (row == 0) {
        ui->twgroupconsists->setRowCount(1);
        ui->twgroupconsists->setItem(0, 0, new QTableWidgetItem("Нет контроллеров в этой группе"));
        ui->twgroupconsists->setItem(0, 1, new QTableWidgetItem(""));
        ui->twgroupconsists->setItem(0, 2, new QTableWidgetItem(""));
    }

    ui->twgroupconsists->resizeColumnsToContents();
}

void CoGroupConsists::on_btnok_clicked()
{
    this->close();
}
