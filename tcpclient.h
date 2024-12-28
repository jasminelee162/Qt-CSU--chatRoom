#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include<QHostAddress>
#include<QFile>
#include<QTime>
#include<QString>
//文件传输客户端
class QTcpSocket;

namespace Ui {
class tcpclient;
}

class tcpclient : public QDialog
{
    Q_OBJECT

public:
    explicit tcpclient(QWidget *parent = nullptr);
    ~tcpclient();
    //设置主机的地址
    void setHostAddress(QHostAddress address);
    //设置要传输的文件名
    void setFileName(QString fileName);

protected:
    //关闭事件
    void closeEvent(QCloseEvent *);

private:
    Ui::tcpclient *ui;

    QTcpSocket*tcpClient;
    quint16 blockSize;
    QHostAddress hostAddress;
    quint16 tcpPort;
    quint64 TotalBytes;
    quint64 bytesReceive;
    quint64 bytesToReceive;
    quint64 fileNameSize;
    QString fileName;
    QFile *localFile;
    QByteArray inBlock;


    QTime time;
    //tcpclient *client;

private slots:
    //与服务端连接
    void newConnect();
    //保存接收文件
    void readMessage();
    //显示错误
    void displayError(QAbstractSocket::SocketError);

    //取消按钮-槽
    void on_on_tcpClientBtn_clicked();
    //关闭按钮-槽
    void on_on_tcpClientCloseBtn_clicked();
};

#endif // TCPCLIENT_H
