#include "networkoptionwindow.h"
#include "ui_networkoptionwindow.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QHostAddress>


NetworkOptionWindow::NetworkOptionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkOptionWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("TMSClient"));
    ui->hostAddressLabel->setAlignment(Qt::AlignRight);
    ui->portNumberLabel->setAlignment(Qt::AlignRight);
    ui->locationLabel->setAlignment(Qt::AlignRight);
    ui->hostAddressLineEdit->setInputMask("000.000.000.000;");
    closeFlag = false;

}

NetworkOptionWindow::~NetworkOptionWindow()
{
    delete ui;
}

void NetworkOptionWindow::afterHostFound()
{
    QMessageBox::information(this, "TMSClient", "HostFound", QMessageBox::Ok);
    tcpSocket->disconnectFromHost();
    closeFlag = true;
    emit networkOptionWindowClose(1);
}

void NetworkOptionWindow::afterHostNotFound()
{
    QMessageBox::warning(this, tr("TMSClient"),
                         tr("Can not connect to the specified server!"),
                         QMessageBox::Ok);
}

void NetworkOptionWindow::closeEvent(QCloseEvent *event)
{
    if(closeFlag){
        event->accept();
    }else
        event->ignore();
}


void NetworkOptionWindow::on_closeButton_clicked()
{
    closeFlag = true;
    emit networkOptionWindowClose(0);
}

void NetworkOptionWindow::on_connectButton_clicked()
{
    if(ui->hostAddressLineEdit->text() == "..."){
        QMessageBox::warning(this, tr("Warining!"),
                             tr("Host Address should not be empty!"),
                             QMessageBox::Ok);
    }else{
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, SIGNAL(connected()), this, SLOT(afterHostFound()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(afterHostNotFound()));
        connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));
        QHostAddress ad(ui->hostAddressLineEdit->text());
        address = ad;
        portNumber = ui->portNumberCombo->currentText().toInt();
        location = ui->locationCombo->currentText();
        tcpSocket->connectToHost(ad, portNumber);
    }
}


