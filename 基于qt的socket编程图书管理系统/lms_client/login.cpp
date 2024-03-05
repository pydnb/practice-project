#include "login.h"
#include "ui_login.h"
#include"person.h"

#include"QDebug"
#include <QTcpSocket>
#include"QAbstractSocket"
#include"QMessageBox"
#define server_IP "127.0.0.1"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    //登录界面
    this->setWindowTitle("欢迎使用图书管理系统");
    //获取账号密码

    //连接服务器
    mysocket = new QTcpSocket(this);
    mysocket->connectToHost(server_IP,50997);
    //调试
    connect(mysocket,&QTcpSocket::connected,[=](){
        //QMessageBox::about(this,"服务器","成功连接");

        //        connect(ui->btn_login,&QPushButton::clicked,this,[=](){
        //            QString tmp = "Iden "+num+" "+pwd;
        //           mysocket->write(tmp.toUtf8().data());
        //           QMessageBox::about(this,"提示","成功向服务器发送消息");
        //        });

    });

    //接收服务器端发送过来的有内容的信号
    connect(mysocket, &QTcpSocket::readyRead,this, [=]()//收到消息才会反应
    {
        QByteArray message = mysocket->readAll();
        QString tmp = QString(message);
        if(message=="登录失败"){
            QMessageBox::information(this,"提醒","登录失败");
        }

        QStringList qlist = tmp.split(" ");
        if(qlist[0]=="Iden"){
            //关闭这个窗口，跳转用户界面，传递账号信息到用户界面
            QString book;
            QString myname = qlist.at(1);
            book = tmp.mid(myname.size() + 6);
            info = myname +" "+ book;
            mysocket->close();

            emit toper();

        }else{
            QMessageBox::information(this,"提醒","账号或密码错误");
        }


        //接受服务器传来的内容
        //        qDebug() << tmp;
        //        QMessageBox::about(this,"登录测试",tmp);

        //将数据写到服务器中
        //        QString tmp = "Iden "+num+" "+pwd;
        //        mysocket->write(tmp.toUtf8().data());
    });

    connect(ui->btn_login,&QPushButton::clicked,this,[=](){
        QString num = ui->lineEdit_num->text();
        QString pwd = ui->lineEdit_pwd->text();
        QString tmp = "Iden "+ num +" "+ pwd;
        mysocket->write(tmp.toUtf8());
        //QMessageBox::about(this,"提示","成功向服务器发送消息");


    });








}

login::~login()
{
    delete ui;
}

QString login::sendnameandbook(){
    return info;
}
