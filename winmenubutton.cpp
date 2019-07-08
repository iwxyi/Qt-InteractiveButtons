#include "winmenubutton.h"

WinMenuButton::WinMenuButton(QWidget* parent)
    : InteractiveButtonBase (parent)
{

}

void WinMenuButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    int w = geometry().width(), h = geometry().height();
    int dx = offset_pos.x(), dy = offset_pos.y();

    QPainter painter(this);
    painter.setPen(QPen(icon_color));

    if (click_ani_appearing)
    {
        double pro = click_ani_progress / 100.0;
        if (!getState())
            pro = 1 - pro;

        int len = w/3;
        int l = w/3, r = w*2/3, t = h/3 + pro*h/3;
        painter.drawLine(QPoint(l+dx/4,t+dy/4), QPoint(r+dx/4,t+dy/4));

        l = w/3+pro*w/24, r = w*2/3-pro*w/24, t = w/2+pro*h*5/18;
        painter.drawLine(QPoint(l+dx/2,t+dy/2), QPoint(r+dx/2,t+dy/2));

        l = w/3+pro*w/12, r = w*2/3-pro*w/12, t=w*2/3+pro*h*2/9;
        painter.drawLine(QPoint(l+dx,t+dy), QPoint(r+dx,t+dy));

        /*int half_len = w/6;//quick_sqrt(w/3*w/3 + h/3*h/3) / 2; // 长度
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
        }*/
    }
    else if (getState())
    {
        painter.drawLine(w/3+dx/4,h*2/3+dy/4, w*2/3+dx/4,h*2/3+dy/4);
        painter.drawLine(w/3+w/24+dx/2, h*7/9+dy/2, w*2/3-w/24+dx/2, h*7/9+dy/2);
        painter.drawLine(w/3+w/12+dx, h*8/9+dy, w*2/3-w/12+dx, h*8/9+dy);

        /*painter.drawLine(QPoint(0.39*w, 0.39*h), QPoint(0.61*w, 0.61*h));
        painter.drawLine(QPoint(0.39*w, 0.61*h), QPoint(0.61*w, 0.39*h));*/
    }
    else
    {
        painter.drawLine(QPoint(w/3+dx/4,h/3+dy/4), QPoint(w*2/3+dx/4,h/3+dy/4));
        painter.drawLine(QPoint(w/3+dx/2,h/2+dy/2), QPoint(w*2/3+dx/2,h/2+dy/2));
        painter.drawLine(QPoint(w/3+dx,h*2/3+dy), QPoint(w*2/3+dx,h*2/3+dy));
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
