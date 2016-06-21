#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <QImage>
#include <QColor>
#include <QPainter>

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(int inputdim = 200, QWidget *parent = 0);
    void setPixel(int i, int j, QRgb color);
    QSize sizeHint() const;
    int dim() const;
    QImage getImage() { return disp; }

protected:
    void paintEvent(QPaintEvent *event);


private:
    QImage disp;
    int dimension;

};

#endif // DISPLAY_H
