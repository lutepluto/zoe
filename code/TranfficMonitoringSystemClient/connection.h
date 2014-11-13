/*Connect with database**********************************************************************************/
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("TMS");
    db.setUserName("root");
    db.setPassword("root");

    if(!db.open()){
        QMessageBox::critical(0,qApp->tr("Cannot open database"),qApp->tr("Unable to establish a database connection"),QMessageBox::Cancel);
        return false;
    }
    return true;

}



#endif // CONNECTION_H
