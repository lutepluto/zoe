#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QDateTime>
#include <QFile>
#include <QString>

class ClientThread : public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(int h, QObject *parent = 0);
    void proccessData(QByteArray& array);

protected:
    void run();

signals:
    void error(int);
    void receiveComplete(QString name, QString loc);
    
public slots:
    void on_socket_readyRead();
    void on_socket_disconnected();
    void displaySocketError(QAbstractSocket::SocketError e);

private:
    QFile file;
    QString fileName;
    QString location;

    QTcpSocket s;
    qint64 blockSize;
    qint64 blockNumber;

};

#endif // CLIENTTHREAD_H
