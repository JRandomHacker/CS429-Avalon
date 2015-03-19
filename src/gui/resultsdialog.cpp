#include "resultsdialog.hpp"
#include "ui_resultsdialog.h"

ResultsDialog::ResultsDialog(QWidget *parent, bool passed) :
    QDialog(parent),
    ui(new Ui::ResultsDialog)
{
    ui->setupUi(this);
    if( passed ) {
        ui->resultsLabel->setText( QString( "PASSED" ) );
    } else {
        ui->resultsLabel->setText( QString( "FAILED" ) );
    }

    connect( ui->acceptButton, SIGNAL( clicked( ) ), this, SLOT( close( ) ) );
}

ResultsDialog::~ResultsDialog()
{
    delete ui;
}
