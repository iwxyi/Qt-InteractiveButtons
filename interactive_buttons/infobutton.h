#ifndef INFOBUTTON_H
#define INFOBUTTON_H

#include "interactivebuttonbase.h"

class InfoButton : public InteractiveButtonBase
{
public:
    InfoButton(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // INFOBUTTON_H
