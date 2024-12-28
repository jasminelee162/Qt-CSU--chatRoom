#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QMouseEvent>
#include<QDialog>
#include<QColorDialog>
#include<QFileDialog>
#include<QMessageBox>
#include<capturescreen.h>
#include <QTextCursor>


MainWindow::MainWindow(QWidget *parent,QString Name) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);
    ui->pushSent->setEnabled(false);
    this->ui->timeBut->setEnabled(false);
    tcpClient->abort();
    connect(tcpClient,&QTcpSocket::readyRead,
            [&](){this->ui->txtData->append(tr("%1 Server Say：%2").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(QString(this->tcpClient->readAll())));});
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));
    connect(&tm,&QTimer::timeout,[&](){
            int i = qrand() % 6;
            this->ui->txtData->append(tr("%1 Timer Sent: %2").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(list.at(i)));
            tcpClient->write(list.at(i).toUtf8());
    });
    connect(tcpClient,&QTcpSocket::disconnected,[](){qDebug()<< "123333" ;});
    list << "早上好" << "吃饭了没" << "今天天气怎么样" << "今天也要开心啊" << "快点哦" <<  "可能要迟到了" << "再见啦" ;
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    this->ui->txtIp->setText("127.0.0.1");
    this->ui->txtPort->setText("6666");


    //字体设置
    connect(ui->fontCbx,&QFontComboBox::currentFontChanged,this,[=](const QFont &f){
        ui->textEdit->setCurrentFont(f);
        ui->textEdit->setFocus();
        });
    // 字号设置
    void (QComboBox:: * cbxSingal)(const QString &text) = &QComboBox::currentIndexChanged;
    connect(ui->sizeCbx,cbxSingal,this,[=](const QString &text){
        ui->textEdit->setFontPointSize(text.toDouble());
        ui->textEdit->setFocus();
        });
    //加粗
    connect(ui->boldTBtn,&QToolButton::clicked,this,[=](bool checked){
        if(checked){
               ui->textEdit->setFontWeight(QFont::Bold);
          }else{
               ui->textEdit->setFontWeight(QFont::Normal);
           }
           ui->textEdit->setFocus();
        });
    //倾斜
    connect(ui->italicTBtn,&QToolButton::clicked,this,[=](bool checked){
     ui->textEdit->setFontItalic(checked);
     ui->textEdit->setFocus();
       });
    //下划线
    connect(ui->underlineTBtn,&QToolButton::clicked,this,[=](bool checked){
         ui->textEdit->setFontUnderline(checked);
         ui->textEdit->setFocus();
       });
    //设置文本颜色
    connect(ui->colorTBtn,&QToolButton::clicked,this,[=](){
     color = QColorDialog::getColor(Qt::green,this);     //color对象可以在widget.h中定义私有成员
     if(color.isValid())
     {
         ui->textEdit->setTextColor(color);
         ui->textEdit->setFocus();
     }
     });
    //保存聊天记录
    connect(ui->saveTBtn,&QToolButton::clicked,this,[=](){
       if(ui->txtData->document()->isEmpty()){
           QMessageBox::warning(this,"警告","聊天记录为空，无法保存！",QMessageBox::Ok);
       }
       else{
           QString fName = QFileDialog::getSaveFileName(this,"保存聊天记录","聊天记录","(*.txt)");
           if(!fName.isEmpty()){
               //保存名称不为空再做保存操作
               QFile file(fName);
               file.open(QIODevice::WriteOnly | QFile::Text);
               QTextStream stream(&file);
               stream << ui->textEdit->toPlainText();
               file.close();
           }
       }
     });
    //清空聊天记录
    connect(ui->clearTBtn,&QToolButton::clicked,[=](){
     ui->txtData->clear();
   });

}

MainWindow::~MainWindow()
{
    delete ui;
    delete tcpClient;
}

void MainWindow::on_pushConnect_clicked()
{
    qDebug() << "点击连接：" ;
    if ("连接" == this->ui->pushConnect->text())
    {
        QString ipAdd(this->ui->txtIp->text()), portd(this->ui->txtPort->text());
        if (ipAdd.isEmpty() || portd.isEmpty())
        {
            this->ui->txtData->append("请输入IP和端口!");
            return;
        }
        tcpClient->connectToHost(ipAdd,portd.toInt());
        if (tcpClient->waitForConnected(1000))
        {
            ui->pushConnect->setText("断开");
            ui->txtData->append("连接服务器成功");
            ui->pushSent->setEnabled(true);
            this->ui->txtIp->setEnabled(false);
            this->ui->txtPort->setEnabled(false);
            this->ui->timeBut->setEnabled(true);
        }
    }
    else
    {
        tcpClient->disconnectFromHost();
        if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000) )
        {
            ui->pushConnect->setText("连接");
            ui->txtData->append("断开服务器");
            ui->pushSent->setEnabled(false);
            this->ui->txtIp->setEnabled(true);
            this->ui->txtPort->setEnabled(true);
            tm.stop();
            this->ui->timeBut->setEnabled(false);
            this->ui->lineEdit->setEnabled(true);
            this->ui->timeBut->setText("启动定时");
        }
    }
}

void MainWindow::on_pushSent_clicked()
{
    qDebug() << "点击发送：" ;
    QString data = this->ui->textEdit->toPlainText();//->text();
    if (data.isEmpty())
    {
        return ;
    }
    tcpClient->write(data.toUtf8());
    ui->txtData->append(tr("Say：%1").arg(data));
}

void MainWindow::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    ui->pushConnect->setText("连接");
    ui->textEdit->append(tr("连接出错：%1").arg(tcpClient->errorString()));
    ui->pushSent->setEnabled(false);
    this->ui->txtIp->setEnabled(true);
    this->ui->txtPort->setEnabled(true);
    tm.stop();
    this->ui->timeBut->setEnabled(false);
    this->ui->lineEdit->setEnabled(true);
    this->ui->timeBut->setText("启动定时");
}

void MainWindow::on_timeBut_clicked()
{
    if (this->ui->lineEdit->text().isEmpty())
    {
        this->ui->txtData->append("请输入时间：");
        return;
    }
    if ("启动定时" == this->ui->timeBut->text())
    {
        int h = this->ui->lineEdit->text().toInt();
        h = h*1000;
        tm.start(h);
        this->ui->lineEdit->setEnabled(false);
        this->ui->timeBut->setText("停止定时");
    }
    else
    {
        tm.stop();
        this->ui->timeBut->setText("启动定时");
        this->ui->lineEdit->setEnabled(true);
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    emit this->closeWidget();//发送关闭当前窗口的信号
    QWidget::closeEvent(e);
}

void MainWindow::on_toolButton_clicked()
{

    CaptureScreen* captureHelper = new CaptureScreen();
    connect(captureHelper, SIGNAL(signalCompleteCature(QPixmap)), this, SLOT(onCompleteCature(QPixmap)));
    captureHelper->show();
    QMessageBox::warning(0,tr("提示"),
                         tr("Esc可以退出截图，Enter可以保存截图！")
                         ,QMessageBox::Ok);
}

void MainWindow::onCompleteCature(QPixmap captureImage)
{
    ui->label->setPixmap(captureImage);
    QClipboard *clip = QApplication::clipboard();
    clip->setPixmap(captureImage);

    QString time = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
    QString path = "C:/Users/HP/Documents/messagechat/jietu";

    captureImage.save(QString("%1/%2.jpg").arg(path,time));

    textEditPhotoPath = QString("%1/%2.jpg").arg(path,time);
    qDebug()<<"保存截图";

}
