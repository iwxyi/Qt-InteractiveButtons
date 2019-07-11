#include "waterfloatbutton.h"

WaterFloatButton::WaterFloatButton(QWidget *parent) : InteractiveButtonBase(parent), in_area(false), mwidth(16), radius(8)
{

}

WaterFloatButton::WaterFloatButton(QString text, QWidget *parent) : InteractiveButtonBase(text, parent), in_area(false), mwidth(16), radius(8), string(text)
{

}

void WaterFloatButton::enterEvent(QEvent *event)
{

}

void WaterFloatButton::leaveEvent(QEvent *event)
{
    if (in_area && !pressing && !inArea(mapFromGlobal(QCursor::pos())))
    {
        in_area = false;
        InteractiveButtonBase::leaveEvent(event);
    }
}

void WaterFloatButton::mousePressEvent(QMouseEvent *event)
{
    if (in_area)
        return InteractiveButtonBase::mousePressEvent(event);
}

void WaterFloatButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressing)
    {
        InteractiveButtonBase::mouseReleaseEvent(event);

        if (!pressing && !inArea(event->pos()))
        {
            in_area = false;
            InteractiveButtonBase::leaveEvent(event);
        }
    }
}

void WaterFloatButton::mouseMoveEvent(QMouseEvent *event)
{
    bool is_in = inArea(event->pos());

    if (!in_area && is_in) // 鼠标移入
    {
        in_area = true;
        InteractiveButtonBase::enterEvent(nullptr);
    }
    else if (in_area && !is_in && !pressing) // 鼠标移出
    {
        in_area = false;
        InteractiveButtonBase::leaveEvent(nullptr);
    }

    if (in_area)
        InteractiveButtonBase::mouseMoveEvent(event);
}

void WaterFloatButton::resizeEvent(QResizeEvent *event)
{
    int w = geometry().width(), h = geometry().height();
    if (h >= w * 4) // 宽度为准
        radius = w / 4;
    else
        radius = h/2;
    mwidth = (w-radius*2);

    return InteractiveButtonBase::resizeEvent(event);
}

void WaterFloatButton::paintEvent(QPaintEvent *event)
{


    return InteractiveButtonBase::paintEvent(event);
}

QPainterPath WaterFloatButton::getBgPainterPath()
{
    QPainterPath path1, path2, path3;
    int w = size().width(), h = size().height();

    QRect mrect(w/2-mwidth/2, h/2-radius, mwidth, radius*2);
    path1.addRect(mrect);

    QPoint o1(w/2-mwidth/2, h/2);
    QPoint o2(w/2+mwidth/2, h/2);
    path2.addEllipse(o1.x()-radius, o1.y()-radius, radius*2, radius*2);
    path3.addEllipse(o2.x()-radius, o2.y()-radius, radius*2, radius*2);

    return path1 | path2 | path3;
}

QPainterPath WaterFloatButton::getWaterPainterPath(InteractiveButtonBase::Water water)
{
    QPainterPath path = InteractiveButtonBase::getWaterPainterPath(water) & getBgPainterPath();
    return path;
}

bool WaterFloatButton::inArea(QPoint point)
{
    int w = size().width(), h = size().height();
    QPoint o1(w/2-mwidth/2, h/2);
    QPoint o2(w/2+mwidth/2, h/2);
    QRect mrect(w/2-mwidth/2, h/2-radius, mwidth, radius*2);

    if (mrect.contains(point))
        return true;
    if ((point-o1).manhattanLength() <= radius ||
            (point-o2).manhattanLength() <= radius)
        return true;
    return false;
}
