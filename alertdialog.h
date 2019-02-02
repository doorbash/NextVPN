#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include <QDialog>

namespace Ui {
class AlertDialog;
}

class AlertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertDialog(QWidget *parent = 0);
    ~AlertDialog();

public slots:
    void setType(bool msg, const QString &Btn = QString("Extend"));
        void setMsg(const QString& msg);
private slots:
    void on_pushButton_clicked();


private:
    Ui::AlertDialog *ui;
};

#endif // ALERTDIALOG_H
