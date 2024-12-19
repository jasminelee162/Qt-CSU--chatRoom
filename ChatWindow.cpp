#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include<QMouseEvent>
#include<QDialog>
#include<QColorDialog>
#include<QFileDialog>
#include<QMessageBox>
#include<QTextStream>
#include<QDate>
#include<QDateTime>
#include<QScrollBar>
#include <QDesktopWidget>
#include <QMovie>
#include<QHostInfo>
#include<QProcess>
#include<QNetworkInterface>
#include<QKeyEvent>
#include<QTextCharFormat>
#include<tcpclient.h>
#include<tcpserver.h>
#include<QFileDialog>
#include<QDebug>
#include<capturescreen.h>
#include <QTextCursor>
#include <QTableView>
#include <QMenu>

/*
 * 使用UDP协议进行聊天
 * 使用TCP协议进行文件传输
 */


//QString userName 用于传递用户名，让他知道具体的用户名是什么
ChatWindow::ChatWindow(QWidget *parent,QString userName,User u) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{

    //去掉边框
    // this->setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    //背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);

    //设置外观
    setAppearance();
    QMenu *ChangeBackGroundMenu=new QMenu(this);
    ChangeBackGroundMenu->addAction(action1);
    ChangeBackGroundMenu->addAction(action2);
    ChangeBackGroundMenu->addAction(action3);
    ui->changeButton->setMenu(ChangeBackGroundMenu);
    connect(action1,&QAction::triggered,this,&ChatWindow::action1_slot);
    connect(action2,&QAction::triggered,this,&ChatWindow::action2_slot);
    connect(action3,&QAction::triggered,this,&ChatWindow::action3_slot);

    QPixmap whiteground;
    whiteground.load(":/new/prefix1/new_image/P3.jpg");
    whiteground = whiteground.scaled(ui->whiteGround->geometry().size());
    ui->whiteGround->setPixmap(whiteground);

    //设置背景
//    QPalette palette;
//    palette.setBrush(/*QPalette::Background*/this->backgroundRole(),
//    QBrush(QPixmap(":/new/prefix1/images/XHS.jpg")));
//    this->setPalette(palette);


    setUser(u);

    udpSocket = new QUdpSocket(this);
    uName=userName; //获取用户名
    port = 45454;   //设置端口号
    newvoice = NULL;
    udpSocket->bind(port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint); //绑定端口
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    sendMessage(NewParticipant);
    //监听别人发出的信号
    connect(udpSocket,&QUdpSocket::readyRead,this,&ChatWindow::processPendingDatagrams);

    //tcp协议
    server = new tcpserver(this);
    connect(server,SIGNAL(sendFileName(QString)),
            this,SLOT(getFileName(QString)));
    qDebug()<<"000";
    connect(ui->msgTextEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this,SLOT(currentFormatChanged(QTextCharFormat)));

    //设置消息过滤器
    ui->msgTextEdit->installEventFilter(this);
    //构造函数中对其调用
    //鼠标回车键事件
    ui->sendBtn->setFocus();
    ui->sendBtn->setDefault(true);
    ui->msgTextEdit->installEventFilter(this);//设置完后自动调用其eventFilter函数


    qDebug()<<users.address;
    qDebug()<<"ip";


    connect(ui->usrTblWidget,SIGNAL(cellClicked(int,int)),this,SLOT(gettext(int ,int )));


    //字体设置
    connect(ui->fontCbx,&QFontComboBox::currentFontChanged,this,[=](const QFont &f){
        ui->msgTextEdit->setCurrentFont(f);
        ui->msgTextEdit->setFocus();
        });
    // 字号设置
    void (QComboBox:: * cbxSingal)(const QString &text) = &QComboBox::currentIndexChanged;
    connect(ui->sizeCbx,cbxSingal,this,[=](const QString &text){
        ui->msgTextEdit->setFontPointSize(text.toDouble());
        ui->msgTextEdit->setFocus();
        });
    //加粗
    connect(ui->boldTBtn,&QToolButton::clicked,this,[=](bool checked){
        if(checked){
               ui->msgTextEdit->setFontWeight(QFont::Bold);
          }else{
               ui->msgTextEdit->setFontWeight(QFont::Normal);
           }
           ui->msgTextEdit->setFocus();
        });
    //倾斜
    connect(ui->italicTBtn,&QToolButton::clicked,this,[=](bool checked){
     ui->msgTextEdit->setFontItalic(checked);
     ui->msgTextEdit->setFocus();
       });
    //下划线
    connect(ui->underlineTBtn,&QToolButton::clicked,this,[=](bool checked){
         ui->msgTextEdit->setFontUnderline(checked);
         ui->msgTextEdit->setFocus();
       });
    //设置文本颜色
    connect(ui->colorTBtn,&QToolButton::clicked,this,[=](){
     color = QColorDialog::getColor(Qt::green,this);     //color对象可以在widget.h中定义私有成员
     if(color.isValid())
     {
         ui->msgTextEdit->setTextColor(color);
         ui->msgTextEdit->setFocus();
     }
     });
    //保存聊天记录
    connect(ui->saveTBtn,&QToolButton::clicked,this,[=](){
       if(ui->msgBrowser->document()->isEmpty()){
           QMessageBox::warning(this,"警告","聊天记录为空，无法保存！",QMessageBox::Ok);
       }
       else{
           QString fName = QFileDialog::getSaveFileName(this,"保存聊天记录","聊天记录","(*.txt)");
           if(!fName.isEmpty()){
               //保存名称不为空再做保存操作
               QFile file(fName);
               file.open(QIODevice::WriteOnly | QFile::Text);
               QTextStream stream(&file);
               stream << ui->msgBrowser->toPlainText();
               file.close();
           }
       }
     });
    //清空聊天记录
    connect(ui->clearTBtn,&QToolButton::clicked,[=](){
     ui->msgBrowser->clear();
   });

}
ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::setAppearance()
{
//    //重新设置窗口大小为1000*800
//    this->resize(1000,800);
//    //隐藏标题栏
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);


    //设置光标在输入框
//    ui->msgTextEdit->setFocusPolicy(Qt::StrongFocus);
//    ui->msgTextEdit->setFocusPolicy(Qt::NoFocus);
//    ui->msgTextEdit->setFocus();

    QPalette palette;
        palette.setBrush(/*QPalette::Background*/this->backgroundRole(),
        QBrush(QPixmap(":/new/prefix1/images/XHS.jpg")));
        this->setPalette(palette);
    //ui->stackedWidget->setStyleSheet("QStackedWidget {border-image: url(:/chat/image/background1.png);}");
    action1=new QAction(this);
    action2=new QAction(this);
    action3=new QAction(this);
    action1->setIcon(QIcon(":/chat/image/icon/icon_menu.png"));
    action1->setText(tr("原始"));
    action2->setIcon(QIcon(":/chat/image/icon/icon_menu.png"));
    action2->setText(tr("款式2"));
    action3->setIcon(QIcon(":/chat/image/icon/icon_menu.png"));
    action3->setText(tr("自定义"));
}





void ChatWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        is_drag_=true;
        //获取鼠标的初始位置
        mouse_start_point=event->globalPos();
        //获取窗口的初始位置
        windows_start_point=this->frameGeometry().topLeft();
    }
}

void ChatWindow::mouseMoveEvent(QMouseEvent *event)
{
    //判断是否拖拽移动
    if(is_drag_)
    {
        //判断鼠标移动的距离
        QPoint move_distance=event->globalPos()-mouse_start_point;
        //改变窗口的位置
        this->move(windows_start_point+move_distance);
    }
}

void ChatWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //放下左键及时停止移动
    if(event->button()==Qt::LeftButton)
    {
        is_drag_=false;
    }
}


//成员加入处理函数
void ChatWindow::newParticipant(QString userName, QString localHostName, QString ipAddress)
{
    //使用姓名判断用户是否已经加入
    bool isEmpty = ui->usrTblWidget
            ->findItems(userName,Qt::MatchExactly).isEmpty();

    //如果用户未加入则向界面右侧的用户列表加入新用户的信息
    if(isEmpty ) {
        ipAddress=getIP();
        QTableWidgetItem *user = new QTableWidgetItem(userName);
        QTableWidgetItem *host = new QTableWidgetItem(localHostName);
        QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);

        qDebug()<<"ipppp";
        qDebug()<<ipAddress;
        ui->usrTblWidget->insertRow(0);
        ui->usrTblWidget->setItem(0,0,user);
        ui->usrTblWidget->setItem(0,1,host);
        ui->usrTblWidget->setItem(0,2,ip);

        //追加聊天记录
        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
        //在线人数更新
        ui->msgBrowser->append(tr("%1 在线").arg(userName));
        ui->UsrNumLbl->setText(tr("在线人数： %1")
                                  .arg(ui->usrTblWidget->rowCount()));
        //发送新用户登录消息
        sendMessage(NewParticipant);
    }
}

//成员离开处理函数
void ChatWindow::participantLeft(QString userName, QString localHostName, QString time)
{
    //更新右侧tableWidget
    int rowNum = ui->usrTblWidget->findItems(localHostName,
                                                Qt::MatchExactly).first()->row();
    ui->usrTblWidget->removeRow(rowNum);
//        bool isEmpty =ui->usrTblWidget->findItems(userName,Qt::MatchExactly).isEmpty();
//        if(!isEmpty)
//        {
//            ui->usrTblWidget->removeRow(isEmpty);
            //追加聊天记录
            ui->msgBrowser->setTextColor(Qt::gray);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman ",10));
            ui->msgBrowser->append(QString("%1 于 %2 之后离开！")
                                   .arg(userName).arg(time));
             ui->UsrNumLbl->setText(tr("在线人数：%1人")
                                    .arg(ui->usrTblWidget->rowCount()));
        }
//}

//广播UDP消息
void ChatWindow::sendMessage(MessageType type, QString serverAddress)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName = QHostInfo::localHostName();   //获取用户名、主机名
    QString address = getIP();
    qDebug()<<getIP();
    out << type << getUserName() << localHostName;

    QString clientAddress;
    int row;

    //判断发送消息的类型
    switch(type)
    {
    case Message:
        //发送的是消息
        if(ui->msgTextEdit->toPlainText() == "")
        {
            QMessageBox::warning(0,tr("警告"),
                                 tr("发送内容不能为空"),QMessageBox::Ok);
            return;
        }
       //第二段数据， 具体的内容
       out << getUserName() << getMessage();//发送的聊天信息  发送格式 消息类型+用户名+发送内容
       ui->msgBrowser->verticalScrollBar()
               ->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
        break;

    case NewParticipant:
        //广播新用户加入的消息
       out << getUserName();
       QSound::play(":/new/prefix1/friend.wav");
       break;

    case ParticipantLeft:
        //广播用户离开的消息
        out<<getUserName();
        break;

    case FileName:
        //发送传输的文件名
        row = ui->usrTblWidget->currentRow();
        clientAddress= ui->usrTblWidget->item(row,2)->text();
        out << address << clientAddress << fileName;
        break;

    case Refuse:
        //发送拒绝接受文件消息
        out << serverAddress;
        break;
    default:
        break;
    }
    //进行udp广播
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);
}




//QString ChatWindow::gettext(int row1,int col1)
//{
//   //qDebug() << "所点击的单元格的row=" << row1 << ",col=" << col1;
//    // 获取所点击的单元格
//    QTableWidgetItem* item =ui->usrTblWidget->item(row1 ,col1);
//    qDebug() << "123";
//    qDebug() << item->text();
//    qDebug() << "123";
//    textt=item->text();
//    //this->destUsers = item->text();
//    return textt;
//}


//判断是否接收、发送文件
void ChatWindow::hasPendingFile(QString userName, QString serverAddress,QString clientAddress, QString fileName)
{

    QString ipAddress = getIP();
    QString username=getUserName();

    if(ipAddress == clientAddress)
        {
            int btn = QMessageBox::information(this,tr("接收文件"),
                                           tr("来自%1(%2)的文件：%3,是否接收?")
                                           .arg(userName).arg(serverAddress)
                                           .arg(fileName),
                                           QMessageBox::Yes,QMessageBox::No);
            if(btn == QMessageBox::Yes) {
                QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileName);
                if(!name.isEmpty())
                {

                    tcpclient *client = new tcpclient(this);
                    client->setFileName(name);
                    client->setHostAddress(QHostAddress(serverAddress));
                    qDebug()<<"弹出窗口client";
                    client->show();
                }
                else
                {
                   tcpclient  *client = new tcpclient(this);
                   client->localFile =new  QFile(fileName);
            }
            }
            else {
                sendMessage(Refuse,serverAddress);
            }
        }
}

//保存接受的文件
bool ChatWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    qDebug()<<"save success";
    if(!file.open(QFile::WriteOnly|QFile::Text)) {

        QMessageBox::warning(this,tr("保存文件"),
                             tr("无法保存文件 %1:\n %2").arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->msgBrowser->toPlainText();
    return true;
}


//关闭事件：在关闭程序时发送用户离开的广播，让其他端点在其用户列表中删除用户
void ChatWindow::closeEvent(QCloseEvent *event)
{
    emit this->closeWidget();//发送关闭当前窗口的自定义信号
    sendMessage(ParticipantLeft);
    //断开udp套接字
    udpSocket->close();
    udpSocket->destroyed();
    QMainWindow::closeEvent(event);

}

//快捷键ctrl+enter发送消息
bool ChatWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->msgTextEdit){

        if(event->type() == QEvent::KeyPress)//回车键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            //按下ctrl+enter发消息
            if(k->key() == Qt::Key_Return )//&&(k->modifiers() & Qt::ControlModifier
            {
                on_sendBtn_clicked();
                return true;
            }
        }
    }
    return false;
}

//获取IP地址
QString ChatWindow::getIP()
{
    //对于所有检测到的地址，如果检测到时ipv4就输出
    QList<QHostAddress>list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
   qDebug()<<users.address;
}

//获取用户名
QString ChatWindow::getUserName()
{
#if 0
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach(QString string,envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if(index != -1) {
            QStringList stringList = environment.at(index).split('=');
            if(stringList.size() == 2)
            {
                qDebug() << stringList.at(1);
                return stringList.at(1);
                break;
            }
        }
    }
    return "unKnow";
#endif
    return this->uName;
}

//获取用户输入的消息并设置
QString ChatWindow::getMessage()
{
    QString msg = ui->msgTextEdit->toHtml();
    ui->msgTextEdit->clear();   //将文本编辑器内容清空
    ui->msgTextEdit->setFocus();
    return msg;
}


//处理接收到的广播消息
void ChatWindow::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());   //调整为接收到数据的大小
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        in >> messageType;
        QString clientAddress,fileName,clientName;
        QString userName,localHostName,ipAddress,message;
        QString time = QDateTime::currentDateTime()
                .toString("yyyy-MM-dd hh:mm:ss");
        switch(messageType)
        {
        //显示发送的消息
        case Message:
            in >> userName >> localHostName >> ipAddress >> message;
            ui->msgBrowser->setTextColor(Qt::blue);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
            ui->msgBrowser->append("[" + userName + "]" + time);
            ui->msgBrowser->append(message);
            break;

        //有新成员加入
        case NewParticipant:
            in >> userName >> localHostName >> ipAddress;
            newParticipant(userName,localHostName,ipAddress);  //加入处理
            break;

        //有成员离开
        case ParticipantLeft:
            in >> userName >> localHostName;
            participantLeft(userName,localHostName,time);  //进行离开处理
            break;


        //要发送的文件名
        case FileName:
            in >> userName >> localHostName >> ipAddress;
            in >> clientAddress >> fileName;
            hasPendingFile(userName,ipAddress,clientAddress,fileName);
            break;

        //拒绝接受的文件
        case Refuse:
            in >> userName >> localHostName;
            QString serverAddress;
            in >> serverAddress;
            QString ipAddress = getIP();

            if(ipAddress == serverAddress)
            {
                server->refused();
            }
            break;
        }
    }
}

//发送按钮--槽
void ChatWindow::on_sendBtn_clicked()
{
    sendMessage(Message);
    ui->sendBtn->setFocus();
}

//给出文件名
void ChatWindow::getFileName(QString name)
{
    fileName = name;
    sendMessage(FileName);   //upd广播要发送的文件名
}

//传输文件按钮--槽
void ChatWindow::on_sendToolBtn_clicked()
{
    if(ui->usrTblWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(0,tr("提醒"),
                             tr("文件将会发送给所有在线的成员!"),QMessageBox::Ok);
        return;
    }
    server->show();
    server->initServer();
}
void ChatWindow::setUser(User u)
{
    users.address = u.address;
    users.hostaddress = u.hostaddress;
    qDebug() <<users.hostaddress<<users.address;
    qDebug()<<"999";
}

void ChatWindow::on_toolButton_clicked()
{

    CaptureScreen* captureHelper = new CaptureScreen();
    connect(captureHelper, SIGNAL(signalCompleteCature(QPixmap)), this, SLOT(onCompleteCature(QPixmap)));
    captureHelper->show();
    QMessageBox::warning(0,tr("提示"),
                         tr("Esc可以退出截图，Enter可以保存截图！")
                         ,QMessageBox::Ok);
}

void ChatWindow::onCompleteCature(QPixmap captureImage)
{
    ui->label->setPixmap(captureImage);
    QClipboard *clip = QApplication::clipboard();
    clip->setPixmap(captureImage);

    QString time = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
    QString path = "E:\\project\\_qt\\chat-room-QT-main\\chat-room-QT-main\\jietu";

    captureImage.save(QString("%1/%2.jpg").arg(path,time));

    textEditPhotoPath = QString("%1/%2.jpg").arg(path,time);
    qDebug()<<"保存截图";

}



////私聊
void ChatWindow::on_userTblWidget_doubleClicked(const QModelIndex &index)
{
    qDebug() << "222222222";
    if(ui->usrTblWidget->item(index.row(),0)->text() == getUserName())
    {
        QMessageBox::warning(0,tr("警告"),tr("不可以和自己聊天！！！"),QMessageBox::Ok);
        return ;
    }
    else
    {
        if(!privateChat)
        {
            qDebug() << "进入了";
            privateChat = new Chat(ui->usrTblWidget->item(index.row(),1)->text(),
                                   ui->usrTblWidget->item(index.row(),2)->text());
            privateChat->show();

        }
          qDebug() << 1;
        QByteArray data;
        QDataStream out(&data,QIODevice::WriteOnly);
        QString localHostName = QHostInfo::localHostName();
        QString address = getIP();
        out << Xchat << getUserName() << localHostName << address;
        //udpSocket->writeDatagram(data,data.length(),QHostAddress::QHostAddress(ui->userTableWidget->item(index.row(),2)->text()),port);
        udpSocket->writeDatagram(data,data.length(),QHostAddress(ui->usrTblWidget->item(index.row(),2)->text()),port);
        privateChat->show();
        privateChat->is_opend = true;
       qDebug() <<  Xchat << getUserName() << localHostName << address;
    }

}

void ChatWindow::showXchat(QString localHostName, QString ipAddress)
{

   /* if(!privateChat1)
    {
        privateChat1 = new Chat(localHostName,ipAddress);
        privateChat1->show();
        privateChat1->is_opend = true;
    }
    */
    if(!privateChat1)
        privateChat1 = new Chat(localHostName, ipAddress);
    privateChat1->show();

}


void ChatWindow::on_usrTblWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    qDebug() << "222222222";
    if(ui->usrTblWidget->item(1,1)->text() == getUserName())
    {
        QMessageBox::warning(0,tr("警告"),tr("不可以和自己聊天！！！"),QMessageBox::Ok);
        return ;
    }
    else
    {
        if(!privateChat)
        {
            qDebug() << "进入了";
            privateChat = new Chat(ui->usrTblWidget->item(1,0)->text(),
                                   ui->usrTblWidget->item(2,0)->text());
            privateChat->show();

        }
          qDebug() << 1;
        QByteArray data;
        QDataStream out(&data,QIODevice::WriteOnly);
        QString localHostName = QHostInfo::localHostName();
        QString address = getIP();
        out << Xchat << getUserName() << localHostName << address;

        //udpSocket->writeDatagram(data,data.length(),QHostAddress::QHostAddress(ui->userTableWidget->item(index.row(),2)->text()),port);
        udpSocket->writeDatagram(data,data.length(),QHostAddress(ui->usrTblWidget->item(2,2)->text()),port);

        privateChat->show();
        privateChat->is_opend = true;

        qDebug() <<  Xchat << getUserName() << localHostName << address;
    }
}

void ChatWindow::on_voiceButton_clicked(bool checked)
{
    checked = !(newvoice == NULL);
    if(checked)
    {
        newvoice->close();
        delete newvoice;
        newvoice = NULL;
        ui->voiceButton->setStyleSheet("QPushButton { background-color: rgb(50, 50, 50,0); border-radius: 6px; color: rgb(255, 255, 255); border-image: url(:/new/prefix1/images/icon_novoice.png);}"
                                       "QPushButton:hover { background-color:rgba(172, 178, 148, 100)}");
    }
    else
    {
        ui->voiceButton->setStyleSheet("QPushButton { background-color: rgb(50, 50, 50,0); border-radius: 6px; color: rgb(255, 255, 255); border-image: url(:/new/prefix1/images/icon_voice.png);}"
                                       "QPushButton:hover { background-color:rgba(172, 178, 148, 100)}");
        newvoice = new GroupVoice(this);
        newvoice->show();
        newvoice->move((QApplication::desktop()->width()-newvoice->width())/3.3,(QApplication::desktop()->height()-newvoice->height())/9);
    }
}

void ChatWindow::action1_slot()//设置菜单  添加图片
{

    QPixmap whiteground;
    whiteground.load(":/new/prefix1/new_image/P3.jpg");
    whiteground = whiteground.scaled(ui->whiteGround->geometry().size());
    ui->whiteGround->setPixmap(whiteground);


//    QPalette palette;
//        palette.setBrush(/*QPalette::Background*/this->backgroundRole(),
//        QBrush(QPixmap(":/new/prefix1/images/XHS.jpg")));
//        this->setPalette(palette);
    //ui->stackedWidget->setStyleSheet("QStackedWidget {border-image: url(:/chat/image/background1.png);}");

}
void ChatWindow::action2_slot()//设置菜单  添加图片
{
//    QLabel *label = new QLabel();
//       QMovie *movie = new QMovie(":/new/prefix1/new_image/20240711102418.gif");
//        label->setMovie(movie); // 1. 设置要显示的 GIF 动画图片
//        movie->start();         // 2. 启动动画
//        label->show();
    //白色背景
    QPixmap whiteground;
    whiteground.load(":/new/prefix1/new_image/P2.jpg");
    whiteground = whiteground.scaled(ui->whiteGround->geometry().size());
    ui->whiteGround->setPixmap(whiteground);
//    QPalette palette;
//        palette.setBrush(/*QPalette::Background*/this->backgroundRole(),
//        QBrush(QPixmap(":/new/prefix1/list/autumn.jpg")));
//        this->setPalette(palette);

//    ui->stackedWidget->setStyleSheet("QStackedWidget {border-image: url(:/chat/image/background2.jpg);}");

}
void ChatWindow::action3_slot()//设置菜单  添加图片
{

    QString FileName=QFileDialog::getOpenFileName(this,tr("背景"),".",tr("Image file(*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm)"));
    if(FileName!="")
    {

        QPixmap whiteground;
        whiteground.load(FileName);
        whiteground = whiteground.scaled(ui->whiteGround->geometry().size());
        ui->whiteGround->setPixmap(whiteground);


//        QPalette palette;
//            palette.setBrush(/*QPalette::Background*/this->backgroundRole(),
//            QBrush(QPixmap(fileName)));
//            this->setPalette(palette);
        //ui->stackedWidget->setStyleSheet("QStackedWidget {border-image: url("+FileName+");}");
    }
}
