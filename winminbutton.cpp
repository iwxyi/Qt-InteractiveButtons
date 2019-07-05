#include "winminbutton.h"

WinMinButton::WinMinButton(QWidget* parent)
	: InteractiveButtonBase(parent)
{

}

void WinMinButton::paintEvent(QPaintEvent* event)
{
	InteractiveButtonBase::paintEvent(event);

    QPoint left(geometry().width()/4, geometry().height()/2),
            right(geometry().width()*3/4, geometry().height()/2),
            mid(geometry().width()/2+offset_pos.x(), geometry().height()/2+offset_pos.y());

    QPainter painter(this);
    QPainterPath path;
    path.moveTo(left);
    path.cubicTo(left, mid, right);
    painter.drawPath(path);
}
