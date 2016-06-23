#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QImage>
#include <QColor>
#include <QPainter>
#include <QLabel>

const double SCALE = 0.25;
const double MAX_PREVIEW_SIZE = 600;
const double MIN_PREVIEW_SIZE = 100;
const double DEFAULT_SIZE = 200;

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(int inputdim = 200, QWidget *parent = 0);
    void setPixel(int i, int j, QRgb color);
    QSize sizeHint() const;
    int dim() const;
    QImage *getImage() { return &disp; }
    void shrink();
    void enlarge();
    void resetSize() {resize(DEFAULT_SIZE, DEFAULT_SIZE);}

protected:
    void paintEvent(QPaintEvent *event);


private:
    QImage disp;
    int dimension;
};



#endif // DISPLAY_H
