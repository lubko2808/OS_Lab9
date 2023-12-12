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
    // Встановлюємо інтервал таймера в мілісекундах (наприклад, кожні 1000 мс, тобто 1 секунда)
    timer->setInterval(5000);
    timer->start(); // Розпочинаємо роботу таймера


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
        ui->statusbar->showMessage("Searching user in list...");
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
        }

        if(!logIn)
        {
            ui->statusbar->showMessage("No such user exists");
        }

    }
    else
    {
        ui->statusbar->showMessage("Connection error");
    }
}


void MainWindow::on_signUpButton_clicked()
{
    bool SignUp = true;

    // Пошук імені у каталозі
    for(int i = 0; i < MaxClients; i++)
    {
        if(strncmp(users->user[i].nickname, ui->loginLineEdit->text().toStdString().c_str(), 60) == 0)
        {
            SignUp = 0;
            break;
        }
    }
    if(!SignUp)
    {
        QMessageBox::information(this, "Error", "User with this name already exists");
    }

    bool isEmpty = false;
    // Перевірка довжини пароля
    if(SignUp)
        if(ui->passwordLineEdit->text().size() < 5){
            isEmpty = true;
            QMessageBox::information(this, "Error", "Password should contain more than 4 symbols");
        }

    if(connected == true && SignUp && !isEmpty)
    {
        bool done = false;
        ui->statusbar->showMessage("Adding new user in list...");
        for(int i = 0; i < MaxClients; i++)
        {
            if(users->user[i].nickname[0] == '\0' && users->user[i].nickname[1] == '\0' &&
                users->user[i].password[0] == '\0' && users->user[i].password[1] == '\0')
            {
                strcpy_s(users->user[i].nickname, sizeof(users->user[i].nickname), ui->loginLineEdit->text().toStdString().c_str());
                strcpy_s(users->user[i].password, sizeof(users->user[i].password), ui->passwordLineEdit->text().toStdString().c_str());
                done = true;
                users->user[i].inUse = true;
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
        if(SignUp && !isEmpty)
            ui->statusbar->showMessage("Connection error");
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


void MainWindow::on_loginLineEdit_textChanged(const QString &arg1)
{
    QString copy = arg1;
    if (copy.length() > 57) {
        copy.truncate(57);
        ui->loginLineEdit->setText(copy);
        QMessageBox::information(this, "Error", "Login contains more than 57 symbols");
    }
    copy.clear();
    bool isRight = 1;
    for(QChar symb: arg1){
        if((symb.isLetter() || symb.isDigit() || symb.isPunct()|| symb.isSpace())&& symb!=':')
            copy.append(symb);
        else
            isRight =0;
    }
    if(!isRight){
        QMessageBox::information(this, "Error", "Not allowed symbols in nickname");
        ui->loginLineEdit->setText(copy);
    }
}


void MainWindow::on_passwordLineEdit_textChanged(const QString &arg1)
{
    // Обмеження на кількість символів ()
    QString copytext = arg1;
    if (copytext.length() > 60) {
        copytext.truncate(60);
        ui->passwordLineEdit->setText(copytext);
        QMessageBox::information(this, "Error", "Text contains more than 50 symbols");
    }
    copytext.clear();
    bool isRight = 1;
    for (QChar symb: arg1){
        if(symb!='\n')
            copytext+=symb;
        else
            isRight =0;
    }
    if(!isRight){
        QMessageBox::information(this, "Error", "Not allowed symbols in message");
        ui->passwordLineEdit->setText(copytext);
    }
}

