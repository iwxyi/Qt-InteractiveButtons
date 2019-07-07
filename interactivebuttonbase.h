#ifndef INTERACTIVEBUTTONBASE_H
#define INTERACTIVEBUTTONBASE_H

#include <QObject>
#include <QPushButton>
#include <QPoint>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QDateTime>

class InteractiveButtonBase : public QPushButton
{
    Q_OBJECT
public:
    InteractiveButtonBase(QWidget* parent = nullptr);

    void setWaterRipple(bool enable = true);
    void setBgColor(QColor hover, QColor press);
    void setIconColor(QColor color = QColor(0,0,0));

    void setState(bool s = true);
    bool getState();

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void paintEvent(QPaintEvent*) override;

private:
    int max(int a, int b);
    int min(int a, int b);
    int quick_sqrt(long X);
    int moveSuitable(int speed, int delta);
    qint64 getTimestamp();

public slots:
    void anchorTimeOut();
    void slotClicked();

protected:
    QPoint enter_pos, press_pos, release_pos, mouse_pos, anchor_pos;
    QPoint offset_pos, effect_pos, release_offset; // 相对中心、相对左上角、弹起时的偏移
    bool pressing, entering; // 状态机
    bool water_ripple, water_finished;
    qint64 hover_timestamp, press_timestamp, release_timestamp; // 各种事件的时间戳
    int hover_bg_duration, press_bg_duration, click_ani_duration; // 各种动画时长

    QTimer* anchor_timer;
    int move_speed;

    QColor icon_color; // 前景颜色
    QColor normal_bg, hover_bg, press_bg; // 各种背景颜色
    int hover_speed, press_start, press_speed; // 颜色渐变速度
    int hover_progress, press_progress; // 颜色渐变进度

    bool click_ani_appearing, click_ani_disappearing; // 是否正在按下的动画效果中
    int click_ani_progress; // 按下的进度（使用时间差计算）

    bool _state; // 一个记录状态的变量，比如是否持续
};

#endif // INTERACTIVEBUTTONBASE_H
