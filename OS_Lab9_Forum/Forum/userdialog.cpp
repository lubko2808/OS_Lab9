#include "userdialog.h"
#include "ui_userdialog.h"

UserDialog::UserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    setFixedSize(1000, 600);
    setWindowTitle("Forum");


    reconnectTimer = new QTimer(this);
    connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(reconnectToServer()));
    // Встановіть інтервал таймера в мілісекундах (наприклад, кожні 1000 мс, тобто 1 секунда)
    reconnectTimer->setInterval(5000);
    reconnectTimer->start(); // Розпочніть роботу таймера

    checkNewMessagesTimer = new QTimer(this);
    connect(checkNewMessagesTimer, SIGNAL(timeout()), this, SLOT(pullMessages()));
    // Встановіть інтервал таймера в мілісекундах (наприклад, кожні 1000 мс, тобто 1 секунда)
    checkNewMessagesTimer->setInterval(1000);
    checkNewMessagesTimer->start(); // Розпочніть роботу таймера

    connect(ui->textEdit, &QTextEdit::textChanged, this, &UserDialog::onTextChanged);

    lastPulledMessage = 1;

    fileMessagesLog = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        L"MessagesLog");


    if(fileMessagesLog == NULL)
    {
        connected = false;
    }else{
        connected = true;

        messages = (MessagesLog*)MapViewOfFile(
            fileMessagesLog,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(MessagesLog));

        ui->topicLabel->setText(QString(messages->message[0]));
    }


}

UserDialog::~UserDialog()
{
    delete ui;
}

void UserDialog::takeUsers(UsersLog *usersLog)
{
    users = usersLog;
}

void UserDialog::takeId(int userId)
{
    id = userId;
}

void UserDialog::on_pushButton_clicked()
{
    if(ui->textEdit->toPlainText() != "\0\0")
    {
        while(users->user[id].signal)
        {
            Sleep(1000);
        }

        strcpy_s(users->user[id].messages, 200, ui->textEdit->toPlainText().toStdString().c_str());
        users->user[id].messages[199] = '\0';
        users->user[id].signal = true;

        ui->textEdit->setText("\0\0");
    }
}

void UserDialog::reconnectToServer()
{
    if(connected == false)
    {
        fileMessagesLog = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            L"MessagesLog");

        if(fileMessagesLog == NULL)
        {
            connected = false;
            QMessageBox::critical(this, "Error", "Connection is lost");
            return;

        }else{
            connected = true;

            messages = (MessagesLog*)MapViewOfFile(
                fileMessagesLog,
                FILE_MAP_ALL_ACCESS,
                0,
                0,
                sizeof(MessagesLog));
            ui->topicLabel->setText(QString(messages->message[0]));
        }
    }
}

void UserDialog::pullMessages()
{
    if(connected == true)
    {
        while(messages->message[lastPulledMessage][0] != '\0' && messages->message[lastPulledMessage][1] != '\0')
        {
            if(messages->canBeRead[lastPulledMessage] == false)
            {
                Sleep(1000);
            }

            if(messages->correct[lastPulledMessage] == true)
            {
                ui->textBrowser->append(QString(messages->message[lastPulledMessage]));

            }
            else
            {
                ui->textBrowser->append(QString("Message was blocked!"));
            }

            lastPulledMessage++;
        }

    }
}

void UserDialog::onTextChanged()
{
    // Обмеження на кількість символів (наприклад, 10)
    const int maxLength = 199;
    QString text = ui->textEdit->toPlainText();

    // Перевірка та скорочення тексту, якщо необхідно
    if (text.length() > maxLength) {
        text.truncate(maxLength);
        ui->textEdit->setPlainText(text);
    }

}

