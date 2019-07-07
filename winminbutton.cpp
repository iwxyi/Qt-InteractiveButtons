#include "winminbutton.h"

WinMinButton::WinMinButton(QWidget* parent)
	: InteractiveButtonBase(parent)
{

}

void WinMinButton::paintEvent(QPaintEvent* event)
{
	InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();
    QPoint left(w/3, h/2), right(w*2/3, h/2),
           mid(w/2-offset_pos.x(), h/2-offset_pos.y());

    QPainter painter(this);
    QPainterPath path;
    path.moveTo(left);
    path.cubicTo(left, mid, right);
    painter.setPen(QPen(icon_color));
    if (left.y() != mid.y())
        painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPath(path);
}
