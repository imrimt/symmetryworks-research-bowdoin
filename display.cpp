#include "display.h"

Display::Display(int inputdim, QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    dimension = inputdim;
    disp = QImage(dimension, dimension, QImage::Format_RGB32);
    disp.fill(0);
}

void Display::setPixel(int i, int j, QRgb color)
{
    disp.setPixel(i, j, color);
}

QSize Display::sizeHint() const
{
    QSize size = disp.size();
    return size;
}

int Display::dim() const
{ 
    return dimension;
}

void Display::paintEvent(QPaintEvent * /* unused */)
{
    QPainter painter(this);
    QColor color;

    for(int i=0; i<disp.width(); ++i)
    {
        for(int j=0; j<disp.height(); ++j)
        {
            color = QColor::fromRgb(disp.pixel(i,j));
            painter.setPen(color);
            painter.drawPoint(i, j);
        }
    }
}
