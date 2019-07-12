#ifndef WATERFLOATBUTTON_H
#define WATERFLOATBUTTON_H

#include "interactivebuttonbase.h"


/**
 * 一个两边圆形中间时矩形的悬浮按钮
 * 注意：这个按钮的主色调就是 hover_bg，而不是 icon_color，后者已经没用了
 */

class WaterFloatButton : public InteractiveButtonBase
{
public:
    WaterFloatButton(QWidget* parent = nullptr);
    WaterFloatButton(QString text, QWidget* parent = nullptr);

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent *event) override;

    QPainterPath getBgPainterPath() override;
    QPainterPath getWaterPainterPath(Water water) override;

    bool inArea(QPoint point);

protected:
    QPoint center_pos;
    bool in_area;
    int mwidth;
    int radius;

    QString string;
};

#endif // WATERFLOATBUTTON_H
