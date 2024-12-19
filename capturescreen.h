#ifndef CAPTURESCREEN_H
#define CAPTURESCREEN_H

#include <QWidget>
#include <QPainter>

class CaptureScreen : public QWidget
{
    Q_OBJECT

public:
    CaptureScreen(QWidget *parent = 0);
    ~CaptureScreen();

Q_SIGNALS:
    void signalCompleteCature(QPixmap catureImage);
    void SendScreenshotWindowPoint(CaptureScreen * );

private:
    void initWindow();//初始化窗口
    void loadBackgroundPixmap();
    QRect getRect(const QPoint &mBeginPoint, const QPoint &mEndPoint);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool m_isMousePress;//鼠标事件
    QPixmap m_loadPixmap, m_capturePixmap;
    int m_screenwidth;//截图宽度
    int m_screenheight;//截图高度
    QPoint m_beginPoint, m_endPoint;
    QPainter m_painter;
    CaptureScreen * m_pScreenshotWindow;// 接收截图窗口的指针地址

};


#endif // CAPTURESCREEN_H
