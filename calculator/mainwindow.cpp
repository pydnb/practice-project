#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QLineEdit"
#include <QPushButton>
#include <QString>
#include <QMessageBox>
#include <QStack>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->lineEdit->setAlignment(Qt::AlignRight);//设置文本框右对齐



    //代码实现功能
    connect(ui->pushButton_1,&QPushButton::clicked,this,[=](){//按下1
        a+= ui->pushButton_1->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_2,&QPushButton::clicked,this,[=](){//按下2
        a+= ui->pushButton_2->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_3,&QPushButton::clicked,this,[=](){//按下3
        a+= ui->pushButton_3->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_4,&QPushButton::clicked,this,[=](){//按下4
        a+= ui->pushButton_4->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_5,&QPushButton::clicked,this,[=](){//按下5
        a+= ui->pushButton_5->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_6,&QPushButton::clicked,this,[=](){//按下6
        a+= ui->pushButton_6->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_7,&QPushButton::clicked,this,[=](){//按下7
        a+= ui->pushButton_7->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_8,&QPushButton::clicked,this,[=](){//按下8
        a+= ui->pushButton_8->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_9,&QPushButton::clicked,this,[=](){//按下9
        a+= ui->pushButton_9->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_0,&QPushButton::clicked,this,[=](){//按下0
        a+= ui->pushButton_0->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_add,&QPushButton::clicked,this,[=](){//按下+
        if(a.at(a.length()-1)!="+"&&a.at(a.length()-1)!="-"&&a.at(a.length()-1)!="/"&&a.at(a.length()-1)!="*"&&a.at(a.length()-1)!="."){
            a+= ui->pushButton_add->text();//获取按钮文本并加入到a字符串
            ui->lineEdit->setText(a);//将a字符串显示到文本框
        }else{
            QMessageBox::warning(this,"警告","请正确输入");
        }
    });

    connect(ui->pushButton_subtract,&QPushButton::clicked,this,[=](){//按下-
        //当按下的不等于加减乘除点时
        if(a.at(a.length()-1)!="+"&&a.at(a.length()-1)!="-"&&a.at(a.length()-1)!="/"&&a.at(a.length()-1)!="*"&&a.at(a.length()-1)!="."){
            a+= ui->pushButton_subtract->text();//获取按钮文本并加入到a字符串
            ui->lineEdit->setText(a);//将a字符串显示到文本框
        }else{
            QMessageBox::warning(this,"警告","请正确输入");
        }
    });

    connect(ui->pushButton_multiply,&QPushButton::clicked,this,[=](){//按下*
        //当按下的不等于加减乘除点时
        if(a.at(a.length()-1)!="+"&&a.at(a.length()-1)!="-"&&a.at(a.length()-1)!="/"&&a.at(a.length()-1)!="*"&&a.at(a.length()-1)!="."){
        a+= ui->pushButton_multiply->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
        }else{
            QMessageBox::warning(this,"警告","请正确输入");
        }
    });

    connect(ui->pushButton_divide,&QPushButton::clicked,this,[=](){//按下/
        //当按下的不等于加减乘除点时
        if(a.at(a.length()-1)!="+"&&a.at(a.length()-1)!="-"&&a.at(a.length()-1)!="/"&&a.at(a.length()-1)!="*"&&a.at(a.length()-1)!="."){
        a+= ui->pushButton_divide->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
        }else{
            QMessageBox::warning(this,"警告","请正确输入");
        }
    });

    connect(ui->pushButton_delete,&QPushButton::clicked,this,[=](){//按下<-
        a.remove(a.length()-1,1);//删除最后一位
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_clear,&QPushButton::clicked,this,[=](){//按下CE
        a.clear();//清空
        ui->lineEdit->setText("0");//将0显示到文本框
    });

    connect(ui->pushButton_point,&QPushButton::clicked,this,[=](){//按下.
        a+= ui->pushButton_point->text();//获取按钮文本并加入到a字符串
        ui->lineEdit->setText(a);//将a字符串显示到文本框
    });

    connect(ui->pushButton_equal,&QPushButton::clicked,this,[=](){//按下=
        QStack<int> s_num,s_opt;        //开两个int型栈  数值栈s_num  符号栈s_opt
            char opt[128] = {0};
            int i = 0 , tmp = 0 , num1 , num2;          //遍历指针i  操作数num1  操作数num2

            //把QString转换成char *
            QByteArray ba;
            ba = a.toUtf8();       //把QString转换成QByteArray
            a="0";
           // ba.append(a);
            strcpy(opt,ba.data());          //data把QByteArray转换成const char *

            //栈操作
            while (opt[i] != '\0' || s_opt.empty() != true)     //字符串未读完或符号栈非空
            {
                 if(opt[i] >= '0' && opt[i] <= '9') //遇到数字
                 {
                     tmp = tmp * 10 +opt[i] - '0';      //将多个char型数组元素转成一个int型数据
                     i++;
                     if(opt[i] < '0' || opt[i] >'9')        //一个数据接收结束 压栈
                     {
                         s_num.push(tmp);
                         tmp = 0;
                     }
                 }

                 else       //遇到操作符
                 {
                     if(s_opt.empty() == true || Priority(opt[i])>Priority (s_opt.top()))
                     //符号栈为空 或  当前读取元素的优先级高于符号栈栈顶元素优先级  或  *多重括号嵌套情况* 即符号栈栈顶为‘（’且当前不为‘）’
                     //后压栈的操作符先参与计算。所以当前运算符优先级比栈顶元素高时则压栈，括号未匹配时压栈。
                     //符号栈为空：第一次遇到运算符 & 式子前面的括号运算全部结束后读取最后一个运算符
                     {
                         s_opt.push(opt[i]);          //当前元素压进符号栈
                         i++;
                         continue;
                     }
                     if(Priority(opt[i]) <= Priority (s_opt.top())||(opt[i] == '\0' && s_opt.empty() != true))
                     {
                      //当前元素优先级不大于符号栈栈顶优先级  或 当前为‘）’且符号栈栈顶不为‘（’  或  字符串读取结束且符号栈不为空
                      //多重括号嵌套在第一步已进行判断。所以此步骤为算式前部分需要比后部分先计算的情况 & 括号匹配后执行括号内计算
                      //字符串读取结束且符号栈不为空情况是最后一步  此时数值栈剩余两个元素  符号栈剩余一个元素
                         char ch = s_opt.top();
                        s_opt.pop();
                        switch(ch)
                        {
                            case '+':
                                num1 = s_num.top();
                                s_num.pop();
                                num2 = s_num.top();
                                s_num.pop();
                                s_num.push(num1 + num2);
                                break;
                            case '-':
                                num1 = s_num.top();
                                s_num.pop();
                                num2 = s_num.top();
                                s_num.pop();
                                s_num.push(num2 - num1);     //注意减数与被减数顺序 num2后出栈 在算式中在num1前方
                                break;
                            case '*':
                                num1 = s_num.top();
                                s_num.pop();
                                num2 = s_num.top();
                                s_num.pop();
                                s_num.push(num1 * num2);
                                break;
                            case '/':
                                num1 = s_num.top();
                                s_num.pop();
                                num2 = s_num.top();
                                s_num.pop();
                                s_num.push(num2 / num1);    //注意除数与被除数顺序
                                break;

                        }
                     }
                 }
            }
            //在显示屏上输出数值栈栈顶元素
            //ui->outputLineEdit->setText(QString::number(s_num.top()));
        ui->lineEdit->setText(QString::number(s_num.top()));//将结果显示到文本框
    });







}

int MainWindow::Priority(char ch)       //定义运算符优先级  右括号不入栈版
{
    switch (ch)
    {
            case'+':
            case'-':
                return 1;
            case'*':
            case'/':
                return 2;
            default:
                return 0;
    }
}


MainWindow::~MainWindow()
{



    delete ui;
}


