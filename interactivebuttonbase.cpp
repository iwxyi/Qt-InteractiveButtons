#include "interactivebuttonbase.h"

InteractiveButtonBase::InteractiveButtonBase(QWidget *parent)
    : QPushButton(parent),
      enter_pos(-1, -1), press_pos(-1, -1), mouse_pos(-1, -1), anchor_pos(-1,  -1),
      pressing(false),
      hover_bg(128, 128, 128, 10), press_bg(128, 128, 128, 20), bg_progress(0)
{
    setMouseTracking(true); // 鼠标没有按下时也能捕获移动事件

    anchor_timer = new QTimer(this);
    anchor_timer->setInterval(10);
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
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawEllipse(QRect(mouse_pos.x()-5, mouse_pos.y()-5, 10, 10));

//    return QPushButton::paintEvent(event);
}

void InteractiveButtonBase::enterEvent(QEvent *event)
{
    if (!anchor_timer->isActive())
    {
    	anchor_timer->start();
    }

    return QPushButton::enterEvent(event);
}


void InteractiveButtonBase::leaveEvent(QEvent *event)
{

    return QPushButton::leaveEvent(event);
}

/**
 * 锚点变成到鼠标位置的定时时钟
 */
void InteractiveButtonBase::anchorTimeOut()
{


    update();
}
