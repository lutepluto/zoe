#ifndef RECOGNITIONSYSTEM_H
#define RECOGNITIONSYSTEM_H

#include <QDialog>
#include <QPoint>
#include <QMouseEvent>
#include <QString>
#include <QProcess>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QSqlQueryModel>
#include <QSqlQuery>

namespace Ui {
class RecognitionSystem;
}

class RecognitionSystem : public QDialog
{
    Q_OBJECT
    
public:
    //original image's path***
//    class   temp{
//    public:
//        static QStringList ori_path;
//    };
    //QStringList ori_img_path;
    //locate items
   // QProcess *myProc;
    //QStringList ori_path;
    //cut items
    //QStringList ori_path;
    const char* ori_imgpath;
    QString ori_path;
    explicit RecognitionSystem(QString path, QWidget *parent = 0);
    ~RecognitionSystem();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *envet);
    
private slots:
    void on_exitButton_clicked();

    void on_locateButton_clicked();

    void on_cutButton_clicked();

    void on_matchButton_clicked();

    void on_allInOneButton_clicked();

    void on_Binaryzation_clicked();

    void on_corrupt_clicked();

    void on_meanfilter_clicked();

    void on_saveButton_clicked();

signals:
    void recognitionSuccess(qint16);

private:
    Ui::RecognitionSystem *ui;
    QPoint relativePos;
};

#endif // RECOGNITIONSYSTEM_H
