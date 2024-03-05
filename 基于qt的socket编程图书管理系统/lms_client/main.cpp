#include "widget.h"
#include"login.h"
#include"person.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login *w = new login();
    w->show();
    QObject::connect(w,&login::toper,[=](){
        //QString tmp = w->getnameandebook();

        QString person_info = w->sendnameandbook();
        person *p = new person(nullptr,person_info);

        //QString str = w->sendnameandbook();
        //p->getnameandbook(str);
        w->hide();
        p->show();
    });


    //登录界面跳转



    return a.exec();
}
