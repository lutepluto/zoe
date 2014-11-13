#include "tcpclient.h"
#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QHostAddress>


TcpClient::TcpClient(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displaySocketError(QAbstractSocket::SocketError)));
}

void TcpClient::sendFile(QString file, QHostAddress address, qint16 portNumber, QString loc)
{
    location = loc;
    filePath = file;
    connectToHost(address, portNumber);
    qDebug() << "Send file " << file;
}

void TcpClient::on_connected()
{
    qDebug() << "Connected";
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit onError(2);
        disconnectFromHost();
        return;
    }
    else
        emit fileSize(file.size());

    emit message(tr(" Start send!"), filePath);
    QString fileName = file.fileName();
    fileName = fileName.remove(0, fileName.lastIndexOf('/')+1);

    qint64 size;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << qint16(0x0001) << fileName.toUtf8();
    size = block.size();

    write((char*)&size, sizeof(qint64));
    write(block.data(), size);
    emit update(size);
    //flush();

    if(!waitForBytesWritten(-1))
    {
        qDebug() << "writen Bytes error " << errorString();
        throw 2;
    }
    do
    {
        block.clear();
        out.device()->seek(0);
        out << qint16(0x0002) << file.read(0xFFF0);
        size = block.size();
        qDebug() << "Send Data " << size << " byte.";

        write((char*)&size, sizeof(qint64));
        write(block.data(), size);

        if(!waitForBytesWritten(-1))
        {
            qDebug() << "writen Bytes error " << errorString();
            throw 2;
        }
    }while(!file.atEnd());

    sendEOF();


    qDebug() << "FileSend finished.";
    emit message(tr("Send Finished"), filePath);
    forever/*(int i=0; i < 10; i++)*/
    {
        if(!waitForDisconnected())
        {
            //sendEOF();
        }
        else
        {
            //qDebug() << "Number is " << i << ".";
            qDebug() << "GoTo return.";
            return;
        }
    }
    qDebug() << "I will disconnected.";
    disconnectFromHost();	// have Error.
}

void TcpClient::displaySocketError(QAbstractSocket::SocketError e)
{
    qDebug() << "Error: " << e << "\n\t" << errorString();
    if(e == QAbstractSocket::HostNotFoundError)
        emit onError(1);
    if(e == QAbstractSocket::ConnectionRefusedError)
        emit onError(3);
}

void TcpClient::sendEOF()
{
    qint64 size;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    QByteArray data = location.toUtf8();
    data.resize(0x8000);
    out << quint16(0x0003) << data << location.toUtf8();
    size = block.size();
    write((char*)&size, sizeof(qint64));
    write(block.data(), size);
    if(!waitForBytesWritten(-1))
        qDebug() << "Send EOF Error: " << errorString();
    else
        qDebug() << "Send EOF";
}

void TcpClient::stopService()
{
    disconnectFromHost();
}


