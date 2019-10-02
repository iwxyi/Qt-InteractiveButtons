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
	void mouseMoveEvent(QMouseEvent * event) override;

	QPainterPath getBgPainterPath() override;
	QPainterPath getWaterPainterPath(InteractiveButtonBase::Water water) override;

private:
	int cha_cheng(QPoint a, QPoint b);
	int dian_cheng(QPoint a, QPoint b);

private:
	QGraphicsDropShadowEffect* shadow_effect;
};

#endif // THREEDIMENBUTTON_H
