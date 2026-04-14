/********************************************************************************
** Form generated from reading UI file 'hraddstaff.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HRADDSTAFF_H
#define UI_HRADDSTAFF_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HrAddStaff
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
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *label_4;
    QLineEdit *lefio;
    QLineEdit *lephone;
    QLabel *label_5;
    QLineEdit *leposition;
    QLineEdit *ledepo;
    QLabel *label_6;
    QLabel *label_8;
    QComboBox *cbiswork;
    QLabel *label_7;
    QPushButton *btnadd;

    void setupUi(QWidget *HrAddStaff)
    {
        if (HrAddStaff->objectName().isEmpty())
            HrAddStaff->setObjectName("HrAddStaff");
        HrAddStaff->resize(500, 350);
        HrAddStaff->setMinimumSize(QSize(500, 350));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/labels/label.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        HrAddStaff->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(HrAddStaff);
        gridLayout_2->setObjectName("gridLayout_2");
        frame = new QFrame(HrAddStaff);
        frame->setObjectName("frame");
        frame->setMinimumSize(QSize(480, 91));
        frame->setMaximumSize(QSize(5000, 91));
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

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_4 = new QLabel(HrAddStaff);
        label_4->setObjectName("label_4");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Carlito")});
        font3.setPointSize(14);
        font3.setBold(true);
        label_4->setFont(font3);

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_4);

        lefio = new QLineEdit(HrAddStaff);
        lefio->setObjectName("lefio");
        lefio->setMinimumSize(QSize(150, 20));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lefio);

        lephone = new QLineEdit(HrAddStaff);
        lephone->setObjectName("lephone");
        lephone->setMinimumSize(QSize(150, 20));

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, lephone);

        label_5 = new QLabel(HrAddStaff);
        label_5->setObjectName("label_5");
        label_5->setFont(font3);

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_5);

        leposition = new QLineEdit(HrAddStaff);
        leposition->setObjectName("leposition");
        leposition->setMinimumSize(QSize(150, 20));

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, leposition);

        ledepo = new QLineEdit(HrAddStaff);
        ledepo->setObjectName("ledepo");
        ledepo->setMinimumSize(QSize(150, 20));

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, ledepo);

        label_6 = new QLabel(HrAddStaff);
        label_6->setObjectName("label_6");
        label_6->setFont(font3);

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_6);

        label_8 = new QLabel(HrAddStaff);
        label_8->setObjectName("label_8");
        label_8->setFont(font3);

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_8);

        cbiswork = new QComboBox(HrAddStaff);
        cbiswork->setObjectName("cbiswork");
        cbiswork->setMinimumSize(QSize(340, 20));

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, cbiswork);

        label_7 = new QLabel(HrAddStaff);
        label_7->setObjectName("label_7");
        label_7->setFont(font3);

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_7);


        verticalLayout_2->addLayout(formLayout);

        btnadd = new QPushButton(HrAddStaff);
        btnadd->setObjectName("btnadd");
        btnadd->setFont(font3);

        verticalLayout_2->addWidget(btnadd);


        gridLayout_2->addLayout(verticalLayout_2, 1, 0, 1, 1);


        retranslateUi(HrAddStaff);

        QMetaObject::connectSlotsByName(HrAddStaff);
    } // setupUi

    void retranslateUi(QWidget *HrAddStaff)
    {
        HrAddStaff->setWindowTitle(QCoreApplication::translate("HrAddStaff", "ACMS", nullptr));
        label->setText(QString());
        label_3->setText(QCoreApplication::translate("HrAddStaff", "\320\220\320\230\320\241 \320\241\320\232\320\243\320\224", nullptr));
        label_2->setText(QCoreApplication::translate("HrAddStaff", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\260", nullptr));
        llogin->setText(QCoreApplication::translate("HrAddStaff", "login", nullptr));
        btnlogout->setText(QString());
        label_4->setText(QCoreApplication::translate("HrAddStaff", "\320\244\320\230\320\236", nullptr));
        label_5->setText(QCoreApplication::translate("HrAddStaff", "\320\235\320\276\320\274\320\265\321\200 \321\202\320\265\320\273\320\265\321\204\320\276\320\275\320\260", nullptr));
        label_6->setText(QCoreApplication::translate("HrAddStaff", "\320\224\320\276\320\273\320\266\320\275\320\276\321\201\321\202\321\214", nullptr));
        label_8->setText(QCoreApplication::translate("HrAddStaff", "\320\236\321\202\320\264\320\265\320\273", nullptr));
        label_7->setText(QCoreApplication::translate("HrAddStaff", "\320\225\321\211\321\221 \321\200\320\260\320\261\320\276\321\202\320\260\320\265\321\202", nullptr));
        btnadd->setText(QCoreApplication::translate("HrAddStaff", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HrAddStaff: public Ui_HrAddStaff {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HRADDSTAFF_H
