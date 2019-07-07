#include "winrestorebutton.h"

WinRestoreButton::WinRestoreButton(QWidget* parent)
    : InteractiveButtonBase(parent)
{

}

void WinRestoreButton::paintEvent(QPaintEvent* event)
{
    InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();
    int dx = offset_pos.x(), dy = offset_pos.y();
    QRect br(w/3+dx, h/3+dy, w/3, h/3);

    // 画原来的圆
    QPainter painter(this);
    painter.setPen(QPen(icon_color));
    painter.drawRect(br);

    dx /= 2; dy /= 2;
    int l = w*4/9+dx, t = h*2/9+dy, r = w*7/9+dx, b = h*5/9+dy;
    QPoint topLeft(l, t), topRight(r, t), bottomLeft(l, b), bottomRight(r, b);
    QList<QPoint>points;

    /* 两个矩形一样大的，所以运行的时候，需要有三大类：
     * 1、完全重合（可以视为下一点任意之一）
     * 2、有一个点落在矩形内（4种情况）
     * 3、完全不重合
     * 根据3大类共6种进行判断
     */
    if (br.topLeft() == topLeft)
    {
        points << topLeft << topRight << bottomRight << bottomLeft << topLeft;
    }
    else if (br.contains(topLeft)) // 左上角在矩形内
    {
        points << QPoint(br.right()+1, t) << topRight << bottomRight << bottomLeft << QPoint(l, br.bottom()+1);
    }
    else if (br.contains(topRight)) // 右上角在矩形内
    {
        points << QPoint(r, br.bottom()+1) << bottomRight << bottomLeft << topLeft << QPoint(br.left(), t);
    }
    else if (br.contains(bottomLeft)) // 左下角在矩形内（默认）
    {
        points << QPoint(l, br.top()) << topLeft << topRight << bottomRight << QPoint(br.right()+1, b);
    }
    else if (br.contains(bottomRight)) // 右下角在矩形内
    {
        points << QPoint(br.left(), b) << bottomLeft << topLeft << topRight << QPoint(r, br.top());
    }
    else // 没有重合
    {
        points << topLeft << topRight << bottomRight << bottomLeft << topLeft;
    }

    if (points.size() > 1)
    {
        QPainterPath path;
        path.moveTo(points.at(0));
        for (int i = 1; i < points.size(); ++i)
            path.lineTo(points.at(i));
        painter.drawPath(path);
    }
}
