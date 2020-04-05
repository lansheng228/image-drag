#include "imagedragwidget.h"
#include <QDebug>

imageDragWidget::imageDragWidget(QQuickPaintedItem *parent) :
        QQuickPaintedItem(parent) {
    //得到屏幕尺寸
    QScreen *screen = QGuiApplication::primaryScreen();
    int screen_width = screen->size().width();
    int screen_height = screen->size().height();
    qDebug() << "屏幕尺寸: " << screen_width << "*" << screen_height;
    //圆圈所在图片尺寸为:452*452; 图标所在图片尺寸为:350*350
    //滑动图标的尺寸128*128
    double widgetScale = (double) 580 / (double) 720;
    qDebug() << "控件占屏幕比例: " << widgetScale;
    //设置控件尺寸
    setContentsSize(QSize(screen_width * widgetScale, screen_width * widgetScale));
    int widget_width = contentsSize().width();
    int widget_height = contentsSize().height();
    qDebug() << "控件尺寸: " << widget_width << "*" << widget_height;
    //接收鼠标左键
    setAcceptedMouseButtons(Qt::LeftButton);
    circle_defaultImg = new QPixmap(":/images/circle_default.png");
    circle_boldImg = new QPixmap(":/images/circle_bold.png");
    power_haloImg = new QPixmap(":/images/power_halo.png");;
    power_solidImg = new QPixmap(":/images/power_solid.png");
    power_defaultImg = new QPixmap(":/images/power_default.png");
    isOut = false;
    circleImg = circle_defaultImg;
    int circle_width = circleImg->width();
    int circle_height = circleImg->height();
    //设置圆圈图片在实际屏幕上的尺寸
    //滑动图标的尺寸128*128
    int circle_width_in_widget = widget_width - 128 * widgetScale;
    int circle_height_in_widget = widget_height - 128 * widgetScale;
    qDebug() << "滑动圆圈尺寸: " << circle_width_in_widget << "*" << circle_height_in_widget;
    widthScale = (double) circle_width_in_widget / (double) circle_width;
    heightScale = (double) circle_height_in_widget / (double) circle_height;
    qDebug() << "圆圈和图标宽度缩放比例为: " << widthScale << "高度缩放比例为: " << heightScale;
    circleImgRect = new QRect(0, 0, circle_width * widthScale, circle_height * heightScale);
    //圆圈图片移到控件中心
    circleImgRect->moveCenter(QPoint(widget_width / 2, widget_height / 2));
    powerImg = power_defaultImg;
    int power_width = powerImg->width();
    int power_height = powerImg->height();
    powerImgRect = new QRect(0, 0, power_width * widthScale, power_height * heightScale);
    //图标图片移到控件中心
    powerImgRect->moveCenter(circleImgRect->center());
    powerCenterPoint = circleImgRect->center();
}

void imageDragWidget::paint(QPainter *painter) {
    painter->drawPixmap(*circleImgRect, *circleImg);
    painter->drawPixmap(*powerImgRect, *powerImg);
}

void imageDragWidget::mouseMoveEvent(QMouseEvent *event) {
    if (pressFlag) {
        //鼠标已按下
        int power_width = powerImgRect->width();
        int power_height = powerImgRect->height();
        int circle_width = circleImgRect->width();
        int circle_height = circleImgRect->height();
        currentMousePoint = event->pos();
        int flag = circleContain();
        if (flag < 0) {
            //鼠标在圆圈内，则图标移动到鼠标位置
            powerImg = power_haloImg;
            circleImg = circle_defaultImg;
            powerImgRect->moveCenter(currentMousePoint);
            powerCenterPoint = currentMousePoint;
            isOut = false;
        } else if (flag == 0) {
            //鼠标在圆圈上，则图标移动到鼠标位置，同时更换图片
            powerImg = power_solidImg;
            circleImg = circle_boldImg;
            powerImgRect->moveCenter(currentMousePoint);
            powerCenterPoint = currentMousePoint;
            isOut = true;
        } else {
            //鼠标在圆圈外
            isOut = true;
            if (powerContain() > 0) {
                //鼠标在圆圈外且在图标外，则等同于鼠标释放。图标回到控件中心。
                powerImg = power_defaultImg;
                powerImgRect->moveCenter(circleImgRect->center());
                pressFlag = false;
                circleImg = circle_defaultImg;
            } else {
                //鼠标在圆圈外且不在图标外，则图标移到鼠标与控件中心连线和圆圈的交点。
                powerImg = power_solidImg;
                circleImg = circle_boldImg;
                powerCenterPoint = GetPoint(currentMousePoint,
                                            circleImgRect->center(), circleImgRect->width() / 2);
                powerImgRect->moveCenter(powerCenterPoint);
            }
        }
        powerImgRect->setHeight(power_height);
        powerImgRect->setWidth(power_width);
        circleImgRect->setHeight(circle_height);
        circleImgRect->setWidth(circle_width);
        update();
        if (pressFlag && (!isOut)) {
            //鼠标按下且在圆圈内
            emit dragMoveIn();
        } else if (pressFlag && isOut) {
            //鼠标按下且在圆圈上
            emit dragMoveOn();
        } else if ((!pressFlag) && (isOut)) {
            //鼠标在圆圈外且在图标外，则等同于鼠标释放。
            emit dragRelease();
        }
        if (isOut && (!pressFlag)) {
            //鼠标在圆圈外且在图标外，确认关机。
            emit dragOut();
        }
    }
}

void imageDragWidget::mousePressEvent(QMouseEvent *event) {
    currentMousePoint = event->pos();
    if (powerContain() <= 0) {
        //鼠标进入到图标内，则表示按下
        pressFlag = true;
        int power_width = powerImgRect->width();
        int power_height = powerImgRect->height();
        powerImg = power_haloImg;
        powerImgRect->setHeight(power_height);
        powerImgRect->setWidth(power_width);
        update();
        emit dragPress();
    }
}

void imageDragWidget::mouseReleaseEvent(QMouseEvent *event) {
    //鼠标释放，图标回到控件中心
    currentMousePoint = event->pos();
    pressFlag = false;
    int power_width = powerImgRect->width();
    int power_height = powerImgRect->height();
    powerCenterPoint = circleImgRect->center();
    powerImg = power_defaultImg;
    powerImgRect->moveCenter(circleImgRect->center());
    powerImgRect->setHeight(power_height);
    powerImgRect->setWidth(power_width);
    int circle_width = circleImgRect->width();
    int circle_height = circleImgRect->height();
    circleImg = circle_defaultImg;
    circleImgRect->setHeight(circle_height);
    circleImgRect->setWidth(circle_width);
    update();
    emit dragRelease();
    if (isOut) {
        emit dragOut();
    }
}

//判断鼠标是否在圆圈内
//1:圆圈外;0:圆圈上;-1:圆圈内
int imageDragWidget::circleContain(void) {
    int delta = 0;
    int raduis = 0;
    QPoint p1 = QPoint(0, 0);
    QPoint p2 = QPoint(0, 0);
    int ret = 0;
    p1 = currentMousePoint;
    p2 = circleImgRect->center();
    delta = qSqrt(qPow(p1.x() - p2.x(), 2) + qPow(p1.y() - p2.y(), 2));
    raduis = circleImgRect->width() / 2;
    if (delta > raduis) {
        ret = 1;
    } else if (delta < raduis) {
        ret = -1;
    } else {
        ret = 0;
    }
    return ret;
}

//判断鼠标是否在图标内
//1:图标外;0:图标上;-1:图标内
int imageDragWidget::powerContain(void) {
    int delta = 0;
    int raduis = 0;
    QPoint p1 = QPoint(0, 0);
    QPoint p2 = QPoint(0, 0);
    int ret = 0;
    p1 = currentMousePoint;
    p2 = powerCenterPoint;
    delta = qSqrt(qPow(p1.x() - p2.x(), 2) + qPow(p1.y() - p2.y(), 2));
    raduis = (powerImgRect->width() / 2) * 128 / 350;   //整张图片350px*350px,图标为128*128
    if (delta > raduis) {
        ret = 1;
    } else if (delta < raduis) {
        ret = -1;
    } else {
        ret = 0;
    }
    return ret;
}

//求线段与圆圈的交点，该线段一个端点为圆心，另一个端点在圆外。
QPoint imageDragWidget::GetPoint(QPoint currentPoint, QPoint circleCenter, int raduis) {
    int cx = circleCenter.x(); //圆心横坐标
    int cy = circleCenter.y(); //圆心纵坐标
    int edx = currentPoint.x(); //圆外点的横坐标
    int edy = currentPoint.y();  //圆外点的纵坐标
    int r = raduis;  //半径
    int x = 0; //交点横坐标
    int y = 0; //交点纵坐标
    QPoint p = QPoint(0, 0); //交点
    if ((edx > cx) && (edy == cy)) {
        //右轴;
        x = cx + r;
        y = cy;
    } else if ((edx == cx) && (edy < cy)) {
        //上轴;
        x = cx;
        y = cy - r;
    } else if ((edx < cx) && (edy == cy)) {
        //左轴;
        x = cx - r;
        y = cy;
    } else if ((edx == cx) && (edy > cy)) {
        //下轴;
        x = cx;
        y = cy + r;
    } else {
        //不在坐标轴上
        //求得直线方程
        double k = ((double) (edy - cy)) / (edx - cx);
        double b = edy - k * edx;
        //列方程
        /*
            (1 + k^2)*x^2 - x*(2*cx -2*k*(b -cy) ) + cx*cx + ( b - cy)*(b - cy) - r*r = 0
          */
        double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        double c = cx * cx + (b - cy) * (b - cy) - r * r;
        double a = (1 + k * k);
        double b1 = (2 * cx - 2 * k * (b - cy));
        //得到下面的简化方程
        // a*x^2 - b1*x + c = 0;
        double tmp = sqrt(b1 * b1 - 4 * a * c);
        x1 = (b1 + tmp) / (2 * a);
        y1 = k * x1 + b;
        x2 = (b1 - tmp) / (2 * a);
        y2 = k * x2 + b;
        if ((edx > cx) && (edy > cy)) {
            //第四象限;
            x = x1;
            y = y1;
        } else if ((edx > cx) && (edy < cy)) {
            //第一象限;
            x = x1;
            y = y1;
        } else if ((edx < cx) && (edy < cy)) {
            //第二象限;
            x = x2;
            y = y2;
        } else if ((edx < cx) && (edy > cy)) {
            //第三象限;
            x = x2;
            y = y2;
        }
    }
    p.setX(x);
    p.setY(y);
    return p;
}

imageDragWidget::~imageDragWidget() {
    delete circle_defaultImg;
    delete circle_boldImg;
    delete power_haloImg;
    delete power_solidImg;
    delete power_defaultImg;
    delete circleImgRect;
    delete powerImgRect;
}
