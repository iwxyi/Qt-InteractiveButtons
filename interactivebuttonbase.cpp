#include "interactivebuttonbase.h"

InteractiveButtonBase::InteractiveButtonBase(QWidget *parent)
    : QPushButton(parent),
      enter_pos(-1, -1), press_pos(-1, -1), current_pos(-1, -1),
      current_anchor(-1,  -1),
      pressing(false)
{
	anchor_timer = new QTimer(this);
    anchor_timer->setInterval(10);
    connect(anchor_timer, SIGNAL(timeout()), this, SLOT(anchorTimeOut()));

}

void InteractiveButtonBase::mousePressEvent(QMouseEvent *event)
{
    current_pos = mapFromGlobal(QCursor::pos());

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

    return QPushButton::mouseMoveEvent(event);
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

}
