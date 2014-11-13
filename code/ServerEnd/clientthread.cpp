#include "clientthread.h"
#include <QHostAddress>

ClientThread::ClientThread(int h, QObject *parent) :
    QThread(parent)
{
    blockSize = 0;
    blockNumber = 0;
    s.setSocketDescriptor(h);
    connect(&s, SIGNAL(disconnected()), this, SLOT(on_socket_disconnected()));
    connect(&s, SIGNAL(readyRead()), this, SLOT(on_socket_readyRead()));
    connect(&s, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displaySocketError(QAbstractSocket::SocketError)));
}

void ClientThread::proccessData(QByteArray &array)
{
    QDataStream in(&array, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);

    quint16 key;
    QByteArray data;
    in >> key >> data;
    blockNumber ++;
    qDebug() << "BlockNumber " << blockNumber << ", size: " << blockSize+sizeof(qint64) << ".";
    qDebug() << "\tKey: " << key << "\t\tSize: " << data.size() << ".";
    switch(key)
    {
    case 0x0001:		// file name
        fileName = "C:/Picture/" + fileName.fromUtf8(data.data(), data.size());
        file.setFileName(fileName);
        if(file.exists(fileName))
        {
            qDebug() << "File was exists. now remove it!";
            file.remove(fileName);
        }
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Can not open file " << file.fileName() << ".";
            throw 2;
        }
        break;
    case 0x0002:		// file data
        file.write(data.data(), data.size());
        file.flush();
        break;
    case 0x0003:		// file EOF
        qDebug() << "File transt finished.";
        location = location.fromUtf8(data.data(), data.size());
        file.close();
        emit receiveComplete(fileName, location);
        s.disconnectFromHost();
        //emit finished();
        break;
    default: ;
    }
}

void ClientThread::run()
{
    qDebug() << "New connect from " << s.peerAddress().toString() << ":" << s.peerPort() << ".";
    exec();
}

void ClientThread::on_socket_disconnected()
{
    qDebug() << "socket disconnected.";
    exit();
}

void ClientThread::on_socket_readyRead()
{
    while(s.bytesAvailable() >= sizeof(quint64))
    {
        if(blockSize == 0)
        {
            if(s.bytesAvailable() < sizeof(qint64))
                return;
            s.read((char*)&blockSize, sizeof(qint64));  // read blockSize
        }

        if(s.bytesAvailable() < blockSize)				// have no enugh data
            return;
        QByteArray data = s.read(blockSize);
        proccessData(data);
        blockSize = 0;
    }
}

void ClientThread::displaySocketError(QAbstractSocket::SocketError e)
{
    qDebug() << "Error: socket have error " << e << "\n\t" << s.errorString() << ".";
    qDebug() << "Removed " << file.fileName() << ".";
    if(file.isOpen())
        file.close();
    else
        return;
    if(!file.fileName().isEmpty())
        file.remove(fileName);
    else
        return;
    emit error(2);
    //exit();
}
