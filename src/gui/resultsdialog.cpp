#include "resultsdialog.h"
#include "ui_resultsdialog.h"

ResultsDialog::ResultsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultsDialog)
{
    ui->setupUi(this);
}

ResultsDialog::~ResultsDialog()
{
    delete ui;
}
