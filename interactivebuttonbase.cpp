#include "interactivebuttonbase.h"

InteractiveButtonBase::InteractiveButtonBase(QWidget *parent)
    : QPushButton(parent), icon(nullptr), text(""), paint_addin(),
      icon_paddings(4,4,4,4),
      self_enable(true), parent_enabled(false), fore_enabled(true),
      show_animation(false), show_foreground(true), show_ani_appearing(false), show_ani_disappearing(false),
      show_duration(300), show_timestamp(0), hide_timestamp(0), show_ani_progress(0), show_ani_point(0,0),
      enter_pos(-1, -1), press_pos(-1, -1), release_pos(-1, -1), mouse_pos(-1, -1), anchor_pos(-1,  -1),
      offset_pos(0, 0), effect_pos(-1, -1), release_offset(0, 0),
      hovering(false), pressing(false),
      hover_timestamp(0), press_timestamp(0), release_timestamp(0),
      hover_bg_duration(100), press_bg_duration(100), click_ani_duration(300),
      move_speed(5),
      icon_color(0, 0, 0), text_color(0,0,0),
      normal_bg(0xF2, 0xF2, 0xF2, 0), hover_bg(128, 128, 128, 32), press_bg(128, 128, 128, 64), border_bg(0,0,0,0),
      hover_speed(5), press_start(40), press_speed(5),
      hover_progress(0), press_progress(0),
      border_width(1), radius_x(0), radius_y(0), font_size(0), fixed_fore_pos(false), fixed_fore_size(false), text_dynamic_size(false),
      click_ani_appearing(false), click_ani_disappearing(false), click_ani_progress(0),
      unified_geometry(false), _l(0), _t(0), _w(32), _h(32),
      jitter_animation(true), elastic_coefficient(1.2), jitter_duration(300),
      water_animation(true), water_press_duration(800), water_release_duration(400), water_finish_duration(300),
      align(Qt::AlignCenter), _state(false), leave_after_clicked(false)
{
    setMouseTracking(true); // 鼠标没有按下时也能捕获移动事件

    model = PaintModel::None;

    anchor_timer = new QTimer(this);
    anchor_timer->setInterval(10);
    connect(anchor_timer, SIGNAL(timeout()), this, SLOT(anchorTimeOut()));

    setWaterRipple();

    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));

    setFocusPolicy(Qt::NoFocus); // 避免一个按钮还获取Tab键焦点
}

InteractiveButtonBase::InteractiveButtonBase(QString text, QWidget *parent)
    : InteractiveButtonBase(parent)
{
    setText(text);
}

InteractiveButtonBase::InteractiveButtonBase(QIcon icon, QWidget *parent)
    : InteractiveButtonBase(parent)
{
    setIcon(icon);
}

InteractiveButtonBase::InteractiveButtonBase(QPixmap pixmap, QWidget *parent)
    : InteractiveButtonBase(parent)
{
    setPixmap(pixmap);
}

void InteractiveButtonBase::setText(QString text)
{
    this->text = text;
    if (model == PaintModel::None) model = PaintModel::Text;
    if (parent_enabled)
        QPushButton::setText(text);

    // 根据字体调整大小
    if (text_dynamic_size)
    {
        if (font_size <= 0)
        {
            QFontMetrics fm(font());
            setMinimumSize(fm.horizontalAdvance(text)+icon_paddings.left+icon_paddings.right, fm.lineSpacing()+icon_paddings.top+icon_paddings.bottom);
        }
        else
        {
            QFont font;
            font.setPointSize(font_size);
            QFontMetrics fm(font);
            setMinimumSize(fm.horizontalAdvance(text)+icon_paddings.left+icon_paddings.right, fm.lineSpacing()+icon_paddings.top+icon_paddings.bottom);
        }
    }
    update();
}

void InteractiveButtonBase::setIconPath(QString path)
{
    setIcon(QIcon(path));
}

void InteractiveButtonBase::setPixmapPath(QString path)
{
    setPixmap(QPixmap(path));
}

void InteractiveButtonBase::setIcon(QIcon icon)
{
    if (model == PaintModel::None)
        model = PaintModel::Icon;
    this->icon = icon;
    if (parent_enabled)
        QPushButton::setIcon(icon);
    update();
}

void InteractiveButtonBase::setPixmap(QPixmap pixmap)
{
    if (model == PaintModel::None)
        model = PaintModel::PixmapMask;
    this->pixmap = getMaskPixmap(pixmap, isEnabled()?icon_color:getOpacityColor(icon_color));
    if (parent_enabled)
        QPushButton::setIcon(QIcon(pixmap));
    update();
}

void InteractiveButtonBase::setPaintAddin(QPixmap pixmap, Qt::Alignment align, QSize size)
{
    QBitmap mask = pixmap.mask();
    pixmap.fill(icon_color);
    pixmap.setMask(mask);
    paint_addin = PaintAddin(pixmap, align, size);
    update();
}

void InteractiveButtonBase::setSelfEnabled(bool e)
{
    self_enable = e;
}

void InteractiveButtonBase::setParentEnabled(bool e)
{
    parent_enabled = e;

    // 传递子类内容到父类去，避免子类关掉后不显示
    if (model == PaintModel::Text || model == PaintModel::IconText || model == PaintModel::PixmapText)
        QPushButton::setText(text);
    if (model == PaintModel::Icon || model == PaintModel::IconText)
        QPushButton::setIcon(icon);
    if (model == PaintModel::PixmapMask || model == PaintModel::PixmapText)
        QPushButton::setIcon(QIcon(pixmap));
}

void InteractiveButtonBase::setForeEnabled(bool e)
{
    fore_enabled = e;
}

void InteractiveButtonBase::setHoverAniDuration(int d)
{
    this->hover_bg_duration = d;
//    hover_progress = 0; // 重置hover效果
}

void InteractiveButtonBase::setPressAniDuration(int d)
{
    this->press_bg_duration = d;
}

void InteractiveButtonBase::setClickAniDuration(int d)
{
    this->click_ani_duration = d;
}

void InteractiveButtonBase::setWaterAniDuration(int press, int release, int finish)
{
    this->water_press_duration = press;
    this->water_release_duration = release;
    this->water_finish_duration = finish;
}

void InteractiveButtonBase::changeEvent(QEvent *event)
{
    QPushButton::changeEvent(event);

    if (event->type() == QEvent::EnabledChange && model == PixmapMask) // 可用状态改变了
    {
        if (isEnabled())
        {
            QColor color = icon_color;
            color.setAlpha(color.alpha() * 2);
            setIconColor(color);
        }
        else
        {
            QColor color = icon_color;
            color.setAlpha(color.alpha() / 2);
            setIconColor(color);
        }
    }
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

void InteractiveButtonBase::setUnifyGeomerey(bool enable)
{
    unified_geometry = enable;
    _l = _t = 0; _w = size().width(); _h = size().height();
}

void InteractiveButtonBase::setBgColor(QColor bg)
{
    setNormalColor(bg);
    update();
}

void InteractiveButtonBase::setBgColor(QColor hover, QColor press)
{
    if (hover != Qt::black)
        setHoverColor(hover);
    if (press != Qt::black)
        setPressColor(press);
    update();
}

void InteractiveButtonBase::setNormalColor(QColor color)
{
    normal_bg = color;
}

void InteractiveButtonBase::setBorderColor(QColor color)
{
    border_bg = color;
}

void InteractiveButtonBase::setHoverColor(QColor color)
{
    hover_bg = color;
}

void InteractiveButtonBase::setPressColor(QColor color)
{
    press_bg = color;
}

void InteractiveButtonBase::setIconColor(QColor color)
{
    icon_color = color;

    if (model == PaintModel::PixmapMask || model == PaintModel::PixmapText)
    {
        pixmap = getMaskPixmap(pixmap, isEnabled()?icon_color:getOpacityColor(icon_color));
    }

    if (paint_addin.enable)
    {
        paint_addin.pixmap = getMaskPixmap(paint_addin.pixmap, isEnabled()?icon_color:getOpacityColor(icon_color));
    }

    update();
}

void InteractiveButtonBase::setTextColor(QColor color)
{
    text_color = color;
    update();
}

void InteractiveButtonBase::setFontSize(int f)
{
    if (!font_size)
    {
        font_size = f;
        QFont font(this->font());
        font.setPointSize(f);
        setFont(font);
        update();
    }
    else
    {
        QPropertyAnimation* ani = new QPropertyAnimation(this, "font_size");
        ani->setStartValue(font_size);
        ani->setEndValue(f);
        ani->setDuration(click_ani_duration);
        connect(ani, &QPropertyAnimation::finished, [=]{
            ani->deleteLater();
        });
        ani->start();
    }
    if (text_dynamic_size)
    {
        QFont font;
        font.setPointSize(f);
        QFontMetrics fms(font);
        setMinimumSize(fms.horizontalAdvance(text)+icon_paddings.left+icon_paddings.right, fms.lineSpacing()+icon_paddings.top+icon_paddings.bottom);
    }
}

int InteractiveButtonBase::getFontSizeT()
{
    return font_size;
}

void InteractiveButtonBase::setFontSizeT(int f)
{
    this->font_size = f;
    QFont font(this->font());
    font.setPointSize(f);
    setFont(font);
    update();
}

/**
 * 如果点击失去焦点的话，即使鼠标移到上面，也不会出现背景
 * 可以用这个方法继续保持悬浮状态
 */
void InteractiveButtonBase::setHover()
{
    if (!hovering && inArea(mapFromGlobal(QCursor::pos())))
        InteractiveButtonBase::enterEvent(new QEvent(QEvent::Type::None));
}

void InteractiveButtonBase::setAlign(Qt::Alignment a)
{
    align = a;
    update();
}

void InteractiveButtonBase::setRadius(int r)
{
    radius_x = radius_y = r;
}

void InteractiveButtonBase::setRadius(int rx, int ry)
{
    radius_x = rx;
    radius_y = ry;
}

void InteractiveButtonBase::setBorderWidth(int x)
{
    border_width = x;
}

void InteractiveButtonBase::setDisabled(bool dis)
{
    if (dis == !isEnabled()) // 相同的
        return ;

    setEnabled(!dis);

    if (parentWidget()!=nullptr)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents, dis); // 点击穿透
    }

    if (model == PixmapMask || model == PixmapText)
    {
        pixmap = getMaskPixmap(pixmap, dis?getOpacityColor(icon_color):icon_color);
    }

    update(); // 修改透明度
}

void InteractiveButtonBase::setPaddings(int l, int r, int t, int b)
{
    icon_paddings.left = l;
    icon_paddings.right = r;
    icon_paddings.top = t;
    icon_paddings.bottom = b;
}

void InteractiveButtonBase::setPaddings(int x)
{
    icon_paddings.left = x;
    icon_paddings.right = x;
    icon_paddings.top = x;
    icon_paddings.bottom = x;
}

void InteractiveButtonBase::setTextDynamicSize(bool d)
{
    text_dynamic_size = d;
}

void InteractiveButtonBase::setFixedTextPos(bool f)
{
    fixed_fore_pos = f;
}

void InteractiveButtonBase::setFixedForePos(bool f)
{
    fixed_fore_pos = f;
}

void InteractiveButtonBase::setFixedForeSize(bool f, int addin)
{
    fixed_fore_size = f;

    if (!f) return ;
    if (model == PaintModel::Text)
    {
        QFont font = this->font();
        if (font_size > 0)
            font.setPointSize(font_size);
        QFontMetrics fm(font);
        setMinimumSize(fm.horizontalAdvance(text)+icon_paddings.left+icon_paddings.right+addin, fm.lineSpacing()+icon_paddings.top+icon_paddings.bottom+addin);
    }
    else if (model == PaintModel::Icon || model == PaintModel::PixmapMask)
    {
        int size = height();
        setMinimumSize(size, size);
    }
}

void InteractiveButtonBase::setLeaveAfterClick(bool l)
{
    leave_after_clicked = l;
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
    waters.clear();
    if (!anchor_timer->isActive())
        anchor_timer->start();
    if (show_ani_disappearing)
        show_ani_disappearing = false;
    show_ani_appearing = true;
    show_timestamp = getTimestamp();
    show_foreground = true;
    show_ani_point = QPoint(0,0);
}

void InteractiveButtonBase::showForeground2(QPoint point)
{
    showForeground();
    if (point == QPoint(0,0))
        point = mapFromGlobal(QCursor::pos()) - QPoint(size().width()/2, size().height()/2); // 相对于按钮中心
    show_ani_point = point;

    if (unified_geometry) // 统一出现动画
        updateUnifiedGeometry();
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

void InteractiveButtonBase::delayShowed(int time, QPoint point)
{
    setShowAni(true);
    QTimer::singleShot(time, [=]{
        showForeground2(point);
        connect(this, &InteractiveButtonBase::showAniFinished, [=]{
            setShowAni(false);
            disconnect(this, SIGNAL(showAniFinished()), nullptr, nullptr);
        });
    });
}

void InteractiveButtonBase::setMenu(QMenu *menu)
{
    // 默认设置了不获取焦点事件，所以如果设置了菜单的话，就不会有Release事件，水波纹动画会一直飘荡
    // 在 focusOut 事件中，模拟了 release 事件，
    this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    QPushButton::setMenu(menu);
}

void InteractiveButtonBase::setState(bool s)
{
    _state = s;
    update();
}

bool InteractiveButtonBase::getState()
{
    return _state;
}

/**
 * 模拟按下开关的效果，并改变状态（如果不使用状态，则出现点击动画）
 */
void InteractiveButtonBase::simulateStatePress(bool s)
{
    if (getState() == s)
        return ;

    if (inArea(mapFromGlobal(QCursor::pos()))) // 点击当前按钮，不需要再模拟了
        return ;

    mousePressEvent(new QMouseEvent(QMouseEvent::Type::None, QPoint(size().width()/2,size().height()/2), Qt::LeftButton, Qt::NoButton, Qt::NoModifier));

    mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::None, QPoint(size().width()/2,size().height()/2), Qt::LeftButton, Qt::NoButton, Qt::NoModifier));

    // if (!inArea(mapFromGlobal(QCursor::pos()))) // 针对模拟release 后面 // 必定成立
        hovering = false;
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
    mouse_pos = event->pos();

    if (event->button() == Qt::LeftButton)
    {
        if (!hovering)
            InteractiveButtonBase::enterEvent(new QEvent(QEvent::Type::None));

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
        if (!inArea(event->pos()))
        {
            hovering = false;
        }
        pressing = false;
        release_pos = event->pos();
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
    if (hovering == false) // 失去焦点又回来了
    {
        enterEvent(nullptr);
    }
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
    _l = _t = 0; _w = size().width(); _h = size().height();

    return QPushButton::resizeEvent(event);
}

/**
 * 已经取消按钮获取焦点，focusIn和focusOut事件都不会触发
 */
void InteractiveButtonBase::focusInEvent(QFocusEvent *event)
{
    if (!hovering && inArea(mapFromGlobal(QCursor::pos())))
        InteractiveButtonBase::enterEvent(new QEvent(QEvent::Type::None));

    return QPushButton::focusInEvent(event);
}

void InteractiveButtonBase::focusOutEvent(QFocusEvent *event)
{
    if (hovering)
    {
        hovering = false;
    }
    if (pressing) // 鼠标一直按住，可能在click事件中移动了焦点
    {
        pressing = false;
        release_pos = mapFromGlobal(QCursor::pos());
        release_timestamp = getTimestamp();

        if (water_animation && waters.size())
        {
            waters.last().release_timestamp = release_timestamp;
        }
    }

    return QPushButton::focusOutEvent(event);
}

void InteractiveButtonBase::paintEvent(QPaintEvent* event)
{
    if (parent_enabled) // 绘制父类（以便使用父类的QSS和各项属性）
        QPushButton::paintEvent(event);
    if (!self_enable) // 不绘制自己
        return ;
    QPainter painter(this);

    // ==== 绘制背景 ====
    QPainterPath path_back = getBgPainterPath();
    painter.setRenderHint(QPainter::Antialiasing,true);

    if (normal_bg.alpha() != 0) // 默认背景
    {
        painter.fillPath(path_back, isEnabled()?normal_bg:getOpacityColor(normal_bg));
    }

    if (border_bg.alpha() != 0)
    {
        painter.save();
        QPen pen;
        pen.setColor(border_bg);
        pen.setWidth(border_width);
        painter.setPen(pen);
        painter.drawPath(path_back);
        painter.restore();
    }

    if (hover_progress) // 悬浮背景
    {
         painter.fillPath(path_back, getOpacityColor(hover_bg, hover_progress / 100.0));
    }

    if (press_progress && !water_animation) // 按下渐变淡化消失
    {
        painter.fillPath(path_back, getOpacityColor(press_bg, press_progress/100.0));
    }
    else if (water_animation && waters.size()) // 水波纹，且至少有一个水波纹
    {
        paintWaterRipple(painter);
    }

    // ==== 绘制前景 ====
    if (fore_enabled/*针对按钮设置*/ && show_foreground/*针对动画设置*/)
    {
        painter.setPen(isEnabled()?icon_color:getOpacityColor(icon_color));

        // 绘制额外内容（可能被前景覆盖）
        if (paint_addin.enable)
        {
            int l = icon_paddings.left, t = icon_paddings.top, r = size().width()-icon_paddings.right, b = size().height()-icon_paddings.bottom;
            int small_edge = min(size().height(), size().width());
            int pw = paint_addin.size.width() ? paint_addin.size.width() : small_edge-icon_paddings.left-icon_paddings.right;
            int ph = paint_addin.size.height() ? paint_addin.size.height() : small_edge-icon_paddings.top-icon_paddings.bottom;
            if (paint_addin.align & Qt::AlignLeft)
                r = l + pw;
            else if (paint_addin.align & Qt::AlignRight)
                l = r - pw;
            else if (paint_addin.align & Qt::AlignHCenter)
            {
                l = size().width()/2-pw/2;
                r = l+pw;
            }
            if (paint_addin.align & Qt::AlignTop)
                b = t + ph;
            else if (paint_addin.align & Qt::AlignBottom)
                t = b - ph;
            else if (paint_addin.align & Qt::AlignVCenter)
            {
                t = size().height()/2-ph/2;
                b = t+ph;
            }
            painter.drawPixmap(QRect(l,t,r-l,b-t), paint_addin.pixmap);
        }

        QRect rect(icon_paddings.left+(fixed_fore_pos?0:offset_pos.x()), icon_paddings.top+(fixed_fore_pos?0:offset_pos.y()), // 原来的位置，不包含点击、出现效果
                   (size().width()-icon_paddings.left-icon_paddings.right),
                   size().height()-icon_paddings.top-icon_paddings.bottom);

        // 抖动出现动画
        if ((show_ani_appearing || show_ani_disappearing) && show_ani_point != QPoint( 0, 0 ) && ! fixed_fore_pos)
        {
            //int w = size().width(), h = size().height();
            int pro = getSpringBackProgress(show_ani_progress, 50);

            // show_ani_point 是鼠标进入的点，那么起始方向应该是相反的
            int x = show_ani_point.x(), y = show_ani_point.y();
            int gen = quick_sqrt(x*x + y*y);
            x = water_radius * x / gen; // 动画起始中心点横坐标 反向
            y = water_radius * y / gen; // 动画起始中心点纵坐标 反向

            rect = QRect(
                rect.left() - x * (100-pro) / 100 + rect.width() * (100-pro) / 100,
                rect.top() - y * (100-pro) / 100 + rect.height() * (100-pro) / 100,
                rect.width() * pro / 100,
                rect.height() * pro / 100
                );

        }
        else if (align == Qt::AlignCenter && model != PaintModel::Text && !fixed_fore_size) // 默认的缩放动画
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

        /*if (this->isEnabled())
        {
            QColor color = icon_color;
            color.setAlpha(color.alpha() / 2);
            painter.setPen(color);
        }*/

        if (model == None)
        {
            // 子类自己的绘制内容
        }
        else if (model == Text)
        {
            // 绘制文字教程： https://blog.csdn.net/temetnosce/article/details/78068464
            painter.setPen(isEnabled()?text_color:getOpacityColor(text_color));
            /*if (show_ani_appearing || show_ani_disappearing)
            {
                int pro = getSpringBackProgress(show_ani_progress, 50);
                QFont font = painter.font();
                int ps = font.pointSize();
                ps = ps * show_ani_progress / 100;
                font.setPointSize(ps);
                painter.setFont(font);
            }*/
            if (font_size > 0)
            {
                QFont font = painter.font();
                font.setPointSize(font_size);
                painter.setFont(font);
            }
            painter.drawText(rect, static_cast<int>(align), text);
        }
        else if (model == Icon) // 绘制图标
        {
            icon.paint(&painter, rect, align);
        }
        else if (model == PixmapMask)
        {
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true); // 可以让边缘看起来平滑一些
            painter.drawPixmap(rect, pixmap);
        }
        else if (model == PixmapText)
        {

        }
    }

    // ==== 绘制鼠标位置 ====
//    painter.drawEllipse(QRect(anchor_pos.x()-5, anchor_pos.y()-5, 10, 10)); // 移动锚点
//    painter.drawEllipse(QRect(effect_pos.x()-2, effect_pos.y()-2, 4, 4)); // 影响位置锚点

    //    return QPushButton::paintEvent(event); // 不绘制父类背景了
}

bool InteractiveButtonBase::inArea(QPoint point)
{
    return !(point.x() < 0 || point.y() < 0 || point.x() > size().width() || point.y() > size().height());
}

QPainterPath InteractiveButtonBase::getBgPainterPath()
{
    QPainterPath path;
    if (radius_x || radius_y)
        path.addRoundedRect(QRect(0,0,size().width(),size().height()), radius_x, radius_y);
    else
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
    if (radius_x || radius_y)
        return path & getBgPainterPath();
    return path;
}

QRect InteractiveButtonBase::getUnifiedGeometry()
{
    // 将动画进度转换为回弹动画进度
    int pro = show_ani_appearing ? getSpringBackProgress(show_ani_progress,50) : show_ani_progress;
    int ul = 0, ut = 0, uw = size().width(), uh = size().height();

    // show_ani_point 是鼠标进入的点，那么起始方向应该是相反的
    int x = show_ani_point.x(), y = show_ani_point.y();
    int gen = quick_sqrt(x*x + y*y);
    x = - water_radius * x / gen; // 动画起始中心点横坐标 反向
    y = - water_radius * y / gen; // 动画起始中心点纵坐标 反向

    ul = ul + x * (100-pro) / 100 + uw * (100-pro) / 200;
    ut = ut + y * (100-pro) / 100 + uh * (100-pro) / 200;
    uw = uw * pro / 100;
    uh = uh * pro / 100;

    return QRect(ul, ut, uw, uh);
}

void InteractiveButtonBase::updateUnifiedGeometry()
{
    _l = 0; _t = 0; _w = geometry().width(); _h = geometry().height();
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
            manh = static_cast<int>(manh / elastic_coefficient);
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

bool InteractiveButtonBase::isLightColor(QColor color)
{
    return color.red()*0.299 + color.green()*0.578 + color.blue()*0.114 >= 192;
}

int InteractiveButtonBase::getSpringBackProgress(int x, int max)
{
    if (x <= max)
        return x * 100 / max;
    if (x <= max + (100-max)/2)
        return (x-max)/2+100;
    return 100 + (100-x)/2;
}

QColor InteractiveButtonBase::getOpacityColor(QColor color, double level)
{
    color.setAlpha(static_cast<int>(color.alpha() * level));
    return color;
}

QPixmap InteractiveButtonBase::getMaskPixmap(QPixmap p, QColor c)
{
    QBitmap mask = p.mask();
    p.fill(c);
    p.setMask(mask);
    return p;
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
        if (press_progress>0) // 如果按下的效果还在，变浅
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
                water.progress = static_cast<int>(100 - 100 * (timestamp-water.finish_timestamp) / water_finish_duration);
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
                        water.progress = static_cast<int>(100 * (water.release_timestamp - water.press_timestamp) / water_press_duration
                                + 100 * (timestamp - water.release_timestamp) / water_release_duration);
                    }
                    else // 鼠标一直按下
                    {
                        water.progress = static_cast<int>(100 * (timestamp - water.press_timestamp) / water_press_duration);
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
                emit showAniFinished();
            }
            else
            {
                show_ani_progress = static_cast<int>(100 * delta / show_duration);
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
                emit hideAniFinished();
            }
            else
            {
                show_ani_progress = static_cast<int>(100 - 100 * delta / show_duration);
                if (show_ani_progress < 0)
                    show_ani_progress = 0;
            }
        }
    }

    // ==== 按下动画 ====
    if (click_ani_disappearing) // 点击动画效果消失
    {
        qint64 delta = getTimestamp()-release_timestamp-click_ani_duration;
        if (delta <= 0) click_ani_progress = 100;
        else click_ani_progress = static_cast<int>(100 - delta*100 / click_ani_duration);
        if (click_ani_progress < 0)
        {
            click_ani_progress = 0;
            click_ani_disappearing = false;
            emit pressAppearAniFinished();
        }
    }
    if (click_ani_appearing) // 点击动画效果
    {
        qint64 delta = getTimestamp()-release_timestamp;
        if (delta <= 0) click_ani_progress = 0;
        else click_ani_progress = static_cast<int>(delta * 100 / click_ani_duration);
        if (click_ani_progress > 100)
        {
            click_ani_progress = 100; // 保持100的状态，下次点击时回到0
            click_ani_appearing = false;
            click_ani_disappearing = true;
            emit pressDisappearAniFinished();
        }
    }

    // ==== 锚点移动 ====
    if (jitters.size() > 0) // 松开时的抖动效果
    {
        // 当前应该是处在最后一个点
        Jitter cur = jitters.first();
        Jitter aim = jitters.at(1);
        int del = static_cast<int>(getTimestamp()-cur.timestamp);
        int dur = static_cast<int>(aim.timestamp - cur.timestamp);
        effect_pos = cur.point + (aim.point-cur.point)*del/dur;
        offset_pos = effect_pos- (geometry().center() - geometry().topLeft());

        if (del >= dur)
            jitters.removeFirst();

        // 抖动结束
        if (jitters.size() == 1)
        {
            jitters.clear();
            emit jitterAniFinished();
        }
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

    // ==== 统一坐标的出现动画 ====
    if (unified_geometry)
    {
        updateUnifiedGeometry();
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

void InteractiveButtonBase::slotCloseState()
{
    setState(false);
}
