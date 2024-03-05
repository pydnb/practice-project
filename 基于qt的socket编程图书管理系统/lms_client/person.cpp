#include "person.h"
#include "ui_person.h"
#include"login.h"
#include <QTcpSocket>
#include"QAbstractSocket"
#include"QMessageBox"
#include"QDebug"
#include"QStandardItemModel"
#define server_IP "127.0.0.1"





person::person(QWidget *parent,QString str) :
    QWidget(parent),
    ui(new Ui::person)
{
    ui->setupUi(this);
    book_view(str);


    //QMessageBox::about(this,"person提示",str);

    QStringList qlist = str.split(" ");
    QString name =qlist.at(0);
    this->setWindowTitle(name);
    this->resize(1800,1200);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置tableview只读不可编辑



    //下面为tableview插入


    //连接服务器
    mysocket = new QTcpSocket(this);
    mysocket->connectToHost(server_IP,50997);
    connect(mysocket,&QTcpSocket::connected,[=](){

    });
    //点击查询按钮，发送消息
    connect(ui->btn_select,&QPushButton::clicked,this,[=](){
        QString book_name = ui->lineEdit_book->text();
        if(book_name.isEmpty()){
            book_view(str);
        } else{


            QString tmp1 = "Sb "+ book_name;
            mysocket->write(tmp1.toUtf8());
            //QMessageBox::about(this,"提示","成功向服务器发送消息");

        }
    });
    //点击借书，发送消息
    connect(ui->btn_borrow,&QPushButton::clicked,this,[=](){
        QString book_name = ui->lineEdit_book->text();

        QString tmp2 = "Bb "+ book_name+" "+name;
        mysocket->write(tmp2.toUtf8());
        //QMessageBox::about(this,"提示","成功向服务器发送消息");


    });
    //点击还书，发送消息
    connect(ui->btn_return,&QPushButton::clicked,this,[=](){
        QString book_name = ui->lineEdit_book->text();

        QString tmp3 = "Rb "+ book_name + " "+name;
        mysocket->write(tmp3.toUtf8());
        //QMessageBox::about(this,"提示","成功向服务器发送消息");


    });
    //点击修改密码，发送消息
    connect(ui->btn_pwd,&QPushButton::clicked,this,[=](){
        QString pwd = ui->lineEdit_pwd->text();

        QString tmp4 = "Mp "+ pwd + " "+ name;
        mysocket->write(tmp4.toUtf8());
        //QMessageBox::about(this,"提示","成功向服务器发送消息");


    });
    //刷新
    connect(ui->btn_refresh,&QPushButton::clicked,this,[=](){
        QString sx = "Rf";
        mysocket->write(sx.toUtf8());

    });


    //接收服务器消息
    connect(mysocket, &QTcpSocket::readyRead,this, [=]()//收到消息才会反应
    {
        QByteArray message = mysocket->readAll();
        QString tmp = QString(message);
        if(message=="还书失败"){
            QMessageBox::information(this,"提醒","还书失败");
        }
        if(message=="查询失败"){
            QMessageBox::information(this,"提醒","查询失败");
        }
        if(message=="借书失败"){
            QMessageBox::information(this,"提醒","借书失败");
        }
        if(message=="修改失败"){
            QMessageBox::information(this,"提醒","修改失败");
        }


        QStringList qlist = tmp.split(" ");
        if(qlist[0]=="Sb"){//查询书籍不需要带上用户id
            book_model->setRowCount(0);//删除除了表头外的行

            book_model->setItem(0,0,new QStandardItem(qlist.at(1)));
            book_model->setItem(0,1,new QStandardItem(qlist.at(2)));
            book_model->setItem(0,2,new QStandardItem(qlist.at(3)));
            book_model->setItem(0,3,new QStandardItem(qlist.at(4)));

        }
        else if(qlist[0]=="Bb"){
            book_model->setRowCount(0);
            book_model->setItem(0,0,new QStandardItem(qlist.at(1)));
            book_model->setItem(0,1,new QStandardItem(qlist.at(2)));
            book_model->setItem(0,2,new QStandardItem(qlist.at(3)));
            book_model->setItem(0,3,new QStandardItem(qlist.at(4)));
            QMessageBox::information(this,"提醒","借书成功");


        }
        else if(qlist[0]=="Rb"){
            book_model->setRowCount(0);
            book_model->setItem(0,0,new QStandardItem(qlist.at(1)));
            book_model->setItem(0,1,new QStandardItem(qlist.at(2)));
            book_model->setItem(0,2,new QStandardItem(qlist.at(3)));
            book_model->setItem(0,3,new QStandardItem(qlist.at(4)));
            QMessageBox::information(this,"提醒","还书成功");



        }
        else if(qlist[0]=="Mp"){
            QMessageBox::information(this,"提醒","修改成功");
        }
        else if(qlist[0]=="Rf"){




            QStringList book_info = tmp.mid(3).split(" ");//得到书籍的列表
            int col=book_info.count();

            for(int j=0;j<col/4;j++){//j为列，i为行
                //插入每一列
                for(int i=0;i<4;i++){
                    //插入一行
                    //setItem函数的第一个参数表示行号，第二个表示列号，第三个为要显示的数据)
                    book_model->setItem(j,i,new QStandardItem(book_info.at(4*j+i)));//按行顺序插入
                }
            }



        }


    });

}

person::~person()
{
    delete ui;
}

void person::book_view(QString str){
    book_model = new QStandardItemModel();
    book_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("ID")));
    book_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("图书名称")));
    book_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("图书作者")));
    book_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("状态")));
    ui->tableView->setModel(book_model);

    QStringList qlist = str.split(" ");
    QString name =qlist.at(0);
    QStringList book_info = str.mid(name.size() + 1).split(" ");//得到书籍的列表
    int col=book_info.count();

    for(int j=0;j<col/4;j++){//j为列，i为行
        //插入每一列
        for(int i=0;i<4;i++){
            //插入一行
            //setItem函数的第一个参数表示行号，第二个表示列号，第三个为要显示的数据)
            book_model->setItem(j,i,new QStandardItem(book_info.at(4*j+i)));//按行顺序插入
        }
    }
}
