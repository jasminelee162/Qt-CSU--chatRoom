#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include<ChatWindow.h>
#include <QIntValidator>
#include<list.h>
#include<QString>
#include <QDesktopWidget>
#include <QApplication>
#include<ChatWindow.h>
#include<register.h>
#include<QMessageBox>
#include<QSqlDatabase>
#include<findpwd.h>
#include<messageme.h>




namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    QString getIP();
    QString getID();
protected:
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent *ev);

private slots:
    void on_pushButton_3_clicked();

    void on_btton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::login *ui;
    ChatWindow * chat;
    QPoint m_pt;  //差值 ：窗口移动的新位置=鼠标移动的位置-差值
    Register *reg;
    QSqlDatabase dp;
    User users;
    login *llg;
};

#endif // LOGIN_H
