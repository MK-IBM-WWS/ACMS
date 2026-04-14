/********************************************************************************
** Form generated from reading UI file 'hreditstaff.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HREDITSTAFF_H
#define UI_HREDITSTAFF_H

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

class Ui_HrEditStaff
{
public:
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QFormLayout *formLayout_3;
    QPushButton *btnlogout;
    QLineEdit *lesearch;
    QPushButton *btnsearch;
    QLabel *llogin;
    QHBoxLayout *horizontalLayout_3;
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
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnback;
    QPushButton *btnupdate;
    QPushButton *btforward;
    QSpacerItem *verticalSpacer;
    QPushButton *btndelete;
    QPushButton *btnsave;

    void setupUi(QWidget *HrEditStaff)
    {
        if (HrEditStaff->objectName().isEmpty())
            HrEditStaff->setObjectName("HrEditStaff");
        HrEditStaff->resize(589, 308);
        HrEditStaff->setMinimumSize(QSize(589, 308));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/labels/label.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        HrEditStaff->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(HrEditStaff);
        gridLayout_2->setObjectName("gridLayout_2");
        frame = new QFrame(HrEditStaff);
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

        label = new QLabel(frame);
        label->setObjectName("label");
        label->setMinimumSize(QSize(61, 71));
        label->setMaximumSize(QSize(61, 71));
        label->setPixmap(QPixmap(QString::fromUtf8(":/labels/label.png")));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName("formLayout_3");
        btnlogout = new QPushButton(frame);
        btnlogout->setObjectName("btnlogout");
        btnlogout->setMinimumSize(QSize(41, 32));
        btnlogout->setMaximumSize(QSize(41, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/labels/logout.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnlogout->setIcon(icon1);

        formLayout_3->setWidget(0, QFormLayout::ItemRole::FieldRole, btnlogout);

        lesearch = new QLineEdit(frame);
        lesearch->setObjectName("lesearch");

        formLayout_3->setWidget(1, QFormLayout::ItemRole::LabelRole, lesearch);

        btnsearch = new QPushButton(frame);
        btnsearch->setObjectName("btnsearch");
        btnsearch->setMinimumSize(QSize(41, 32));
        btnsearch->setMaximumSize(QSize(41, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/labels/search.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnsearch->setIcon(icon2);

        formLayout_3->setWidget(1, QFormLayout::ItemRole::FieldRole, btnsearch);

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

        formLayout_3->setWidget(0, QFormLayout::ItemRole::LabelRole, llogin);


        gridLayout->addLayout(formLayout_3, 0, 3, 1, 1);


        gridLayout_2->addWidget(frame, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_4 = new QLabel(HrEditStaff);
        label_4->setObjectName("label_4");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Carlito")});
        font3.setPointSize(14);
        font3.setBold(true);
        label_4->setFont(font3);

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_4);

        lefio = new QLineEdit(HrEditStaff);
        lefio->setObjectName("lefio");
        lefio->setMinimumSize(QSize(250, 20));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lefio);

        lephone = new QLineEdit(HrEditStaff);
        lephone->setObjectName("lephone");
        lephone->setMinimumSize(QSize(250, 20));

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, lephone);

        label_5 = new QLabel(HrEditStaff);
        label_5->setObjectName("label_5");
        label_5->setFont(font3);

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_5);

        leposition = new QLineEdit(HrEditStaff);
        leposition->setObjectName("leposition");
        leposition->setMinimumSize(QSize(250, 20));

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, leposition);

        ledepo = new QLineEdit(HrEditStaff);
        ledepo->setObjectName("ledepo");
        ledepo->setMinimumSize(QSize(250, 20));

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, ledepo);

        label_6 = new QLabel(HrEditStaff);
        label_6->setObjectName("label_6");
        label_6->setFont(font3);

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_6);

        label_8 = new QLabel(HrEditStaff);
        label_8->setObjectName("label_8");
        label_8->setFont(font3);

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_8);

        cbiswork = new QComboBox(HrEditStaff);
        cbiswork->setObjectName("cbiswork");
        cbiswork->setMinimumSize(QSize(250, 20));

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, cbiswork);

        label_7 = new QLabel(HrEditStaff);
        label_7->setObjectName("label_7");
        label_7->setFont(font3);

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_7);


        verticalLayout_2->addLayout(formLayout);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnback = new QPushButton(HrEditStaff);
        btnback->setObjectName("btnback");
        btnback->setMinimumSize(QSize(40, 40));
        btnback->setMaximumSize(QSize(40, 40));
        btnback->setFont(font3);

        horizontalLayout_2->addWidget(btnback);

        btnupdate = new QPushButton(HrEditStaff);
        btnupdate->setObjectName("btnupdate");
        btnupdate->setMinimumSize(QSize(40, 40));
        btnupdate->setMaximumSize(QSize(40, 40));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/labels/update.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnupdate->setIcon(icon3);

        horizontalLayout_2->addWidget(btnupdate);

        btforward = new QPushButton(HrEditStaff);
        btforward->setObjectName("btforward");
        btforward->setMinimumSize(QSize(40, 40));
        btforward->setMaximumSize(QSize(40, 40));
        btforward->setFont(font3);

        horizontalLayout_2->addWidget(btforward);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        btndelete = new QPushButton(HrEditStaff);
        btndelete->setObjectName("btndelete");
        btndelete->setFont(font3);

        verticalLayout_3->addWidget(btndelete);

        btnsave = new QPushButton(HrEditStaff);
        btnsave->setObjectName("btnsave");
        btnsave->setFont(font3);

        verticalLayout_3->addWidget(btnsave);


        horizontalLayout_3->addLayout(verticalLayout_3);


        gridLayout_2->addLayout(horizontalLayout_3, 1, 0, 1, 1);


        retranslateUi(HrEditStaff);

        QMetaObject::connectSlotsByName(HrEditStaff);
    } // setupUi

    void retranslateUi(QWidget *HrEditStaff)
    {
        HrEditStaff->setWindowTitle(QCoreApplication::translate("HrEditStaff", "ACMS", nullptr));
        label_3->setText(QCoreApplication::translate("HrEditStaff", "\320\220\320\230\320\241 \320\241\320\232\320\243\320\224", nullptr));
        label_2->setText(QCoreApplication::translate("HrEditStaff", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200 \320\270 \321\200\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265\n"
"                  \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\276\320\262", nullptr));
        label->setText(QString());
        btnlogout->setText(QString());
        btnsearch->setText(QString());
        llogin->setText(QCoreApplication::translate("HrEditStaff", "login", nullptr));
        label_4->setText(QCoreApplication::translate("HrEditStaff", "\320\244\320\230\320\236 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\260", nullptr));
        label_5->setText(QCoreApplication::translate("HrEditStaff", "\320\235\320\276\320\274\320\265\321\200 \321\202\320\265\320\273\320\265\321\204\320\276\320\275\320\260", nullptr));
        label_6->setText(QCoreApplication::translate("HrEditStaff", "\320\224\320\276\320\273\320\266\320\275\320\276\321\201\321\202\321\214", nullptr));
        label_8->setText(QCoreApplication::translate("HrEditStaff", "\320\236\321\202\320\264\320\265\320\273", nullptr));
        label_7->setText(QCoreApplication::translate("HrEditStaff", "\320\225\321\211\321\221 \321\200\320\260\320\261\320\276\321\202\320\260\320\265\321\202", nullptr));
        btnback->setText(QCoreApplication::translate("HrEditStaff", "<", nullptr));
        btnupdate->setText(QString());
        btforward->setText(QCoreApplication::translate("HrEditStaff", ">", nullptr));
        btndelete->setText(QCoreApplication::translate("HrEditStaff", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        btnsave->setText(QCoreApplication::translate("HrEditStaff", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HrEditStaff: public Ui_HrEditStaff {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HREDITSTAFF_H
