#include "list.h"
#include "ui_list.h"

#include <QString>
#include<QToolButton>
#include<QToolBox>
#include<QPixmap>
#include<QVBoxLayout>
#include<QGroupBox>
#include<QHBoxLayout>
#include<QLabel>
#include<QMessageBox>
#include<QDebug>


list::list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::list)
{
    ui->setupUi(this);
    //设置图标

    //设置背景
//    QPalette palette;
//    palette.setBrush(/*QPalette::Background*/this->backgroundRole(),
//    QBrush(QPixmap(":/new/prefix1/new_image/OIP-C.jpg")));
//    this->setPalette(palette);

    this->setWindowIcon(QIcon(":/new/prefix1/images/cmanc.jpg"));
    //设置名称
    this->setWindowTitle("Chat");

    QList<QString>nameList;
    nameList<<"Happy Duo"<<"Dragon Duo"<<"Circle Duo"<<"Traveler Duo"<<"Sleepin Duo"<<"Party Duo"<<"Excited Duo"<<"Fly Duo";
    QStringList iconNameList;//图标资源列表
    iconNameList<<"a(1)(1)"<<"b(1)(1)"<<"c(1)(1)"<<"d(1)(1)"<<"e(1)(1)"<<"f(1)(1)"<<"g(1)(1)"<<"h(1)(1)"<<"i(1)(1)";
    QVector<QToolButton*>vToolBtn;
    for(int i=0;i<8;i++)
    {
    QToolButton * btn=new QToolButton(this);
    //QToolButton * btn2=new QToolButton(this);
    //QVBoxLayout * pLayout=new QVBoxLayout;
    //设置网名
    btn->setText(QString("%1").arg(nameList[i]));
    //设置头像   :/new/prefix1/imagesss/a.jpg
    btn->setIcon(QPixmap(QString(":/new/prefix1/imagesss/%1.jpg").arg(iconNameList[i])));
    //设置图片大小
    btn->setIconSize(QPixmap(QString(":/new/prefix1/imagesss/%1.jpg").arg(iconNameList[i])).size());
    //设置透明
    btn->setAutoRaise(true);
    //设置显示的格式
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //放到vlayout
    ui->vLayout->addWidget(btn);
    //ui->vLayout2->addWidget(btn2);
    vToolBtn.push_back(btn);
    //九个槽函数默认都是false
    isShow.push_back(false);
    }
    //对九个按钮进行添加信号槽
    for(int i=0;i<vToolBtn.size();i++){
    connect(vToolBtn[i],&QToolButton::clicked,[=](){
        //如果被打开了，就不要再一次打开了
        if(isShow[i]){
            QString str=QString ("和%1聊天对话框已经被打开了").arg(vToolBtn[i]->text());
            QMessageBox::warning(this,"警告",str);
            return;
        }
        isShow[i]=true;


        //弹出聊天对话框
        //构造聊天窗口时候，告诉这个窗口他的名字，参数1 顶层方式弹出
        //注意！widget构造函数 并没有这两个参数
        ChatWindow * chat = new ChatWindow(0,vToolBtn[i]->text());
        chat->setWindowTitle(vToolBtn[i]->text());
        chat->setWindowIcon(vToolBtn[i]->icon());
        chat->show();
        //如何窗口关闭将标志位设置回来
        connect(chat,&ChatWindow::closeWidget,[=](){
            isShow[i]=false;
        });
    });
    }
 }
list::~list()
{
    delete ui;
}



