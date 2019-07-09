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
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent*) override;

    void setPainterPathGeometry(QPainterPath& path) override;
    void paintWaterRipple(QPainter& painter) override;

protected:
    QPoint center_pos;
    bool in_circle;
    int radius;
};

#endif // WATERCIRCLEBUTTON_H
