#include "winminbutton.h"

WinMinButton::WinMinButton(QWidget* parent)
	: InteractiveButtonBase(parent)
{

}

void WinMinButton::paintEvent(QPaintEvent* event)
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
    QPoint left(_l+w/3, _t+h/2), right(_l+w*2/3, _t+h/2),
           mid(_l+w/2+offset_pos.x(), _t+h/2+offset_pos.y());

    if (click_ani_appearing || click_ani_disappearing)
    {
        double pro = click_ani_progress / 800.0;
        left.setX(left.x()-left.x() * pro);
        right.setX(right.x()+(w-right.x()) * pro);
    }

    QPainter painter(this);
    QPainterPath path;
    path.moveTo(left);
    path.cubicTo(left, mid, right);
    painter.setPen(QPen(icon_color));
    if (left.y() != mid.y())
        painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPath(path);
}
