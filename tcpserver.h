#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDialog>
#include<QTime>
#include<QFile>
//文件传输服务器

namespace Ui {
class tcpserver;
}

class QTcpServer;
class QTcpSocket;
class QFile;

class tcpserver : public QDialog
{
    Q_OBJECT

public:
    explicit tcpserver(QWidget *parent = nullptr);
    ~tcpserver();
    //用于初始化
    void initServer();
    //拒绝接收文件
    void refused();
    tcpserver *server;

protected:
    //关闭事件
    void closeEvent(QCloseEvent *);

private:
    Ui::tcpserver *ui;
    qint16 tcpPort;
    QTcpServer *tcpServer;
    QString fileName;//文件名
    QString theFileName;
    QFile *localFile;
    qint64 paylocalSize;
    qint64 totalBytes;//存放总大小信息
    qint64 bytesWritten;
    qint64 bytesToWrite;
    QByteArray outBlock;
    QTcpSocket *clientConnection;
    QTime time;//定时器
private slots:
    //发送数据
    void sendMessage();
    //更新数据——槽
    void updateClientProgress(qint64 numBytes);

    //打开按钮——槽
    void on_serverOpenBtn_clicked();
    //发送按钮——槽
    void on_serverSendBtn_clicked();
    //关闭按钮——槽
    void on_serverCloseBtn_clicked();
signals:
    void sendFileName(QString fileName);
};

#endif // TCPSERVER_H
