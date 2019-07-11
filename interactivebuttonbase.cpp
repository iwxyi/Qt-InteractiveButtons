#include "interactivebuttonbase.h"

InteractiveButtonBase::InteractiveButtonBase(QWidget *parent)
    : QPushButton(parent), icon(nullptr), text(""),
      icon_paddings(4,4,4,4),
      show_animation(false), show_foreground(true), show_ani_appearing(false), show_ani_disappearing(false),
      show_duration(300), show_timestamp(0), hide_timestamp(0), show_ani_progress(0), show_ani_point(0,0),
      enter_pos(-1, -1), press_pos(-1, -1), release_pos(-1, -1), mouse_pos(-1, -1), anchor_pos(-1,  -1),
      offset_pos(0, 0), effect_pos(-1, -1), release_offset(0, 0),
      pressing(false), hovering(false),
      hover_timestamp(0), press_timestamp(0), release_timestamp(0),
      hover_bg_duration(100), press_bg_duration(100), click_ani_duration(300),
      move_speed(5),
      icon_color(0, 0, 0),
      normal_bg(128, 128, 128, 0), hover_bg(128, 128, 128, 32), press_bg(128, 128, 128, 64),
      hover_speed(5), press_start(40), press_speed(5),
      hover_progress(0), press_progress(0),
      click_ani_appearing(false), click_ani_disappearing(false), click_ani_progress(0),
      jitter_animation(true), elastic_coefficient(1.2), jitter_duration(300),
      water_animation(true), water_press_duration(800), water_release_duration(400), water_finish_duration(300),
      _state(false)
{
    setMouseTracking(true); // 鼠标没有按下时也能捕获移动事件

    model = PaintModel::None;

    anchor_timer = new QTimer(this);
    anchor_timer->setInterval(10);
    connect(anchor_timer, SIGNAL(timeout()), this, SLOT(anchorTimeOut()));

    setWaterRipple();

    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

InteractiveButtonBase::InteractiveButtonBase(QString text, QWidget *parent)
    : InteractiveButtonBase(parent)
{
    this->text = text;
    model = PaintModel::Text;
}

InteractiveButtonBase::InteractiveButtonBase(QIcon icon, QWidget *parent)
    : InteractiveButtonBase(parent)
{
    this->icon = icon;
    model = PaintModel::Icon;
}

InteractiveButtonBase::InteractiveButtonBase(QPixmap pixmap, QWidget *parent)
    : InteractiveButtonBase(parent)
{
//    pixmap.save("pixmap.png");
    QBitmap mask = pixmap.mask();
    pixmap.fill(icon_color);
    pixmap.setMask(mask);
    this->pixmap = pixmap;
    model = PaintModel::PixmapMask;
}

void InteractiveButtonBase::setWaterRipple(bool enable)
{
    if (water_animation == enable) return ;
    water_animation = enable;
}

void InteractiveButtonBase::setJitterAni(bool enable)
{
    jitter_animation = enable;
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

    if (model == PaintModel::PixmapMask)
    {
        QBitmap mask = pixmap.mask();
        pixmap.fill(icon_color);
        pixmap.setMask(mask);
    }

    update();
}

void InteractiveButtonBase::setShowAni(bool enable)
{
    show_animation = enable;

    if (!show_animation) // 关闭隐藏前景
    {
        show_foreground = true;
    }
    else if (show_animation) // 开启隐藏前景
    {
        if (!hovering && !pressing) // 应该是隐藏状态
        {
            show_ani_appearing = show_ani_disappearing = show_foreground = false;
            show_ani_progress = 0;
        }
        else // 应该是显示状态
        {
            show_foreground = true;
            show_ani_appearing = show_ani_disappearing = false;
            show_ani_progress = 100;
        }
    }
}

void InteractiveButtonBase::showForeground()
{
    if (!show_animation) return ;
    if (!anchor_timer->isActive())
        anchor_timer->start();
    if (show_ani_disappearing)
        show_ani_disappearing = false;
    show_ani_appearing = true;
    show_timestamp = getTimestamp();
    show_foreground = true;
    show_ani_point = QPoint(0,0);
}

void InteractiveButtonBase::showForeground2()
{
    showForeground();
    show_ani_point = mapFromGlobal(QCursor::pos());
}

void InteractiveButtonBase::hideForeground()
{
    if (!show_animation) return ;
    if (!anchor_timer->isActive())
        anchor_timer->start();
    if (show_ani_appearing)
        show_ani_appearing = false;
    show_ani_disappearing = true;
    hide_timestamp = getTimestamp();

}

void InteractiveButtonBase::setState(bool s)
{
    _state = s;
}

bool InteractiveButtonBase::getState()
{
    return _state;
}

void InteractiveButtonBase::enterEvent(QEvent *event)
{
    if (!anchor_timer->isActive())
    {
        anchor_timer->start();
    }
    hovering = true;
    if (mouse_pos == QPoint(-1,-1))
        mouse_pos = mapFromGlobal(QCursor::pos());

    return QPushButton::enterEvent(event);
}

void InteractiveButtonBase::leaveEvent(QEvent *event)
{
    hovering = false;
    if (!pressing)
        mouse_pos = QPoint(geometry().width()/2, geometry().height()/2);

    return QPushButton::leaveEvent(event);
}

void InteractiveButtonBase::mousePressEvent(QMouseEvent *event)
{
    mouse_pos = mapFromGlobal(QCursor::pos());

    if (event->button() == Qt::LeftButton)
    {
        pressing = true;
        press_pos = mouse_pos;
        press_timestamp = getTimestamp();
        if (water_animation)
        {
            waters << Water(press_pos, press_timestamp);
        }
        else // 透明渐变
        {
            if (press_progress < press_start)
                press_progress = press_start; // 直接设置为按下效果初始值（避免按下反应慢）
        }
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

        // 添加抖动效果
        if (jitter_animation)
        {
            setJitter();
        }

        if (water_animation && waters.size())
        {
            waters.last().release_timestamp = release_timestamp;
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
    if (!pressing && !hovering)
    {
        mouse_pos = QPoint(geometry().width()/2, geometry().height()/2);
        anchor_pos = mouse_pos;
    }
    water_radius = static_cast<int>(max(geometry().width(), geometry().height()) * 1.42); // 长边
    int short_side = min(geometry().width(), geometry().height()); // 短边
    int padding = short_side/4;//static_cast<int>(short_side * (1 - GOLDEN_RATIO) / 2);
    icon_paddings.left = icon_paddings.top = icon_paddings.right = icon_paddings.bottom = padding;

    return QPushButton::resizeEvent(event);
}

void InteractiveButtonBase::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    // ==== 绘制背景 ====
    QPainterPath path_back = getBgPainterPath();
    painter.setRenderHint(QPainter::Antialiasing,true);

    if (hover_progress) // 悬浮背景
    {
        QColor bg_color = hover_bg;
        bg_color.setAlpha(hover_bg.alpha() * hover_progress / 100);
        painter.fillPath(path_back, QBrush(bg_color));
    }

    if (press_progress && !water_animation) // 按下渐变淡化消失
    {
        QColor bg_color = press_bg;
        bg_color.setAlpha(press_bg.alpha() * press_progress / 100);
        painter.fillPath(path_back, QBrush(bg_color));
    }
    else if (water_animation && waters.size()) // 水波纹，且至少有一个水波纹
    {
        paintWaterRipple(painter);
    }

    // ==== 绘制前景 ====
    if (show_foreground)
    {
        painter.setPen(icon_color);
        QRect rect(icon_paddings.left+offset_pos.x(), icon_paddings.top+offset_pos.y(), // 原来的位置，不包含点击、出现效果
                   (size().width()-icon_paddings.left-icon_paddings.right),
                   size().height()-icon_paddings.top-icon_paddings.bottom);

        // 另一种出现动画
        if ((show_ani_appearing || show_ani_disappearing) && show_ani_point != QPoint( 0, 0 ))
        {
            int w = size().width(), h = size().height();
            int pro; // 将动画进度转换为回弹动画进度
            if (show_ani_appearing)
            {
                if (show_ani_progress <= 50)
                    pro = show_ani_progress * 2;
                else if (show_ani_progress <= 75)
                    pro = (show_ani_progress-50)/2 + 100;
                else
                    pro = 100 + (100-show_ani_progress)/2;
            }
            else
            {
                pro = show_ani_progress;
            }

            // show_ani_point 是鼠标进入的点，那么起始方向应该是相反的
            int x = show_ani_point.x(), y = show_ani_point.y();

//            double sita = (x == 0? PI/2 : atan(y/x)) + PI; // 翻转180°
//            x = cos(sita) * water_radius / 2 ;
//            y = sin(sita) * water_radius / 2;

            int gen = quick_sqrt(x*x + y*y);
            x = water_radius * x / gen;
            y = water_radius * y / gen;

            rect = QRect(
                rect.left() - x * (100-pro) / 100 + rect.width() * (100-pro) / 100,
                rect.top() - y * (100-pro) / 100 + rect.height() * (100-pro) / 100,
                rect.width() * pro / 100,
                rect.height() * pro / 100
                );

        }
        // 默认的缩放动画
        else
        {
            int delta_x = 0, delta_y = 0;
            if (click_ani_progress != 0) // 图标缩放
            {
                delta_x = rect.width() * click_ani_progress / 400;
                delta_y = rect.height() * click_ani_progress / 400;
            }
            else if (show_ani_appearing)
            {
                int pro; // 将动画进度转换为回弹动画进度
                if (show_ani_progress <= 50)
                    pro = show_ani_progress * 2;
                else if (show_ani_progress <= 75)
                    pro = (show_ani_progress-50)/2 + 100;
                else
                    pro = 100 + (100-show_ani_progress)/2;

                delta_x = rect.width() * (100-pro) / 100;
                delta_y = rect.height() * (100-pro) / 100;
            }
            else if (show_ani_disappearing)
            {
                delta_x = rect.width() * (100-show_ani_progress) / 100;
                delta_y = rect.height() * (100-show_ani_progress) / 100;
            }
            if (delta_x || delta_y)
                rect = QRect(rect.left()+delta_x, rect.top()+delta_y,
                            rect.width()-delta_x*2, rect.height()-delta_y*2);
        }

        if (model == None)
        {
            // 子类自己的绘制内容
        }
        else if (model == Text)
        {
            // 绘制文字教程： https://blog.csdn.net/temetnosce/article/details/78068464
            painter.drawText(rect, Qt::AlignCenter, text);
        }
        else // 绘制图标
        {

            if (model == Icon)
            {
                icon.paint(&painter, rect, Qt::AlignCenter);
            }
            else if (model == PixmapMask)
            {
                painter.setRenderHint(QPainter::SmoothPixmapTransform, true); // 可以让边缘看起来平滑一些
                painter.drawPixmap(rect, pixmap);
            }
        }
    }

    // ==== 绘制鼠标位置 ====
//    painter.drawEllipse(QRect(anchor_pos.x()-5, anchor_pos.y()-5, 10, 10)); // 移动锚点
//    painter.drawEllipse(QRect(effect_pos.x()-2, effect_pos.y()-2, 4, 4)); // 影响位置锚点

    //    return QPushButton::paintEvent(event); // 不绘制父类背景了
}

QPainterPath InteractiveButtonBase::getBgPainterPath()
{
    QPainterPath path;
    path.addRect(QRect(0,0,size().width(),size().height()));
    return path;
}

QPainterPath InteractiveButtonBase::getWaterPainterPath(InteractiveButtonBase::Water water)
{
    QRect circle(water.point.x() - water_radius*water.progress/100,
                water.point.y() - water_radius*water.progress/100,
                water_radius*water.progress/50,
                water_radius*water.progress/50);
    QPainterPath path;
    path.addEllipse(circle);
    return path;
}

void InteractiveButtonBase::paintWaterRipple(QPainter& painter)
{
    QColor water_finished_color(press_bg);
    for (int i = 0; i < waters.size(); i++)
    {
        Water water = waters.at(i);
        if (water.finished) // 渐变消失
        {
            water_finished_color.setAlpha(press_bg.alpha() * water.progress / 100);
            QPainterPath path_back = getBgPainterPath();
//                painter.setPen(water_finished_color);
            painter.fillPath(path_back, QBrush(water_finished_color));
        }
        else // 圆形出现
        {
            QPainterPath path = getWaterPainterPath(water);
            painter.fillPath(path, QBrush(press_bg));
        }
    }
}

void InteractiveButtonBase::setJitter()
{
    jitters.clear();
    QPoint center_pos = geometry().center()-geometry().topLeft();
    int full_manh = (anchor_pos-center_pos).manhattanLength(); // 距离
    if (full_manh > (geometry().topLeft() - geometry().bottomRight()).manhattanLength()) // 距离超过外接圆半径，开启抖动
    {
        QPoint jitter_pos(effect_pos);
        full_manh = (jitter_pos-center_pos).manhattanLength();
        int manh = full_manh;
        int duration = jitter_duration;
        qint64 timestamp = release_timestamp;
        while (manh > elastic_coefficient)
        {
            jitters << Jitter(jitter_pos, timestamp);
            jitter_pos = center_pos - (jitter_pos - center_pos) / elastic_coefficient;
            duration = jitter_duration * manh / full_manh;
            timestamp += duration;
            manh /= elastic_coefficient;
        }
        jitters << Jitter(center_pos, timestamp);
        anchor_pos = mouse_pos = center_pos;
    }
}

int InteractiveButtonBase::quick_sqrt(long X) const
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

int InteractiveButtonBase::max(int a, int b) const { return a > b ? a : b; }

int InteractiveButtonBase::min(int a, int b) const { return a < b ? a : b; }

int InteractiveButtonBase::moveSuitable(int speed, int delta) const
{
    if (speed >= delta)
        return delta;

    if ((speed<<3) < delta)
        return delta >> 3;

    return speed;
}

qint64 InteractiveButtonBase::getTimestamp() const
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

/**
 * 锚点变成到鼠标位置的定时时钟
 */
void InteractiveButtonBase::anchorTimeOut()
{
    // ==== 背景色 ====
    if (pressing) // 鼠标按下
    {
        if (press_progress < 100) // 透明渐变，且没有完成
        {
            press_progress += press_speed;
            if (press_progress > 100)
                press_progress = 100;
        }
    }
    else // 鼠标悬浮
    {
        if (press_progress>0 && !water_animation) // 如果按下的效果还在，变浅
        {
            press_progress -= press_speed;
            if (press_progress < 0)
                press_progress = 0;
        }

        if (hovering) // 在框内：加深
        {
            if (hover_progress < 100)
            {
                hover_progress += hover_speed;
                if (hover_progress > 100)
                    hover_progress = 100;
            }
        }
        else // 在框外：变浅
        {
            if (hover_progress > 0)
            {
                hover_progress -= hover_speed;
                if (hover_progress < 0)
                    hover_progress = 0;
            }
        }
    }

    // ==== 按下背景水波纹动画 ====
    if (water_animation)
    {
        qint64 timestamp = getTimestamp();
        for (int i = 0; i < waters.size(); i++)
        {
            Water& water = waters[i];
            if (water.finished)
            {
                water.progress = 100 - 100 * (timestamp-water.finish_timestamp) / water_finish_duration;
                if (water.progress <= 0)
                    waters.removeAt(i--);
            }
            else // 正在出现状态
            {
                if (water.progress >= 100)
                {
                    water.progress = 100;
                    if (water.release_timestamp) // 鼠标已经松开了
                    {
                        water.finished = true; // 准备结束
                        water.finish_timestamp = timestamp;
                    }
                }
                else // 动画中的
                {
                    if (water.release_timestamp) // 鼠标已经松开了
                    {
                        water.progress = 100 * (water.release_timestamp - water.press_timestamp) / water_press_duration
                                + 100 * (timestamp - water.release_timestamp) / water_release_duration;
                    }
                    else // 鼠标一直按下
                    {
                        water.progress = 100 * (timestamp - water.press_timestamp) / water_press_duration;
                    }
                    if (water.progress > 100)
                        water.progress = 100;
                }
            }
        }
    }

    // ==== 出现动画 ====
    if (show_animation)
    {
        if (show_ani_appearing) // 出现
        {
            qint64 delta = getTimestamp() - show_timestamp;
            if (show_ani_progress >= 100) // 出现结束
            {
                show_ani_appearing = false;
            }
            else
            {
                show_ani_progress = 100 * delta / show_duration;
                if (show_ani_progress > 100)
                    show_ani_progress = 100;
            }
        }
        if (show_ani_disappearing) // 消失
        {
            qint64 delta = getTimestamp() - hide_timestamp;
            if (show_ani_progress <= 0) // 消失结束
            {
                show_ani_disappearing = false;
                show_foreground = false;
                show_ani_point = QPoint(0,0);
            }
            else
            {
                show_ani_progress = 100 - 100 * delta / show_duration;
                if (show_ani_progress < 0)
                    show_ani_progress = 0;
            }
        }
    }

    // ==== 按下动画 ====
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

    // ==== 锚点移动 ====
    if (jitters.size() > 0) // 松开时的抖动效果
    {
        // 当前应该是处在最后一个点
        Jitter cur = jitters.first();
        Jitter aim = jitters.at(1);
        int del = getTimestamp()-cur.timestamp;
        int dur = aim.timestamp - cur.timestamp;
        effect_pos = cur.point + (aim.point-cur.point)*del/dur;
        offset_pos = effect_pos- (geometry().center() - geometry().topLeft());

        if (del >= dur)
            jitters.removeFirst();

        // 抖动结束
        if (jitters.size() == 1)
            jitters.clear();
    }
    else if (anchor_pos != mouse_pos)
    {
        int delta_x = anchor_pos.x() - mouse_pos.x(),
            delta_y = anchor_pos.y() - mouse_pos.y();

        anchor_pos.setX( anchor_pos.x() - quick_sqrt(delta_x) );
        anchor_pos.setY( anchor_pos.y() - quick_sqrt(delta_y) );

        offset_pos.setX(quick_sqrt(static_cast<long>(anchor_pos.x()-(geometry().width()>>1))));
        offset_pos.setY(quick_sqrt(static_cast<long>(anchor_pos.y()-(geometry().height()>>1))));
        effect_pos.setX( (geometry().width() >>1) + offset_pos.x());
        effect_pos.setY( (geometry().height()>>1) + offset_pos.y());
    }
    else if (!pressing && !hovering && !hover_progress && !press_progress
             && !click_ani_appearing && !click_ani_disappearing && !jitters.size() && !waters.size()
             && !show_ani_appearing && !show_ani_disappearing)
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

    jitters.clear(); // 清除抖动
}
