#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>
#include <QString>
#include <QMouseEvent>
#include "tcpclient.h"
#include "networkoptionwindow.h"
#include "logindig.h"

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ClientWindow(QWidget *parent = 0);
    ~ClientWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *envet);
    NetworkOptionWindow *option;

    
private:
    Ui::ClientWindow *ui;
    int sendBytes;  //�Ѿ����͵����ݴ�С
    qint64 blockNumber;  //�������ݰ��ı��
    qint64 maxBytes;  //�����ļ����ܴ�С
    QPoint relativePos;

private slots:

    void on_sendButton_clicked();
    void setProcessBar(qint64);
    void updateProgressBar(qint64 v);
    void updateStatus(const QString& status, QString file);
    void on_closeButton_clicked();
    void on_socketDisconnected();
    void on_socketError(qint32);

    void on_browseButton_clicked();
    void on_emptyButton_clicked();
    void on_selectAllButton_clicked();
    void on_deleteButton_clicked();
    void on_selectInverseButton_clicked();
    void on_minimizeButton_clicked();
    void whenNetworkOptionWindowClose(qint32);
};

#endif // CLIENTWINDOW_H
