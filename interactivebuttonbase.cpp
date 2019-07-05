#include "interactivebuttonbase.h"

InteractiveButtonBase::InteractiveButtonBase(QWidget *parent)
    : QPushButton(parent),
      enter_pos(-1, -1), press_pos(-1, -1), mouse_pos(-1, -1), anchor_pos(-1,  -1), effect_pos(-1, -1),
      pressing(false), entering(false),
      water_ripple(false), water_finished(false),
      move_speed(5),
      normal_bg(128, 128, 128, 0), hover_bg(128, 128, 128, 25), press_bg(255, 128, 128, 200),
      hover_speed(10), press_speed(20),
      hover_progress(0), press_progress(0)
{
    setMouseTracking(true); // 鼠标没有按下时也能捕获移动事件

    anchor_timer = new QTimer(this);
    anchor_timer->setInterval(20);
    connect(anchor_timer, SIGNAL(timeout()), this, SLOT(anchorTimeOut()));

    setWaterRipple();
}

void InteractiveButtonBase::setWaterRipple(bool enable)
{
    if (water_ripple == enable) return ;

    water_ripple = enable;
    if (water_ripple)
        press_speed >>= 1; // 水波纹模式需要减慢动画速度
    else
        press_speed <<= 1; // 恢复到原来的速度
}

void InteractiveButtonBase::mousePressEvent(QMouseEvent *event)
{
    mouse_pos = mapFromGlobal(QCursor::pos());

    if (event->button() == Qt::LeftButton)
    {
    	pressing = true;
        press_pos = mouse_pos;
        if (water_ripple)
            water_finished = false;
    }

    return QPushButton::mousePressEvent(event);
}

void InteractiveButtonBase::mouseReleaseEvent(QMouseEvent* event)
{
    if (pressing && event->button() == Qt::LeftButton)
	{
		pressing = false;
	}

    return QPushButton::mouseReleaseEvent(event);
}

void InteractiveButtonBase::mouseMoveEvent(QMouseEvent *event)
{
    mouse_pos = mapFromGlobal(QCursor::pos());

    return QPushButton::mouseMoveEvent(event);
}

void InteractiveButtonBase::resizeEvent(QResizeEvent *event)
{
    if (!pressing && !entering)
    {
        mouse_pos = QPoint(geometry().width()/2, geometry().height()/2);
        anchor_pos = mouse_pos;
    }

    return QPushButton::resizeEvent(event);
}

void InteractiveButtonBase::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    // 绘制背景
    QPainterPath path_back;
    path_back.setFillRule(Qt::WindingFill);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::Antialiasing,true);
    path_back.addRect(QRect(QPoint(0,0), geometry().size()));

    if (hover_progress)
    {
    	QColor bg_color = hover_bg;
    	bg_color.setAlpha(hover_bg.alpha() * hover_progress / 100);
    	painter.fillPath(path_back, QBrush(bg_color));
    }

    if (press_progress)
    {
        if (!water_ripple || water_finished) // 淡化消失：无水波纹，或者水波纹出现结束了
        {
            QColor bg_color = press_bg;
            bg_color.setAlpha(press_bg.alpha() * press_progress / 100);
            painter.fillPath(path_back, QBrush(bg_color));
        }
        else // 水波纹出现
        {
            int radius = static_cast<int>((geometry().width() > geometry().height() ? geometry().width() : geometry().height()) * 1.42);
            QRect circle(press_pos.x() - radius*press_progress/100,
                        press_pos.y() - radius*press_progress/100,
                        radius*press_progress/50,
                        radius*press_progress/50);
            QPainterPath path;
            path.addEllipse(circle);
            painter.fillPath(path, QBrush(press_bg));
        }
    }


    // 绘制鼠标位置
    //painter.drawEllipse(QRect(anchor_pos.x()-5, anchor_pos.y()-5, 10, 10));
    //painter.drawEllipse(QRect(effect_pos.x()-2, effect_pos.y()-2, 4, 4));

//    return QPushButton::paintEvent(event); // 不绘制父类背景了
}

void InteractiveButtonBase::enterEvent(QEvent *event)
{
    if (!anchor_timer->isActive())
    {
    	anchor_timer->start();
    }
    entering = true;
    if (mouse_pos == QPoint(-1,-1))
        mouse_pos = mapFromGlobal(QCursor::pos());
qDebug() << mouse_pos;
    return QPushButton::enterEvent(event);
}


void InteractiveButtonBase::leaveEvent(QEvent *event)
{
    entering = false;
    if (!pressing)
        mouse_pos = QPoint(geometry().width()/2, geometry().height()/2);

    return QPushButton::leaveEvent(event);
}

int quick_sqrt(long X)
{
    bool fu = false;
    if (X < 0)
    {
        fu = true;
        X = -X;
    }
    unsigned long M = static_cast<unsigned long>(X);
    unsigned int N, i;
    unsigned long tmp, ttp; // 结果、循环计数
    if (M == 0) // 被开方数，开方结果也为0
        return 0;
    N = 0;
    tmp = (M >> 30); // 获取最高位：B[m-1]
    M <<= 2;
    if (tmp > 1) // 最高位为1
    {
        N ++; // 结果当前位为1，否则为默认的0
        tmp -= N;
    }
    for (i = 15; i > 0; i--) // 求剩余的15位
    {
        N <<= 1; // 左移一位
        tmp <<= 2;
        tmp += (M >> 30); // 假设
        ttp = N;
        ttp = (ttp << 1) + 1;
        M <<= 2;
        if (tmp >= ttp) // 假设成立
        {
            tmp -= ttp;
            N ++;
        }
    }
    return (fu ? -1 : 1) * static_cast<int>(N); // 不知道为什么计算出来的结果是反过来的
}

/**
 * 锚点变成到鼠标位置的定时时钟
 */
void InteractiveButtonBase::anchorTimeOut()
{
    // 背景色
    if (pressing) // 鼠标按下
    {
        if (press_progress < 100)
            press_progress += press_speed;
    }
    else // 鼠标悬浮
    {
        if (press_progress>0) // 如果按下的效果还在
        {
            if (water_ripple) // 水波纹
            {
                if (!water_finished) // 按下的水波纹动画还没有结束
                {
                    press_progress += press_speed;
                    if (press_progress >= 100)
                        water_finished = true;
                }
                else
                {
                    press_progress -= press_speed;
                }
            }
            else
            {
                press_progress -= press_speed;
            }
        }

        if (entering) // 在框内：加深
        {
            if (hover_progress < 100)
                hover_progress += hover_speed;
        }
        else // 在框外：变浅
        {
            if (hover_progress > 0)
                hover_progress -= hover_speed;
        }
    }

    // 锚点
    if (anchor_pos != mouse_pos)
    {
        int delta_x = anchor_pos.x() - mouse_pos.x(),
    		delta_y = anchor_pos.y() - mouse_pos.y();
    	
    	if (delta_x < 0) // 右移
    		anchor_pos.setX( anchor_pos.x() + (move_speed > -delta_x ? -delta_x : move_speed) );
    	else if (delta_x > 0) // 左移
    		anchor_pos.setX( anchor_pos.x() - (move_speed > delta_x ? delta_x : move_speed) );

    	if (delta_y < 0) // 右移
    		anchor_pos.setY( anchor_pos.y() + (move_speed > -delta_y ? -delta_y : move_speed) );
    	else if (delta_y > 0) // 左移
    		anchor_pos.setY( anchor_pos.y() - (move_speed > delta_y ? delta_y : move_speed) );

        offset_pos.setX(quick_sqrt(static_cast<long>(anchor_pos.x()-(geometry().width()>>1))));
        offset_pos.setY(quick_sqrt(static_cast<long>(anchor_pos.y()-(geometry().height()>>1))));
        effect_pos.setX( (geometry().width() >>1) + offset_pos.x());
        effect_pos.setY( (geometry().height()>>1) + offset_pos.y());
    }

    update();
}
