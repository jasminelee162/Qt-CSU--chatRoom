#include "voice.h"
#include "ui_voice.h"
#include <QtNetwork/QUdpSocket>
#include <QDebug>
#include <QHostAddress>
#include <QHostInfo>
#include <QPainter>
#include <QPaintEvent>

//为了显示的图片有变化，使用一个静态成员变量累加再模40
int VoiceWindow::i = 0;

VoiceWindow::VoiceWindow(QString userip) :
    ui(new Ui::VoiceWindow)
{
    ui->setupUi(this);
    //设置窗体背景透明
    setAttribute(Qt::WA_TranslucentBackground, true);
    //设置窗体无边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    ip = userip;
    udpSocket= new QUdpSocket(this);
    //绑定10004端口
    udpSocket->bind(QHostAddress(getIP()),10004);

    //定义音频处理的类型
    QAudioFormat format;
    //采集频率为1s 8000次
    format.setSampleRate(8000);
    //设定声道数目，mono(平声道)的声道数目是1；stero(立体声)的声道数目是2
    format.setChannelCount(1);
    //设定采样大小，8也行，发送端和接收端要一致
    format.setSampleSize(16);
    //设定为PCM编码
    format.setCodec("audio/pcm");

    format.setSampleType(QAudioFormat::SignedInt);
    //设定高低位,LittleEndian（低位优先）/LargeEndian(高位优先)
    format.setByteOrder(QAudioFormat::LittleEndian);
    //获取默认的音频输入设备，判断是否支持指定的格式，如果不支持则使用一个邻近的格式
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format))
    {
        format = info.nearestFormat(format);
    }

    input = new QAudioInput(format,this);
    //input开始读入输入的音频信号，写入QIODevice
    inputDevice = input->start();
    output = new QAudioOutput(format, this);
    outputDevice = output->start ();
    //槽函数，当inputDevice收到input写入的音频数据之后,调用onReadyRead函数，发送数据到目标主机上
    connect(inputDevice,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(ReadyReadSlot()));
}

VoiceWindow::~VoiceWindow()
{
    input->stop();
    output->stop();
    delete output;
    delete input;
    qDebug() <<"end!" << endl;
    delete ui;
}

void VoiceWindow::onReadyRead(){

    //为了使图片表现出动态的变化，并且变化较慢
    if(i%40>=0 && i%40<=9)
    {
        this->setStyleSheet("border-image: url(:/new/prefix1/new_image/voice0.png);");
    }
    else if(i%40>=10 && i%40<=19)
    {
        this->setStyleSheet("border-image: url(:/new/prefix1/new_image/voice1.png);");
    }
    else if(i%40>=20 && i%40<=29)
    {
        this->setStyleSheet("border-image: url(:/new/prefix1/new_image/voice2.png);");
    }
    else
    {
        this->setStyleSheet("border-image: url(:/new/prefix1/new_image/voice3.png);");
    }
    i++;

    qDebug()<<"It's sending audio!"<<endl;
    video vp;
    memset(&vp,0,sizeof(vp));

    //读取音频
    vp.lens = inputDevice->read(vp.audiodata,1024);
    //将此结构体发送给目标主机，端口是10004
    udpSocket->writeDatagram((const char*)&vp,sizeof(vp),QHostAddress (ip),10004);



}


QString VoiceWindow::getIP()  // 获取ip地址的方法定义

{

    QString localName = QHostInfo::localHostName();  // 获取本地计算机的主机名
    QList<QHostAddress> list = QHostInfo::fromName(localName).addresses();  // 根据主机名获取所有网络地址
    foreach (QHostAddress address, list)  // 遍历所有网络地址
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)  // 检查地址是否为IPv4协议
         return address.toString();  // 如果是，返回该IPv4地址的字符串表示
    }
    return 0;  // 如果没有找到IPv4地址，这里返回0是不正确的，应该返回一个空字符串或错误信息

}


void VoiceWindow::ReadyReadSlot()

{

    qDebug() << "video reveiver...";  // 打印调试信息，但这里可能是个笔误，应该是"video receiver..."
    video vp1;                         // 定义一个`video`类型的变量`vp1`,这里假设`video`是一个自定义结构体或类
    memset(&vp1, 0, sizeof(vp1));  // 使用memset将`vp1`的内存区域全部设置为0（这通常用于清理原始内存，但这里是否必要取决于`video`的结构）
    udpSocket->readDatagram ((char*)&vp1, sizeof(vp1));  // 从UDP套接字读取数据到`vp1`的指针指向的内存中
    outputDevice->write(vp1.audiodata, vp1.lens);  // 假设`outputDevice`是一个指向某种输出设备的指针，这里将`vp1`中的音频数据写入该设备

}
