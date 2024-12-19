#ifndef FINDPWD_H
#define FINDPWD_H

#include <QWidget>
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QSqlDriver>
#include <QIntValidator>

namespace Ui {
class findpwd;
}

class findpwd : public QWidget
{
    Q_OBJECT

public:
    explicit findpwd(QWidget *parent = nullptr);
    ~findpwd();

private slots:
    void on_deleteBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::findpwd *ui;
    QSqlDatabase dp;
};

#endif // FINDPWD_H
