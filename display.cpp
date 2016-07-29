#include "display.h"

Display::Display(double imageWidth, double imageHeight, QWidget *parent) :
    QWidget(parent)
{
    // setAttribute(Qt::WA_StaticContents);
    // setAttribute(Qt::WA_LayoutOnEntireRect);
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    width = imageWidth;
    height = imageHeight;
    disp = QImage(width, height, QImage::Format_RGB32);
    disp.fill(0);

    // for (int i = 0; i < imageSize; i++) {
    //     for (int j = 0; j < imageSize; j++) {
    //         colorMap[i][j] = QColor().rgb();
    //     }
    // }

    colorMap = QVector<QVector<QRgb>>(width, QVector<QRgb>(height));
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


void Display::shrink() 
{
    int newSize = disp.width();
    if (disp.width() * (1 - SCREEN_SCALING_FACTOR) >= MIN_PREVIEW_IMAGE_SIZE) {
        newSize = (int)(newSize * (1 - SCREEN_SCALING_FACTOR));
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
    if (disp.width() * (1 + SCREEN_SCALING_FACTOR) <= MAX_PREVIEW_IMAGE_SIZE) {
        newSize = (int)(newSize * (1 + SCREEN_SCALING_FACTOR));
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

void Display::mousePressEvent(QMouseEvent *event) 
{
    if (event->button() == Qt::LeftButton) {
        mouseMoving = true;
        emit displayPressed(event->pos());
    }
}

void Display::mouseReleaseEvent(QMouseEvent *event) 
{
    if (event->button() == Qt::LeftButton) {
        mouseMoving = false;
        emit displayReleased();
    }
}

void Display::mouseMoveEvent(QMouseEvent *event) 
{
    if (mouseMoving) {
        emit displayMoved(event->pos());
    }   
}

QSize Display::changeDisplayDimensions(double width, double height) {
    
    if (width > height) {
        //qDebug() << "width" << width << "height" << height;
        this->height = this->width * (double)(height/width);
    } else {
        this->width = this->height * (double)(width/height);
    }
    
    //resize(this->width, this->height);
    //disp = disp.scaled(this->width, this->height);
    resetSize();
    
    return QSize(this->width, this->height);
    
   // update();
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
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
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
