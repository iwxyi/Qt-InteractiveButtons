#include "winmaxbutton.h"

WinMaxButton::WinMaxButton(QWidget *parent)
    : InteractiveButtonBase (parent)
{

}

void WinMaxButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();
    int dx = offset_pos.x(), dy = offset_pos.y();
    QRect r;
    if (click_ani_appearing || click_ani_disappearing)
    {
        double pro = click_ani_progress / 800.0;
        r = QRect(
                    (w/3+dx) - (w/3+dx)*pro,
                    (h/3+dy) - (h/3+dy)*pro,
                    w/3 + (w*2/3)*pro,
                    h/3 + (h*2/3)*pro
                    );
    }
    else
    {
        r = QRect(w/3+dx, h/3+dy, w/3, h/3);
    }


    QPainter painter(this);
    painter.setPen(QPen(icon_color));
    painter.drawRect(r);
}
