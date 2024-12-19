#ifndef LIST_H
#define LIST_H

#include <QWidget>
#include<ChatWindow.h>
#include<QIcon>

namespace Ui {
class list;
}

class list : public QWidget
{
    Q_OBJECT

public:
    explicit list(QWidget *parent = nullptr);
    ~list();

private:
    Ui::list *ui;
    list *list1;
    QVector<bool>isShow;//代表是否打开窗口的标识   false代表没有打开，true代表打开
    ChatWindow * chat;
};

#endif // LIST_H
