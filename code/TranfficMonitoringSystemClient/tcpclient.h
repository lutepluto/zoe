#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QAbstractSocket>


class TcpClient : public QTcpSocket
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = 0);
    static QHostAddress address;
    void sendFile(QString file, QHostAddress address, qint16 portNumber, QString loc);
    void sendEOF();
    
signals:
    void fileSize(qint64 size);
    void message(const QString& str, QString fileName);
    void onError(qint32);
    void update(qint64);
    
public slots:
    void on_connected();
    void displaySocketError(QAbstractSocket::SocketError e);
    void stopService();

private:
    QString filePath;
    QString location;


};

#endif // TCPCLIENT_H
