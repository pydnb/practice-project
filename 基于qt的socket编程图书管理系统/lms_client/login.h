#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include"widget.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    QString sendnameandbook();
    QString info;

signals:
    void toper();
    //void send(QString);

private:
    Ui::login *ui;
    QTcpSocket* mysocket;




};
#endif // LOGIN_H
