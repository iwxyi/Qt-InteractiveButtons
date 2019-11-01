#include "waterzoombutton.h"

WaterZoomButton::WaterZoomButton(QString text, QWidget *parent) : InteractiveButtonBase(text, parent)
{
    choking = 10;
}

void WaterZoomButton::setChoking(int c)
{
    choking = c;
}

void WaterZoomButton::setChokingProp(double p)
{
    choking = min(width(), height()) * p;
    choking_prop = p;
}

QPainterPath WaterZoomButton::getBgPainterPath()
{
    QPainterPath path;
    int c = choking * (100 - hover_progress) / 100;
    if (radius_x || radius_y)
        path.addRoundedRect(QRect(c,c,size().width()-c*2,size().height()-c*2), radius_x, radius_y);
    else
        path.addRect(QRect(c,c,size().width()-c*2,size().height()-c*2));
    return path;
}

void WaterZoomButton::resizeEvent(QResizeEvent *event)
{
    InteractiveButtonBase::resizeEvent(event);

    if (abs(choking_prop)>0.0001)
    {
        choking = min(width(), height()) * choking_prop;
    }
}
