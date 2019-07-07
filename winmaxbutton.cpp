#include "winmaxbutton.h"

WinMaxButton::WinMaxButton(QWidget *parent)
    : InteractiveButtonBase (parent)
{

}

void WinMaxButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();
    QRect r;
    if (click_ani_appearing || click_ani_disappearing)
    {
        r = QRect(
                    (w/3+release_offset.x()) - (w/3+release_offset.x())*click_ani_progress/800,
                    (h/3+offset_pos.y()) - (h/3+offset_pos.y())*click_ani_progress/800,
                    w/3 + (w*2/3)*click_ani_progress/800,
                    h/3 + (h*2/3)*click_ani_progress/800
                    );
    }
    else
    {
        r = QRect(w/3+offset_pos.x(), h/3+offset_pos.y(), w/3, h/3);
    }


    QPainter painter(this);
    painter.setPen(QPen(icon_color));
    painter.drawRect(r);
}
