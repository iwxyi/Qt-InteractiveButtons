#include "waterfloatbutton.h"

WaterFloatButton::WaterFloatButton(QWidget *parent) : InteractiveButtonBase(parent), in_area(false), mwidth(16), radius(8)
{

}

WaterFloatButton::WaterFloatButton(QString text, QWidget *parent) : InteractiveButtonBase(text, parent), in_area(false), mwidth(16), radius(8)
{

}

void WaterFloatButton::enterEvent(QEvent *event)
{

}

void WaterFloatButton::leaveEvent(QEvent *event)
{

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

        if (!pressing && inArea(event->pos()))
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

}

void WaterFloatButton::paintEvent(QPaintEvent *event)
{
    return InteractiveButtonBase::paintEvent(event);
}

QPainterPath WaterFloatButton::getBgPainterPath()
{

}

QPainterPath WaterFloatButton::getWaterPainterPath(InteractiveButtonBase::Water water)
{

}

bool WaterFloatButton::inArea(QPoint point)
{
    return true;
}
