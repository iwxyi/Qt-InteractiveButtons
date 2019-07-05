#ifndef INTERACTIVEBUTTONBASE_H
#define INTERACTIVEBUTTONBASE_H

#include <QObject>
#include <QPushButton>
#include <QPoint>
#include <QTimer>
#include <QMouseEvent>

#define MOVE_SPEED 1

class InteractiveButtonBase : public QPushButton
{
    Q_OBJECT
public:
    InteractiveButtonBase(QWidget* parent);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

public slots:
    void anchorTimeOut();

protected:
    QPoint enter_pos;
    QPoint press_pos;
    QPoint current_pos;
    QPoint current_anchor;
    bool pressing;

    QTimer* anchor_timer;
};

#endif // INTERACTIVEBUTTONBASE_H
