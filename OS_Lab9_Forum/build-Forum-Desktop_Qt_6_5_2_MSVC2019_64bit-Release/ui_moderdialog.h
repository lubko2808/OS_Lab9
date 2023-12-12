/********************************************************************************
** Form generated from reading UI file 'moderdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODERDIALOG_H
#define UI_MODERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_ModerDialog
{
public:

    void setupUi(QDialog *ModerDialog)
    {
        if (ModerDialog->objectName().isEmpty())
            ModerDialog->setObjectName("ModerDialog");
        ModerDialog->resize(1000, 600);

        retranslateUi(ModerDialog);

        QMetaObject::connectSlotsByName(ModerDialog);
    } // setupUi

    void retranslateUi(QDialog *ModerDialog)
    {
        ModerDialog->setWindowTitle(QCoreApplication::translate("ModerDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModerDialog: public Ui_ModerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODERDIALOG_H
