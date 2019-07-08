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
#include <QList>

#define PI 3.1415926

class InteractiveButtonBase : public QPushButton
{
    Q_OBJECT
public:
    InteractiveButtonBase(QWidget* parent = nullptr);

    struct Jitter {
        Jitter(QPoint p, qint64 t) : point(p), timestamp(t) {}
        QPoint point;
        qint64 timestamp;
    };

    struct Water {
        Water(QPoint p, qint64 t) : point(p), progress(0), press_timestamp(t), release_timestamp(0) {}
        QPoint point;
        int progress;
        qint64 press_timestamp;
        qint64 release_timestamp;
    };

    void setWaterRipple(bool enable = true);
    void setJitterAni(bool enable = true);
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

protected:
    int max(int a, int b) const;
    int min(int a, int b) const;
    int quick_sqrt(long X) const;
    int moveSuitable(int speed, int delta) const;
    qint64 getTimestamp() const;

public slots:
    void anchorTimeOut();
    virtual void slotClicked();

protected:
    QPoint enter_pos, press_pos, release_pos, mouse_pos, anchor_pos/*渐渐靠近鼠标*/;
    QPoint offset_pos, effect_pos, release_offset; // 相对中心、相对左上角、弹起时的平方根偏移
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

    bool jitter_animation; // 是否开启鼠标松开时的抖动效果
    double elastic_coefficient; // 弹性系数
    QList<Jitter>jitters;
    int jitter_duration; // 抖动一次，多次效果叠加

    bool water_animation; // 是否开启水波纹动画
    QList<Water>waters;
    int water_press_duration, water_release_duration;

    bool _state; // 一个记录状态的变量，比如是否持续
};


#endif // INTERACTIVEBUTTONBASE_H
