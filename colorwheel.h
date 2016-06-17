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

#include "geomath.h"

#define image_dim 300

const unsigned int ICOS_FACES_SIZE = 20;

class ColorWheel : public QObject
{
    Q_OBJECT

  public:
    // CONSTRUCTORS
    ColorWheel(QObject *parent = 0);

    // ACCESS FUNCTIONS
    QRgb operator() (std::complex<double> zin);
    void loadImage(QString filename);

  private:

    // FUNCTIONAL VARIABLES
    int currentSel;
    QImage image;

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

    // COMPONENT FUNCTIONS
    QRgb toQRgb(RGB in) { return qRgb(in.Rv,in.Gv,in.Bv); }
    v3 initV3(double x1, double y1, double z1);
    v3 tilt(v3 Vin);
    v3 stereo(std::complex<double> Zin);
    v3 crV(v3 Vin);
    v3 r35V(v3 Vin);
    vect initVect(double x1, double y1, double z1, double u1, double v1);
    vect Bvect(double a, double b, double c);
    RGB initRGB(int Rin, int Gin, int Bin);
    RGB Colors(v3 Vin);
    double dp(v3 A1, v3 A2);
    double crt(double nu);
    double r35(double nu);

    v3 icosFaces[ICOS_FACES_SIZE] = 
    {   
        initV3(1.0/q3,1.0/q3,1.0/q3),
        initV3(gold/q3,(gold-1.0)/q3,0.0),
        initV3(0.0,gold/q3,(gold-1.0)/q3),
        initV3((gold-1.0)/q3,0.0,gold/q3),
        initV3(gold/q3,-(gold-1.0)/q3,0.0),
        initV3(0.0,gold/q3,-(gold-1.0)/q3),
        initV3(-(gold-1.0)/q3,0.0,gold/q3),
        initV3(1.0/q3,-1.0/q3,1.0/q3),
        initV3(1.0/q3,1.0/q3,-1.0/q3),
        initV3(-1.0/q3,1.0/q3,1.0/q3),
        initV3(-1.0/q3,1.0/q3,-1.0/q3),
        initV3(-1.0/q3,-1.0/q3,1.0/q3),
        initV3(1.0/q3,-1.0/q3,-1.0/q3),
        initV3(-gold/q3,(gold-1.0)/q3,0.0),
        initV3(0.0,-gold/q3,(gold-1.0)/q3),
        initV3((gold-1.0)/q3,0.0,-gold/q3),
        initV3(-gold/q3,-(gold-1.0)/q3,0.0),
        initV3(0.0,-gold/q3,-(gold-1.0)/q3),
        initV3(-(gold-1.0)/q3,0.0,-gold/q3),
        initV3(-1.0/q3,-1.0/q3,-1.0/q3) 
    };

private slots:
    void setCurrent(int index);


};

#endif // COLORWHEEL_H
