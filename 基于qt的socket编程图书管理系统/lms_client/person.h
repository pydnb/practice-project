#ifndef PERSON_H
#define PERSON_H

#include <QWidget>
#include"login.h"
#include"QSqlDatabase"
#include"widget.h"
#include"QStandardItemModel"

namespace Ui {
class person;
}

class person : public QWidget
{
    Q_OBJECT

public:
    explicit person(QWidget *parent,QString str);
    ~person();
QString getnameandbook(QString);
QStandardItemModel *book_model;


void book_view(QString);


private:
    Ui::person *ui;

    QTcpSocket *mysocket;
    QSqlDatabase db;



//private slots:
//    void getnameandbook(QString);

};

#endif // PERSON_H
