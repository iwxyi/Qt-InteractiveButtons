#ifndef THREEDIMENBUTTON_H
#define THREEDIMENBUTTON_H

#include <QObject>
#include <QGraphicsDropShadowEffect>
#include "interactivebuttonbase.h"

class ThreeDimenButton : public InteractiveButtonBase
{
	#define AOPER 10
	#define SHADE 10
    Q_OBJECT
public:
    ThreeDimenButton(QWidget* parent = nullptr);

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void anchorTimeOut() override;

	QPainterPath getBgPainterPath() override;
	QPainterPath getWaterPainterPath(InteractiveButtonBase::Water water) override;

    void simulateStatePress(bool s = true, bool a = false) override;
    bool inArea(QPoint point) override;

private:
	int cha_cheng(QPoint a, QPoint b);
	int dian_cheng(QPoint a, QPoint b);
	QPoint limitPointXY(QPoint v, int w, int h);

protected:
	QGraphicsDropShadowEffect* shadow_effect;
    bool in_rect;
    int aop_w, aop_h;
};

#endif // THREEDIMENBUTTON_H
