#ifndef IMAGEDRAGWIDGET_H
#define IMAGEDRAGWIDGET_H

#include <QQuickPaintedItem>
#include <QtCore>
#include <QtGui>

class imageDragWidget : public QQuickPaintedItem {
    Q_OBJECT
public:
    explicit imageDragWidget(QQuickPaintedItem *parent = 0);

    ~imageDragWidget();

public:
    signals:

    //鼠标在圆圈内移动
    void dragMoveIn();

    //鼠标按下
    void dragPress();

    //鼠标在圆圈上移动
    void dragMoveOn();

    //鼠标释放
    void dragRelease();

    //鼠标移出圆圈,确认关机
    void dragOut();

public
    slots:

protected:
    void paint(QPainter *painter);

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

private:
    //判断鼠标和圆圈的位置关系：圆圈外、圆圈上、圆圈内
    int circleContain(void);

    //判断鼠标和图标的位置关系：图标外、图标上、图标内
    int powerContain(void);

    //得到鼠标与圆心连线和圆圈的交点
    QPoint GetPoint(QPoint currentPoint, QPoint circleCenter, int raduis);

private:
    QPixmap *circle_defaultImg;
    QPixmap *circle_boldImg;
    QPixmap *power_haloImg;
    QPixmap *power_solidImg;
    QPixmap *power_defaultImg;
    //当前圆圈图片
    QPixmap *circleImg;
    //圆圈图片所在矩形
    QRect *circleImgRect;
    //当前图标图片
    QPixmap *powerImg;
    //图标图片所在矩形
    QRect *powerImgRect;
    //当前鼠标所在位置
    QPoint currentMousePoint;
    //图标所在位置
    QPoint powerCenterPoint;
    //鼠标是否按下的标志
    bool pressFlag;
    //鼠标是否移出的标志
    bool isOut;
    //宽度缩放比例
    double widthScale;
    //高度缩放比例
    double heightScale;
};

#endif // IMAGEDRAGWIDGET_H
