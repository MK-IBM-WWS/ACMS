/********************************************************************************
** Form generated from reading UI file 'settingswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSWINDOW_H
#define UI_SETTINGSWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsWindow
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lehostname;
    QLabel *label_2;
    QLineEdit *leport;
    QPushButton *btncheckconn;
    QTextEdit *testatus;

    void setupUi(QWidget *SettingsWindow)
    {
        if (SettingsWindow->objectName().isEmpty())
            SettingsWindow->setObjectName("SettingsWindow");
        SettingsWindow->resize(460, 236);
        SettingsWindow->setMinimumSize(QSize(460, 236));
        SettingsWindow->setMaximumSize(QSize(460, 236));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/labels/label.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        SettingsWindow->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(SettingsWindow);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label_3 = new QLabel(SettingsWindow);
        label_3->setObjectName("label_3");
        QFont font;
        font.setFamilies({QString::fromUtf8("Carlito")});
        font.setPointSize(14);
        font.setBold(true);
        font.setUnderline(true);
        label_3->setFont(font);

        verticalLayout->addWidget(label_3);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(SettingsWindow);
        label->setObjectName("label");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Carlito")});
        font1.setPointSize(14);
        font1.setBold(true);
        label->setFont(font1);

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        lehostname = new QLineEdit(SettingsWindow);
        lehostname->setObjectName("lehostname");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lehostname);

        label_2 = new QLabel(SettingsWindow);
        label_2->setObjectName("label_2");
        label_2->setFont(font1);

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        leport = new QLineEdit(SettingsWindow);
        leport->setObjectName("leport");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, leport);

        btncheckconn = new QPushButton(SettingsWindow);
        btncheckconn->setObjectName("btncheckconn");
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Carlito")});
        font2.setBold(true);
        btncheckconn->setFont(font2);

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, btncheckconn);

        testatus = new QTextEdit(SettingsWindow);
        testatus->setObjectName("testatus");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, testatus);


        verticalLayout->addLayout(formLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(SettingsWindow);

        QMetaObject::connectSlotsByName(SettingsWindow);
    } // setupUi

    void retranslateUi(QWidget *SettingsWindow)
    {
        SettingsWindow->setWindowTitle(QCoreApplication::translate("SettingsWindow", "ACMS", nullptr));
        label_3->setText(QCoreApplication::translate("SettingsWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217 \320\272 \321\201\320\265\321\200\320\262\320\265\321\200\321\203 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217 \320\241\320\232\320\243\320\224", nullptr));
        label->setText(QCoreApplication::translate("SettingsWindow", "\320\230\320\274\321\217 \321\205\320\276\321\201\321\202\320\260", nullptr));
        label_2->setText(QCoreApplication::translate("SettingsWindow", "\320\237\320\276\321\200\321\202", nullptr));
        btncheckconn->setText(QCoreApplication::translate("SettingsWindow", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsWindow: public Ui_SettingsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSWINDOW_H
