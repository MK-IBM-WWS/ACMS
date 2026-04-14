/********************************************************************************
** Form generated from reading UI file 'printwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINTWINDOW_H
#define UI_PRINTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrintWindow
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTextEdit *teexample;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnback;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnprint;

    void setupUi(QWidget *PrintWindow)
    {
        if (PrintWindow->objectName().isEmpty())
            PrintWindow->setObjectName("PrintWindow");
        PrintWindow->resize(720, 769);
        PrintWindow->setMinimumSize(QSize(720, 700));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/labels/label.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        PrintWindow->setWindowIcon(icon);
        gridLayout = new QGridLayout(PrintWindow);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        teexample = new QTextEdit(PrintWindow);
        teexample->setObjectName("teexample");
        teexample->setMinimumSize(QSize(700, 700));

        verticalLayout->addWidget(teexample);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btnback = new QPushButton(PrintWindow);
        btnback->setObjectName("btnback");
        QFont font;
        font.setFamilies({QString::fromUtf8("Carlito")});
        font.setPointSize(14);
        font.setBold(true);
        btnback->setFont(font);

        horizontalLayout->addWidget(btnback);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnprint = new QPushButton(PrintWindow);
        btnprint->setObjectName("btnprint");
        btnprint->setFont(font);

        horizontalLayout->addWidget(btnprint);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(PrintWindow);

        QMetaObject::connectSlotsByName(PrintWindow);
    } // setupUi

    void retranslateUi(QWidget *PrintWindow)
    {
        PrintWindow->setWindowTitle(QCoreApplication::translate("PrintWindow", "ACMS", nullptr));
        btnback->setText(QCoreApplication::translate("PrintWindow", "\320\235\320\260\320\267\320\260\320\264", nullptr));
        btnprint->setText(QCoreApplication::translate("PrintWindow", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\262 PDF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrintWindow: public Ui_PrintWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTWINDOW_H
