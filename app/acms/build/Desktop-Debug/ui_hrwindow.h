/********************************************************************************
** Form generated from reading UI file 'hrwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HRWINDOW_H
#define UI_HRWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HrWindow
{
public:
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *llogin;
    QPushButton *btnlogout;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btnaddstaff;
    QPushButton *btneditstaff;
    QPushButton *btnadddepo;
    QPushButton *btneditdepo;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *leposition;
    QPushButton *btnpositionreport;
    QFrame *line;
    QFormLayout *formLayout;
    QLabel *label_5;
    QLineEdit *lestart;
    QLabel *label_6;
    QLineEdit *leend;
    QPushButton *btnaccessreport;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *HrWindow)
    {
        if (HrWindow->objectName().isEmpty())
            HrWindow->setObjectName("HrWindow");
        HrWindow->resize(790, 430);
        HrWindow->setMinimumSize(QSize(790, 430));
        HrWindow->setMaximumSize(QSize(790, 430));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/labels/label.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        HrWindow->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(HrWindow);
        gridLayout_2->setObjectName("gridLayout_2");
        frame = new QFrame(HrWindow);
        frame->setObjectName("frame");
        frame->setMinimumSize(QSize(770, 91));
        frame->setMaximumSize(QSize(770, 91));
        frame->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        frame->setAutoFillBackground(false);
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(186, 255, 134);"));
        frame->setFrameShape(QFrame::Shape::NoFrame);
        frame->setFrameShadow(QFrame::Shadow::Plain);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setMinimumSize(QSize(61, 71));
        label->setMaximumSize(QSize(61, 71));
        label->setPixmap(QPixmap(QString::fromUtf8(":/labels/label.png")));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

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


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

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


        gridLayout->addLayout(horizontalLayout, 0, 3, 1, 1);


        gridLayout_2->addWidget(frame, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        groupBox_2 = new QGroupBox(HrWindow);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setMinimumSize(QSize(400, 300));
        groupBox_2->setMaximumSize(QSize(400, 300));
        groupBox_2->setFont(font1);
        groupBox_2->setStyleSheet(QString::fromUtf8("    QGroupBox {\n"
"        border: 1px solid gray;\n"
"        border-radius: 5px;\n"
"        margin-top: 10px;\n"
"        padding-top: 5px;\n"
"    }\n"
"    QGroupBox::title {\n"
"        subcontrol-origin: margin;\n"
"        subcontrol-position: top left;\n"
"        left: 100px;\n"
"        padding: 0 5px 0 5px;\n"
"    }"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        btnaddstaff = new QPushButton(groupBox_2);
        btnaddstaff->setObjectName("btnaddstaff");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Carlito")});
        font3.setPointSize(14);
        font3.setBold(true);
        btnaddstaff->setFont(font3);

        verticalLayout_2->addWidget(btnaddstaff);

        btneditstaff = new QPushButton(groupBox_2);
        btneditstaff->setObjectName("btneditstaff");
        btneditstaff->setFont(font3);

        verticalLayout_2->addWidget(btneditstaff);

        btnadddepo = new QPushButton(groupBox_2);
        btnadddepo->setObjectName("btnadddepo");
        btnadddepo->setFont(font3);

        verticalLayout_2->addWidget(btnadddepo);

        btneditdepo = new QPushButton(groupBox_2);
        btneditdepo->setObjectName("btneditdepo");
        btneditdepo->setFont(font3);

        verticalLayout_2->addWidget(btneditdepo);


        horizontalLayout_2->addWidget(groupBox_2);

        groupBox = new QGroupBox(HrWindow);
        groupBox->setObjectName("groupBox");
        groupBox->setMinimumSize(QSize(350, 300));
        groupBox->setMaximumSize(QSize(350, 300));
        groupBox->setFont(font1);
        groupBox->setStyleSheet(QString::fromUtf8("    QGroupBox {\n"
"        border: 1px solid gray;\n"
"        border-radius: 5px;\n"
"        margin-top: 10px;\n"
"        padding-top: 5px;\n"
"    }\n"
"    QGroupBox::title {\n"
"        subcontrol-origin: margin;\n"
"        subcontrol-position: top left;\n"
"        left: 135px;\n"
"        padding: 0 5px 0 5px;\n"
"    }"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setFont(font1);

        horizontalLayout_3->addWidget(label_4);

        leposition = new QLineEdit(groupBox);
        leposition->setObjectName("leposition");

        horizontalLayout_3->addWidget(leposition);


        verticalLayout_3->addLayout(horizontalLayout_3);

        btnpositionreport = new QPushButton(groupBox);
        btnpositionreport->setObjectName("btnpositionreport");
        btnpositionreport->setFont(font3);

        verticalLayout_3->addWidget(btnpositionreport);

        line = new QFrame(groupBox);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout_3->addWidget(line);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setFont(font1);

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_5);

        lestart = new QLineEdit(groupBox);
        lestart->setObjectName("lestart");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lestart);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");
        label_6->setFont(font1);

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_6);

        leend = new QLineEdit(groupBox);
        leend->setObjectName("leend");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, leend);


        verticalLayout_3->addLayout(formLayout);

        btnaccessreport = new QPushButton(groupBox);
        btnaccessreport->setObjectName("btnaccessreport");
        btnaccessreport->setFont(font3);

        verticalLayout_3->addWidget(btnaccessreport);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout_2->addWidget(groupBox);


        gridLayout_2->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        retranslateUi(HrWindow);

        QMetaObject::connectSlotsByName(HrWindow);
    } // setupUi

    void retranslateUi(QWidget *HrWindow)
    {
        HrWindow->setWindowTitle(QCoreApplication::translate("HrWindow", "ACMS", nullptr));
        label->setText(QString());
        label_3->setText(QCoreApplication::translate("HrWindow", "\320\220\320\230\320\241 \320\241\320\232\320\243\320\224", nullptr));
        label_2->setText(QCoreApplication::translate("HrWindow", "\320\236\321\202\320\264\320\265\320\273 \320\272\320\260\320\264\321\200\320\276\320\262", nullptr));
        llogin->setText(QCoreApplication::translate("HrWindow", "login", nullptr));
        btnlogout->setText(QString());
        groupBox_2->setTitle(QCoreApplication::translate("HrWindow", "\320\222\320\275\320\265\321\201\320\265\320\275\320\270\320\265 \320\270 \320\277\321\200\320\276\321\201\320\274\320\276\321\202\321\200 \320\264\320\260\320\275\320\275\321\213\321\205", nullptr));
        btnaddstaff->setText(QCoreApplication::translate("HrWindow", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\260", nullptr));
        btneditstaff->setText(QCoreApplication::translate("HrWindow", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200 \320\270 \321\200\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\276\320\262", nullptr));
        btnadddepo->setText(QCoreApplication::translate("HrWindow", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\276\321\202\320\264\320\265\320\273\320\260", nullptr));
        btneditdepo->setText(QCoreApplication::translate("HrWindow", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200 \320\270 \321\200\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\276\321\202\320\264\320\265\320\273\320\276\320\262", nullptr));
        groupBox->setTitle(QCoreApplication::translate("HrWindow", "\320\236\321\202\321\207\320\265\321\202\321\213", nullptr));
        label_4->setText(QCoreApplication::translate("HrWindow", "\320\224\320\276\320\273\320\266\320\275\320\276\321\201\321\202\321\214", nullptr));
        btnpositionreport->setText(QCoreApplication::translate("HrWindow", "\320\236\321\202\321\207\320\265\321\202 \320\277\320\276 \320\264\320\276\320\273\320\266\320\275\320\276\321\201\321\202\320\270", nullptr));
        label_5->setText(QCoreApplication::translate("HrWindow", "\320\224\320\260\321\202\320\260 \320\275\320\260\321\207\320\260\320\273\320\260", nullptr));
        label_6->setText(QCoreApplication::translate("HrWindow", "\320\224\320\260\321\202\320\260 \320\272\320\276\320\275\321\206\320\260", nullptr));
        btnaccessreport->setText(QCoreApplication::translate("HrWindow", "\320\236\321\202\321\207\320\265\321\202 \320\277\320\276 \320\264\320\276\321\201\321\202\321\203\320\277\320\260\320\274 \320\267\320\260 \320\277\321\200\320\276\320\274\320\265\320\266\321\203\321\202\320\276\320\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HrWindow: public Ui_HrWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HRWINDOW_H
