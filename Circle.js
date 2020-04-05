//判断点是否在圆内
function contain(x1, y1, x2, y2, raduis) {
    var delta = 0;
    var ret = 0;
    delta = Math.round(Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2)));
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
function getPoint(currentPointX, currentPointY, circleCenterX, circleCenterY, raduis) {
    var cx = circleCenterX; //圆心横坐标
    var cy = circleCenterY; //圆心纵坐标
    var edx = currentPointX; //圆外点的横坐标
    var edy = currentPointY;  //圆外点的纵坐标
    var r = raduis;  //半径
    var x = 0; //交点横坐标
    var y = 0; //交点纵坐标
    if ((edx > cx) && (edy === cy)) {
        //右轴;
        x = cx + r;
        y = cy;
    } else if ((edx === cx) && (edy < cy)) {
        //上轴;
        x = cx;
        y = cy - r;
    } else if ((edx < cx) && (edy === cy)) {
        //左轴;
        x = cx - r;
        y = cy;
    } else if ((edx === cx) && (edy > cy)) {
        //下轴;
        x = cx;
        y = cy + r;
    } else {
        //不在坐标轴
        //求得直线方程
        var k = (edy - cy) / (edx - cx);
        var b = edy - k * edx;
        //列方程
        /*
            (1 + k^2)*x^2 - x*(2*cx -2*k*(b -cy) ) + cx*cx + ( b - cy)*(b - cy) - r*r = 0
          */
        var x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        var c = cx * cx + (b - cy) * (b - cy) - r * r;
        var a = (1 + k * k);
        var b1 = (2 * cx - 2 * k * (b - cy));
        //得到下面的简化方程
        // a*x^2 - b1*x + c = 0;
        var tmp = Math.sqrt(b1 * b1 - 4 * a * c);
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
    return [Math.round(x), Math.round(y)];
}
