#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

#include <QDialog>

namespace Ui {
class ResultsDialog;
}

class ResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultsDialog(QWidget *parent = 0);
    ~ResultsDialog();

private:
    Ui::ResultsDialog *ui;
};

#endif // RESULTSDIALOG_H
