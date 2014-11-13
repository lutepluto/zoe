#include "clientwindow.h"
#include "ui_clientwindow.h"

#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>

ClientWindow::ClientWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("ClientWindow{background-color:qlineargradient(spread:pad, x1:0.761364, y1:0.08, x2:1, y2:1, stop:0 rgba(0, 78, 152, 255), stop:1 rgba(136, 255, 255, 255));}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    option = new NetworkOptionWindow(this);


    connect(option, SIGNAL(networkOptionWindowClose(qint32)),
            this, SLOT(whenNetworkOptionWindowClose(qint32)));
    option->setModal(true);

    //********************UI Setting****************
    ui->fileTableWidget->setColumnWidth(0, 100);
    ui->fileTableWidget->setColumnWidth(1, 320);
    ui->fileTableWidget->setColumnWidth(2, 110);
    ui->fileTableWidget->setColumnWidth(3, 119);

    ui->stopButton->setEnabled(false);
    setWindowTitle(tr("TMSClient"));

    //***********************************************

    //******************File Transfer****************

    sendBytes = 0;
    blockNumber = 0;
    maxBytes = 0;

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    //***********************************************
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::on_browseButton_clicked()
{
    QStringList fileList = QFileDialog::getOpenFileNames(this, tr("Opne Files"),
                                                         "C:/Users/Public/Pictures/Sample Pictures",
                                                         tr("Images(*.png *.jpg *.bmp"));
    QStringList::Iterator iterator;
    int rowNumber = 0;
    QString fileName;
    QString filePath;
    for(iterator = fileList.begin(); iterator != fileList.end(); iterator++){
        ui->fileTableWidget->insertRow(rowNumber);
        filePath = fileList.value(rowNumber);
        ui->fileTableWidget->setItem(rowNumber, 1, new QTableWidgetItem(QString(filePath)));
        fileName = filePath.right(filePath.size() - filePath.lastIndexOf('\\') - 1);
        ui->fileTableWidget->setItem(rowNumber, 0 , new QTableWidgetItem(QString(fileName)));
        ui->fileTableWidget->item(rowNumber, 0)->setCheckState(Qt::Unchecked);
        QFile file(filePath, this);
        ui->fileTableWidget->setItem(rowNumber, 2, new QTableWidgetItem(QString("%1KB").arg(file.size()/1024)));
        ui->fileTableWidget->setItem(rowNumber, 3, new QTableWidgetItem(QString("Ready To Send")));
        rowNumber++;
    }
}

void ClientWindow::on_sendButton_clicked()
{
    ui->stopButton->setEnabled(true);
    ui->stopButton->setStyleSheet("QPushButton{font: 9pt 'Calibri';background-color:rgb(47, 47, 47);color: rgb(255, 255, 255);}");

    for(int row = 0; row < ui->fileTableWidget->rowCount(); row++){
        if(ui->fileTableWidget->item(row, 0)->checkState() == Qt::Checked){

            sendBytes = 0;
            blockNumber = 0;
            maxBytes = 0;
            TcpClient *tcpClient = new TcpClient(this);
            connect(tcpClient, SIGNAL(disconnected()), tcpClient, SLOT(deleteLater()));
            connect(tcpClient, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));
            connect(tcpClient, SIGNAL(fileSize(qint64)), this, SLOT(setProcessBar(qint64)));
            //connect(tcpClient, SIGNAL(bytesWritten(qint64)), this, SLOT(updateProgressBar(qint64)));
            connect(tcpClient, SIGNAL(update(qint64)), this, SLOT(updateProgressBar(qint64)));
            connect(tcpClient, SIGNAL(onError(qint32)), this, SLOT(on_socketError(qint32)));
            connect(tcpClient, SIGNAL(message(QString, QString)), this, SLOT(updateStatus(QString, QString)));

            connect(ui->stopButton, SIGNAL(clicked()), tcpClient, SLOT(stopService()));

            tcpClient->sendFile(ui->fileTableWidget->item(row, 1)->text(), option->address, option->portNumber, option->location);
        }
    }

}

void ClientWindow::setProcessBar(qint64 r)
{
    maxBytes = r;
    ui->progressBar->setRange(0, 150);

}

void ClientWindow::updateProgressBar(qint64 v)
{
    blockNumber++;
    sendBytes += v;
    ui->progressBar->setValue(sendBytes);
    qDebug() << "sended number " << blockNumber << ", cur " << v << ". "
             << sendBytes << " bytes. total " << maxBytes << " bytes.";

}

void ClientWindow::updateStatus(const QString &status, QString file)
{
    QString st;
    st = status;
    for(int row = 0; row < ui->fileTableWidget->rowCount(); row++){
        if(ui->fileTableWidget->item(row, 1)->text() == file){
            ui->fileTableWidget->item(row, 3)->setText(st);
        }
    }
}

void ClientWindow::on_socketDisconnected()
{
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(100);

}

void ClientWindow::on_closeButton_clicked()
{
    if(QMessageBox::question(this, tr("TMSClient"),
                            tr("Do you really want to quit?"),
                            QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
        close();
}

void ClientWindow::on_socketError(qint32 e)
{
    switch(e)
    {
    case 1:
        QMessageBox::warning(this, tr("Error"),
                             QString::fromUtf8("Host Addrees Wrong!"),
                             QMessageBox::Close);
        break;
    case 2:
        QMessageBox::warning(this, tr("Error"),
                             QString::fromUtf8("File Path Wrong!"),
                             QMessageBox::Close);
        break;
    case 3:
        QMessageBox::warning(this, tr("Error"),
                             QString::fromUtf8("Connection Refused Error!"),
                             QMessageBox::Close);
    }
}

void ClientWindow::on_emptyButton_clicked()
{
    ui->fileTableWidget->setRowCount(0);
}

void ClientWindow::on_selectAllButton_clicked()
{
    for(int i = 0; i < ui->fileTableWidget->rowCount(); i++){
        ui->fileTableWidget->item(i, 0)->setCheckState(Qt::Checked);
    }
}

void ClientWindow::on_deleteButton_clicked()
{
    int rowCount = ui->fileTableWidget->rowCount();
    for(int row = 0; row < rowCount; row++){
        if(ui->fileTableWidget->item(row, 0)->checkState() == Qt::Checked){
            ui->fileTableWidget->removeRow(row);
            row--;
            rowCount = ui->fileTableWidget->rowCount();
        }
    }
}

void ClientWindow::on_selectInverseButton_clicked()
{
    for(int row = 0; row < ui->fileTableWidget->rowCount(); row++){
        if(ui->fileTableWidget->item(row, 0)->checkState() == Qt::Checked){
            ui->fileTableWidget->item(row, 0)->setCheckState(Qt::Unchecked);
        }else{
            ui->fileTableWidget->item(row, 0)->setCheckState(Qt::Checked);
        }
    }
}


void ClientWindow::whenNetworkOptionWindowClose(qint32 e)
{
    if(e == 0){
        option->close();
        close();
    }

    if(e == 1){
        option->close();
    }
}

void ClientWindow::mousePressEvent(QMouseEvent *event)
{
    relativePos=this->pos()-event->globalPos();
}

void ClientWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+ relativePos);
}

void ClientWindow::on_minimizeButton_clicked()
{
    this->showMinimized();
}
