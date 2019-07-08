#include "watercirclebutton.h"

WaterCircleButton::WaterCircleButton(QWidget* parent)
    : InteractiveButtonBase (parent)
{
    QString style = "border: 1px #888888; border-radius:3px";
    setStyleSheet(style);
}

void WaterCircleButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);
}
