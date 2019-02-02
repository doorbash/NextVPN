#include "alertdialog.h"
#include "ui_alertdialog.h"

AlertDialog::AlertDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertDialog)
{
    ui->setupUi(this);
    this->setStyleSheet(QString("#txtMsg{width: 400px;text-align:center; }#pushButton_2{color:blue;background-color:transparent;}"));
    this->setWindowTitle("NextVPN");
}

AlertDialog::~AlertDialog()
{
    delete ui;
}

void AlertDialog::on_pushButton_clicked()
{

}

void AlertDialog::setMsg(const QString &msg)
{
    ui->txtMsg->setText(msg);
}
void AlertDialog::setType(bool msg,const QString& Btn){
    if(msg){
        ui->pushButton->setText("Close");
        ui->pushButton_2->setVisible(false);
    }else{
        ui->pushButton->setText(Btn);
        ui->pushButton_2->setVisible(true);

    }
}
