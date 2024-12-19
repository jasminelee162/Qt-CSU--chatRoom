#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include<QUdpSocket>
#include <QTableView>
#include<tcpserver.h>
#include<tcpclient.h>
#include<chat.h>
#include "groupvoice.h"
#include<QClipboard>
#include<QtMultimedia/QSound>
#include <QTableWidgetItem>


namespace Ui {
class ChatWindow;
}

//enum MessageType{
//    Message,
//    NewParticipant,
//    ParticipantLeft,
//    Refuse,



//};

class QUdpSocket;
class TcpServer;
class QTextCharFormat;
class tcpclient;


struct User{
    QString hostaddress;
    QString address;
};

class ChatWindow : public QMainWindow
{
    Q_OBJECT
//    QString destUsers;

public:
    explicit ChatWindow(QWidget *parent,QString Name,User u= {"",""});
    ~ChatWindow();
    //用于从登陆端接收用户信息
    void setUser(User u);
    QString getUsr();//获取用户名
    //QString getMsg();//获取聊天信息
    //void onCompleteCature(QPixmap captureImage);

signals:
    //关闭窗口发送关闭信息
    void closeWidget();
    //void on_usrTblWidget_itemDoubleClicked()
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
    //新成员加入
    void newParticipant(QString userName,
                        QString localHostName,QString ipAddress);
    //成员离开
    void participantLeft(QString userName,
                         QString localHostName,QString time);
    //广播用户登录信息
    void sendMessage(MessageType type,QString serverAddress = "");
    //判断是否接收、发送文件
    void hasPendingFile(QString userName, QString serverAddress,
                        QString clientAddress,QString fileName);
    //保存接受的文件
    bool saveFile(const QString &fileName);
    //关闭事件
    void closeEvent(QCloseEvent *event);
    //事件过滤器，按下回车发送信息
    bool eventFilter(QObject *obj, QEvent *event);

    //获取IP地址
    QString getIP();
    //获取用户名
    QString getUserName();
    //获取消息
    QString getMessage();

    void processPendingDatagrams();

private slots:
    //传输文件按钮--槽
    void on_sendToolBtn_clicked();
    //获取发送过来的文件名
    void getFileName(QString);
    //发送消息
    void on_sendBtn_clicked();

    void on_toolButton_clicked();

    void onCompleteCature(QPixmap captureImage);

    void on_userTblWidget_doubleClicked(const QModelIndex &index);

    void showXchat(QString localHostName, QString ipAddress);

    //void on_usrTblWidget_itemDoubleClicked(QTableWidgetItem *item);

    //QString gettext(int,int);


    //void on_usrTblWidget_itemClicked(QTableWidgetItem *item);

    //void on_usrTblWidget_cellDoubleClicked(int row, int column);


    void on_usrTblWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_voiceButton_clicked(bool checked);

    void action1_slot();

    void action2_slot();
    void action3_slot();

private:
    Ui::ChatWindow *ui;

    bool is_drag_=false;

    QPoint mouse_start_point;
    QPoint windows_start_point;
    QColor color;//设置字体颜色
    QUdpSocket *udpSocket;//udp套接字
    qint16 port;//端口
    QString uName;//用户名
    void ReceiveMessage();//接受UDP的消息
    void setAppearance();
    QString fileName;
    QString clientname;
    tcpserver *server;
    tcpclient *client;
    GroupVoice *newvoice;

    Chat *privateChat;
    Chat *privateChat1;

    User users;

    QString textEditPhotoPath;

    QAction *action1;
    QAction *action2;
    QAction *action3;


};

#endif // CHATWINDOW_H
