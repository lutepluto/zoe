#include <QtGui/QApplication>
#include <QMainWindow>
#include <QtSql/QSqlDatabase>

#include "logindig.h"
#include "ui_logindig.h"
#include "server_end.h"
#include "ui_server_end.h"
#include "connection.h"

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
   if(!createConnection())
           return 1;

   Server_end serverend;
   LoginDig login;

   if(login.exec()==QDialog::Accepted)
   {
        serverend.show();
        return app.exec();
   }
   else return -1;
}


