#include "tcpserver.h"
#include "clientthread.h"
#include "server_end.h"

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{
}

bool TcpServer::startServer(qint16 portNumber)
{
    if(!listen(QHostAddress::Any, portNumber)){
        qDebug() << "Server starts error!";
        return false;
    }else
        return true;
}

void TcpServer::stopServer()
{
    close();
}

void TcpServer::incomingConnection(int handle)
{
    ClientThread* client = new ClientThread(handle, this);
    Server_end *object = (Server_end*)client->parent()->parent();

    connect(client, SIGNAL(error(int)), this, SLOT(displayError(int)));
    connect(client, SIGNAL(finished()), this, SLOT(finished()));
    connect(client, SIGNAL(receiveComplete(QString, QString)), object, SLOT(imageFinished(QString, QString)));
    connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
    client->start();
}

void TcpServer::displayError(int e)
{
    qDebug() << "The client thread have a error. " << e << ".";
}

void TcpServer::finished()
{
    qDebug() << "a clientThread finished.";
}
