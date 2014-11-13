/*Login**********************************************************************************/
#ifndef LOGINDIG_H
#define LOGINDIG_H

#include <QDialog>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class LoginDig;
}

class LoginDig : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDig(QWidget *parent = 0);
    ~LoginDig();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *envet);
    
private slots:
    void on_loginButton_clicked();


private:
    Ui::LoginDig *ui;
    QPoint relativePos;


};

#endif // LOGINDIG_H
