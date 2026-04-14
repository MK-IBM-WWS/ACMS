/********************************************************************************
** Form generated from reading UI file 'hrreportaccess.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HRREPORTACCESS_H
#define UI_HRREPORTACCESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HrReportAccess
{
public:

    void setupUi(QWidget *HrReportAccess)
    {
        if (HrReportAccess->objectName().isEmpty())
            HrReportAccess->setObjectName("HrReportAccess");
        HrReportAccess->resize(400, 300);

        retranslateUi(HrReportAccess);

        QMetaObject::connectSlotsByName(HrReportAccess);
    } // setupUi

    void retranslateUi(QWidget *HrReportAccess)
    {
        HrReportAccess->setWindowTitle(QCoreApplication::translate("HrReportAccess", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HrReportAccess: public Ui_HrReportAccess {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HRREPORTACCESS_H
