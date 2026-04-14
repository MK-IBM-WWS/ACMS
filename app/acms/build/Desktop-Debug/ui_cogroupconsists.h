/********************************************************************************
** Form generated from reading UI file 'cogroupconsists.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COGROUPCONSISTS_H
#define UI_COGROUPCONSISTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CoGroupConsists
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLabel *lagroup;
    QLabel *label_6;
    QLabel *ladate;
    QLabel *label_8;
    QTableWidget *twgroupconsists;
    QPushButton *btnok;

    void setupUi(QWidget *CoGroupConsists)
    {
        if (CoGroupConsists->objectName().isEmpty())
            CoGroupConsists->setObjectName("CoGroupConsists");
        CoGroupConsists->resize(700, 382);
        CoGroupConsists->setMinimumSize(QSize(700, 382));
        CoGroupConsists->setMaximumSize(QSize(700, 382));
        gridLayout = new QGridLayout(CoGroupConsists);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_4 = new QLabel(CoGroupConsists);
        label_4->setObjectName("label_4");
        QFont font;
        font.setFamilies({QString::fromUtf8("Carlito")});
        font.setPointSize(14);
        font.setBold(true);
        font.setUnderline(true);
        label_4->setFont(font);

        horizontalLayout->addWidget(label_4);

        lagroup = new QLabel(CoGroupConsists);
        lagroup->setObjectName("lagroup");
        lagroup->setMinimumSize(QSize(250, 20));
        lagroup->setMaximumSize(QSize(250, 20));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Carlito")});
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setUnderline(false);
        lagroup->setFont(font1);

        horizontalLayout->addWidget(lagroup);

        label_6 = new QLabel(CoGroupConsists);
        label_6->setObjectName("label_6");
        label_6->setFont(font);

        horizontalLayout->addWidget(label_6);

        ladate = new QLabel(CoGroupConsists);
        ladate->setObjectName("ladate");
        ladate->setMinimumSize(QSize(120, 20));
        ladate->setMaximumSize(QSize(120, 20));
        ladate->setFont(font1);

        horizontalLayout->addWidget(ladate);


        verticalLayout->addLayout(horizontalLayout);

        label_8 = new QLabel(CoGroupConsists);
        label_8->setObjectName("label_8");
        label_8->setFont(font);

        verticalLayout->addWidget(label_8);

        twgroupconsists = new QTableWidget(CoGroupConsists);
        twgroupconsists->setObjectName("twgroupconsists");

        verticalLayout->addWidget(twgroupconsists);

        btnok = new QPushButton(CoGroupConsists);
        btnok->setObjectName("btnok");
        btnok->setMinimumSize(QSize(200, 30));

        verticalLayout->addWidget(btnok);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(CoGroupConsists);

        QMetaObject::connectSlotsByName(CoGroupConsists);
    } // setupUi

    void retranslateUi(QWidget *CoGroupConsists)
    {
        CoGroupConsists->setWindowTitle(QCoreApplication::translate("CoGroupConsists", "Form", nullptr));
        label_4->setText(QCoreApplication::translate("CoGroupConsists", "\320\223\321\200\321\203\320\277\320\277\320\260 \320\264\320\276\321\201\321\202\321\203\320\277\320\260", nullptr));
        lagroup->setText(QCoreApplication::translate("CoGroupConsists", "group", nullptr));
        label_6->setText(QCoreApplication::translate("CoGroupConsists", "\320\224\320\260\321\202\320\260 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217", nullptr));
        ladate->setText(QCoreApplication::translate("CoGroupConsists", "date", nullptr));
        label_8->setText(QCoreApplication::translate("CoGroupConsists", "\320\241\320\276\321\201\321\202\320\260\320\262 \320\263\321\200\321\203\320\277\320\277\321\213 \320\264\320\276\321\201\321\202\321\203\320\277\320\260:", nullptr));
        btnok->setText(QCoreApplication::translate("CoGroupConsists", "\320\236\320\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CoGroupConsists: public Ui_CoGroupConsists {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COGROUPCONSISTS_H
