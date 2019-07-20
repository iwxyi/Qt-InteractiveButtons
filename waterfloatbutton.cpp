#include "waterfloatbutton.h"

WaterFloatButton::WaterFloatButton(QWidget *parent) : InteractiveButtonBase(parent), in_area(false), mwidth(16), radius_x(8)
{

}

WaterFloatButton::WaterFloatButton(QString text, QWidget *parent) : InteractiveButtonBase(parent), in_area(false), mwidth(16), radius_x(8), string(text)
{

}

void WaterFloatButton::enterEvent(QEvent *event)
{

}

void WaterFloatButton::leaveEvent(QEvent *event)
{
    if (in_area && !pressing && !inArea(mapFromGlobal(QCursor::pos())))
    {
        in_area = false;
        InteractiveButtonBase::leaveEvent(event);
    }
}

void WaterFloatButton::mousePressEvent(QMouseEvent *event)
{
    if (in_area)
        return InteractiveButtonBase::mousePressEvent(event);
}

void WaterFloatButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressing)
    {
        InteractiveButtonBase::mouseReleaseEvent(event);

        if (!pressing && !inArea(event->pos()))
        {
            in_area = false;
            InteractiveButtonBase::leaveEvent(event);
        }
    }
}

void WaterFloatButton::mouseMoveEvent(QMouseEvent *event)
{
    bool is_in = inArea(event->pos());

    if (!in_area && is_in) // 鼠标移入
    {
        in_area = true;
        InteractiveButtonBase::enterEvent(nullptr);
    }
    else if (in_area && !is_in && !pressing) // 鼠标移出
    {
        in_area = false;
        InteractiveButtonBase::leaveEvent(nullptr);
    }

    if (in_area)
        InteractiveButtonBase::mouseMoveEvent(event);
}

void WaterFloatButton::resizeEvent(QResizeEvent *event)
{
    int w = geometry().width(), h = geometry().height();
    if (h >= w * 4) // 宽度为准
        radius_x = w / 4;
    else
        radius_x = h/2;
    mwidth = (w-radius_x*2);

    return InteractiveButtonBase::resizeEvent(event);
}

void WaterFloatButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    QPainter painter(this);
//    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::Antialiasing,true);

    // 进度
    QColor edge_color = hover_bg;
    int pro = 0;
    if (hover_progress > 0 || press_progress || waters.size())
    {
        if (water_animation)
        {
            /** 不用判断 water 是出现还是消失状态
              * 如果一直悬浮的话，颜色不会变
              * 而如果是点一下立马移开，文字会出现一种“渐隐渐现”的效果
              */
            if (waters.size())
                pro = max(hover_progress, waters.last().progress);
            else
                pro = hover_progress;
        }
        else
        {
            max(hover_progress, press_progress);
        }
        edge_color.setAlpha(255 * (100 - pro) / 100);
    }

    // 画边框
    QPainterPath path;
    if (show_foreground)
    {
        path = getBgPainterPath(); // 整体背景

        // 出现动画
        if (show_ani_appearing && show_ani_progress != 100)
        {
            int pw = size().width() * show_ani_progress / 100;
            QRect rect(0, 0, pw, size().height());
            QPainterPath rect_path;
            rect_path.addRect(rect);
            path &= rect_path;

            int x = show_ani_point.x(), y = show_ani_point.y();
            int gen = quick_sqrt(x*x + y*y);
            x = - water_radius * x / gen; // 动画起始中心点横坐标 反向
            y = - water_radius * y / gen; // 动画起始中心点纵坐标 反向
        }

        painter.setPen(hover_bg);
        painter.drawPath(path);
    }

    // 画文字
    if (!string.isEmpty())
    {
        QRect rect = QRect(QPoint(0,0), size());
        QColor color;
        if (pro)
        {
            QColor aim_color = isLightColor(hover_bg) ? QColor(0,0,0) : QColor(255,255,255);
            color = QColor(
                        hover_bg.red() + (aim_color.red()-hover_bg.red()) * pro / 100,
                        hover_bg.green() + (aim_color.green()-hover_bg.green()) * pro / 100,
                        hover_bg.blue() + (aim_color.blue()-hover_bg.blue()) * pro / 100,
                        255
                        );
            painter.setPen(color);
        }
        else
        {
            color = hover_bg;
            color.setAlpha(255);
        }
        painter.setPen(color);
        painter.drawText(rect, Qt::AlignCenter, string);
    }
}

QPainterPath WaterFloatButton::getBgPainterPath()
{
    QPainterPath path1, path2, path3;
    int w = size().width(), h = size().height();

    QRect mrect(w/2-mwidth/2, h/2-radius_x, mwidth, radius_x*2);
    path1.addRect(mrect);

    QPoint o1(w/2-mwidth/2, h/2);
    QPoint o2(w/2+mwidth/2, h/2);
    path2.addEllipse(o1.x()-radius_x, o1.y()-radius_x, radius_x*2, radius_x*2);
    path3.addEllipse(o2.x()-radius_x, o2.y()-radius_x, radius_x*2, radius_x*2);

    return path1 | path2 | path3;
}

QPainterPath WaterFloatButton::getWaterPainterPath(InteractiveButtonBase::Water water)
{
    QPainterPath path = InteractiveButtonBase::getWaterPainterPath(water) & getBgPainterPath();
    return path;
}

bool WaterFloatButton::inArea(QPoint point)
{
    int w = size().width(), h = size().height();
    QPoint o1(w/2-mwidth/2, h/2);
    QPoint o2(w/2+mwidth/2, h/2);
    QRect mrect(w/2-mwidth/2, h/2-radius_x, mwidth, radius_x*2);

    if (mrect.contains(point))
        return true;
    if ((point-o1).manhattanLength() <= radius_x ||
            (point-o2).manhattanLength() <= radius_x)
        return true;
    return false;
}
