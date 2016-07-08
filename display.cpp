#include "display.h"

Display::Display(int inputDim, int imageDim, QWidget *parent) :
    QWidget(parent)
{
    // setAttribute(Qt::WA_StaticContents);
    // setAttribute(Qt::WA_LayoutOnEntireRect);
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    dimension = inputDim;
    imageSize = imageDim;
    disp = QImage(imageSize, imageSize, QImage::Format_RGB32);
    disp.fill(0);

    // for (int i = 0; i < imageSize; i++) {
    //     for (int j = 0; j < imageSize; j++) {
    //         colorMap[i][j] = QColor().rgb();
    //     }
    // }

    colorMap = QVector<QVector<QRgb>>(imageSize, QVector<QRgb>(imageSize));
}

void Display::setPixel(int i, int j, QRgb color)
{
    // disp.setPixel(i, j, color);
    colorMap[i][j] = color;
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
    int newSize = disp.width();
    if (disp.width() * (1 - SCALE) >= MIN_PREVIEW_IMAGE_SIZE) {
        newSize = (int)(newSize * (1 - SCALE));
    }
    else {
        newSize = MIN_PREVIEW_IMAGE_SIZE;
    }
    // resize(dimension, dimension);
    disp = disp.scaled(newSize, newSize, Qt::KeepAspectRatio);
    update();
}

void Display::enlarge()
{
    int newSize = disp.width();
    if (disp.width() * (1 + SCALE) <= MAX_PREVIEW_IMAGE_SIZE) {
        newSize = (int)(newSize * (1 + SCALE));
    }
    else {
        newSize = MAX_PREVIEW_IMAGE_SIZE;
    }
    // qDebug() <<"new size for image:" << newSize;
    // resize(dimension, dimension);
    disp = disp.scaled(newSize, newSize, Qt::KeepAspectRatio);
    // qDebug() << "new size:" << disp.width();
    update();
}

void Display::paintEvent(QPaintEvent * /* unused */)
{
    QPainter painter(this);
    QColor color;

    // qDebug() << "drawing from" << QPoint((int)((dimension - disp.width())/2), (int)((dimension - disp.height())/2)) << "to" << 
    // QPoint((int)((dimension - disp.width())/2) + disp.width(), (int)((dimension - (int)disp.height())/2) + disp.height());

    // for(int i = (int)((dimension - disp.width())/2); i < (int)((dimension - disp.width())/2) + disp.width(); i++)
    // // for (int i = 0; i < dimension; i++)
    // {
    //     for(int j = (int)((dimension - (int)disp.height())/2); j < (int)((dimension - (int)disp.height())/2) + disp.height(); j++)
    //     // for (int j = 0; j < dimension; j++)
    //     {
    //         color = QColor::fromRgb(disp.pixel(i,j));
    //         painter.setPen(color);
    //         painter.drawPoint(i, j);
    //     }
    // }

    // qDebug() << "size of image:" << disp.width() << "x" << disp.height();

    // for (int i = 0; i < disp.width(); i++) {
        // for (int j = 0; j < disp.height(); j++) {
    for (int i = 0; i < imageSize; i++) {
        for (int j = 0; j < imageSize; j++) {
            // color = QColor::fromRgb(disp.pixel(i,j));
            color = QColor::fromRgb(colorMap[i][j]);
            painter.setPen(color);
            painter.drawPoint(i, j);
        }
    }

    // for (int i = topLeft.x(); i < bottomRight.x(); i++) {
    //     for (int j = topLeft.y(); i < bottomRight.y(); j++) {
    //         color = QColor::fromRgb(disp.pixel(i,j));
    //         painter.setPen(color);
    //         painter.drawPoint(i, j);
    //     }
    // }
}
