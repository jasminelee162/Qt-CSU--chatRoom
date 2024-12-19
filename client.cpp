#include "client.h"
#include "ui_client.h"


#include <QtCore>
#include <QtNetwork>

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);

}

client::~client()
{
    delete ui;
}
