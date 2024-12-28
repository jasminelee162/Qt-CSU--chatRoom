#ifndef MESSAGEME_H
#define MESSAGEME_H

#include <QMainWindow>
#include<list.h>
#include<login.h>
#include<mainwindow.h>
#include<QIcon>

namespace Ui {
class messageMe;
}

class messageMe : public QMainWindow
{
    Q_OBJECT

public:
    explicit messageMe(QWidget *parent =nullptr);
    ~messageMe();

private slots:
    void on_pushButton_3_clicked();


private:
    Ui::messageMe *ui;
    QVector<bool>isShow;//代表是否打开窗口的标识   false代表没有打开，true代表打开
    messageMe *Me;
    MainWindow * chatt;
};

#endif // MESSAGEME_H
