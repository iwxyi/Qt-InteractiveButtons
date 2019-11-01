#ifndef WATERZOOMBUTTON_H
#define WATERZOOMBUTTON_H

#include <QObject>
#include "interactivebuttonbase.h"

class WaterZoomButton : public InteractiveButtonBase
{
public:
    WaterZoomButton(QString text = "", QWidget* parent = nullptr);

    void setChoking(int c);
    void setChokingProp(double p);

protected:
    QPainterPath getBgPainterPath() override;
    void resizeEvent(QResizeEvent *event) override;

private:
    int choking;
    double choking_prop;
};

#endif // WATERZOOMBUTTON_H
