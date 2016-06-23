#include "display.h"

Display::Display(int inputdim, QWidget *parent) :
    QWidget(parent)
{
    // setAttribute(Qt::WA_StaticContents);
    // setAttribute(Qt::WA_LayoutOnEntireRect);
    // setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

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
    return disp.size();
}

int Display::dim() const
{
    return dimension;
}

void Display::shrink() 
{
    if (dim() * (1 - SCALE) >= MIN_PREVIEW_SIZE) {
        dimension *= (1 - SCALE);
    }
    else {
        dimension = MIN_PREVIEW_SIZE;
    }
    resize(dimension, dimension);
    disp.scaled(dimension, Qt::KeepAspectRatio);
}

void Display::enlarge()
{
    if (dim() * (1 + SCALE) <= MAX_PREVIEW_SIZE) {
        dimension *= (1 + SCALE);
    }
    else {
        dimension = MAX_PREVIEW_SIZE;
    }
    resize(dimension, dimension);
    disp.scaled(dimension, Qt::KeepAspectRatio);
}

void Display::paintEvent(QPaintEvent * /* unused */)
{
    QPainter painter(this);
    QColor color;

    for(int i = 0; i < disp.width(); ++i)
    // for (int i = 0; i < dimension; i++)
    {
        for(int j = 0; j<disp.height(); ++j)
        // for (int j = 0; j < dimension; j++)
        {
            color = QColor::fromRgb(disp.pixel(i,j));
            painter.setPen(color);
            painter.drawPoint(i, j);
        }
    }
}
