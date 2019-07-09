#include "watercirclebutton.h"

WaterCircleButton::WaterCircleButton(QWidget* parent)
    : InteractiveButtonBase (parent), in_circle(false), radius(16)
{
    QString style = "border: 1px #888888; border-radius:3px";
    setStyleSheet(style);
}

void WaterCircleButton::enterEvent(QEvent *event)
{

}

void WaterCircleButton::leaveEvent(QEvent *event)
{

}

void WaterCircleButton::mousePressEvent(QMouseEvent *event)
{
    if (in_circle)
        return InteractiveButtonBase::mousePressEvent(event);
}

void WaterCircleButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressing)
        return InteractiveButtonBase::mouseReleaseEvent(event);
}

void WaterCircleButton::mouseMoveEvent(QMouseEvent *event)
{
    int manh = (event->pos() - center_pos).manhattanLength();

    if (!in_circle)
    {
        if (manh <= radius) // 鼠标移入
        {
            in_circle = true;
            InteractiveButtonBase::enterEvent(nullptr);
        }
    }
    else if (in_circle)
    {
        if (manh > radius) // 鼠标移出
        {
            in_circle = false;
            InteractiveButtonBase::leaveEvent(nullptr);
        }
    }

    if (in_circle)
        InteractiveButtonBase::mouseMoveEvent(event);
}

void WaterCircleButton::resizeEvent(QResizeEvent *event)
{
    center_pos = geometry().center() - geometry().topLeft();
    radius = min(size().width(), size().height())/ 2;
    qDebug() << center_pos << radius;

    return InteractiveButtonBase::resizeEvent(event);
}

void WaterCircleButton::paintEvent(QPaintEvent *event)
{
    return InteractiveButtonBase::paintEvent(event);
}

void WaterCircleButton::setPainterPathGeometry(QPainterPath &path)
{
    path.addEllipse(QRect(0,0,size().width(),size().height()));
}

void WaterCircleButton::paintWaterRipple(QPainter &painter)
{

}
