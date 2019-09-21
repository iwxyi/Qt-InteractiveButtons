#ifndef POINTMENUBUTTON_H
#define POINTMENUBUTTON_H

#include <QWidget>
#include <QObject>
#include "interactivebuttonbase.h"

class PointMenuButton : public InteractiveButtonBase
{
public:
    PointMenuButton(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*event);
};

#endif // POINTMENUBUTTON_H
