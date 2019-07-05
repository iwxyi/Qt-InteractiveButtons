#ifndef INTERACTIVEBUTTONBASE_H
#define INTERACTIVEBUTTONBASE_H

#include <QObject>
#include <QPushButton>
#include <QPoint>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

class InteractiveButtonBase : public QPushButton
{
    Q_OBJECT
public:
    InteractiveButtonBase(QWidget* parent = nullptr);

    void setWaterRipple(bool enable = true);
    void setIconColor(QColor color = QColor(0,0,0));

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void paintEvent(QPaintEvent*) override;

public slots:
    void anchorTimeOut();

protected:
    QPoint enter_pos, press_pos, mouse_pos, anchor_pos;
    QPoint offset_pos, effect_pos;
    bool pressing, entering;
    bool water_ripple, water_finished;

    QTimer* anchor_timer;
    int move_speed;

    QColor icon_color;
    QColor normal_bg, hover_bg, press_bg;
    int hover_speed, press_start, press_speed;
    int hover_progress, press_progress;
};

#endif // INTERACTIVEBUTTONBASE_H
