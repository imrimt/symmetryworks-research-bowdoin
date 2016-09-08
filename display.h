#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QImage>
#include <QColor>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QVector>
#include <QMouseEvent>

const double SCREEN_SCALING_FACTOR = 0.25;
const double MAX_PREVIEW_IMAGE_SIZE = 600;
const double MIN_PREVIEW_IMAGE_SIZE = 100;

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(double imageWidth = 200, double imageHeight = 200, QWidget *parent = 0);
    void setPixel(int i, int j, QRgb color);
    QSize sizeHint() const { return disp.size(); }
    QImage *getImage() { return &disp; }
//    void shrink();
//    void enlarge();
    int getWidth() { return width;}
    int getHeight() { return height;}
    void resetSize() {resize(width, height); update();}
    QSize changeDisplayDimensions(double width, double height);
    
signals:
    void displayPressed(const QPoint &point);
    void displayReleased(const QPoint &point);
    void displayMoved(const QPoint &point);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private:
    
    QImage disp;
    QVector<QVector<QRgb>> colorMap;
    double width, height;

    QPoint topLeft;
    QPoint bottomRight;

    bool mouseMoving;
    // QPoint prevMousePos;
};



#endif // DISPLAY_H
