#include "interactivebuttonbase.h"

InteractiveButtonBase::InteractiveButtonBase(QWidget *parent)
    : QPushButton(parent),
      enter_pos(-1, -1), press_pos(-1, -1), mouse_pos(-1, -1), anchor_pos(-1,  -1),
      pressing(false), entering(false),
      normal_bg(128, 128, 128, 0), hover_bg(128, 128, 128, 25), press_bg(128, 128, 128, 50),
      hover_speed(10), press_speed(20),
      hover_progress(0), press_progress(0)
{
    setMouseTracking(true); // 鼠标没有按下时也能捕获移动事件

    anchor_timer = new QTimer(this);
    anchor_timer->setInterval(20);
    connect(anchor_timer, SIGNAL(timeout()), this, SLOT(anchorTimeOut()));

}

void InteractiveButtonBase::mousePressEvent(QMouseEvent *event)
{
    mouse_pos = mapFromGlobal(QCursor::pos());

    if (event->button() == Qt::LeftButton)
    {
    	pressing = true;
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

void InteractiveButtonBase::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    // 绘制背景
    QPainterPath path_back;
    path_back.setFillRule(Qt::WindingFill);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    path_back.addRect(QRect(QPoint(0,0), geometry().size()));
    /*QColor bg_color(0, 0, 0, 0); // 背景颜色
    if (hover_progress || press_progress)
    {
        float sum_weight = hover_progress + press_progress;
        float hover_weigth = hover_progress / sum_weight;
        float press_weight = press_progress / sum_weight;
        int red = hover_bg.red() * hover_weigth + press_bg.red() * press_weight;
        int green = hover_bg.green() * hover_weigth + press_bg.green() * press_weight;
        int blue = hover_bg.blue() * hover_weigth + press_bg.blue() * press_weight;
        int alpha = hover_bg.alpha() * hover_weigth + press_bg.alpha() * press_weight;
        bg_color = QColor(red, green, blue, alpha);
    }
    qDebug() << hover_progress << press_progress << bg_color;
    painter.fillPath(path_back, QBrush(bg_color));*/

    if (hover_progress)
    {
    	QColor bg_color = hover_bg;
    	bg_color.setAlpha(hover_bg.alpha() * hover_progress / 100);
    	painter.fillPath(path_back, QBrush(bg_color));
    }

    if (press_progress)
    {
    	QColor bg_color = press_bg;
    	bg_color.setAlpha(press_bg.alpha() * press_progress / 100);
    	painter.fillPath(path_back, QBrush(bg_color));
    }


    // 绘制鼠标位置
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawEllipse(QRect(mouse_pos.x()-5, mouse_pos.y()-5, 10, 10));

//    return QPushButton::paintEvent(event); // 不绘制父类背景了
}

void InteractiveButtonBase::enterEvent(QEvent *event)
{
    if (!anchor_timer->isActive())
    {
    	anchor_timer->start();
    }
    entering = true;

    return QPushButton::enterEvent(event);
}


void InteractiveButtonBase::leaveEvent(QEvent *event)
{
    entering = false;

    return QPushButton::leaveEvent(event);
}

/**
 * 锚点变成到鼠标位置的定时时钟
 */
void InteractiveButtonBase::anchorTimeOut()
{
    if (pressing) // 鼠标按下
    {
        if (press_progress < 100)
            press_progress += press_speed;
    }
    else // 鼠标悬浮
    {
        if (press_progress>0) // 如果按下的效果还在
            press_progress -= press_speed;

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

    update();
}
