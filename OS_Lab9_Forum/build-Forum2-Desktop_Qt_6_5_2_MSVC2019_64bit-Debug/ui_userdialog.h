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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserDialog
{
public:
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTextBrowser *textBrowser;
    QWidget *tab_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QSpacerItem *verticalSpacer_2;
    QLabel *topicLabel;

    void setupUi(QDialog *UserDialog)
    {
        if (UserDialog->objectName().isEmpty())
            UserDialog->setObjectName("UserDialog");
        UserDialog->resize(1022, 605);
        pushButton = new QPushButton(UserDialog);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(860, 510, 111, 61));
        textEdit = new QTextEdit(UserDialog);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(30, 510, 821, 61));
        tabWidget = new QTabWidget(UserDialog);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(30, 110, 941, 381));
        tab = new QWidget();
        tab->setObjectName("tab");
        textBrowser = new QTextBrowser(tab);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(0, 0, 941, 401));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        layoutWidget = new QWidget(tab_2);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(7, 0, 931, 341));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label = new QLabel(layoutWidget);
        label->setObjectName("label");
        QFont font;
        font.setFamilies({QString::fromUtf8("Times New Roman")});
        font.setPointSize(14);
        font.setItalic(true);
        label->setFont(font);

        verticalLayout->addWidget(label);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        tabWidget->addTab(tab_2, QString());
        topicLabel = new QLabel(UserDialog);
        topicLabel->setObjectName("topicLabel");
        topicLabel->setGeometry(QRect(30, 10, 941, 81));
        QFont font1;
        font1.setPointSize(14);
        topicLabel->setFont(font1);

        retranslateUi(UserDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(UserDialog);
    } // setupUi

    void retranslateUi(QDialog *UserDialog)
    {
        UserDialog->setWindowTitle(QCoreApplication::translate("UserDialog", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("UserDialog", ">", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("UserDialog", "Tab 1", nullptr));
        label->setText(QCoreApplication::translate("UserDialog", "<html><head/><body><p align=\"center\">Here will be shown the discussion</p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("UserDialog", "Tab 2", nullptr));
        topicLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class UserDialog: public Ui_UserDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERDIALOG_H
