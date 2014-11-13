#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

    bool startServer(qint16 portNumber);
    void stopServer();

protected:
    void incomingConnection(int handle);

signals:
    
public slots:
    void displayError(int);
    void finished();
    
};

#endif // TCPSERVER_H
