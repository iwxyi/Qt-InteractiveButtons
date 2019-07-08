#ifndef WATERCIRCLEBUTTON_H
#define WATERCIRCLEBUTTON_H

#include <QObject>
#include <QWidget>
#include "interactivebuttonbase.h"

class WaterCircleButton : public InteractiveButtonBase
{
public:
    WaterCircleButton(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*event);
};

#endif // WATERCIRCLEBUTTON_H
