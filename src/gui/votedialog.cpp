#include "votedialog.h"
#include "ui_votedialog.h"

VoteDialog::VoteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoteDialog)
{
    ui->setupUi(this);
}

VoteDialog::~VoteDialog()
{
    delete ui;
}
