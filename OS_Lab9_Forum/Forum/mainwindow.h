#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "dataStructs.h"
#include "userdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:

private slots:   
    void on_logInButton_clicked();
    void on_signUpButton_clicked();

    void reconnect();

private:
    Ui::MainWindow *ui;

    QTimer *timer;


    UserDialog* userPage;
    int id;
    HANDLE fileUsersLog;
    UsersLog* users;
    bool connected;
};
#endif // MAINWINDOW_H
