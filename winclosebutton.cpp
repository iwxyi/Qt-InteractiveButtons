#include "winclosebutton.h"

WinCloseButton::WinCloseButton(QWidget *parent)
    : InteractiveButtonBase (parent)
{

}

void WinCloseButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    QPainter painter(this);

    int w = geometry().width(), h = geometry().height();
    int l = w/3, t = h/3, r = w*2/3, b = h*2/3;
    int mx = w/2+offset_pos.x(), my = h/2+offset_pos.y();

    if (click_ani_appearing)
    {
        double pro = click_ani_progress / 100.0;
        l -= l * pro;
        t -= t * pro;
        r += (w-r) * pro;
        b += (h-b) * pro;

        /*painter.translate(w/2,h/2);
        painter.rotate(50*pro);
        painter.translate(-w/2, -h/2);*/
    }

    painter.setPen(QPen(icon_color));
    painter.setRenderHint(QPainter::Antialiasing,true);
    if (offset_pos != QPoint(0,0))
    {
        QPainterPath path;
        path.moveTo(QPoint(l,t));
        path.cubicTo(QPoint(l,t), QPoint(mx,my), QPoint(r,b));
        path.moveTo(QPoint(r,t));
        path.cubicTo(QPoint(r,t), QPoint(mx,my), QPoint(l,b));

        painter.drawPath(path);
    }
    else
    {
        painter.drawLine(QPoint(l,t), QPoint(r,b));
        painter.drawLine(QPoint(r,t), QPoint(l,b));
    }


}
