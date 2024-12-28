#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>
#include <QString>
#include<QClipboard>
#include<QtMultimedia/QSound>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent ,QString Name);
    ~MainWindow();

public slots:
    void ReadError(QAbstractSocket::SocketError);

    void on_pushSent_clicked();
    void on_pushConnect_clicked();
    void on_timeBut_clicked();

signals:
    //关闭窗口发出关闭信息
    void closeWidget();

protected:
    //关闭事件
    void closeEvent(QCloseEvent *);
private:
    Ui::MainWindow * ui;
    QTcpSocket * tcpClient;
    QTimer tm;
    QStringList list;
    MainWindow *chatt;
    QColor color;//设置字体颜色

    QString textEditPhotoPath;

private slots:
    void on_toolButton_clicked();

    void onCompleteCature(QPixmap captureImage);
};

#endif // MAINWINDOW_H
