#include "widget.h"
#include "ui_widget.h"
#include"QDebug"
#include"QTcpServer"
#include"QHostAddress"
#include <QSqlQuery>
#include"QString"
#include"QStringList"
#include"QStandardItemModel"
#include"QSqlTableModel"
#include"QMessageBox"
#include"QSql"
#include"QSqlRecord"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //连接数据库
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("new_schema");
    db.setUserName("root");
    db.setPassword("d54321");
    if (!db.open()) {
        qDebug() << "connect failed";
        qDebug() << db.lastError(); // 输出数据库错误信息
    }
    else qDebug() << "database connect success";
    //初始化服务器server对象

    myserver = new QTcpServer(this);
    myserver->listen(QHostAddress("127.0.0.1"),50997);//监听, If port is 0, a port is chosen automatically
    qDebug()<<"监听";
    connect(myserver,&QTcpServer::newConnection,this,[=](){

        //QMessageBox::about(this,"服务器","成功连接");
         mysocket = myserver->nextPendingConnection();


         //摘录
         //socket的write方法其实是有几个重载方法的，如果为了方便，可以直接用QString类型的变量作为参数。
         //当有数据可以接受，socket对象发出信号readyRead
         connect(mysocket, &QTcpSocket::readyRead, this, [=]()
             {



                 msgType type = recvmsg(mysocket);


                 //将数据写到客户端中

             });

         //断开连接
         connect(mysocket, &QTcpSocket::disconnected, this, [=]()
             {
                 mysocket->close();
                 mysocket->deleteLater();//释放指向的内存
                 qDebug() << "结束服务器";
             });












        //有新连接进入，将该套接字加入列表
//        client.append(mysocket);
        //cli_name.append("");



//        connect(mysocket,&QTcpSocket::readyRead,this,[=](){
//            QString date = mysocket->readAll();

//            QMessageBox::about(this,"提示","客户端发送消息");
//            qDebug()<<date;
//            recvmsg(mysocket);


//        });



        // 关联接收客户端数据信号readyRead信号（客户端有数据就会发readyRead信号）

//        connect(mysocket,&QIODevice::readyRead,this,[=](){
//            // 调用readall接收该客户端信息，存储在msg中
//            qDebug()<<"客户端发送信号";
//            msgType type = recvmsg(mysocket);
            //recvmsg函数在下面定义了
//        });


    });






    //以下是服务器端直接对书籍及用户的管理操作
    this->setWindowTitle("图书管理系统服务器端");
    this->resize(1800,1200);
    ui->tabWidget->setCurrentIndex(0);//设置tabwidget为第一个


    //图书管理界面显示
    book_view();

    //         model->removeColumn(1);//不显示第二列,如果这时添加记录，则该属性的值添加不上

    //用户管理界面显示
    user_view();
    //查询书籍


    connect(ui->btn_selBook,&QPushButton::clicked,this,[=](){

        QString bname =ui->lineEdit_bname->text();
        QString bno = ui->lineEdit_bid->text();
        QString bAuthor = ui->lineEdit_bAuthor->text();
        qDebug()<<"id"<<bno<<"书名"<<bname<<"作者"<<bAuthor;
        if(bname.isEmpty()&&bno.isEmpty()&&bAuthor.isEmpty()){
            qDebug()<<"执行图书查询";
            book_view();
        }else{
            //ceshi

            //

            //
            model->setFilter(QObject::tr("bno = '%1' or bname ='%2' or bAuthor = '%3'").arg(bno).arg(bname).arg(bAuthor));

            model->select();
        }

    });
    //增加书籍
    connect(ui->btn_addBook,&QPushButton::clicked,this,[=](){
        QString bname =ui->lineEdit_bname->text();
        QString bno = ui->lineEdit_bid->text();
        QString bAuthor = ui->lineEdit_bAuthor->text();

        qDebug()<<"id"<<bno<<"书名"<<bname<<"作者"<<bAuthor;
        QSqlQuery query(db);

        QString bstatus = "在库";
        QString cmd = QString("insert into book(bno,bname,bAuthor,bstatus) values ('%1','%2','%3','%4')").arg(bno).arg(bname).arg(bAuthor).arg(bstatus);
        bool re = query.exec(cmd);
        if(!re){
            QMessageBox::warning(this,"警告","图书已存在");
        }else{
            QMessageBox::about(this,"提醒","添加成功");
            book_view();
        }


        //           model->setFilter(QObject::tr("bno = '%1' or bname ='%2' or bAuthor = '%3'").arg(bno).arg(bname).arg(bAuthor));
        //            qDebug()<<model;

        //            QString add_book_search = QString("select bno from book where bno = '%1'").arg(bno);
        //            QSqlQuery bsearch;

        //            bsearch.exec(add_book_search);

        //            while(bsearch.next()){
        //                qDebug()<<bsearch.value(0).toString();
        //                if(bsearch.value(0).toString()==bno){
        //                    QMessageBox::warning(this,"警告","图书已存在");

        //                }
        //                else{
        //                    qDebug()<<"进入插入函数";
        //                    QString bstatus = "在库";
        //                    QString add_book_insert = QString("insert into book(bno,bname,bAuthor,bstatus) values ('%1','%2','%3','%4')").arg(bno).arg(bname).arg(bAuthor).arg(bstatus);
        //                    QSqlQuery binsert;
        //                    binsert.exec(add_book_insert);
        //                    qDebug()<<bstatus;
        //                    book_view();
        //                }
        //            }
    });
    //删除书籍
    connect(ui->btn_delBook,&QPushButton::clicked,this,[=](){
        QString bname =ui->lineEdit_bname->text();
        QString bno = ui->lineEdit_bid->text();
        QString bAuthor = ui->lineEdit_bAuthor->text();

        qDebug()<<"id"<<bno<<"书名"<<bname<<"作者"<<bAuthor;
        QSqlQuery query(db);


        QString cmd = QString("delete from book where bno='%1'").arg(bno);
        bool re = query.exec(cmd);
        if(!re){
            QMessageBox::warning(this,"警告","图书不存在");
        }else{
            QMessageBox::about(this,"提醒","删除成功");
            book_view();
        }



    });

    //用户查询
    connect(ui->btn_selUser,&QPushButton::clicked,this,[=](){

        QString sname =ui->lineEdit_sname->text();
        QString sno = ui->lineEdit_sno->text();
        QString sex = ui->lineEdit_sex->text();
        QString grade = ui->lineEdit_grade->text();
        QString dep = ui->lineEdit_dep->text();
        QString pwd = ui->lineEdit_pwd->text();
        if(sno.isEmpty()){
            qDebug()<<"执行用户查询";
            QMessageBox::warning(this,"警告","请输入用户ID");
            user_view();
        }else{
            model2->setFilter(QObject::tr("sno = '%1'").arg(sno));
            qDebug()<<model2;
            model2->select();
        }
    });

    //用户增加
    connect(ui->btn_addUser,&QPushButton::clicked,this,[=](){
        QString sname =ui->lineEdit_sname->text();
        QString sno = ui->lineEdit_sno->text();
        QString sex = ui->lineEdit_sex->text();
        QString grade = ui->lineEdit_grade->text();
        QString dep = ui->lineEdit_dep->text();
        QString pwd = ui->lineEdit_pwd->text();

        QSqlQuery query(db);

        QString cmd = QString("insert into student(sno,sname,sex,department,grade,sPassword) values ('%1','%2','%3','%4','%5','%6')").arg(sno).arg(sname).arg(sex).arg(dep).arg(grade).arg(pwd);
        bool re = query.exec(cmd);
        if(!re){
            QMessageBox::warning(this,"警告","用户已存在");
        }else{
            QMessageBox::about(this,"提醒","添加成功");
            user_view();
        }
    });

    //用户删除
    connect(ui->btn_delUser,&QPushButton::clicked,this,[=](){
        QString sname =ui->lineEdit_sname->text();
        QString sno = ui->lineEdit_sno->text();
        QString sex = ui->lineEdit_sex->text();
        QString grade = ui->lineEdit_grade->text();
        QString dep = ui->lineEdit_dep->text();
        QString pwd = ui->lineEdit_pwd->text();


        QSqlQuery query(db);
        QString cmd = QString("delete from student where sno='%1'").arg(sno);
        bool re = query.exec(cmd);
        if(!re){
            QMessageBox::warning(this,"警告","用户不存在");
        }else{
            QMessageBox::about(this,"提醒","删除成功");
            user_view();
        }
    });
}

Widget::~Widget()
{
    delete ui;
}
//图书管理显示函数
void Widget::book_view(){
    model = new QSqlTableModel(this);
    model->setTable("book");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "图书名称");
    model->setHeaderData(2, Qt::Horizontal, "图书作者");
    model->setHeaderData(3, Qt::Horizontal, "状态");
    ui->tableView_book->setModel(model);

    QSqlQuery sql1;
    QString book_data = QString("select * from book");
    sql1.exec(book_data);
    QSqlRecord rec = sql1.record();
    while(sql1.next()){
        rec = sql1.record();
        int bnocol = rec.indexOf("bno");
        int bnamecol = rec.indexOf("bname");
        int bAuthorcol = rec.indexOf("bAuthor");
        int bstatuscol = rec.indexOf("bstatus");
        //将需要传输给用户的图书信息赋值给QString后再传入msg
        QString user_bnocol = sql1.value(bnocol).toString();
        QString user_bnamecol = sql1.value(bnamecol).toString();
        QString user_bAuthorcol = sql1.value(bAuthorcol).toString();
        QString user_bstatuscol = sql1.value(bstatuscol).toString();
        //qDebug()<<user_bnocol.toUtf8().data()<<user_bnamecol.toUtf8().data()<<user_bAuthorcol.toUtf8().data()<<user_bstatuscol.toUtf8().data();
        //qDebug()<<user_bnocol.toUtf8().data()<<user_bnamecol.toUtf8().data();
        //采用toUtf8将QString转为 QByteArray，再用data将QByteArray转为* char
        //得到的字符串中间隔了一个空格，如"001 数据结构教程 李春葆 在库".再用split处理一下即可得到需要的信息。
        //但存在一个问题，四个为一组，中间是有空格隔开的，这个可以用split处理成一个个字符集，但是每一组直接没有空格，而是另起一行；
    }
}
//用户管理显示函数
void Widget::user_view(){
    model2 = new QSqlTableModel(this);
    model2->setTable("student");
    model2->setEditStrategy(QSqlTableModel::OnFieldChange);
    model2->select();
    model2->setHeaderData(0, Qt::Horizontal, "ID");
    model2->setHeaderData(1, Qt::Horizontal, "姓名");
    model2->setHeaderData(2, Qt::Horizontal, "性别");
    model2->setHeaderData(3, Qt::Horizontal, "学院");
    model2->setHeaderData(4, Qt::Horizontal, "年级");
    model2->setHeaderData(5, Qt::Horizontal, "密码");
    ui->tableView_user->setModel(model2);
}





void Widget::sndmsg(QTcpSocket *msocket,QString msg, msgType type){

    if(type == Iden){
        QStringList tmp = msg.split(" ");

        QString user_num = tmp.at(0);//账号
        QString user_pwd = tmp.at(1);//密码
        qDebug()<<"账号："<<user_num<<"密码："<<user_pwd;
        QSqlQuery sql; // 数据库对象
        QString cunzai = QString("select * from student where sno = '%1' and sPassword = '%2'").arg(user_num).arg(user_pwd);
        sql.exec(cunzai);
        QString sel_num;
        QString sel_pwd;
        if(sql.first()){
            sel_num = sql.value(0).toString();//查询到的账号
            sel_pwd = sql.value(5).toString();
        }




        if (sel_num==user_num&&sel_pwd==user_pwd)//存在
        {
            //查询book表，把book表全部信息发送给客户端，并把账户信息也发送过去，之后作为客户端的识别码
            //通过.value和for循环结合，得到所有结果
            //value()括号中可以为下标或者QString类型，
            //下面进行book表信息传递
            QSqlQuery sql1;
            QString book_data = QString("select * from book");
            sql1.exec(book_data);
            QSqlRecord rec = sql1.record();
            QString user_bnocol;
            QString user_bnamecol;
            QString user_bAuthorcol;
            QString user_bstatuscol;
            QString kongge = " ";
//            QString bno1;
//            QString bname1;
//            QString bAuthor1;
//            QString bstatus1;
            QString book_info;

            while(sql1.next()){
                rec = sql1.record();
                int bnocol = rec.indexOf("bno");
                int bnamecol = rec.indexOf("bname");
                int bAuthorcol = rec.indexOf("bAuthor");
                int bstatuscol = rec.indexOf("bstatus");
                //将需要传输给用户的图书信息赋值给QString后再传入msg
                user_bnocol = sql1.value(bnocol).toString();
                user_bnamecol = sql1.value(bnamecol).toString();
                user_bAuthorcol = sql1.value(bAuthorcol).toString();
                user_bstatuscol = sql1.value(bstatuscol).toString();
                kongge;//每一行之后输出一个空格
                book_info =book_info + user_bnocol +" "+ user_bnamecol +" "+ user_bAuthorcol +" "+ user_bstatuscol +" ";
//                bno1 =bno1 + user_bnocol +" ";
//                bname1 = bname1 + user_bnamecol +" ";
//                bAuthor1 = bAuthor1 + user_bAuthorcol +" ";
//                bstatus1 = bstatus1 + user_bstatuscol +" ";
                //每次循环对同一列的下一行进行拼接

                //qDebug()<<user_bnocol.toUtf8().data()<<user_bnamecol.toUtf8().data()<<user_bAuthorcol.toUtf8().data()<<user_bstatuscol.toUtf8().data();
                //qDebug()<<user_bnocol.toUtf8().data()<<user_bnamecol.toUtf8().data()<<user_bAuthorcol.toUtf8().data()<<user_bstatuscol.toUtf8().data();
                //采用toUtf8将QString转为 QByteArray，再用data将QByteArray转为* char,这个操作需要在客户端进行转换，因为msg为QString
                //得到的字符串中间隔了一个空格，如"001 数据结构教程 李春葆 在库".再用split处理一下即可得到需要的信息。
                //但存在一个问题，四个为一组，中间是有空格隔开的，这个可以用split处理成一个个字符集，但是每一组直接没有空格，而是另起一行；
            }

            //若msg传输的信息出现问题，考虑此处添加空格分隔
            msg="Iden "+user_num+" "+book_info;
                    //bno1+" "+bname1+" "+bAuthor1+" "+bstatus1;
            //msg = "Iden " + msg;
            //select * from book;

            msocket->write(msg.toUtf8().data());//传输过去的msg，第一个为Msg类型信息，第二个为账号信息，第三个为图书数据信息
            //write参数为* char，也就是可以用此方法直接传
            //msg中要把所有表的数据传入进去，登陆成功后在客户端那边进行插入tableview显示出来

            //qDebug() << "查找成功"<<msg.toUtf8().data();
        }else{
            msocket->write("登录失败");
        }
    }
    else if(type == Sb){//查询书籍
        QStringList tmp = msg.split(" ");
        QString book_sel = tmp.at(0);
        QSqlQuery sql; // 数据库对象
        QString cunzai = QString("select * from book where bname = '%1'").arg(book_sel);
        sql.exec(cunzai);
        QString book_no;
        QString book_name;
        QString book_author;
        QString book_status;
        if(sql.first()){
            book_no = sql.value(0).toString();//得到查询的bno值
            book_name = sql.value(1).toString();
            book_author = sql.value(2).toString();
            book_status = sql.value(3).toString();

        }
            qDebug()<<book_no<<" "<<book_name<<" "<<book_author<<" "<<book_status;
        if(book_name==book_sel){

        msg = "Sb "+ book_no+" "+book_name+" "+ book_author+" "+book_status;
        qDebug()<<msg.toUtf8();
        msocket->write(msg.toUtf8().data());
        }else{
            msocket->write("查询失败");
        }
    }
    else if(type ==Bb){//借书
        QStringList tmp = msg.split(" ");
        QString book_sel = tmp.at(0);//书名
        QSqlQuery sql;
        QString cunzai = QString("select * from book where bname = '%1'").arg(book_sel);
        sql.exec(cunzai);
        QString book_no;

        QString book_author;
        QString name = tmp.at(1);//借书用户no
        QString status;
        if(sql.first()){
            book_no = sql.value(0).toString();//书no

            book_author = sql.value(2).toString();
            status = sql.value(3).toString();

        }
        qDebug()<<"图书状态是："<<status;
        //判断是否在库，如果在库，将bno和sno插入bstatus。如果不在库，返回借书失败消息
        if(status=="在库"){
        QString jieshu = QString("insert into bookstatus(bno,sno) values ('%1','%2')").arg(book_no).arg(name);
        QSqlQuery sql1;
        bool istrue = sql1.exec(jieshu);
        if(istrue){
            //改变bstatus状态
            QString zhuangtai = "借出";
            QString gaibian = QString("update book set bstatus = '%1' where bno = '%2'").arg(zhuangtai).arg(book_no);
            sql1.exec(gaibian);
            msg = "Bb "+book_no+" "+book_sel+" "+book_author+" "+zhuangtai;
            qDebug()<<msg.toUtf8().data();
            msocket->write(msg.toUtf8().data());
        }else{
            msocket->write("借书失败");
        }
        }else{
            msocket->write("借书失败");
        }
    }
    else if(type == Rb){//还书
        QStringList tmp = msg.split(" ");
        QString book_sel = tmp.at(0);//书名
        QSqlQuery sql;

        QString cunzai = QString("select * from book where bname = '%1'").arg(book_sel);

        sql.exec(cunzai);
        QString book_no;
        QString book_author;
        QString name = tmp.at(1);//还书用户no
        QString status;
        if(sql.first()){
            book_no = sql.value(0).toString();//书no
            book_author = sql.value(2).toString();
            status = sql.value(3).toString();
        }
        qDebug()<<"图书状态是："<<status;
        QString yanzheng = QString("select sno from bookstatus where bno= '%1'").arg(book_no);
        QSqlQuery sql2;
        sql2.exec(yanzheng);
        QString stu_no;
        if(sql2.first()){
            stu_no = sql2.value(0).toString();//获得查询图书借阅表的sno结果
        }
        qDebug()<<"借阅表中sno为:"<<stu_no;
        if(stu_no==name){
        //判断是否在库，如果在库，将bno和sno插入bstatus。如果不在库，返回借书失败消息
        if(status=="借出"){
        QString jieshu = QString("delete from bookstatus where  bno= '%1' and sno = '%2'").arg(book_no).arg(name);
        QSqlQuery sql1;
        bool istrue = sql1.exec(jieshu);
        if(istrue){
            //改变bstatus状态
            QString zhuangtai = "在库";
            QString gaibian = QString("update book set bstatus = '%1' where bno = '%2'").arg(zhuangtai).arg(book_no);
            sql1.exec(gaibian);
            msg = "Rb "+book_no+" "+book_sel+" "+book_author+" "+zhuangtai;
            qDebug()<<msg.toUtf8().data();
            msocket->write(msg.toUtf8().data());
        }else{
            msocket->write("还书失败");
        }
        }else{
            msocket->write("还书失败");
                    }
        }else{
            msocket->write("还书失败");
        }
    }
    else if(type ==Mp){//修改密码
        QStringList tmp = msg.split(" ");
        QString pwd = tmp.at(0);//书名
        QString name = tmp.at(1);
        QString xiugai = QString("update student set sPassword ='%1' where sno = '%2'").arg(pwd).arg(name);
        QSqlQuery sql1;
        bool istrue = sql1.exec(xiugai);
        if(istrue){
            msocket->write("Mp");
        }else{
            msocket->write("修改失败");
        }


    }
    else if(type==Rf){//刷新


            //查询book表，把book表全部信息发送给客户端，并把账户信息也发送过去，之后作为客户端的识别码
            //通过.value和for循环结合，得到所有结果
            //value()括号中可以为下标或者QString类型，
            //下面进行book表信息传递
            QSqlQuery sql1;
            QString book_data = QString("select * from book");
            sql1.exec(book_data);
            QSqlRecord rec = sql1.record();
            QString user_bnocol;
            QString user_bnamecol;
            QString user_bAuthorcol;
            QString user_bstatuscol;
            QString kongge = " ";
//            QString bno1;
//            QString bname1;
//            QString bAuthor1;
//            QString bstatus1;
            QString book_info;

            while(sql1.next()){
                rec = sql1.record();
                int bnocol = rec.indexOf("bno");
                int bnamecol = rec.indexOf("bname");
                int bAuthorcol = rec.indexOf("bAuthor");
                int bstatuscol = rec.indexOf("bstatus");
                //将需要传输给用户的图书信息赋值给QString后再传入msg
                user_bnocol = sql1.value(bnocol).toString();
                user_bnamecol = sql1.value(bnamecol).toString();
                user_bAuthorcol = sql1.value(bAuthorcol).toString();
                user_bstatuscol = sql1.value(bstatuscol).toString();
                kongge;//每一行之后输出一个空格
                book_info =book_info + user_bnocol +" "+ user_bnamecol +" "+ user_bAuthorcol +" "+ user_bstatuscol +" ";
//                bno1 =bno1 + user_bnocol +" ";
//                bname1 = bname1 + user_bnamecol +" ";
//                bAuthor1 = bAuthor1 + user_bAuthorcol +" ";
//                bstatus1 = bstatus1 + user_bstatuscol +" ";
                //每次循环对同一列的下一行进行拼接

                //qDebug()<<user_bnocol.toUtf8().data()<<user_bnamecol.toUtf8().data()<<user_bAuthorcol.toUtf8().data()<<user_bstatuscol.toUtf8().data();
                //qDebug()<<user_bnocol.toUtf8().data()<<user_bnamecol.toUtf8().data()<<user_bAuthorcol.toUtf8().data()<<user_bstatuscol.toUtf8().data();
                //采用toUtf8将QString转为 QByteArray，再用data将QByteArray转为* char,这个操作需要在客户端进行转换，因为msg为QString
                //得到的字符串中间隔了一个空格，如"001 数据结构教程 李春葆 在库".再用split处理一下即可得到需要的信息。
                //但存在一个问题，四个为一组，中间是有空格隔开的，这个可以用split处理成一个个字符集，但是每一组直接没有空格，而是另起一行；
            }

            //若msg传输的信息出现问题，考虑此处添加空格分隔
            msg="Rf "+book_info;
                    //bno1+" "+bname1+" "+bAuthor1+" "+bstatus1;
            //msg = "Iden " + msg;
            //select * from book;

            msocket->write(msg.toUtf8().data());//传输过去的msg，第一个为Msg类型信息，第二个为账号信息，第三个为图书数据信息
            //write参数为* char，也就是可以用此方法直接传
            //msg中要把所有表的数据传入进去，登陆成功后在客户端那边进行插入tableview显示出来

            //qDebug() << "查找成功"<<msg.toUtf8().data();


    }



}

msgType Widget::recvmsg(QTcpSocket *msocket){
    msgType type;
    QString msg;
    QByteArray message = msocket->readAll();//接收客户端发来的信息
    QString tmp = QString(message);
    qDebug() << "recv: " << tmp;

    QStringList qlist = tmp.split(" ");//根据空格把组合信息分成信息子集
    QString stype = qlist.at(0);


    //Iden,Sb,Bb,Rb,Mp

    if (stype == "Iden") type = Iden;
    else if (stype == "Sb") type = Sb;
    else if (stype == "Bb") type = Bb;
    else if (stype == "Rb") type = Rb;
    else if (stype == "Mp") type = Mp;
    else if (stype == "Rf") type = Rf;

    msg = tmp.mid(stype.size() + 1);//mid，从索引后开始取后面的字符串
    qDebug()<<msg<<"type为："<<type;
    //QString msg = "Iden " + lineEdit_num + " " + lineEdit_pwd;进行tmp.mid处理后msg为lineEdit_num+" "+lineEdit_pwd
    sndmsg(msocket,msg,type);//传给发送信息函数进行下一步信息处理
    return type;


}

