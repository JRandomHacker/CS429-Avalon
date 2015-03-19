#ifndef VOTEDIALOG_H
#define VOTEDIALOG_H

#include <QDialog>

namespace Ui {
class VoteDialog;
}

class VoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VoteDialog(QWidget *parent = 0);
    ~VoteDialog();

private:
    Ui::VoteDialog *ui;
};

#endif // VOTEDIALOG_H
