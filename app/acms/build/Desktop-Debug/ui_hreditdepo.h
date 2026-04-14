/********************************************************************************
** Form generated from reading UI file 'hreditdepo.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HREDITDEPO_H
#define UI_HREDITDEPO_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
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

class Ui_HrEditDepo
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
    QFormLayout *formLayout_3;
    QLabel *llogin;
    QPushButton *btnlogout;
    QLineEdit *lesearch;
    QPushButton *btnsearch;
    QHBoxLayout *horizontalLayout_2;
    QFormLayout *formLayout;
    QLabel *label_4;
    QLineEdit *ledeponame;
    QLabel *label_5;
    QLineEdit *leaddress;
    QLabel *label_6;
    QLineEdit *lefio;
    QLabel *label_8;
    QLineEdit *lework;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnback;
    QPushButton *btnupdate;
    QPushButton *btnforward;
    QSpacerItem *verticalSpacer;
    QPushButton *btnsavereport;
    QPushButton *btnabout;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *twstaff;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btndelete;
    QPushButton *btnsave;

    void setupUi(QWidget *HrEditDepo)
    {
        if (HrEditDepo->objectName().isEmpty())
            HrEditDepo->setObjectName("HrEditDepo");
        HrEditDepo->resize(721, 523);
        HrEditDepo->setMinimumSize(QSize(721, 523));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/labels/label.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        HrEditDepo->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(HrEditDepo);
        gridLayout_2->setObjectName("gridLayout_2");
        frame = new QFrame(HrEditDepo);
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
        label_2->setMinimumSize(QSize(201, 40));
        label_2->setMaximumSize(QSize(201, 40));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Carlito")});
        font1.setPointSize(12);
        font1.setBold(true);
        label_2->setFont(font1);

        verticalLayout->addWidget(label_2, 0, Qt::AlignmentFlag::AlignHCenter);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName("formLayout_3");
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


        gridLayout->addLayout(formLayout_3, 0, 3, 1, 1);


        gridLayout_2->addWidget(frame, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_4 = new QLabel(HrEditDepo);
        label_4->setObjectName("label_4");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Carlito")});
        font3.setPointSize(14);
        font3.setBold(true);
        label_4->setFont(font3);

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_4);

        ledeponame = new QLineEdit(HrEditDepo);
        ledeponame->setObjectName("ledeponame");
        ledeponame->setMinimumSize(QSize(300, 20));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, ledeponame);

        label_5 = new QLabel(HrEditDepo);
        label_5->setObjectName("label_5");
        label_5->setFont(font3);

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_5);

        leaddress = new QLineEdit(HrEditDepo);
        leaddress->setObjectName("leaddress");
        leaddress->setMinimumSize(QSize(300, 20));

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, leaddress);

        label_6 = new QLabel(HrEditDepo);
        label_6->setObjectName("label_6");
        label_6->setFont(font3);

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_6);

        lefio = new QLineEdit(HrEditDepo);
        lefio->setObjectName("lefio");
        lefio->setMinimumSize(QSize(300, 20));

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, lefio);

        label_8 = new QLabel(HrEditDepo);
        label_8->setObjectName("label_8");
        label_8->setFont(font3);

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_8);

        lework = new QLineEdit(HrEditDepo);
        lework->setObjectName("lework");
        lework->setMinimumSize(QSize(300, 20));

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, lework);


        horizontalLayout_2->addLayout(formLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_2 = new QSpacerItem(80, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        btnback = new QPushButton(HrEditDepo);
        btnback->setObjectName("btnback");
        btnback->setMinimumSize(QSize(40, 40));
        btnback->setMaximumSize(QSize(40, 40));
        btnback->setFont(font3);

        horizontalLayout_3->addWidget(btnback);

        btnupdate = new QPushButton(HrEditDepo);
        btnupdate->setObjectName("btnupdate");
        btnupdate->setMinimumSize(QSize(40, 40));
        btnupdate->setMaximumSize(QSize(40, 40));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/labels/update.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnupdate->setIcon(icon3);

        horizontalLayout_3->addWidget(btnupdate);

        btnforward = new QPushButton(HrEditDepo);
        btnforward->setObjectName("btnforward");
        btnforward->setMinimumSize(QSize(40, 40));
        btnforward->setMaximumSize(QSize(40, 40));
        btnforward->setFont(font3);

        horizontalLayout_3->addWidget(btnforward);


        verticalLayout_3->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        btnsavereport = new QPushButton(HrEditDepo);
        btnsavereport->setObjectName("btnsavereport");
        btnsavereport->setFont(font3);

        verticalLayout_3->addWidget(btnsavereport);

        btnabout = new QPushButton(HrEditDepo);
        btnabout->setObjectName("btnabout");
        btnabout->setFont(font3);

        verticalLayout_3->addWidget(btnabout);


        horizontalLayout_2->addLayout(verticalLayout_3);


        gridLayout_2->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        twstaff = new QTableWidget(HrEditDepo);
        twstaff->setObjectName("twstaff");
        twstaff->setMinimumSize(QSize(580, 183));

        verticalLayout_2->addWidget(twstaff);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        btndelete = new QPushButton(HrEditDepo);
        btndelete->setObjectName("btndelete");
        btndelete->setFont(font3);

        horizontalLayout_4->addWidget(btndelete);

        btnsave = new QPushButton(HrEditDepo);
        btnsave->setObjectName("btnsave");
        btnsave->setFont(font3);

        horizontalLayout_4->addWidget(btnsave);


        verticalLayout_2->addLayout(horizontalLayout_4);


        gridLayout_2->addLayout(verticalLayout_2, 2, 0, 1, 1);


        retranslateUi(HrEditDepo);

        QMetaObject::connectSlotsByName(HrEditDepo);
    } // setupUi

    void retranslateUi(QWidget *HrEditDepo)
    {
        HrEditDepo->setWindowTitle(QCoreApplication::translate("HrEditDepo", "ACMS", nullptr));
        label->setText(QString());
        label_3->setText(QCoreApplication::translate("HrEditDepo", "\320\220\320\230\320\241 \320\241\320\232\320\243\320\224", nullptr));
        label_2->setText(QCoreApplication::translate("HrEditDepo", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200 \320\270 \321\200\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265\n"
"                   \320\276\321\202\320\264\320\265\320\273\320\276\320\262", nullptr));
        llogin->setText(QCoreApplication::translate("HrEditDepo", "login", nullptr));
        btnlogout->setText(QString());
        btnsearch->setText(QString());
        label_4->setText(QCoreApplication::translate("HrEditDepo", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265  \320\276\321\202\320\264\320\265\320\273\320\260", nullptr));
        label_5->setText(QCoreApplication::translate("HrEditDepo", "\320\220\320\264\321\200\320\265\321\201", nullptr));
        label_6->setText(QCoreApplication::translate("HrEditDepo", "\320\244\320\230\320\236 \320\275\320\260\321\207\320\260\320\273\321\214\320\275\320\270\320\272\320\260", nullptr));
        label_8->setText(QCoreApplication::translate("HrEditDepo", "\320\240\320\276\320\264 \320\264\320\265\321\217\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\320\270", nullptr));
        btnback->setText(QCoreApplication::translate("HrEditDepo", "<", nullptr));
        btnupdate->setText(QString());
        btnforward->setText(QCoreApplication::translate("HrEditDepo", ">", nullptr));
        btnsavereport->setText(QCoreApplication::translate("HrEditDepo", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\276\321\202\321\207\320\265\321\202 \320\277\320\276 \320\276\321\202\320\264\320\265\320\273\321\203", nullptr));
        btnabout->setText(QCoreApplication::translate("HrEditDepo", "\320\237\320\276\320\264\321\200\320\276\320\261\320\275\320\265\320\265 \320\276 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\265", nullptr));
        btndelete->setText(QCoreApplication::translate("HrEditDepo", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        btnsave->setText(QCoreApplication::translate("HrEditDepo", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HrEditDepo: public Ui_HrEditDepo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HREDITDEPO_H
