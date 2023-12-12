/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *loginLineEdit;
    QLineEdit *passwordLineEdit;
    QLabel *label;
    QPushButton *signUpButton;
    QPushButton *logInButton;
    QLabel *label_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(400, 500);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        loginLineEdit = new QLineEdit(centralwidget);
        loginLineEdit->setObjectName("loginLineEdit");
        loginLineEdit->setGeometry(QRect(40, 80, 311, 41));
        passwordLineEdit = new QLineEdit(centralwidget);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setGeometry(QRect(40, 180, 311, 41));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 40, 81, 31));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        label->setFont(font);
        signUpButton = new QPushButton(centralwidget);
        signUpButton->setObjectName("signUpButton");
        signUpButton->setGeometry(QRect(155, 230, 80, 24));
        logInButton = new QPushButton(centralwidget);
        logInButton->setObjectName("logInButton");
        logInButton->setGeometry(QRect(130, 370, 131, 61));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 140, 91, 31));
        label_2->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 400, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        signUpButton->setText(QCoreApplication::translate("MainWindow", "Sign up", nullptr));
        logInButton->setText(QCoreApplication::translate("MainWindow", ">", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
