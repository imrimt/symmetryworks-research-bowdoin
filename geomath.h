#ifndef GEOMATH_H
#define GEOMATH_H

#include <QtCore/qmath.h>
#include <complex>

const double pi=3.1415926535;
const int deg=3;
const double rx[deg]={1.0,-2.0,1.0};
const double ry[deg]={0.0,1.0,-2.0};
const double gold=(1.0+sqrt(5.0))/2.0;
const double ma=sqrt(5.0/2.0);
const double q2=sqrt(2.0);  const double q3=sqrt(3.0);
const double q7=sqrt(7.0);
const double  c2=cos(2.0* pi/5.0);const double s2= sin(2.0* pi/5.0);
const double  c4=cos(4.0* pi/5.0);const double s4= sin(4.0* pi/5.0);
const double  c6=cos(6.0* pi/5.0);const double s6= sin(6.0* pi/5.0);
const double  c8=cos(8.0* pi/5.0);const double s8= sin(8.0* pi/5.0);
const int Fib=0;

inline std::complex<double> ei(double in)
{
    std::complex<double> ans(qCos(in), qSin(in));
    return ans;
}

struct RGB
{
    int Rv;
    int Gv;
    int Bv;
};

struct v3
{
    double xv;
    double yv;
    double zv;
};

struct vect
{
    double xv;
    double yv;
    double zv;
    double uv;
    double vv;
};

#endif // GEOMATH_H
