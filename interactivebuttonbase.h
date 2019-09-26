#ifndef INTERACTIVEBUTTONBASE_H
#define INTERACTIVEBUTTONBASE_H

#include <QObject>
#include <QPushButton>
#include <QPoint>
#include <QTimer>
#include <QPropertyAnimation>
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
    Q_PROPERTY(int font_size READ getFontSizeT WRITE setFontSizeT)
    Q_PROPERTY(QString text WRITE setText)
    Q_PROPERTY(QColor icon_color WRITE setIconColor)
    Q_PROPERTY(QColor text_color WRITE setTextColor)
    Q_PROPERTY(QColor background_color WRITE setNormalColor)
    Q_PROPERTY(QColor border_color WRITE setBorderColor)
    Q_PROPERTY(QColor hover_color WRITE setHoverColor)
    Q_PROPERTY(QColor press_color WRITE setPressColor)
    Q_PROPERTY(int hover_duration WRITE setHoverAniDuration)
    Q_PROPERTY(int press_duration WRITE setPressAniDuration)
    Q_PROPERTY(int click_duration WRITE setClickAniDuration)
    Q_PROPERTY(int radius WRITE setRadius)
    Q_PROPERTY(int border_width WRITE setBorderWidth)
    Q_PROPERTY(bool fixed_fore_pos WRITE setFixedTextPos)
    Q_PROPERTY(bool text_dynamic_size WRITE setTextDynamicSize)
    Q_PROPERTY(bool leave_after_clicked WRITE setLeaveAfterClick)
    Q_PROPERTY(bool show_animation WRITE setShowAni)
    Q_PROPERTY(bool water_animation WRITE setWaterRipple)
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

    struct PaintAddin {
        PaintAddin() : enable(false) {}
        PaintAddin(QPixmap p, Qt::Alignment a, QSize s) : enable(true), pixmap(p), align(a), size(s) {}
        bool enable;
        QPixmap pixmap;
        Qt::Alignment align;
        QSize size;
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
        EdgeVal() {}
        EdgeVal(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
        int left, top, right, bottom;
    };

    virtual void setText(QString text);
    virtual void setIcon(QIcon icon);
    virtual void setPixmap(QPixmap pixmap);
    virtual void setPaintAddin(QPixmap pixmap, Qt::Alignment align = Qt::AlignRight, QSize size = QSize(0,0));

    void setHoverAniDuration(int d);
    void setPressAniDuration(int d);
    void setClickAniDuration(int d);
    void setWaterAniDuration(int press, int release, int finish);
    void setWaterRipple(bool enable = true);
    void setJitterAni(bool enable = true);
    void setUnifyGeomerey(bool enable = true);
    void setBgColor(QColor bg);
    void setBgColor(QColor hover, QColor press);
    void setNormalColor(QColor color);
    void setBorderColor(QColor color);
    void setHoverColor(QColor color);
    void setPressColor(QColor color);
    void setIconColor(QColor color = QColor(0,0,0));
    void setTextColor(QColor color = QColor(0,0,0));
    void setFontSize(int f);
    void setHover();
    void setAlign(Qt::Alignment a);
    void setRadius(int r);
    void setRadius(int rx, int ry);
    void setBorderWidth(int x);
    void setDisabled(bool dis = true);
    void setPaddings(int l, int r, int t, int b);
    void setPaddings(int x);
    void setFixedTextPos(bool f = true);
    void setFixedForeSize(bool f = true, int addin = 0);
    void setTextDynamicSize(bool d = true);
    void setLeaveAfterClick(bool l = true);

    void setShowAni(bool enable = true);
    void showForeground();
    void showForeground2(QPoint point = QPoint(0,0));
    void hideForeground();
    void delayShowed(int time, QPoint point = QPoint(0,0));

    void setMenu(QMenu* menu);
    void setState(bool s = true);
    bool getState();
    virtual void simulateStatePress(bool s = true);

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void changeEvent(QEvent* event) override;
    void paintEvent(QPaintEvent*) override;

    virtual bool inArea(QPoint point);
    virtual QPainterPath getBgPainterPath();
    virtual QPainterPath getWaterPainterPath(Water water);

    QRect getUnifiedGeometry();
    void updateUnifiedGeometry();
    void paintWaterRipple(QPainter &painter);
    void setJitter();

    int getFontSizeT();
    void setFontSizeT(int f);

    int max(int a, int b) const;
    int min(int a, int b) const;
    int quick_sqrt(long X) const;
    int moveSuitable(int speed, int delta) const;
    qint64 getTimestamp() const;
    bool isLightColor(QColor color);
    int getSpringBackProgress(int x, int max);
    QColor getOpacityColor(QColor color, double level = 0.5);
    QPixmap getMaskPixmap(QPixmap p, QColor c);

signals:
    void showAniFinished();
    void hideAniFinished();
    void pressAppearAniFinished();
    void pressDisappearAniFinished();
    void jitterAniFinished();

public slots:
    void anchorTimeOut();
    virtual void slotClicked();
    void slotCloseState();

public:
    PaintModel model;
    QIcon icon;
    QString text;
    QPixmap pixmap;
    PaintAddin paint_addin;
    EdgeVal icon_paddings;

protected:
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

    QColor icon_color, text_color; // 前景颜色
    QColor normal_bg, hover_bg, press_bg, border_bg; // 各种背景颜色
    int hover_speed, press_start, press_speed; // 颜色渐变速度
    int hover_progress, press_progress; // 颜色渐变进度
    int border_width;
    int radius_x, radius_y;
    int font_size;
    bool fixed_fore_pos;    // 鼠标进入时是否固定文字位置
    bool fixed_fore_size;   // 鼠标进入/点击时是否固定前景大小
    bool text_dynamic_size; // 设置字体时自动调整最小宽高

    bool click_ani_appearing, click_ani_disappearing; // 是否正在按下的动画效果中
    int click_ani_progress; // 按下的进度（使用时间差计算）

    bool unified_geometry; // 统一图标绘制区域尺寸 // 上面用不到的话，这个也用不到……
    int _l, _t, _w, _h;

    bool jitter_animation; // 是否开启鼠标松开时的抖动效果
    double elastic_coefficient; // 弹性系数
    QList<Jitter>jitters;
    int jitter_duration; // 抖动一次，多次效果叠加

    bool water_animation; // 是否开启水波纹动画
    QList<Water>waters;
    int water_press_duration, water_release_duration, water_finish_duration;
    int water_radius;

    Qt::Alignment align;
    bool _state; // 一个记录状态的变量，比如是否持续
    bool leave_after_clicked; // 鼠标点击后是否取消聚焦（针对子类异形按钮）
};


#endif // INTERACTIVEBUTTONBASE_H
