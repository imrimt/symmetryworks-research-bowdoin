#ifndef GEOMATH_H
#define GEOMATH_H

#include <QtCore/qmath.h>
#include <complex>
#include <QVector3D>
#include <QColor>
#include <QDebug>

const double pi = 3.1415926535;
const int deg = 3;
const double rx[deg] = {1.0,-2.0,1.0};
const double ry[deg] = {0.0,1.0,-2.0};
const double gold = (1.0+qSqrt(5.0))/2.0;
const double ma = qSqrt(5.0/2.0);
const double q2 = qSqrt(2.0); const double q3 = qSqrt(3.0);
const double q7 = qSqrt(7.0);
const double c2 = cos(2.0 * pi/5.0); const double s2 = sin(2.0 * pi/5.0);
const double c4 = cos(4.0 * pi/5.0); const double s4 = sin(4.0 * pi/5.0);
const double c6 = cos(6.0 * pi/5.0); const double s6 = sin(6.0 * pi/5.0);
const double c8 = cos(8.0 * pi/5.0); const double s8 = sin(8.0 * pi/5.0);
const int Fib = 0;
const int MAX_RGB = 255;
const int MIN_RGB = 0;

struct vect5
{
    double xv;
    double yv;
    double zv;
    double uv;
    double vv;
};

inline vect5 initVect5(double x1, double y1, double z1, double u1, double v1)
{
    vect5 vec;
    vec.xv=x1;
    vec.yv=y1;
    vec.zv=z1;
    vec.uv=u1;
    vec.vv=v1;
    return vec;
}

inline vect5 Bvect5(double a, double b, double c)
{
    vect5 vec;
    double x5,y5,z5,u5,v5;
    x5=a/ma+c/qSqrt(5.0);
    y5=a*c2/ma+b*s2/ma+c/qSqrt(5.0);
    z5=a*c4/ma+b*s4/ma+c/qSqrt(5.0);
    u5=a*c6/ma+b*s6/ma+c/qSqrt(5.0);
    v5=a*c8/ma+b*s8/ma+c/qSqrt(5.0);
    vec = initVect5(x5,y5,z5,u5,v5);
    return vec;
}

inline std::complex<double> ei(double in)
{
    std::complex<double> ans(qCos(in), qSin(in));
    return ans;
}


inline double dotProduct(QVector3D vec1, QVector3D vec2)
{
    return vec1.x() * vec2.x() + vec1.y() * vec2.y() + vec1.z() * vec2.z();
};


inline double cubeRoot(double nu)
{
    static double xt;
    
    if(nu==0.0)
        return 0.0;
    
    if (nu>0.0)
        xt=exp(log(nu)/3.0);
    
    else
        xt=0.0-exp(log(0.0-nu)/3.0);
    
    return xt;
};

inline QVector3D cubeRootVec(QVector3D v)  //cube root of vector entries
{
    
    return QVector3D(cubeRoot(v.x()),cubeRoot(v.y()),cubeRoot(v.z()));
    
}

inline double pow35(double nu) // three fifths power
{
    static double xt;
    
    if(nu==0.0)
        return 0.0;
    
    if (nu>0.0)
        xt=exp(log(nu)*3.0/5.0);
    else
        xt=0.0-exp(log(0.0-nu)*3.0/5.0);
    
    return xt;
}



inline QVector3D pow35Vec(QVector3D v) // 3/5ths power of vector entries
{
    return QVector3D(pow35(v.x()),pow35(v.y()),pow35(v.z()));;
}



inline QVector3D tilt(QVector3D v)
{
    return QVector3D((q2*v.x() + v.z()) / q3, -v.x() / q2 / q3 + v.y() / q2 + v.z() / q3, -v.x() / q2 / q3 - v.y() / q2 + v.z() / q3);
    
}

inline QVector3D stereo(std::complex<double> z)
{
    double r2;
    double x = z.real();
    double y = z.imag();
    
    r2 = 1 + x*x + y*y;
    return QVector3D(2.0 * x / r2, 2.0 * y / r2, 2.0 / r2 - 1.0);
}


inline QRgb RgbFromVec3(QVector3D v)
{
    
    QColor color(int(double(MAX_RGB) *(1.0 + v.x())/2.0), int(double(MAX_RGB) * (1.0 + v.y())/2.0), int(double(MAX_RGB) * (1.0 + v.z())/2.0));
    
    return color.rgb();
}






#endif // GEOMATH_H
