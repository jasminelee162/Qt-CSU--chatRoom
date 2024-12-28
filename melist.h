#ifndef MELIST_H
#define MELIST_H

#include <QWidget>
#include<mainwindow.h>
#include<QIcon>
#include <QString>

namespace Ui {
class melist;
}

class melist : public QWidget
{
    Q_OBJECT

public:
    explicit melist(QWidget *parent ,QString Name);
    ~melist();

private:
    Ui::melist *ui;
    melist *list1;
    QVector<bool>isShow;//代表是否打开窗口的标识   false代表没有打开，true代表打开
    MainWindow * chatt;
};

#endif // MELIST_H
