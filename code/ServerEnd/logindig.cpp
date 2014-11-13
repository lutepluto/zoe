/*Login**********************************************************************************/
#include <QtGui>
#include <QtSql>
#include "logindig.h"
#include "ui_logindig.h"
#include "connection.h"

LoginDig::LoginDig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDig)
{
    ui->setupUi(this);
    this->setStyleSheet("LoginDig{background-color:#FFFFFF;border-style:solid;border-width:1px;border-color:black;};");
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->logoLabel->setStyleSheet("QLabel{background-image:url(image/logo.jpg)};");
}

LoginDig::~LoginDig()
{
    delete ui;
}

void LoginDig::on_LoginButton_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT userAccount,userPass FROM Userinfo WHERE userAccount=?");
    QString userAccount= ui->UserlineEdit->text();
    query.addBindValue(userAccount);
    query.exec();
    query.next();
    QString account=query.value(0).toString();
    QString password=query.value(1).toString();

    if(ui->UserlineEdit->text()==NULL&&ui->pwdlineEdit->text()==NULL)
     {
        QMessageBox::warning(this,tr("warning"),tr("Please input user name and password!"),QMessageBox::Yes);
      }
    else if(ui->UserlineEdit->text()==account&&ui->pwdlineEdit->text()== password)
     {
        accept();
      }
    else
    {
        QMessageBox::warning(this,tr("warning"),tr("User name or password error"),QMessageBox::Yes);
        ui->UserlineEdit->clear();
        ui->pwdlineEdit->clear();
        ui->UserlineEdit->setFocus();
    }
}

void LoginDig::mousePressEvent(QMouseEvent *event)
{
    relativePos=this->pos()-event->globalPos();
}

void LoginDig::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+ relativePos);
}