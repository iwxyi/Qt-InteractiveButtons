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
    void setRadiusZoom(int radius);
    void setRadius(int x, int x2);

protected:
    QPainterPath getBgPainterPath() override;
    void resizeEvent(QResizeEvent *event) override;

private:
    int choking;
    double choking_prop;
    int radius_zoom;
};

#endif // WATERZOOMBUTTON_H
