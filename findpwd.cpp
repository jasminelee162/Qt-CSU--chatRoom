#include "findpwd.h"
#include "ui_findpwd.h"

#include<QDebug>
#include<QMessageBox>

findpwd::findpwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::findpwd)
{
    ui->setupUi(this);
    //设置图标
    this->setWindowIcon(QIcon(":/images/snd.jpg"));
    //设置名称
    this->setWindowTitle("Retrieve password");
    //获取用户名输入框的数据
    QString Username = ui->idEdt->text();
    //获取用户的昵称
    QString name=ui->nameEdt->text();
    //获取密码输入框的数据
    QString Password = ui->pwdEdt->text();
    //获取确认密码
    QString passwordagain=ui->pwdEdtagain->text();
    //sql语句在数据库中进行查询验证
    qDebug()<<QSqlDatabase::drivers();//显示驱动
    dp=QSqlDatabase::addDatabase("QSQLITE");
    dp.setDatabaseName("./chat.db");//设置文件名称
    bool ok = dp.open();//如果不存在就创建，存在就打开
        if(ok)
        {
            qDebug()<<"数据库创建（打开）成功了";
        }
        else
        {
            qDebug()<<dp.lastError().text();//调用上一次出错的原因
            exit(-1);
        }
    QSqlQuery query(dp);//设置一个对象

    ui->idEdt->setValidator(new QIntValidator(ui->idEdt));
    ui->pwdEdt->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]+$")));
    ui->pwdEdt->setEchoMode(QLineEdit::Password);
    ui->pwdEdtagain->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]+$")));
    ui->pwdEdtagain->setEchoMode(QLineEdit::Password);

//    QString sqlCreate=QString("create table user(id integer primary key autoincrement,"
//                                  "username int,"
//                                  "password int,"
//                                   "name varchar char(40),"
//                                   "age int,"
//                                   "gender varchar char(40));");
}

findpwd::~findpwd()
{
    delete ui;
}

void findpwd::on_deleteBtn_clicked()
{
    QMessageBox::warning(0,tr("警告"),
                      tr("你真的要注销该账号吗？"),QMessageBox::Ok);

    //获取用户名输入框的数据
    QString Username = ui->idEdt->text();
    qDebug()<<"注销账号";
    QString sqlDelete=QString("delete from user where username ='%1'").arg(Username);
    QSqlQuery query(sqlDelete);
             if(!query.exec(sqlDelete))
               {
                  qDebug()<<"delete data error"<<dp.lastError();
               }//删除操作
    this->close();
}

void findpwd::on_okBtn_clicked()
{
    QByteArray ch = ui->pwdEdtagain->text().toLatin1();
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

    if(ui->idEdt->text() == "" || ui->pwdEdt->text() == "")
        {
        QMessageBox::warning(0,tr("警告"),
                             tr("密码和账户不能为空"),QMessageBox::Ok);
        return;
        }
    //密码长度小于8
    else if (ui->pwdEdt->text().count() < 8)
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
    //获取用户名输入框的数据
    QString Username = ui->idEdt->text();
    //获取用户的昵称
    QString name=ui->nameEdt->text();
    //获取密码输入框的数据
    QString Password = ui->pwdEdt->text();
    //获取确认密码
//    QString Name;
    QString Passwordagain=ui->pwdEdtagain->text();
    if(Password!=Passwordagain){
        QMessageBox::warning(0,tr("警告"),
                             tr("两次输入的密码不一致"),QMessageBox::Ok);
        return;

    }
    QSqlQuery query(dp);//设置一个对象
    QString sqlSelect=QString("select * from user where username='%1';").arg(Username);
    if(!query.exec(sqlSelect))
    {
        qDebug()<<"select data error"<<dp.lastError();
    }else{
      while (query.next()) {
          qDebug()<<query.value("name").toString();
          QString Name=query.value("name").toString();
          if(name!=Name){
              QMessageBox::warning(0,tr("警告"),
                                   tr("昵称错误"),QMessageBox::Ok);
              return;

          }
      }
    }//查询操作
    QString sqlUpdate=QString("update user set password='%1'  where username='%2';")
                                 .arg(Password).arg(Username);
    //QSqlQuery query(sqlUpdate);
             if(!query.exec(sqlUpdate))
             {
                qDebug()<<"update date error"<<dp.lastError();
             }//更新操作
     this->close();
}
