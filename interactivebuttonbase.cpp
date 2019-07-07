#include "interactivebuttonbase.h"

InteractiveButtonBase::InteractiveButtonBase(QWidget *parent)
    : QPushButton(parent),
      enter_pos(-1, -1), press_pos(-1, -1), release_pos(-1, -1), mouse_pos(-1, -1), anchor_pos(-1,  -1),
      offset_pos(0, 0), effect_pos(-1, -1), release_offset(0, 0),
      pressing(false), entering(false),
      water_ripple(false), water_finished(false),
      hover_timestamp(0), press_timestamp(0), release_timestamp(0),
      hover_bg_duration(100), press_bg_duration(100), click_ani_duration(300),
      move_speed(5),
      icon_color(0, 0, 0),
      normal_bg(128, 128, 128, 0), hover_bg(128, 128, 128, 32), press_bg(128, 128, 128, 64),
      hover_speed(10), press_start(40), press_speed(10),
      hover_progress(0), press_progress(0),
      click_ani_appearing(false), click_ani_disappearing(false), click_ani_progress(0),
      _state(false)
{
    setMouseTracking(true); // 鼠标没有按下时也能捕获移动事件

    anchor_timer = new QTimer(this);
    anchor_timer->setInterval(20);
    connect(anchor_timer, SIGNAL(timeout()), this, SLOT(anchorTimeOut()));

    setWaterRipple();

    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

void InteractiveButtonBase::setWaterRipple(bool enable)
{
    if (water_ripple == enable) return ;

    water_ripple = enable;
    /*if (water_ripple)
        press_speed >>= 1; // 水波纹模式需要减慢动画速度
    else
        press_speed <<= 1; // 恢复到原来的速度 */
}

void InteractiveButtonBase::setBgColor(QColor hover, QColor press)
{
    if (hover != Qt::black)
        hover_bg = hover;
    if (press != Qt::black)
        press_bg = press;
}

void InteractiveButtonBase::setIconColor(QColor color)
{
    icon_color = color;
    update();
}

void InteractiveButtonBase::setState(bool s)
{
    _state = s;
}

bool InteractiveButtonBase::getState()
{
    return _state;
}

void InteractiveButtonBase::mousePressEvent(QMouseEvent *event)
{
    mouse_pos = mapFromGlobal(QCursor::pos());

    if (event->button() == Qt::LeftButton)
    {
        pressing = true;
        press_pos = mouse_pos;
        press_timestamp = getTimestamp();
        if (water_ripple)
            water_finished = false;
        else
            if (press_progress < press_start)
                press_progress = press_start;
    }

    return QPushButton::mousePressEvent(event);
}

void InteractiveButtonBase::mouseReleaseEvent(QMouseEvent* event)
{
    if (pressing && event->button() == Qt::LeftButton)
    {
        pressing = false;
        release_pos = mapFromGlobal(QCursor::pos());
        release_timestamp = getTimestamp();

        if ((release_pos - press_pos).manhattanLength() < 2) // 单击
        {
            // slotClicked()
        }
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
//    painter.drawEllipse(QRect(anchor_pos.x()-5, anchor_pos.y()-5, 10, 10));
//    painter.drawEllipse(QRect(effect_pos.x()-2, effect_pos.y()-2, 4, 4));

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

    return QPushButton::enterEvent(event);
}


void InteractiveButtonBase::leaveEvent(QEvent *event)
{
    entering = false;
    if (!pressing)
        mouse_pos = QPoint(geometry().width()/2, geometry().height()/2);

    return QPushButton::leaveEvent(event);
}

int InteractiveButtonBase::quick_sqrt(long X)
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

int InteractiveButtonBase::max(int a, int b) { return a > b ? a : b; }

int InteractiveButtonBase::min(int a, int b) { return a < b ? a : b; }

int InteractiveButtonBase::moveSuitable(int speed, int delta)
{
    if (speed >= delta)
        return delta;

    if ((speed<<3) < delta)
        return delta >> 3;

    return speed;
}

qint64 InteractiveButtonBase::getTimestamp()
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
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
        {
            if (water_ripple) // 按下时水波纹速度减半
                press_progress += press_speed>>1;
            else
                press_progress += press_speed;
        }
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

    // 按下动画
    if (click_ani_disappearing) // 按下动画效果消失
    {
        qint64 delta = getTimestamp()-release_timestamp-click_ani_duration;
        if (delta <= 0) click_ani_progress = 100;
        else click_ani_progress = 100 - delta*100 / click_ani_duration;
        if (click_ani_progress < 0)
        {
            click_ani_progress = 0;
            click_ani_disappearing = false;
        }
    }
    if (click_ani_appearing) // 按下动画效果
    {
        qint64 delta = getTimestamp()-release_timestamp;
        if (delta <= 0) click_ani_progress = 0;
        else click_ani_progress = delta * 100 / click_ani_duration;
        if (click_ani_progress > 100)
        {
            click_ani_progress = 100; // 保持100的状态，下次点击时回到0
            click_ani_appearing = false;
            click_ani_disappearing = true;
        }
    }

    if (hover_progress > 100) hover_progress = 100;
    if (hover_progress < 0) hover_progress = 0;
    if (press_progress > 100) press_progress = 100;
    if (press_progress < 0) press_progress = 0;

    // 锚点
    if (anchor_pos != mouse_pos)
    {
        int delta_x = anchor_pos.x() - mouse_pos.x(),
            delta_y = anchor_pos.y() - mouse_pos.y();

        if (delta_x < 0) // 右移
            anchor_pos.setX( anchor_pos.x() + moveSuitable(move_speed, -delta_x) );
        else if (delta_x > 0) // 左移
            anchor_pos.setX( anchor_pos.x() - moveSuitable(move_speed, delta_x) );

        if (delta_y < 0) // 右移
            anchor_pos.setY( anchor_pos.y() + moveSuitable(move_speed, -delta_y) );
        else if (delta_y > 0) // 左移
            anchor_pos.setY( anchor_pos.y() - moveSuitable(move_speed, delta_y) );

        offset_pos.setX(quick_sqrt(static_cast<long>(anchor_pos.x()-(geometry().width()>>1))));
        offset_pos.setY(quick_sqrt(static_cast<long>(anchor_pos.y()-(geometry().height()>>1))));
        effect_pos.setX( (geometry().width() >>1) + offset_pos.x());
        effect_pos.setY( (geometry().height()>>1) + offset_pos.y());
    }
    else if (!pressing && !entering && !hover_progress && !press_progress && !click_ani_appearing && !click_ani_disappearing)
    {
        anchor_timer->stop();
    }

    update();
}

void InteractiveButtonBase::slotClicked()
{
    click_ani_appearing = true;
    click_ani_disappearing = false;
    click_ani_progress = 0;
    release_offset = offset_pos;
}
