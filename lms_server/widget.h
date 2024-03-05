#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include"QTcpSocket"
#include"QSqlDatabase"
#include"QTcpServer"
#include <QSqlError>
#include"QSqlTableModel"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

enum msgType {Iden,Sb,Bb,Rb,Mp,Rf};//方法,用户端需求：登录验证，查询书，借书，还书，修改密码:modify password；五个

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void sndmsg(QTcpSocket *msocket,QString msg, msgType);
    msgType recvmsg(QTcpSocket *);
//    void initModel(QSqlTableModel *);//tableview的数据库操作函数
    void book_view();
    void user_view();

private:
    Ui::Widget *ui;
    QList<QTcpSocket*> client;
    QList<QString> cli_name;
    QSqlDatabase db;
    QTcpServer *myserver;
    QSqlTableModel *model;
    QSqlTableModel *model2;
    QTcpSocket * mysocket;


};
#endif // WIDGET_H
