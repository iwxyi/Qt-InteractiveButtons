#ifndef WATERFLOATBUTTON_H
#define WATERFLOATBUTTON_H

#include "interactivebuttonbase.h"

/**
 * 瀑布流按钮控件组
 * 可动态添加多个按钮，并设置/获取选中情况（开关）
 * 实时调整内部按钮的位置，使之可见
 */

class WaterFloatButton : public InteractiveButtonBase
{
public:
    WaterFloatButton(QWidget* parent = nullptr);
    WaterFloatButton(QString s, QWidget* parent = nullptr);

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

    bool inArea(QPoint point) override;

protected:
    QPoint center_pos;
    bool in_area;
    int mwidth;
    int radius;
};

#endif // WATERFLOATBUTTON_H
