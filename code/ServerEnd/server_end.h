/*ServerEnd**********************************************************************************/
#ifndef SERVER_END_H
#define SERVER_END_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QPoint>
#include <QMouseEvent>
#include "tcpserver.h"

namespace Ui {
class Server_end;
}

class Server_end : public QDialog
{
    Q_OBJECT
    
public:
    explicit Server_end(QWidget *parent = 0);
    ~Server_end();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *envet);

private slots:
    void on_StartButton_clicked();
    void on_StopButton_clicked();
    void on_WaitingTable_clicked();
 //   void on_SubmitButton_2_clicked();
    void on_QueryButton_clicked();
    void on_DeleteButton_clicked();
    void on_EmptyButton_clicked();
    void on_SubmitButton_clicked();
    void on_ExitButton_clicked();
    void on_minimizeButton_clicked();
    void picStateChange(qint16);

    void imageFinished(QString name, QString loc);

    void on_RecogniseButton_clicked();

    void on_SuccessTable_clicked();

    void on_FailTable_clicked();


private:
    Ui::Server_end *ui;
    QSqlQueryModel *FileModel;
    QSqlQueryModel *SuccessModel;
    QSqlQueryModel *FailModel;
    QSqlQueryModel *model;
    QPoint relativePos;
    TcpServer *tcpServer;
};

#endif // SERVER_END_H
