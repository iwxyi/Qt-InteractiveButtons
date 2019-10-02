#include "threedimenbutton.h"

ThreeDimenButton::ThreeDimenButton(QWidget* parent) : InteractiveButtonBase (parent)
{
	aop_w = width() / AOPER;
	aop_h = height() / AOPER;

    shadow_effect = new QGraphicsDropShadowEffect(this);
	shadow_effect->setOffset(0, SHADE);
	shadow_effect->setColor(QColor(0x88, 0x88, 0x88, 0x88));
	shadow_effect->setBlurRadius(10);
	setGraphicsEffect(shadow_effect);

	setJitterAni(false);
}

void ThreeDimenButton::enterEvent(QEvent *event)
{

}

void ThreeDimenButton::leaveEvent(QEvent *event)
{
    if (in_circle && !pressing && !inArea(mapFromGlobal(QCursor::pos())))
    {
        in_circle = false;
        InteractiveButtonBase::leaveEvent(event);
    }
}

void ThreeDimenButton::mousePressEvent(QMouseEvent *event)
{
    if (in_circle)
        return InteractiveButtonBase::mousePressEvent(event);
}

void ThreeDimenButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressing)
    {
        InteractiveButtonBase::mouseReleaseEvent(event);

        if (leave_after_clicked || (!inArea(event->pos()) && !pressing)) // 鼠标移出
        {
            in_circle = false;
            InteractiveButtonBase::leaveEvent(nullptr);
        }
    }
}

void ThreeDimenButton::mouseMoveEvent(QMouseEvent *event)
{
    bool is_in = inArea(event->pos());

    if (is_in && !in_circle)// 鼠标移入
    {
        in_circle = true;
        InteractiveButtonBase::enterEvent(nullptr);
    }
    else if (!is_in && in_circle && !pressing) // 鼠标移出
    {
        in_circle = false;
        InteractiveButtonBase::leaveEvent(nullptr);
    }

    if (in_circle)
        InteractiveButtonBase::mouseMoveEvent(event);
}

void ThreeDimenButton::resizeEvent(QResizeEvent *event)
{
	aop_w = width() / AOPER;
	aop_h = height() / AOPER;
    return InteractiveButtonBase::resizeEvent(event);
}

void ThreeDimenButton::anchorTimeOut()
{
	InteractiveButtonBase::anchorTimeOut();

	// 修改阴影的位置
	if (offset_pos == QPoint(0,0))
	    shadow_effect->setOffset(0, 0);
	else
	{
		if (offset_pos.manhattanLength() > SHADE)
		{
			double sx = -SHADE * offset_pos.x() / offset_pos.manhattanLength();
			double sy = -SHADE * offset_pos.y() / offset_pos.manhattanLength();
			shadow_effect->setOffset(sx, sy);
		}
	    else
	    {
	    	shadow_effect->setOffset(-offset_pos.x(), -offset_pos.y());
	    }
	}
}

QPainterPath ThreeDimenButton::getBgPainterPath()
{
	QPainterPath path;
	if (hover_progress) // 鼠标悬浮效果
	{
		/**
		 * 位置比例 = 悬浮比例 × 距离比例
		 * 坐标位置 ≈ 鼠标方向偏移
		 */
		QPoint o(width()/2, height()/2);
		QPoint m(mapFromGlobal(QCursor::pos()));
		QPoint& f = offset_pos;
		double hp = hover_progress / 100.0;

		QPoint lt, lb, rb, rt;
		QList<double> list;
		// 左上角
		{
			QPoint p(aop_w, aop_h);
			double prob = dian_cheng(m-o, p-o) / (double)dian_cheng(p-o, p-o);
			prob *= hp;
			// qDebug() << m-o << p-o << dian_cheng(m-o, p-o) << dian_cheng(p-o, p-o) << prob;
			lt = o + (p-o) * (1-prob/AOPER);
			list << prob;
		}
		// 右上角
		{
			QPoint p(width() - aop_w, aop_h);
			double prob = dian_cheng(m-o, p-o) / (double)dian_cheng(p-o, p-o);
			prob *= hp;
			rt = o + (p-o) * (1-prob/AOPER);
			list << prob;
		}
		// 左下角
		{
			QPoint p(aop_w, height() - aop_h);
			double prob = dian_cheng(m-o, p-o) / (double)dian_cheng(p-o, p-o);
			prob *= hp;
			lb = o + (p-o) * (1-prob/AOPER);
			list << prob;
		}
		// 右下角
		{
			QPoint p(width() - aop_w, height() - aop_h);
			double prob = dian_cheng(m-o, p-o) / (double)dian_cheng(p-o, p-o);
			prob *= hp;
			rb = o + (p-o) * (1-prob/AOPER);
			list << prob;
		}

		path.moveTo(lt);
		path.lineTo(lb);
		path.lineTo(rb);
		path.lineTo(rt);
		path.lineTo(lt);

		// qDebug() << list << f;
		
		/*// 直接使用偏移量（无法不规则）
		QPoint 	lt(-offset_pos.x(), -offset_pos.y()),
				lb(-offset_pos.x(), height()-offset_pos.y()),
				rt(width()-offset_pos.x(), -offset_pos.y()),
				rb(width()-offset_pos.x(), height()-offset_pos.y());
		path.moveTo(lt);
		path.lineTo(lb);
		path.lineTo(rb);
		path.lineTo(rt);
		path.lineTo(lt);*/
	}
	else
	{
		// 简单的path，提升性能用
		path.addRect(aop_w, aop_h, width()-aop_w*2, height()-aop_h*2);
	}

    return path;
}

QPainterPath ThreeDimenButton::getWaterPainterPath(InteractiveButtonBase::Water water)
{
    QRect circle(water.point.x() - water_radius*water.progress/100,
                water.point.y() - water_radius*water.progress/100,
                water_radius*water.progress/50,
                water_radius*water.progress/50);
    QPainterPath path;
    path.addEllipse(circle);
    return path & getBgPainterPath();
}

void ThreeDimenButton::simulateStatePress(bool s)
{
    in_circle = true;
    InteractiveButtonBase::simulateStatePress(s);
    in_circle = false;
}

bool ThreeDimenButton::inArea(QPoint point)
{
	return !(point.x() < aop_w
		|| point.x() > width()-aop_w
		|| point.y() < aop_h
		|| point.y() > height()-aop_h);
}

/**
 * 计算两个向量的叉积
 * 获取压力值
 */
int ThreeDimenButton::cha_cheng(QPoint a, QPoint b)
{
	return a.x() * b.y() - b.x()* a.y();
}

int ThreeDimenButton::dian_cheng(QPoint a, QPoint b)
{
	return a.x() * b.x() + a.y() * b.y();
}