#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QMessageBox>
#include <QTabBar>

#include "dataStructs.h"

namespace Ui {
class UserDialog;
}

class UserDialog : public QDialog
{
    Q_OBJECT
    bool hasMessages;
public:
    explicit UserDialog(QWidget *parent = nullptr);
    ~UserDialog();

    void takeUsers(UsersLog* usersLog);
    void takeId(int userId);


private slots:
    void on_pushButton_clicked();

    void reconnectToServer();
    void pullMessages();

    void onTextChanged();

private:
    Ui::UserDialog *ui;
    QTimer *reconnectTimer;
    QTimer *checkNewMessagesTimer;
    int lastPulledMessage;

    UsersLog *users;
    int id;

    HANDLE fileMessagesLog;
    MessagesLog *messages;
    bool connected; // змінна, яка визначає, чи вже
    // встановлено з'єднання із сервером
};

#endif // USERDIALOG_H
