#include "winmaxbutton.h"

WinMaxButton::WinMaxButton(QWidget *parent)
    : InteractiveButtonBase (parent)
{

}

void WinMaxButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();

    QPainter painter(this);
    painter.setPen(QPen(icon_color));
    painter.drawRect(QRect(w/4+offset_pos.x(), h/4+offset_pos.y(), w/2, h/2));
}
