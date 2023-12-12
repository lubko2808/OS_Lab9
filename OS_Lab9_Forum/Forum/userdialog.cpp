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
    // Встановлюємо інтервал таймера в мілісекундах (наприклад, кожні 1000 мс, тобто 1 секунда)
    reconnectTimer->setInterval(5000);
    reconnectTimer->start(); // Розпочинаємо роботу таймера

    checkNewMessagesTimer = new QTimer(this);
    connect(checkNewMessagesTimer, SIGNAL(timeout()), this, SLOT(pullMessages()));
    // Встановлюємо інтервал таймера в мілісекундах (наприклад, кожні 1000 мс, тобто 1 секунда)
    checkNewMessagesTimer->setInterval(1000);
    checkNewMessagesTimer->start(); // Розпочинаємо роботу таймера

    connect(ui->textEdit, &QTextEdit::textChanged, this, &UserDialog::onTextChanged);

    lastPulledMessage = 1;
    hasMessages = 0;
    ui->tabWidget->setCurrentWidget(ui->tab_2);
    ui->tabWidget->tabBar()->hide();

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
        QString messageWithNickname = QString(users->user[id].nickname);
        messageWithNickname+=":\n";
        messageWithNickname+=ui->textEdit->toPlainText();
        messageWithNickname+='\0';
        strcpy(users->user[id].messages, messageWithNickname.toStdString().c_str());
        users->user[id].messages[MaxMessageWithUserLength-1] = '\0';
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
            QString message = QString(messages->message[lastPulledMessage]);

            if(messages->correct[lastPulledMessage] == true)
            {
                int pos = message.indexOf(':'); // Знаходимо позицію першого ':'

                if (pos != -1)
                {
                    // Формуємо з жирним текстом до ':' та звичайним текстом після ':'
                    QString formattedMessage = "<b>" + message.left(pos + 1) + "</b>" + message.mid(pos + 1);

                    ui->textBrowser->append(formattedMessage);
                }
                else {
                    // Якщо ':' не знайдено, виводимо повідомлення без змін
                    ui->textBrowser->append(message);
                }
            }
            else
            {
                ui->textBrowser->append(QString("Message was blocked!"));
            }

            lastPulledMessage++;
        }
        if(!hasMessages){
            QString text = ui->textBrowser->toPlainText();
            if(!text.isEmpty()){
                ui->tabWidget->setCurrentWidget(ui->tab);
                hasMessages = 1;
            }
        }

    }
}

void UserDialog::onTextChanged()
{
    // Обмеження на кількість символів ()

    QString text = ui->textEdit->toPlainText();

    // Перевірка та скорочення тексту, якщо необхідно
    if (text.length() > MaxMessageLength) {
        text.truncate(MaxMessageLength);
        ui->textEdit->setPlainText(text);
        QMessageBox::information(this, "Error", "Text contains more than 199 symbols");
    }
    /*bool isRight = 1;
    QString copytext;
    for (QChar symb: text){
        if(symb.isLetter() || symb.isDigit() || symb.isPunct() || symb.isSpace())
            copytext+=symb;
        else
            isRight =0;
    }
    if(!isRight){
        QMessageBox::information(this, "Error", "Not allowed symbols in message");
        ui->textEdit->setPlainText(copytext);
    }*/
}

