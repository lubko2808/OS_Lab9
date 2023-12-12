#include "moderdialog.h"
#include "ui_moderdialog.h"

ModerDialog::ModerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModerDialog)
{
    ui->setupUi(this);
}

ModerDialog::~ModerDialog()
{
    delete ui;
}
