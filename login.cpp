#include "login.h"
#include "ui_login.h"
#include<QMovie>
#include<QMouseEvent>
#include<QMessageBox>
#include <QNetworkInterface>
#include<register.h>
#include<QSqlError>
#include<QSqlQuery>


login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    //去掉边框
    setWindowFlags(Qt::WindowType::FramelessWindowHint);
    //背景透明
    setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);
//    //设置动画
//    QMovie * movie=new QMovie(this);
//    movie->setFileName("");
//    movie->start();
//    ui->widget->setMovie(movie);
      //设置鼠标可以拖动界面


    //设置账户和密码的默认值
    ui->username->setPlaceholderText("账号");
    ui->password->setPlaceholderText("密码");
    //设置头像为圆形
   // ui->touxiang->setPixmap(QPixmap("://:/images/20220808164925.jpg"));
    ui->touxiang->setMask(QRegion(ui->touxiang->rect(),QRegion::RegionType::Ellipse));
    ui->username->setValidator(new QIntValidator(ui->username));
    ui->password->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]+$")));
    ui->password->setEchoMode(QLineEdit::Password);


}

login::~login()
{
    delete ui;
}
QString login::getIP()
{
    //对于所有检测到的地址，如果检测到时ipv4就输出
    QList<QHostAddress>list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
}

QString login::getID()
{
    QString ID=ui->username->text();
    return ID;
    qDebug()<<ID;
}

void login::mouseMoveEvent(QMouseEvent *event)
{
    //只允许左键移动 持续动作
    if(event->buttons()&Qt::LeftButton)//buttons是处理长事件，button是处理短事件
    {
        //窗口随鼠标移动

        //窗口移动的新位置=鼠标移动的位置-差值
        move(event->globalPos()-m_pt);
    }
}

void login::mousePressEvent(QMouseEvent *ev)
{
    //鼠标左键按下  单击
    if(ev->button()&Qt::LeftButton)
    {
        //求差值=当前鼠标的位置-窗口左上角的点
        m_pt=ev->globalPos()-this->geometry().topLeft();//geometry()是矩形窗口 topLeft()是左上角的信息
    }
}


void login::on_pushButton_3_clicked()
{
    this->close();
}


void login::on_pushButton_4_clicked()
{
    this->showMinimized();
}


void login::on_btton_clicked()
{

    QByteArray ch = ui->password->text().toLatin1();
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

    if(ui->username->text() == "" || ui->password->text() == "")
        {
        QMessageBox::warning(0,tr("警告"),
                             tr("密码和账户不能为空"),QMessageBox::Ok);
        return;
        }
    //密码长度小于8
    else if (ui->password->text().count() < 8)
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
        QString Username = ui->username->text();
        //获取密码输入框的数据
        QString Password = ui->password->text();
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
            //查询验证
            QString sql=QString("select * from user where username='%1'and password='%2'")
                                  .arg(Username).arg(Password);
            QSqlQuery query(sql);
            if(!query.next()){
                qDebug()<<"shibai";
                QMessageBox::warning(0,tr("警告"),
                              tr("账号或密码错误"),QMessageBox::Ok);
             return;
            }
            users.hostaddress = getIP();
            users.address = getIP();
            qDebug()<<getIP();
            qDebug()<<users.address;
            qDebug()<<"登陆成功！";
        this->close();
        dp.close();
        messageMe *me=new messageMe;
        me->show();

//        list *list1 =new list;
//        list1->show();
}

//注册账号
void login::on_pushButton_clicked()
{
    Register *reg=new Register;
    reg->show();
}

//找回密码
void login::on_pushButton_5_clicked()
{
    findpwd *pwd=new findpwd;
    pwd->show();
    QMessageBox::warning(0,tr("警告"),
                  tr("需要填写正确的个人信息才能修改密码"),QMessageBox::Ok);
}
