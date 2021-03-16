#ifndef APPENDBUTTON_H
#define APPENDBUTTON_H

#include "interactivebuttonbase.h"

class AppendButton : public InteractiveButtonBase
{
public:
    AppendButton(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent *event) override;

public slots:
    virtual void anchorTimeOut() override;

private:
    int add_angle = 0; // 旋转角度
};

#endif // APPENDBUTTON_H
