#include <QtGui>
#include <QtSql>
#include <QString>
#include "connection.h"
#include "recognitionsystem.h"
#include "ui_recognitionsystem.h"



RecognitionSystem::RecognitionSystem(QString path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecognitionSystem)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("RecognitionSystem{background-color:white;border-style:solid;border-width:1px;border-color:black;}");
    ui->locateButton->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");

    //显示原始图像
    //QString ori_path;
    //this->ori_path.replace(0,ori_path);
    ori_path = path;
    QPixmap pixmap(ori_path);
    //QPixmap fitpix = pixmap.scaled(ui->Image->width(),ui->Image->height(),Qt::KeepAspectRatio);
    ui->Image->setPixmap(pixmap);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));


}

RecognitionSystem::~RecognitionSystem()
{
    delete ui;
}
//退出
void RecognitionSystem::on_exitButton_clicked()
{
    int ExitMessage=QMessageBox::question(this,tr("EXIT"),tr("Do you want to exit the recognition system?"),QMessageBox::Yes,QMessageBox::No);
    if(ExitMessage==QMessageBox::Yes)
    {
        this->close();
    }
}

//移动窗口
void RecognitionSystem::mousePressEvent(QMouseEvent *event)
{
    relativePos=this->pos()-event->globalPos();
}

void RecognitionSystem::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()+ relativePos);
}


void RecognitionSystem::on_locateButton_clicked()
{

    QProcess* myProc = new QProcess;
    QStringList ori_img;
    //const char* ori_imgpath;
    //ori_img << ori_imgpath;
    ori_img.append(ori_path);
    myProc->start("locate.exe",ori_img);
    myProc->terminate();
    myProc->waitForFinished(12000);

    const char* locate_img = "C:/plate_output/test_output.bmp";
    QPixmap locatePix(locate_img);
    ui->operateImage->setPixmap(locatePix);
    ui->cutButton->setEnabled(true);
    ui->cutButton->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
}

//class   temp{
//private:
//    QStringList temp::ori_path;
//}

void RecognitionSystem::on_cutButton_clicked()
{
    QProcess* myPrc2 = new QProcess;
    QStringList locate_img;
    locate_img.append("C:/plate_output/test_output.bmp");

    myPrc2->start("cut.exe",locate_img);
    myPrc2->terminate();
    myPrc2->waitForFinished(10000);

    const char* cut_pic1 = "C:/cut_charater_output/1.bmp";
    const char* cut_pic2 = "C:/cut_charater_output/2.bmp";
    const char* cut_pic3 = "C:/cut_charater_output/3.bmp";
    const char* cut_pic4 = "C:/cut_charater_output/4.bmp";
    const char* cut_pic5 = "C:/cut_charater_output/5.bmp";
    const char* cut_pic6 = "C:/cut_charater_output/6.bmp";
    const char* cut_pic7 = "C:/cut_charater_output/7.bmp";

    QPixmap pic1(cut_pic1);
    QPixmap pic2(cut_pic2);
    QPixmap pic3(cut_pic3);
    QPixmap pic4(cut_pic4);
    QPixmap pic5(cut_pic5);
    QPixmap pic6(cut_pic6);
    QPixmap pic7(cut_pic7);

    ui->plate1->setPixmap(pic1);
    ui->plate2->setPixmap(pic2);
    ui->plate3->setPixmap(pic3);
    ui->plate4->setPixmap(pic4);
    ui->plate5->setPixmap(pic5);
    ui->plate6->setPixmap(pic6);
    ui->plate7->setPixmap(pic7);

    ui->matchButton->setEnabled(true);
    ui->Binaryzation->setEnabled(true);
    ui->corrupt->setEnabled(true);
    ui->meanfilter->setEnabled(true);
    ui->matchButton->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
    ui->Binaryzation->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
    ui->corrupt->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
    ui->meanfilter->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
}

void RecognitionSystem::on_matchButton_clicked()
{
    QProcess* myProc3 = new QProcess;
    QFile file("c:/platestring_output.txt");

    myProc3->start("match.exe",QIODevice::Text|QIODevice::ReadOnly);
    myProc3->terminate();
    myProc3->waitForFinished(11000);

    if(!file.open(QIODevice::ReadOnly |QIODevice::Text)){
        qDebug() << "Can not open the file!" << endl;
    }
    while(!file.atEnd()){
        QByteArray line = file.readLine();
        QString str(line);
        //qDebug() << str;
        ui->plateNumOutput->append(str);
    }
}

void RecognitionSystem::on_allInOneButton_clicked()
{
    QProcess* myProc4 = new QProcess;
    QStringList ori_img_path;
    ori_img_path << ori_path;
    myProc4->start("all_in_one.exe",ori_img_path);
    myProc4->terminate();
    myProc4->waitForFinished(13000);
    myProc4->kill();

    const char* loc_img = "c:/plate_output/test_output.bmp";
    QPixmap pix_loc(loc_img);
    ui->operateImage->setPixmap(pix_loc);

    const char* cut_pic1 = "C:/cut_charater_output/1.bmp";
    const char* cut_pic2 = "C:/cut_charater_output/2.bmp";
    const char* cut_pic3 = "C:/cut_charater_output/3.bmp";
    const char* cut_pic4 = "C:/cut_charater_output/4.bmp";
    const char* cut_pic5 = "C:/cut_charater_output/5.bmp";
    const char* cut_pic6 = "C:/cut_charater_output/6.bmp";
    const char* cut_pic7 = "C:/cut_charater_output/7.bmp";

    QPixmap pic1(cut_pic1);
    QPixmap pic2(cut_pic2);
    QPixmap pic3(cut_pic3);
    QPixmap pic4(cut_pic4);
    QPixmap pic5(cut_pic5);
    QPixmap pic6(cut_pic6);
    QPixmap pic7(cut_pic7);

    ui->plate1->setPixmap(pic1);
    ui->plate2->setPixmap(pic2);
    ui->plate3->setPixmap(pic3);
    ui->plate4->setPixmap(pic4);
    ui->plate5->setPixmap(pic5);
    ui->plate6->setPixmap(pic6);
    ui->plate7->setPixmap(pic7);

    QFile file("c:/platestring_output.txt");
    if(!file.open(QIODevice::ReadOnly |QIODevice::Text)){
        qDebug() << "Can not open the file!" << endl;
    }
    while(!file.atEnd()){
        QByteArray line = file.readLine();
        QString str(line);
        //qDebug() << str;
        ui->plateNumOutput->append(str);
    }

    ui->Binaryzation->setEnabled(true);
    ui->corrupt->setEnabled(true);
    ui->meanfilter->setEnabled(true);
    ui->Binaryzation->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
    ui->corrupt->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
    ui->meanfilter->setStyleSheet("QPushButton{background-color: rgb(0, 127, 190);font: 9pt 'Calibri';color: rgb(255, 255, 255);}");
}

void RecognitionSystem::on_Binaryzation_clicked()
{
    const char* pixbinar = "c:/details/cut/two-value.bmp";
    QPixmap pix(pixbinar);
    ui->Image->setPixmap(pix);
}

void RecognitionSystem::on_corrupt_clicked()
{
     const char* pixcorr = "c:/details/cut/corrupt_expand.bmp";
     QPixmap pix(pixcorr);
     ui->Image->setPixmap(pix);
}

void RecognitionSystem::on_meanfilter_clicked()
{
      const char* pixmean = "c:/details/cut/meanFilter.bmp";
      QPixmap pix(pixmean);
      ui->Image->setPixmap(pix);
}

void RecognitionSystem::on_saveButton_clicked()
{
    QSqlQuery getCarId;
    QSqlQuery Recognition;
    QString carNum=ui->plateNumOutput->toPlainText().left(7);

    getCarId.prepare("SELECT car_id FROM Car WHERE car_num=?");
    getCarId.addBindValue(carNum);
    getCarId.exec();
    getCarId.next();
    int carID = getCarId.value(0).toInt();
    if(carID != 0){
        Recognition.prepare("UPDATE trafficPic set car_id = ?, picState = 1 WHERE VehicleImage = ?");
        Recognition.addBindValue(carID);
        Recognition.addBindValue(ori_path);
        Recognition.exec();
        emit recognitionSuccess(1);
        QMessageBox::information(this, tr("TMSServer"),tr("Save successfully!"),
                                 QMessageBox::Ok);
        close();
    }else{
        Recognition.prepare("UPDATE trafficPic set picState = 2 WHERE VehicleImage = ?");
        Recognition.addBindValue(ori_path);
        Recognition.exec();
        emit recognitionSuccess(0);
        QMessageBox::information(this, tr("TMSServer"),tr("Recognition failed!"),
                                 QMessageBox::Ok);
        close();
    }
}
