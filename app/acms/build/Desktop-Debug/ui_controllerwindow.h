/********************************************************************************
** Form generated from reading UI file 'controllerwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLLERWINDOW_H
#define UI_CONTROLLERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControllerWindow
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *llogin;
    QPushButton *btnlogout;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QFormLayout *formLayout;
    QLabel *label_5;
    QComboBox *cbaccesstype;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *letime;
    QPushButton *lenow;
    QLabel *label_7;
    QLineEdit *lepass;
    QLabel *label_8;
    QLineEdit *lecontrol;
    QLabel *label_9;
    QComboBox *cbsolution;
    QPushButton *btnregister;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_11;
    QLineEdit *lecheckpass;
    QPushButton *lecheckgroup;
    QTableWidget *twgroups;
    QPushButton *btngroupconsist;

    void setupUi(QWidget *ControllerWindow)
    {
        if (ControllerWindow->objectName().isEmpty())
            ControllerWindow->setObjectName("ControllerWindow");
        ControllerWindow->resize(710, 400);
        ControllerWindow->setMinimumSize(QSize(710, 400));
        ControllerWindow->setMaximumSize(QSize(710, 400));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/labels/label.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ControllerWindow->setWindowIcon(icon);
        gridLayout = new QGridLayout(ControllerWindow);
        gridLayout->setObjectName("gridLayout");
        frame = new QFrame(ControllerWindow);
        frame->setObjectName("frame");
        frame->setMinimumSize(QSize(690, 91));
        frame->setMaximumSize(QSize(690, 91));
        frame->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        frame->setAutoFillBackground(false);
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(186, 255, 134);"));
        frame->setFrameShape(QFrame::Shape::NoFrame);
        frame->setFrameShadow(QFrame::Shadow::Plain);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setMinimumSize(QSize(61, 71));
        label->setMaximumSize(QSize(61, 71));
        label->setPixmap(QPixmap(QString::fromUtf8(":/labels/label.png")));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        QFont font;
        font.setFamilies({QString::fromUtf8("Carlito")});
        font.setPointSize(16);
        font.setBold(true);
        label_3->setFont(font);

        verticalLayout->addWidget(label_3, 0, Qt::AlignmentFlag::AlignHCenter);

        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Carlito")});
        font1.setPointSize(12);
        font1.setBold(true);
        label_2->setFont(font1);

        verticalLayout->addWidget(label_2, 0, Qt::AlignmentFlag::AlignHCenter);


        horizontalLayout_2->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        llogin = new QLabel(frame);
        llogin->setObjectName("llogin");
        llogin->setMinimumSize(QSize(130, 20));
        llogin->setMaximumSize(QSize(130, 20));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Carlito")});
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setUnderline(true);
        llogin->setFont(font2);
        llogin->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        llogin->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(llogin, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop);

        btnlogout = new QPushButton(frame);
        btnlogout->setObjectName("btnlogout");
        btnlogout->setMinimumSize(QSize(41, 32));
        btnlogout->setMaximumSize(QSize(41, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/labels/logout.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnlogout->setIcon(icon1);

        horizontalLayout->addWidget(btnlogout, 0, Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTop);


        horizontalLayout_2->addLayout(horizontalLayout);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_4 = new QLabel(ControllerWindow);
        label_4->setObjectName("label_4");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Carlito")});
        font3.setPointSize(14);
        font3.setBold(true);
        font3.setUnderline(true);
        label_4->setFont(font3);

        verticalLayout_2->addWidget(label_4, 0, Qt::AlignmentFlag::AlignHCenter);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_5 = new QLabel(ControllerWindow);
        label_5->setObjectName("label_5");
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Carlito")});
        font4.setPointSize(14);
        font4.setBold(true);
        font4.setUnderline(false);
        label_5->setFont(font4);

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_5);

        cbaccesstype = new QComboBox(ControllerWindow);
        cbaccesstype->setObjectName("cbaccesstype");
        cbaccesstype->setMinimumSize(QSize(100, 0));
        cbaccesstype->setMaximumSize(QSize(100, 30));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, cbaccesstype);

        label_6 = new QLabel(ControllerWindow);
        label_6->setObjectName("label_6");
        label_6->setFont(font4);

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        letime = new QLineEdit(ControllerWindow);
        letime->setObjectName("letime");
        letime->setMinimumSize(QSize(100, 30));
        letime->setMaximumSize(QSize(100, 30));

        horizontalLayout_3->addWidget(letime);

        lenow = new QPushButton(ControllerWindow);
        lenow->setObjectName("lenow");
        lenow->setMinimumSize(QSize(75, 30));
        lenow->setMaximumSize(QSize(75, 30));
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Carlito")});
        font5.setBold(true);
        lenow->setFont(font5);

        horizontalLayout_3->addWidget(lenow);


        formLayout->setLayout(1, QFormLayout::ItemRole::FieldRole, horizontalLayout_3);

        label_7 = new QLabel(ControllerWindow);
        label_7->setObjectName("label_7");
        label_7->setFont(font4);

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_7);

        lepass = new QLineEdit(ControllerWindow);
        lepass->setObjectName("lepass");
        lepass->setMinimumSize(QSize(183, 30));
        lepass->setMaximumSize(QSize(183, 30));

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, lepass);

        label_8 = new QLabel(ControllerWindow);
        label_8->setObjectName("label_8");
        label_8->setFont(font4);

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_8);

        lecontrol = new QLineEdit(ControllerWindow);
        lecontrol->setObjectName("lecontrol");
        lecontrol->setMinimumSize(QSize(183, 30));
        lecontrol->setMaximumSize(QSize(183, 30));

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, lecontrol);

        label_9 = new QLabel(ControllerWindow);
        label_9->setObjectName("label_9");
        label_9->setFont(font4);

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_9);

        cbsolution = new QComboBox(ControllerWindow);
        cbsolution->setObjectName("cbsolution");
        cbsolution->setMinimumSize(QSize(100, 30));
        cbsolution->setMaximumSize(QSize(100, 30));

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, cbsolution);


        verticalLayout_2->addLayout(formLayout);

        btnregister = new QPushButton(ControllerWindow);
        btnregister->setObjectName("btnregister");
        btnregister->setFont(font5);

        verticalLayout_2->addWidget(btnregister);


        horizontalLayout_5->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_10 = new QLabel(ControllerWindow);
        label_10->setObjectName("label_10");
        label_10->setFont(font3);

        verticalLayout_3->addWidget(label_10, 0, Qt::AlignmentFlag::AlignHCenter);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_11 = new QLabel(ControllerWindow);
        label_11->setObjectName("label_11");
        label_11->setFont(font4);
        label_11->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_4->addWidget(label_11);

        lecheckpass = new QLineEdit(ControllerWindow);
        lecheckpass->setObjectName("lecheckpass");
        lecheckpass->setMinimumSize(QSize(100, 30));
        lecheckpass->setMaximumSize(QSize(100, 30));

        horizontalLayout_4->addWidget(lecheckpass);

        lecheckgroup = new QPushButton(ControllerWindow);
        lecheckgroup->setObjectName("lecheckgroup");
        lecheckgroup->setMinimumSize(QSize(75, 30));
        lecheckgroup->setMaximumSize(QSize(75, 30));
        lecheckgroup->setFont(font5);

        horizontalLayout_4->addWidget(lecheckgroup);


        verticalLayout_3->addLayout(horizontalLayout_4);

        twgroups = new QTableWidget(ControllerWindow);
        twgroups->setObjectName("twgroups");
        twgroups->setMinimumSize(QSize(315, 20));
        twgroups->setMaximumSize(QSize(315, 178));

        verticalLayout_3->addWidget(twgroups);

        btngroupconsist = new QPushButton(ControllerWindow);
        btngroupconsist->setObjectName("btngroupconsist");
        btngroupconsist->setFont(font5);

        verticalLayout_3->addWidget(btngroupconsist);


        horizontalLayout_5->addLayout(verticalLayout_3);


        gridLayout->addLayout(horizontalLayout_5, 1, 0, 1, 1);


        retranslateUi(ControllerWindow);

        QMetaObject::connectSlotsByName(ControllerWindow);
    } // setupUi

    void retranslateUi(QWidget *ControllerWindow)
    {
        ControllerWindow->setWindowTitle(QCoreApplication::translate("ControllerWindow", "ACMS", nullptr));
        label->setText(QString());
        label_3->setText(QCoreApplication::translate("ControllerWindow", "\320\220\320\230\320\241 \320\241\320\232\320\243\320\224", nullptr));
        label_2->setText(QCoreApplication::translate("ControllerWindow", "\320\232\320\276\320\275\321\202\321\200\320\276\320\273\321\214 \320\264\320\276\321\201\321\202\321\203\320\277\320\260", nullptr));
        llogin->setText(QCoreApplication::translate("ControllerWindow", "login", nullptr));
        btnlogout->setText(QString());
        label_4->setText(QCoreApplication::translate("ControllerWindow", "\320\240\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\206\320\270\321\217 \321\204\320\260\320\272\321\202\320\260 \320\264\320\276\321\201\321\202\321\203\320\277\320\260", nullptr));
        label_5->setText(QCoreApplication::translate("ControllerWindow", "\320\242\320\270\320\277 \320\264\320\276\321\201\321\202\321\203\320\277\320\260", nullptr));
        label_6->setText(QCoreApplication::translate("ControllerWindow", "\320\222\321\200\320\265\320\274\321\217 \320\264\320\276\321\201\321\202\321\203\320\277\320\260", nullptr));
        lenow->setText(QCoreApplication::translate("ControllerWindow", "\320\241\320\265\320\271\321\207\320\260\321\201", nullptr));
        label_7->setText(QCoreApplication::translate("ControllerWindow", "\320\235\320\276\320\274\320\265\321\200 \320\277\321\200\320\276\320\277\321\203\321\201\320\272\320\260", nullptr));
        label_8->setText(QCoreApplication::translate("ControllerWindow", "\320\232\320\276\320\275\321\202\321\200\320\276\320\273\320\273\320\265\321\200 \320\264\320\276\321\201\321\202\321\203\320\277\320\260", nullptr));
        label_9->setText(QCoreApplication::translate("ControllerWindow", "\320\240\320\265\321\210\320\265\320\275\320\270\320\265", nullptr));
        btnregister->setText(QCoreApplication::translate("ControllerWindow", "\320\227\320\260\321\200\320\265\320\263\320\265\321\201\321\202\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        label_10->setText(QCoreApplication::translate("ControllerWindow", "\320\237\321\200\320\276\320\262\320\265\321\200\320\272\320\260 \320\277\321\200\320\276\320\277\321\203\321\201\320\272\320\260", nullptr));
        label_11->setText(QCoreApplication::translate("ControllerWindow", "\320\237\321\200\320\276\320\277\321\203\321\201\320\272 \342\204\226", nullptr));
        lecheckgroup->setText(QCoreApplication::translate("ControllerWindow", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214", nullptr));
        btngroupconsist->setText(QCoreApplication::translate("ControllerWindow", "\320\237\320\276\320\264\321\200\320\276\320\261\320\275\320\265\320\265 \320\276 \320\263\321\200\321\203\320\277\320\277\320\265 \320\264\320\276\321\201\321\202\321\203\320\277\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ControllerWindow: public Ui_ControllerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLLERWINDOW_H
