#ifndef QTSERVE_H
#define QTSERVE_H

#include <QMainWindow>

namespace Ui {
class QtServe;
}

class QtServe : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtServe(QWidget *parent = nullptr);
    ~QtServe();

private:
    Ui::QtServe *ui;
};

#endif // QTSERVE_H
