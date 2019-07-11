#include "watercirclebutton.h"

WaterCircleButton::WaterCircleButton(QWidget* parent) : InteractiveButtonBase (parent), in_circle(false), radius(16)
{

}

WaterCircleButton::WaterCircleButton(QIcon icon, QWidget *parent) : InteractiveButtonBase (icon, parent), in_circle(false), radius(16)
{

}

WaterCircleButton::WaterCircleButton(QPixmap pixmap, QWidget *parent) : InteractiveButtonBase (pixmap, parent), in_circle(false), radius(16)
{

}

void WaterCircleButton::enterEvent(QEvent *event)
{

}

void WaterCircleButton::leaveEvent(QEvent *event)
{
    if (in_circle && !pressing && !inArea(mapFromGlobal(QCursor::pos())))
    {
        in_circle = false;
        InteractiveButtonBase::leaveEvent(event);
    }
}

void WaterCircleButton::mousePressEvent(QMouseEvent *event)
{
    if (in_circle)
        return InteractiveButtonBase::mousePressEvent(event);
}

void WaterCircleButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressing)
    {
        InteractiveButtonBase::mouseReleaseEvent(event);

        if (!inArea(event->pos()) && !pressing) // 鼠标移出
        {
            in_circle = false;
            InteractiveButtonBase::leaveEvent(nullptr);
        }
    }
}

void WaterCircleButton::mouseMoveEvent(QMouseEvent *event)
{
    bool is_in = inArea(event->pos());

    if (is_in && !in_circle)// 鼠标移入
    {
        in_circle = true;
        InteractiveButtonBase::enterEvent(nullptr);
    }
    else if (!is_in && in_circle && !pressing) // 鼠标移出
    {
        in_circle = false;
        InteractiveButtonBase::leaveEvent(nullptr);
    }

    if (in_circle)
        InteractiveButtonBase::mouseMoveEvent(event);
}

void WaterCircleButton::resizeEvent(QResizeEvent *event)
{
    center_pos = geometry().center() - geometry().topLeft();
    radius = min(size().width(), size().height())/ 2;

    return InteractiveButtonBase::resizeEvent(event);
}

void WaterCircleButton::paintEvent(QPaintEvent *event)
{
    return InteractiveButtonBase::paintEvent(event);
}

QPainterPath WaterCircleButton::getBgPainterPath()
{
    QPainterPath path;
    path.addEllipse(QRect(0,0,size().width(),size().height()));
    return path;
}

QPainterPath WaterCircleButton::getWaterPainterPath(InteractiveButtonBase::Water water)
{
    QPainterPath path = InteractiveButtonBase::getWaterPainterPath(water) & getBgPainterPath();
    return path;
}

bool WaterCircleButton::inArea(QPoint point)
{
    int manh = (point - center_pos).manhattanLength();
    return manh <= radius;
}
