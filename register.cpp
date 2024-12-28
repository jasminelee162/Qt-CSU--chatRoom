#include "register.h"
#include "ui_register.h"

#include<QButtonGroup>
#include<QMessageBox>
#include<QRegExp>
#include<QSqlQuery>
#include<QSqlError>
#include<QtDebug>
#include<QListWidgetItem>
#include<QString>


Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    //设置图标
    this->setWindowIcon(QIcon(":/images/cmanc.jpg"));
    //设置名称
    this->setWindowTitle("Register");
    qDebug()<<QSqlDatabase::drivers();//显示驱动
    QString aFile="./chat.db";//设置文件
    if(aFile.isEmpty())
    {
        qDebug()<<"数据库文件加载失败";
        return;
    }//判断文件是否设置成功
    dp=QSqlDatabase::addDatabase("QSQLITE");//引入数据库QSQLITE
    dp.setDatabaseName(aFile);//设置文件名称
    if(!dp.open())
    {
        qDebug()<<"Error failed to open"<<dp.lastError();
    }//判断文件是否可以打开
    QSqlQuery query(dp);//设置一个对象
    qDebug()<<"44";
    QString sqlCreate=QString("create table user(id integer primary key autoincrement,"
                                  "username int,"
                                  "password int,"
                                   "name varchar char(40),"
                                   "age int,"
                                   "gender varchar char(40));");
        query.exec(sqlCreate);//创建一个表
        qDebug()<<"ooo";
        ui->ledit_username->setValidator(new QIntValidator(ui->ledit_username));
        ui->ledit_pwd->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]+$")));
        ui->ledit_age->setValidator(new QIntValidator(ui->ledit_age));
        qDebug()<<"3";
}

Register::~Register()
{
    delete ui;
    qDebug()<<"8";
}


void Register::on_btn_login_clicked()
{
    qDebug()<<"009";

    QByteArray ch = ui->ledit_pwd->text().toLatin1();
    char *s = ch.data();
    char *p = ch.data();
    while (*s && *s >= '0' && *s <= '9')
         {
             s++;//如果是数字，指针后移
         }

         while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z'))
         {
             p++;//如果是字母，指针后移
         }

     if(ui->ledit_username->text() == "" || ui->ledit_pwd->text() == "")
         {
         QMessageBox::warning(0,tr("警告"),
                              tr("密码和账户不能为空"),QMessageBox::Ok);
         return;
         }
     if(ui->ledit_name->text() == "" )
         {
         QMessageBox::warning(0,tr("警告"),
                              tr("请填写您的爱好"),QMessageBox::Ok);
         return;
         }
     if(ui->ledit_age->text() == "" )
         {
         QMessageBox::warning(0,tr("警告"),
                              tr("请填写您的年龄"),QMessageBox::Ok);
         return;
         }
     if(ui->ledit_age->text().count() < 0 || ui->ledit_age->text().count() > 3)
         {
         QMessageBox::warning(0,tr("警告"),
                              tr("年龄不正常"),QMessageBox::Ok);
         return;
         }
     if(ui->ledit_name->text().count() < 0 || ui->ledit_age->text().count() > 10)
         {
         QMessageBox::warning(0,tr("警告"),
                              tr("昵称不能超过10个字"),QMessageBox::Ok);
         return;
         }
     if(ui->rbtn_male->text() == "" || ui->rbtn_female->text() == "")
         {
         QMessageBox::warning(0,tr("警告"),
                              tr("请勾选您的性别"),QMessageBox::Ok);
         return;
         }

     //密码长度小于8
     else if (ui->ledit_pwd->text().count() < 8)
     {
         QMessageBox::warning(0,tr("警告"),
                              tr("密码长度不能小于8"),QMessageBox::Ok);
         return;
     }
     else if (*s == NULL)
     {
         QMessageBox::warning(0,tr("警告"),
                              tr("密码必须是数字和字母的组合"),QMessageBox::Ok);
         return;
      }
     else if (*p == NULL)
     {//纯字母
         QMessageBox::warning(0,tr("警告"),
                              tr("密码必须是数字和字母的组合"),QMessageBox::Ok);
         return;
     }

    int username = ui->ledit_username->text().toInt();
    QString password = ui->ledit_pwd->text();
    QString name = ui->ledit_name->text();
    int age = ui->ledit_age->text().toInt();

    QButtonGroup *bg=new QButtonGroup(this);
    bg->addButton(ui->rbtn_male,0);//一个值为0
    bg->addButton(ui->rbtn_female,1);//一个值为1

    int sel=bg->checkedId();//取到你所选的radioButton的值

    QString gender;

    switch(sel)
    {
    case 0:
      gender="男";
      break;
    case 1:
      gender="女";
      break;
    default:
      gender="";
    break;

    }


    //插入操作
    QSqlQuery query(dp);
    query.prepare("select username from user where username=:username");
    query.bindValue(":username", username);
    query.exec();
    qDebug()<<"33";
    if(query.next())
    {

        QMessageBox::information(this,"警告","用户名已存在！",QMessageBox::Ok);

    }
    else
    {

        query.prepare("insert into user(username,password,name,age,gender)"
                      "values(:username,:password,:name,:age,:gender)");
        query.bindValue(":username", username);
        query.bindValue(":password",password);
        query.bindValue(":name", name);
        query.bindValue(":age", age);
        query.bindValue(":gender", gender);
        qDebug()<<"0000";

    query.exec();
    QMessageBox::information(this,"恭喜","注册成功！",QMessageBox::Ok);
    this->close();
    dp.close();

    }
}

