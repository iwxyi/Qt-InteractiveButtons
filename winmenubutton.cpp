#include "winmenubutton.h"

WinMenuButton::WinMenuButton(QWidget* parent)
    : InteractiveButtonBase (parent)
{

}

void WinMenuButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();

    QPainter painter(this);
    painter.setPen(QPen(icon_color));

    if (click_ani_appearing)
    {
        double pro = click_ani_progress / 100.0;
        if (!getState())
            pro = 1 - pro;

        int half_len = w/6;//quick_sqrt(w/3*w/3 + h/3*h/3) / 2; // 长度
        painter.setRenderHint(QPainter::Antialiasing,true);

        // 第一个点
        {
            int mx = w/2;
            int my = h/3 + pro*h/6;
            double angle = pro*PI*5/4;
            int sx = mx - half_len*cos(angle);
            int sy = my - half_len*sin(angle);
            int ex = mx + half_len*cos(angle);
            int ey = my + half_len*sin(angle);
            painter.drawLine(QPoint(sx,sy), QPoint(ex,ey));
        }

        // 第三个点
        {
            int mx = w/2;
            int my = h*2/3 - pro*h/6;
            double angle = pro*PI*3/4;
            int sx = mx - half_len*cos(angle);
            int sy = my - half_len*sin(angle);
            int ex = mx + half_len*cos(angle);
            int ey = my + half_len*sin(angle);
            painter.drawLine(QPoint(sx,sy), QPoint(ex,ey));
        }

        // 第二个点(设置透明度)
        {
            int mx = w/2;
            int my = h/2;
            double angle = pro*PI;
            int sx = mx - half_len*cos(angle);
            int sy = my - half_len*sin(angle);
            int ex = mx + half_len*cos(angle);
            int ey = my + half_len*sin(angle);
            QColor color(icon_color);
            color.setAlpha(color.alpha() * (1-pro));
            painter.setPen(QPen(color));
            painter.drawLine(QPoint(sx,sy), QPoint(ex,ey));
        }

    }
    else if (getState())
    {
        painter.drawLine(QPoint(0.39*w, 0.39*h), QPoint(0.61*w, 0.61*h));
        painter.drawLine(QPoint(0.39*w, 0.61*h), QPoint(0.61*w, 0.39*h));
    }
    else
    {
        painter.drawLine(QPoint(w/3,h/3), QPoint(w*2/3,h/3));
        painter.drawLine(QPoint(w/3,h/2), QPoint(w*2/3,h/2));
        painter.drawLine(QPoint(w/3,h*2/3), QPoint(w*2/3,h*2/3));
    }
}

void WinMenuButton::slotClicked()
{
    if (getState())
        setState(false);
    else
        setState(true);
    return InteractiveButtonBase::slotClicked();
}
