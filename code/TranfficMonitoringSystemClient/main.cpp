#include <QtGui/QApplication>
#include "clientwindow.h"
#include "logindig.h"
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createConnection())
        return 1;

    ClientWindow w;



    LoginDig logindig;
    if(logindig.exec() == QDialog::Accepted){
        w.option->show();
        w.show();
        return a.exec();
    }else
        return -1;

}
