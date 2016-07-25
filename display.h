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

const double SCALE = 0.25;
const double MAX_PREVIEW_IMAGE_SIZE = 600;
const double MIN_PREVIEW_IMAGE_SIZE = 100;

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(int inputdim = 600, int imageDim = 200, QWidget *parent = 0);
    void setPixel(int i, int j, QRgb color);
    QSize sizeHint() const;
    int dim() const;
    QImage *getImage() { return &disp; }
    void shrink();
    void enlarge();
    int width() { return imageSize;}
    int height() { return imageSize;}
    void resetSize() {resize(imageSize, imageSize); update();}

signals:
    void displayPressed(const QPoint &point);
    void displayReleased();
    void displayMoved(const QPoint &point);

protected:
    void paintEvent(QPaintEvent *event);
    // void resizeEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private:
    
    QImage disp;
    QVector<QVector<QRgb>> colorMap;
    int dimension;
    int imageSize;

    QPoint topLeft;
    QPoint bottomRight;

    bool mouseMoving;
    QPoint prevMousePos;
};



#endif // DISPLAY_H
