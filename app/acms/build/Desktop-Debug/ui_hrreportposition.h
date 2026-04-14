/********************************************************************************
** Form generated from reading UI file 'hrreportposition.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HRREPORTPOSITION_H
#define UI_HRREPORTPOSITION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HrReportPosition
{
public:

    void setupUi(QWidget *HrReportPosition)
    {
        if (HrReportPosition->objectName().isEmpty())
            HrReportPosition->setObjectName("HrReportPosition");
        HrReportPosition->resize(400, 300);

        retranslateUi(HrReportPosition);

        QMetaObject::connectSlotsByName(HrReportPosition);
    } // setupUi

    void retranslateUi(QWidget *HrReportPosition)
    {
        HrReportPosition->setWindowTitle(QCoreApplication::translate("HrReportPosition", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HrReportPosition: public Ui_HrReportPosition {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HRREPORTPOSITION_H
