#include "qtserve.h"
#include "ui_qtserve.h"

QtServe::QtServe(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtServe)
{
    ui->setupUi(this);
}

QtServe::~QtServe()
{
    delete ui;
}
