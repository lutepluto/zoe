#include <QtGui>
#include <QtSql>
#include <QDateTime>
#include "server_end.h"
#include "ui_server_end.h"
#include "connection.h"
#include "recognitionsystem.h"

Server_end::Server_end(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server_end)
{
    ui->setupUi(this);
    this->setStyleSheet("Server_end{background-color:#FCFCFC;border-style:solid;border-color:black;border-width:1px;};");
    ui->logoLabel->setStyleSheet("QLabel{background-image:url(image/logo.jpg)};");
    ui->RunningIfo->setStyleSheet("QTextEdit{background-color:#F0F0F0}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->PortLine->setFocus();

}

Server_end::~Server_end()
{
    delete ui;
}

//开始服务
void Server_end::on_StartButton_clicked()
{
    //这里可加入监听的代码（需要加代码！）
    QDateTime currentTime = QDateTime::currentDateTime();
    tcpServer = new TcpServer(this);
    qint16 portNumber = qint16(ui->PortLine->text().toInt());
    if(portNumber < 1025){
        ui->RunningIfo->append(currentTime.toString("yyyy-mm-dd hh:mm:ss") +
                               tr("    Error: port number should be in the interval of [1025, 65535]\n"));
        return;
    }else if(!tcpServer->startServer(portNumber)){
        ui->RunningIfo->append(currentTime.toString("yyyy-mm-dd hh:mm:ss") +
                               tr("    Error: Server fails to start service.\n"));
    }else{
        ui->RunningIfo->append(currentTime.toString("yyyy-mm-dd hh:mm:ss") +
                               tr("    Server starts successfully.\n"));
    }

}

void Server_end::on_StopButton_clicked()
{
    tcpServer->stopServer();
    QDateTime currentTime = QDateTime::currentDateTime();
    ui->RunningIfo->append(currentTime.toString("yyyy-mm-dd hh:mm:ss") +
                           tr("    Service Stopped!\n"));
}

//获取车辆的图片
void Server_end::on_WaitingTable_clicked()
{   //get current index;
    int curFile=ui->WaitingTable->currentIndex().row();
    QModelIndex indexFile=FileModel->index(curFile,0,QModelIndex());
    QString str_indexFile;
    str_indexFile=indexFile.data().toString();

    //read current picture;
    QSqlQuery carPicQuery;
    carPicQuery.prepare("SELECT VehicleImage FROM TrafficPic WHERE traffic_id=?");
    carPicQuery.addBindValue(str_indexFile);
    carPicQuery.exec();
    carPicQuery.next();
    QString car_pic=carPicQuery.value(0).toString();
    ui->VehicleImage->setPixmap(car_pic);
}

void Server_end::on_RecogniseButton_clicked()
{
   //get current index;
    int curFile=ui->WaitingTable->currentIndex().row();
    QModelIndex indexFile=FileModel->index(curFile,0,QModelIndex());
    QString str_indexFile;
    str_indexFile=indexFile.data().toString();

    //read current picture;
    QSqlQuery carPicQuery;
    carPicQuery.prepare("SELECT VehicleImage FROM TrafficPic WHERE traffic_id=?");
    carPicQuery.addBindValue(str_indexFile);
    carPicQuery.exec();
    carPicQuery.next();
    QString car_pic=carPicQuery.value(0).toString();

    RecognitionSystem *option=new RecognitionSystem(car_pic);
    connect(option, SIGNAL(recognitionSuccess(qint16)), this, SLOT(picStateChange(qint16)));
    option->show();
}

void Server_end::on_SuccessTable_clicked()
{
    QSqlQuery TrafficInfoQuery;
    QSqlQuery carNumQuery;

    //get current index;
    int curFile=ui->SuccessTable->currentIndex().row();
    QModelIndex indexFile=SuccessModel->index(curFile,0,QModelIndex());
    QString str_indexFile;
    str_indexFile=indexFile.data().toString();


    QSqlQuery carPicQuery;
    carPicQuery.prepare("SELECT VehicleImage FROM TrafficPic WHERE traffic_id=?");
    carPicQuery.addBindValue(str_indexFile);
    carPicQuery.exec();
    carPicQuery.next();
    QString car_pic=carPicQuery.value(0).toString();
    ui->VehicleImage->setPixmap(car_pic);


    //read current car_information;
    TrafficInfoQuery.prepare("SELECT ViolatingTime,traffic_id FROM TrafficPic WHERE traffic_id=?");
    TrafficInfoQuery.addBindValue(str_indexFile);
    TrafficInfoQuery.exec();
    TrafficInfoQuery.next();
    QString license_pic="C:/plate_output/test_output.bmp";
    QString violating_time=TrafficInfoQuery.value(0).toString();
    QString traffic_id=TrafficInfoQuery.value(1).toString();
    ui->LicenseImage->setPixmap(license_pic);
    ui->TimeLine->setText(violating_time);
    ui->ViolatingIDLine->setText(traffic_id);

    carNumQuery.prepare("SELECT car_num FROM Car WHERE car_id in (SELECT car_id FROM TrafficPic WHERE traffic_id=?)");
    carNumQuery.addBindValue(str_indexFile);
    carNumQuery.exec();
    carNumQuery.next();
    QString car_num=carNumQuery.value(0).toString();
    ui->LicenseLine->setText(car_num);

}

void Server_end::on_FailTable_clicked()
{
    QSqlQuery TrafficInfoQuery;
    QSqlQuery carNumQuery;

    //get current index;
    int curFile=ui->FailTable->currentIndex().row();
    QModelIndex indexFile=FailModel->index(curFile,0,QModelIndex());
    QString str_indexFile;
    str_indexFile=indexFile.data().toString();

    QSqlQuery carPicQuery;
    carPicQuery.prepare("SELECT VehicleImage FROM TrafficPic WHERE traffic_id=?");
    carPicQuery.addBindValue(str_indexFile);
    carPicQuery.exec();
    carPicQuery.next();
    QString car_pic=carPicQuery.value(0).toString();
    ui->VehicleImage->setPixmap(car_pic);

    //read current car_information;
    TrafficInfoQuery.prepare("SELECT ViolatingTime,traffic_id FROM TrafficPic WHERE traffic_id=?");
    TrafficInfoQuery.addBindValue(str_indexFile);
    TrafficInfoQuery.exec();
    TrafficInfoQuery.next();

    QString license_pic="C:/plate_output/test_output.bmp";
    QString violating_time=TrafficInfoQuery.value(0).toString();
    QString traffic_id=TrafficInfoQuery.value(1).toString();
    ui->LicenseImage->setPixmap(license_pic);
    ui->TimeLine->setText(violating_time);
    ui->ViolatingIDLine->setText(traffic_id);

    carNumQuery.prepare("SELECT car_num FROM Car WHERE car_id in (SELECT car_id FROM TrafficPic WHERE traffic_id=?)");
    carNumQuery.addBindValue(str_indexFile);
    carNumQuery.exec();
    carNumQuery.next();
    QString car_num=carNumQuery.value(0).toString();
    ui->LicenseLine->setText(car_num);
}


//点击submit，获取信息
/*
void Server_end::on_SubmitButton_2_clicked()
{
    QSqlQuery TrafficInfoQuery;
    QSqlQuery carNumQuery;

    //get current index;
    int curFile=ui->WaitingTable->currentIndex().row();
    QModelIndex indexFile=FileModel->index(curFile,0,QModelIndex());
    QString str_indexFile;
    str_indexFile=indexFile.data().toString();

    //read current car_information;
    TrafficInfoQuery.prepare("SELECT LicenseImage,ViolatingTime,traffic_id FROM TrafficPic WHERE traffic_id=?");
    TrafficInfoQuery.addBindValue(str_indexFile);
    TrafficInfoQuery.exec();
    TrafficInfoQuery.next();
    QString license_pic=TrafficInfoQuery.value(0).toString();
    QString violating_time=TrafficInfoQuery.value(1).toString();
    QString traffic_id=TrafficInfoQuery.value(2).toString();
    ui->LicenseImage->setPixmap(license_pic);
    ui->TimeLine->setText(violating_time);
    ui->ViolatingIDLine->setText(traffic_id);

    carNumQuery.prepare("SELECT car_num FROM Car WHERE car_id in (SELECT car_id FROM TrafficPic WHERE traffic_id=?)");
    carNumQuery.addBindValue(str_indexFile);
    carNumQuery.exec();
    carNumQuery.next();
    QString car_num=carNumQuery.value(0).toString();
    ui->LicenseLine->setText(car_num);

    ui->RunningIfo->append("Get the information about violating vehicle...");
}*/

//点击Query，获取违规车辆的信息
void Server_end::on_QueryButton_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT traffic_id,car_num,ViolatingTime,driver_name,driver_id_number,driver_email FROM trafficPic,Car,driver WHERE trafficPic.car_id=Car.car_id AND Car.driver_id=driver.driver_id");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("Violating ID"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("License plate No."));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Violating time"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("Name"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("ID card No."));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("E-mail"));
    ui->ViolatingList->setModel(model);

    ui->SubmitButton->setEnabled(true);
    ui->DeleteButton->setEnabled(true);
    ui->EmptyButton->setEnabled(true);
    ui->SubmitButton->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
    ui->DeleteButton->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
    ui->EmptyButton->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
    ui->RunningIfo->append("Get the violating vehicle list...");
}

//删除操作
void Server_end::on_DeleteButton_clicked()
{
    int curRow=ui->ViolatingList->currentIndex().row();
    QModelIndex index=model->index(curRow,0,QModelIndex());
    QString cur_index;
    cur_index =index.data().toString();
    qDebug();
    QSqlQuery DeleteQuery=model->query();
    DeleteQuery.prepare("DELETE FROM trafficPic WHERE traffic_id=?");
    DeleteQuery.addBindValue(cur_index);
    DeleteQuery.exec();
    model->setQuery("SELECT traffic_id,car_num,ViolatingTime,driver_name,driver_id_number,driver_email FROM trafficPic,Car,driver WHERE trafficPic.car_id=Car.car_id AND Car.driver_id=driver.driver_id");
    ui->ViolatingList->setModel(model);
}

//清空操作
void Server_end::on_EmptyButton_clicked()
{
    int EmptyMessage=QMessageBox::warning(this,tr("Empty"),tr("Do you want to empty the violating vehicle list?"),QMessageBox::Yes,QMessageBox::No);
    if(EmptyMessage==QMessageBox::Yes)
    {
        model->setQuery("DELETE FROM trafficPic");
        ui->ViolatingList->setModel(model);
        ui->RunningIfo->append("Empty all the data in violating vehicle list");
    }
    else
        model->revert();
        ui->RunningIfo->append("Revert the empty operation...");
}

//查询顺序操作
void Server_end::on_SubmitButton_clicked()
{
    if(ui->IDMethod->isChecked())
    {
       model->setQuery("SELECT traffic_id,car_num,ViolatingTime,driver_name,driver_id_number,driver_email FROM trafficPic,Car,driver WHERE trafficPic.car_id=Car.car_id AND Car.driver_id=driver.driver_id ORDER BY driver_id_number");
       ui->ViolatingList->setModel(model);
       ui->RunningIfo->append("Order by the driver's ID No.");
    }
    else if(ui->NameMethod->isChecked())
    {
       model->setQuery("SELECT traffic_id,car_num,ViolatingTime,driver_name,driver_id_number,driver_email FROM trafficPic,Car,driver WHERE trafficPic.car_id=Car.car_id AND Car.driver_id=driver.driver_id ORDER BY driver_name");
       ui->ViolatingList->setModel(model);
       ui->RunningIfo->append("Order by the driver's name");
    }
    else if(ui->LicenseMethod->isChecked())
    {
        QSqlQuery InputQuery=model->query();
        InputQuery.prepare("SELECT traffic_id,car_num,ViolatingTime,driver_name,driver_id_number,driver_email FROM trafficPic,Car,driver WHERE trafficPic.car_id=Car.car_id AND Car.driver_id=driver.driver_id AND car_num=?");
        QString car_num=ui->LicenseNumLine->text();
        InputQuery.addBindValue(car_num);
        InputQuery.exec();
        InputQuery.next();
        QString LicenseNum=InputQuery.value(1).toString();
        if(ui->LicenseNumLine->text()==NULL)
        {
         QMessageBox::warning(this,tr("Attention Please"),tr("Please input the license plate No.!"),QMessageBox::Yes);
        }
        else if(ui->LicenseNumLine->text()==LicenseNum)
        {
          model->setQuery(InputQuery);
          ui->ViolatingList->setModel(model);
          ui->RunningIfo->append("Get the information!");
        }
        else if(ui->LicenseNumLine->text()!=LicenseNum)
            QMessageBox::warning(this,tr("Sorry"),tr("Can not find related information"),QMessageBox::Yes);
    }

    else if(ui->TimeMethod->isChecked())
    {
        QSqlQuery TimeQuery=model->query();
        TimeQuery.prepare("SELECT traffic_id,car_num,ViolatingTime,driver_name,driver_id_number,driver_email FROM trafficPic,Car,driver WHERE trafficPic.car_id=Car.car_id AND Car.driver_id=driver.driver_id AND ViolatingTime > ? AND  ViolatingTime < ? ORDER BY ViolatingTime");
        QString from=ui->dateTime->dateTime().toString("yyyy/MM/dd HH:mm:ss");
        QString to=ui->dateTime_2->dateTime().toString("yyyy/MM/dd HH:mm:ss");
        TimeQuery.addBindValue(from);
        TimeQuery.addBindValue(to);
        TimeQuery.exec();
        TimeQuery.next();
        model->setQuery(TimeQuery);
        ui->ViolatingList->setModel(model);
    }
 }

//退出操作
void Server_end::on_ExitButton_clicked()
{
    int ExitMessage=QMessageBox::question(this,tr("EXIT"),tr("Do you want to exit the traffic monitoring system?"),QMessageBox::Yes,QMessageBox::No);
    if(ExitMessage==QMessageBox::Yes)
    {
        this->close();
    }
    else
    {
        ui->RunningIfo->append("Revert the exit opreation...");
    }
}

//移动窗口位置
void Server_end::mousePressEvent(QMouseEvent *event)
{
    relativePos=this->pos()-event->globalPos();
}

void Server_end::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+ relativePos);
}
//最小化
void Server_end::on_minimizeButton_clicked()
{
    this->showMinimized();
}

//********************************************************************
void Server_end::imageFinished(QString name, QString loc)
{
    QSqlQuery TrafficIDQuery;
    TrafficIDQuery.exec("SELECT MAX(traffic_id) FROM TrafficPic");
    TrafficIDQuery.next();
    int count_traffic=TrafficIDQuery.value(0).toInt();

    int traffic_id;
    traffic_id=count_traffic+1;


    //得到的文件名（需要修改此处代码！）

    QString fileLoc = name;
    QString location = loc;
    QDateTime currentTime = QDateTime::currentDateTime();
    QString time=currentTime.toString("yyyy/MM/dd HH:mm:ss");



    //将违规ID，收到的图片加入数据库
    QSqlQuery LocationQuery;
    LocationQuery.prepare("INSERT INTO trafficPic(traffic_id,VehicleImage,ViolatingTime,Location,picState) VALUES (?,?,?,?,?)");
    LocationQuery.addBindValue(traffic_id);
    LocationQuery.addBindValue(fileLoc);
    LocationQuery.addBindValue(time);
    LocationQuery.addBindValue(location);
    LocationQuery.addBindValue(0);
    LocationQuery.exec();
    ui->RunningIfo->append("Insert successful");

    //此处创建了waiting那部分的表格
    FileModel = new QSqlQueryModel();
    FileModel->setQuery("SELECT traffic_id,VehicleImage,ViolatingTime,Location FROM trafficPic WHERE picState = 0");
    FileModel->setHeaderData(0,Qt::Horizontal,QObject::tr("ViolatingID"));
    FileModel->setHeaderData(1,Qt::Horizontal,QObject::tr("File path"));
    FileModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Violating Time"));

    ui->WaitingTable->setModel(FileModel);
    ui->RecogniseList->setCurrentIndex(0);
   // ui->SubmitButton_2->setEnabled(true);
    //ui->SubmitButton_2->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");


}

void Server_end::picStateChange(qint16 flag)
{
    if(flag == 0){//Recognition failed

        FileModel->setQuery("SELECT traffic_id,VehicleImage,ViolatingTime,Location FROM trafficPic WHERE picState = 0");
        FileModel->setHeaderData(0,Qt::Horizontal,QObject::tr("ViolatingID"));
        FileModel->setHeaderData(1,Qt::Horizontal,QObject::tr("File path"));
        FileModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Violating Time"));

        ui->WaitingTable->setModel(FileModel);

        FailModel = new QSqlQueryModel();
        FailModel->setQuery("SELECT traffic_id,VehicleImage,ViolatingTime,Location FROM trafficPic WHERE picState = 2");
        FailModel->setHeaderData(0,Qt::Horizontal,QObject::tr("ViolatingID"));
        FailModel->setHeaderData(1,Qt::Horizontal,QObject::tr("File path"));
        FailModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Violating Time"));

        ui->FailTable->setModel(FailModel);
        ui->RecogniseList->setCurrentIndex(2);
    }else{//Recognition successfully

        FileModel->setQuery("SELECT traffic_id,VehicleImage,ViolatingTime,Location FROM trafficPic WHERE picState = 0");
        FileModel->setHeaderData(0,Qt::Horizontal,QObject::tr("ViolatingID"));
        FileModel->setHeaderData(1,Qt::Horizontal,QObject::tr("File path"));
        FileModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Violating Time"));

        ui->WaitingTable->setModel(FileModel);
        SuccessModel = new QSqlQueryModel();
        SuccessModel->setQuery("SELECT traffic_id,VehicleImage,ViolatingTime,Location FROM trafficPic WHERE picState = 1");
        SuccessModel->setHeaderData(0,Qt::Horizontal,QObject::tr("ViolatingID"));
        SuccessModel->setHeaderData(1,Qt::Horizontal,QObject::tr("File path"));
        SuccessModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Violating Time"));
        ui->SuccessTable->setModel(SuccessModel);
        ui->RecogniseList->setCurrentIndex(1);
    }
}


