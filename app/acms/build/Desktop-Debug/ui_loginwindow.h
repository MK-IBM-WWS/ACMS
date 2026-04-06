/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLineEdit *lelogin;
    QLabel *label_2;
    QLineEdit *lepass;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QPushButton *btnlogin;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnsettings;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_4;

    void setupUi(QMainWindow *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->resize(480, 267);
        LoginWindow->setMinimumSize(QSize(480, 267));
        LoginWindow->setMaximumSize(QSize(480, 267));
        LoginWindow->setSizeIncrement(QSize(20, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/labels/label.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        LoginWindow->setWindowIcon(icon);
        centralwidget = new QWidget(LoginWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(121);
        sizePolicy.setVerticalStretch(131);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(121, 131));
        label->setMaximumSize(QSize(121, 131));
        label->setPixmap(QPixmap(QString::fromUtf8(":/labels/label.png")));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        lelogin = new QLineEdit(centralwidget);
        lelogin->setObjectName("lelogin");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(180);
        sizePolicy1.setVerticalStretch(33);
        sizePolicy1.setHeightForWidth(lelogin->sizePolicy().hasHeightForWidth());
        lelogin->setSizePolicy(sizePolicy1);
        lelogin->setMinimumSize(QSize(180, 33));
        QFont font;
        font.setFamilies({QString::fromUtf8("Carlito")});
        font.setPointSize(12);
        font.setBold(true);
        lelogin->setFont(font);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lelogin);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Carlito")});
        font1.setPointSize(14);
        font1.setBold(true);
        label_2->setFont(font1);

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        lepass = new QLineEdit(centralwidget);
        lepass->setObjectName("lepass");
        lepass->setFont(font);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, lepass);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setFont(font1);

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_3);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnlogin = new QPushButton(centralwidget);
        btnlogin->setObjectName("btnlogin");
        btnlogin->setFont(font1);

        verticalLayout->addWidget(btnlogin);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        btnsettings = new QPushButton(centralwidget);
        btnsettings->setObjectName("btnsettings");

        horizontalLayout_2->addWidget(btnsettings);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Carlito")});
        font2.setBold(true);
        font2.setItalic(false);
        label_4->setFont(font2);

        horizontalLayout_2->addWidget(label_4);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        LoginWindow->setCentralWidget(centralwidget);

        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QMainWindow *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "ACMS", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("LoginWindow", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        label_3->setText(QCoreApplication::translate("LoginWindow", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        btnlogin->setText(QCoreApplication::translate("LoginWindow", "\320\222\320\276\320\271\321\202\320\270", nullptr));
        btnsettings->setText(QCoreApplication::translate("LoginWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        label_4->setText(QCoreApplication::translate("LoginWindow", "\302\251 2026 OSDA IBM", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
