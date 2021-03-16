#include "infobutton.h"

InfoButton::InfoButton(QWidget *parent) : InteractiveButtonBase(parent)
{
    setUnifyGeomerey(true);
}

void InfoButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    if (!show_foreground) return ; // 不显示前景

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(icon_color);
    QPainterPath path;

    double cu = 2; // 点的粗细
    if (!hover_progress) // 显示感叹号
    {
        // 画点
        path.addEllipse(QRectF(_l+_w/2-cu, _t+_h/4, cu*2, cu*2));

        // 画线
        path.addRoundedRect(QRectF(_l+_w/2-cu/2, _t+_h*3/8+cu, cu, _h/2 - cu - cu/2), cu/2, cu/2);
    }
    else if (hover_progress < 100) // 转换动画
    {
        double prop = hover_progress / 100.0;

        // 眼睛出现
        double ra = cu * prop;
        path.addEllipse(_l+_w/4-ra, _t+_h/4-ra, ra*2, ra*2);
        path.addEllipse(_l+_w*3/4-ra, _t+_h/4-ra, ra*2, ra*2);

        // 鼻子下降
        double top = _t+_h/4 + (_h/4-cu/2) * prop;
        path.addEllipse(QRectF(_l+_w/2-cu, top, cu*2, cu*2));

        double h_mv = _w / 4 * prop;

        // 左胡子移动
        QPainterPath pathl;
        pathl.moveTo(_l+_w/2, top);
        pathl.cubicTo(QPointF(_l+_w*8/16, _t+_h*7/8), QPointF(_l+_w*5/16, _t+_h*6/8), QPointF(_l+_w/2-h_mv, _t+_h*11/16));
        painter.drawPath(pathl);

        // 右胡子移动
        QPainterPath pathr;
        pathr.moveTo(_l+_w/2, top);
        pathr.cubicTo(QPointF(_l+_w*8/16, _t+_h*7/8), QPointF(_l+_w*11/16, _t+_h*6/8), QPointF(_l+_w/2+h_mv, _t+_h*11/16));
        painter.drawPath(pathr);
    }
    else // 显示笑脸
    {
        // 眼睛
        if (pressing) // 横线
        {
            painter.drawLine(_l+_w/4-cu*2, _t+_h/4, _l+_w/4+cu*2, _t+_h/4);
            painter.drawLine(_l+_w*3/4-cu*2, _t+_h/4, _l+_w*3/4+cu*2, _t+_h/4);
        }
        else // 点
        {
            path.addEllipse(_l+_w/4-cu, _t+_h/4-cu, cu*2, cu*2);
            path.addEllipse(_l+_w*3/4-cu, _t+_h/4-cu, cu*2, cu*2);
        }

        // 鼻子
        path.addEllipse(_l+_w/2-cu, _t+_h/2-cu/2, cu*2, cu*2);

        // 左胡子
        QPainterPath pathl;
        pathl.moveTo(_l+_w/2, _t+_h*4/8);
        pathl.cubicTo(QPointF(_l+_w*8/16, _t+_h*7/8), QPointF(_l+_w*5/16, _t+_h*6/8), QPointF(_l+_w/4, _t+_h*11/16));
        painter.drawPath(pathl);

        // 右胡子
        QPainterPath pathr;
        pathr.moveTo(_l+_w/2, _t+_h*4/8);
        pathr.cubicTo(QPointF(_l+_w*8/16, _t+_h*7/8), QPointF(_l+_w*11/16, _t+_h*6/8), QPointF(_l+_w*3/4, _t+_h*11/16));
        painter.drawPath(pathr);
    }
    painter.fillPath(path, icon_color);
}
