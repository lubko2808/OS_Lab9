/********************************************************************************
** Form generated from reading UI file 'userdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERDIALOG_H
#define UI_USERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_UserDialog
{
public:
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QTextBrowser *textBrowser;
    QLabel *topicLabel;

    void setupUi(QDialog *UserDialog)
    {
        if (UserDialog->objectName().isEmpty())
            UserDialog->setObjectName("UserDialog");
        UserDialog->resize(1000, 600);
        pushButton = new QPushButton(UserDialog);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(860, 520, 111, 61));
        textEdit = new QTextEdit(UserDialog);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(30, 520, 821, 61));
        textBrowser = new QTextBrowser(UserDialog);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(30, 120, 941, 381));
        topicLabel = new QLabel(UserDialog);
        topicLabel->setObjectName("topicLabel");
        topicLabel->setGeometry(QRect(30, 20, 941, 81));
        QFont font;
        font.setPointSize(14);
        topicLabel->setFont(font);

        retranslateUi(UserDialog);

        QMetaObject::connectSlotsByName(UserDialog);
    } // setupUi

    void retranslateUi(QDialog *UserDialog)
    {
        UserDialog->setWindowTitle(QCoreApplication::translate("UserDialog", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("UserDialog", ">", nullptr));
        topicLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class UserDialog: public Ui_UserDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERDIALOG_H
