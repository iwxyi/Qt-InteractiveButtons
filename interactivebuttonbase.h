#ifndef INTERACTIVEBUTTONBASE_H
#define INTERACTIVEBUTTONBASE_H

#include <QObject>
#include <QPushButton>
#include <QPoint>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

#define MOVE_SPEED 1

class InteractiveButtonBase : public QPushButton
{
    Q_OBJECT
public:
    InteractiveButtonBase(QWidget* parent = nullptr);

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent*) override;

public slots:
    void anchorTimeOut();

protected:
    QPoint enter_pos;
    QPoint press_pos;
    QPoint mouse_pos;
    QPoint anchor_pos;
    bool pressing;

    QTimer* anchor_timer;
    QTimer* bg_timer;

    QColor hover_bg;
    QColor press_bg;
    int bg_progress;
};

#endif // INTERACTIVEBUTTONBASE_H
