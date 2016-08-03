#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <complex>
#include <time.h>

#include <QObject>
#include <QColor>
#include <QImage>
#include <QVector3D>

#include "geomath.h"

#define image_dim 300

const unsigned int ICOS_FACES_SIZE = 20;
const unsigned int ZONE_VECT_SIZE = 32;

class ColorWheel : public QObject
{
    Q_OBJECT
    
public:
    // CONSTRUCTORS
    ColorWheel(QObject *parent = 0);
    
    // ACCESS FUNCTIONS
    QRgb operator() (std::complex<double> zin);
    void loadImage(QString filename);
    
    ColorWheel* clone();
    
    QColor getOverflowColor() { return overflowColor; }
    
private:
    
    // FUNCTIONAL VARIABLES
    int currentSel;
    QImage image;
    QColor overflowColor;
    
    // COLOR WHEEL FUNCTIONS
    QRgb IcosColor(std::complex<double> zin);
    QRgb IcosColorC(std::complex<double> zin);
    QRgb StCol(std::complex<double> zin);
    QRgb StColC(std::complex<double> zin);
    QRgb StCol35(std::complex<double> zin);
    QRgb ZoneCol(std::complex<double> zin);
    QRgb SectCol(std::complex<double> zin);
    QRgb Sect6Col(std::complex<double> zin);
    QRgb WinCol(std::complex<double> zin);
    QRgb FromImage(std::complex<double> zin);
    
    // COMPONENT VARIABLES
    QVector3D icosFaces[ICOS_FACES_SIZE] =
    {
        QVector3D(1.0/q3,1.0/q3,1.0/q3),
        QVector3D(gold/q3,(gold-1.0)/q3,0.0),
        QVector3D(0.0,gold/q3,(gold-1.0)/q3),
        QVector3D((gold-1.0)/q3,0.0,gold/q3),
        QVector3D(gold/q3,-(gold-1.0)/q3,0.0),
        QVector3D(0.0,gold/q3,-(gold-1.0)/q3),
        QVector3D(-(gold-1.0)/q3,0.0,gold/q3),
        QVector3D(1.0/q3,-1.0/q3,1.0/q3),
        QVector3D(1.0/q3,1.0/q3,-1.0/q3),
        QVector3D(-1.0/q3,1.0/q3,1.0/q3),
        QVector3D(-1.0/q3,1.0/q3,-1.0/q3),
        QVector3D(-1.0/q3,-1.0/q3,1.0/q3),
        QVector3D(1.0/q3,-1.0/q3,-1.0/q3),
        QVector3D(-gold/q3,(gold-1.0)/q3,0.0),
        QVector3D(0.0,-gold/q3,(gold-1.0)/q3),
        QVector3D((gold-1.0)/q3,0.0,-gold/q3),
        QVector3D(-gold/q3,-(gold-1.0)/q3,0.0),
        QVector3D(0.0,-gold/q3,-(gold-1.0)/q3),
        QVector3D(-(gold-1.0)/q3,0.0,-gold/q3),
        QVector3D(-1.0/q3,-1.0/q3,-1.0/q3)
    };
    
    QVector3D zoneVect[ZONE_VECT_SIZE];
    
    public slots:
    void setCurrent(int index);
    void changeOverflowColor(const QColor &color) { overflowColor = color; }
    
    
};

#endif // COLORWHEEL_H
