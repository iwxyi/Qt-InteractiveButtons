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
#include <QBitmap>

#define PI 3.1415926
#define GOLDEN_RATIO 0.618

class InteractiveButtonBase : public QPushButton
{
    Q_OBJECT
public:
    InteractiveButtonBase(QWidget* parent = nullptr);
    InteractiveButtonBase(QString text, QWidget* parent = nullptr);
    InteractiveButtonBase(QIcon icon, QWidget* parent = nullptr);
    InteractiveButtonBase(QPixmap pixmap, QWidget* parent = nullptr);

    enum PaintModel {
        None,
        Text,
        Icon,
        PixmapMask,
        IconText,
        PixmapText
    };

    struct Jitter {
        Jitter(QPoint p, qint64 t) : point(p), timestamp(t) {}
        QPoint point;
        qint64 timestamp;
    };

    struct Water {
        Water(QPoint p, qint64 t) : point(p), progress(0), press_timestamp(t),
            release_timestamp(0), finish_timestamp(0), finished(false) {}
        QPoint point;
        int progress;
        qint64 press_timestamp;
        qint64 release_timestamp;
        qint64 finish_timestamp;
        bool finished;
    };

    struct EdgeVal {
        EdgeVal(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
        int left, top, right, bottom;
    };


    void setWaterRipple(bool enable = true);
    void setJitterAni(bool enable = true);
    void setBgColor(QColor hover, QColor press);
    void setIconColor(QColor color = QColor(0,0,0));

    void setShowAni(bool enable = true);
    void showForeground();
    void showForeground2();
    void hideForeground();

    void setState(bool s = true);
    bool getState();

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void changeEvent(QEvent* event) override;
    void paintEvent(QPaintEvent*) override;

protected:
    virtual QPainterPath getBgPainterPath();
    virtual QPainterPath getWaterPainterPath(Water water);

    void paintWaterRipple(QPainter &painter);
    void setJitter();

    int max(int a, int b) const;
    int min(int a, int b) const;
    int quick_sqrt(long X) const;
    int moveSuitable(int speed, int delta) const;
    qint64 getTimestamp() const;
    bool isLightColor(QColor color);

public slots:
    void anchorTimeOut();
    virtual void slotClicked();

protected:
    PaintModel model;
    QIcon icon;
    QString text;
    QPixmap pixmap;
    EdgeVal icon_paddings;

    bool show_animation, show_foreground;
    bool show_ani_appearing, show_ani_disappearing;
    int show_duration;
    qint64 show_timestamp, hide_timestamp;
    int show_ani_progress;
    QPoint show_ani_point;

    QPoint enter_pos, press_pos, release_pos, mouse_pos, anchor_pos/*渐渐靠近鼠标*/;
    QPoint offset_pos, effect_pos, release_offset; // 相对中心、相对左上角、弹起时的平方根偏移
    bool pressing, hovering; // 状态机
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
    int water_press_duration, water_release_duration, water_finish_duration;
    int water_radius;

    bool _state; // 一个记录状态的变量，比如是否持续
};


#endif // INTERACTIVEBUTTONBASE_H
