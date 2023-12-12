#ifndef MODERDIALOG_H
#define MODERDIALOG_H

#include <QDialog>

namespace Ui {
class ModerDialog;
}

class ModerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModerDialog(QWidget *parent = nullptr);
    ~ModerDialog();

private:
    Ui::ModerDialog *ui;
};

#endif // MODERDIALOG_H
