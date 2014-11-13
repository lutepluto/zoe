#ifndef NETWORKOPTIONWINDOW_H
#define NETWORKOPTIONWINDOW_H

#include <QDialog>
#include <QHostAddress>
#include <QtNetwork>

namespace Ui {
class NetworkOptionWindow;
}

class NetworkOptionWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit NetworkOptionWindow(QWidget *parent = 0);
    ~NetworkOptionWindow();
    QHostAddress address;
    qint16 portNumber;
    QString location;

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::NetworkOptionWindow *ui;
    QTcpSocket *tcpSocket;
    bool closeFlag;

signals:
    void networkOptionWindowClose(qint32);

private slots:
    void on_closeButton_clicked();
    void on_connectButton_clicked();
    void afterHostFound();
    void afterHostNotFound();
};

#endif // NETWORKOPTIONWINDOW_H
