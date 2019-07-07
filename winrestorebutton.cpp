#include "winrestorebutton.h"

WinRestoreButton::WinRestoreButton(QWidget* parent)
    : InteractiveButtonBase(parent)
{

}

void WinRestoreButton::paintEvent(QPaintEvent* event)
{
    InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();

    QPainter painter(this);
    painter.setPen(QPen(icon_color));
    painter.drawRect(QRect(w/4+offset_pos.x(), h/4+offset_pos.y(), w/2, h/2));

    QPainterPath path;
    path.moveTo(w/2+offset_pos.x()/2, h/4+offset_pos.y()/2);
    path.lineTo(w/2+offset_pos.x()/2, h/8+offset_pos.y()/2);
    path.lineTo(w*7/8+offset_pos.x()/2, h/8+offset_pos.y()/2);
    path.lineTo(w*7/8+offset_pos.x()/2, h/2+offset_pos.y()/2);
    path.lineTo(w*3/4+offset_pos.x()/2, h/2+offset_pos.y()/2);
    painter.drawPath(path);
}
