#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(400,500);
    setWindowTitle("Login");

    userPage = new UserDialog();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(reconnect()));
    // Встановіть інтервал таймера в мілісекундах (наприклад, кожні 1000 мс, тобто 1 секунда)
    timer->setInterval(5000);
    timer->start(); // Розпочніть роботу таймера


    // Open "Server" for login
    fileUsersLog = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        L"UsersLog");

    if(fileUsersLog == NULL)
    {
        connected = false;
    }else{
        connected = true;

        users = (UsersLog*)MapViewOfFile(
            fileUsersLog,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(UsersLog));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_logInButton_clicked()
{
    bool logIn = false;

    if(connected == true)
    {
        for(int i = 0; i < MaxClients; i++)
        {
            if(!std::strcmp(users->user[i].nickname, ui->loginLineEdit->text().toStdString().c_str()))
            {

                if(  !std::strcmp(users->user[i].password, ui->passwordLineEdit->text().toStdString().c_str()))
                {
                    userPage->takeUsers(users);
                    id = users->user[i].id;
                    userPage->takeId(id);
                    userPage->show();
                    logIn = true;
                    this->hide();
                    break;
                }
                else
                {
                    ui->statusbar->showMessage("Incorrect password");
                    return;
                }
            }
            else
            {
                ui->statusbar->showMessage("No such user exists");
            }
        }

        if(!logIn)
        {
            ui->statusbar->showMessage("No such user exists");
        }

    }
    else
    {
        ui->statusbar->showMessage("Connect error");
    }
}


void MainWindow::on_signUpButton_clicked()
{
    if(connected == true)
    {
        bool done = false;

        for(int i = 0; i < MaxClients; i++)
        {
            if(users->user[i].nickname[0] == '\0' && users->user[i].nickname[1] == '\0' &&
               users->user[i].password[0] == '\0' && users->user[i].password[1] == '\0')
            {
                strcpy_s(users->user[i].nickname, sizeof(users->user[i].nickname), ui->loginLineEdit->text().toStdString().c_str());
                strcpy_s(users->user[i].password, sizeof(users->user[i].password), ui->passwordLineEdit->text().toStdString().c_str());
                done = true;
                ui->statusbar->showMessage("Sign up is complete!");
                break;
            }
        }
        if(!done){
            ui->statusbar->showMessage("Server is overload!");
        }
    }
    else
    {
        ui->statusbar->showMessage("Connect error");
    }
}

void MainWindow::reconnect()
{
    fileUsersLog = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        L"UsersLog");

    if(fileUsersLog == NULL)
    {
        connected = false;
    }else{
        connected = true;
        timer->stop();

        users = (UsersLog*)MapViewOfFile(
            fileUsersLog,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(UsersLog));
    }
}

