#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QTableView>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
//    int username;
//    QString password;

private slots:

    void on_btn_login_clicked();

private:
    Ui::Register *ui;
    QSqlDatabase dp;
};

#endif // REGISTER_H
