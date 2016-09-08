#include "display.h"

Display::Display(double imageWidth, double imageHeight, QWidget *parent) :
    QWidget(parent)
{
    width = imageWidth;
    height = imageHeight;
    disp = QImage(width, height, QImage::Format_RGB32);
    disp.fill(0);

    colorMap = QVector<QVector<QRgb>>(width, QVector<QRgb>(height));
}

void Display::setPixel(int i, int j, QRgb color)
{
    // disp.setPixel(i, j, color);
    colorMap[i][j] = color;
}

//void Display::shrink() 
//{
//    int newSize = disp.width();
//    if (disp.width() * (1 - SCREEN_SCALING_FACTOR) >= MIN_PREVIEW_IMAGE_SIZE) {
//        newSize = (int)(newSize * (1 - SCREEN_SCALING_FACTOR));
//    }
//    else {
//        newSize = MIN_PREVIEW_IMAGE_SIZE;
//    }
// 
//    disp = disp.scaled(newSize, newSize, Qt::KeepAspectRatio);
//    update();
//}
//
//void Display::enlarge()
//{
//    int newSize = disp.width();
//    if (disp.width() * (1 + SCREEN_SCALING_FACTOR) <= MAX_PREVIEW_IMAGE_SIZE) {
//        newSize = (int)(newSize * (1 + SCREEN_SCALING_FACTOR));
//    }
//    else {
//        newSize = MAX_PREVIEW_IMAGE_SIZE;
//    }
//   
//    disp = disp.scaled(newSize, newSize, Qt::KeepAspectRatio);
//    update();
//}

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
        emit displayReleased(event->pos());
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
        this->height = this->width * (double)(height/width);
    } else {
        this->width = this->height * (double)(width/height);
    }

    resetSize();
    
    return QSize(this->width, this->height);

}

void Display::paintEvent(QPaintEvent * /* unused */)
{
    QPainter painter(this);
    QColor color;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            color = QColor::fromRgb(colorMap[i][j]);
            painter.setPen(color);
            painter.drawPoint(i, j);
        }
    }

}
