#include "winclosebutton.h"

WinCloseButton::WinCloseButton(QWidget *parent)
    : InteractiveButtonBase (parent)
{

}

void WinCloseButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    if (!show_foreground) return ; // 不显示前景

    int _l = 0, _t = 0, _w = geometry().width(), _h = geometry().height();
    if ((show_ani_appearing || show_ani_disappearing) && show_ani_point != QPoint( 0, 0 ))
    {
        int pro; // 将动画进度转换为回弹动画进度
        pro = show_ani_appearing ? getSpringBackProgress(show_ani_progress,50) : show_ani_progress;

        // show_ani_point 是鼠标进入的点，那么起始方向应该是相反的
        int x = show_ani_point.x(), y = show_ani_point.y();
        int gen = quick_sqrt(x*x + y*y);
        x = - water_radius * x / gen; // 动画起始中心点横坐标 反向
        y = - water_radius * y / gen; // 动画起始中心点纵坐标 反向

        _l = _l + x * (100-pro) / 100 + _w * (100-pro) / 200;
        _t = _t + y * (100-pro) / 100 + _h * (100-pro) / 200;
        _w = _w * pro / 100;
        _h = _h * pro / 100;
    }

    int w = _w, h = _h;
    int l = _l+w/3, t = _t+h/3, r = w*2/3, b = h*2/3;
    int mx = _l+w/2+offset_pos.x(), my = _t+h/2+offset_pos.y();

    if (click_ani_appearing || click_ani_disappearing)
    {
        double pro = click_ani_progress / 100.0;
        l -= l * pro;
        t -= t * pro;
        r += (w-r) * pro;
        b += (h-b) * pro;
    }

    QPainter painter(this);
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
