#include "mainwindow.h"
#include"login.h"
#include <QTextCodec>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    //w.show();
    login n;
    n.show();
    return a.exec();
}
