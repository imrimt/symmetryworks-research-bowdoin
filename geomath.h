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
const double q2 = qSqrt(2.0);
const double q3 = qSqrt(3.0);
const double q5 = qSqrt(5.0);
const double q7 = qSqrt(7.0);
const double c2 = cos(2.0 * pi/5.0); const double s2 = sin(2.0 * pi/5.0);
const double c4 = cos(4.0 * pi/5.0); const double s4 = sin(4.0 * pi/5.0);
const double c6 = cos(6.0 * pi/5.0); const double s6 = sin(6.0 * pi/5.0);
const double c8 = cos(8.0 * pi/5.0); const double s8 = sin(8.0 * pi/5.0);
const int Fib = 0;
const int MAX_RGB = 255;
const int MIN_RGB = 0;
const std::complex<double> Eye(0,1);
const std::complex<double> a5(gold,-1.0-gold);//for rho5 function. This is phi(1-i*phi)
const std::complex<double> b5(1.0,2.0);

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

//A function to create inversive symmetry
inline std::complex<double> ave2(std::complex<double> in, int nin, int min)
{    std::complex<double> ans(in.real(), in.imag());
    std::complex<double> ansC(in.real(), 0.0-in.imag());
    std::complex<double> ans1=(pow(ans,nin)*pow(ansC,min));
    std::complex<double> ans2=(pow(ans1,-1));
    return (ans1+ans2)/2.0;}


inline std::complex<double> rho3(std::complex<double> in)
{    std::complex<double> tmpz(in.real(), in.imag());
    //std::complex<double> tmpz1(in.real(), in.imag());
    std::complex<double> ans1(1.0,0.0);
    if((tmpz.real()==0)&&(tmpz.imag()==1.0)){ans1=1000;} else{
        ans1=(tmpz+Eye)*pow((tmpz-Eye),-1);}
    return ans1;
}


inline std::complex<double> ave3(std::complex<double> in,int nin,int min)
{    std::complex<double> ans(in.real(), in.imag());
    std::complex<double> tmpz=rho3(ans);
    std::complex<double> tmpz1=rho3(tmpz);
    std::complex<double> ans1=ave2(ans,nin,min);
    std::complex<double> ans2=ave2(tmpz,nin,min);
    std::complex<double> ans3=ave2(tmpz1,nin,min);
    return (ans1+ans2+ans3)/3.0;}

inline std::complex<double> Project(double dis, std::complex<double> zin)
{double x,y,t,X,Y,Z,D2,R2;
    x=zin.real();y=zin.imag();
    D2=dis*dis;
    R2= qSqrt(x*x+y*y);
    if(R2<D2/(D2-1)) {
        t=(R2-qSqrt(D2+R2-R2*D2))/(D2+R2);
        X=((1-t)*x*q2+t)/q3;
        Y=(t-(1-t)*x*q2/2)/q3+(1-t)*y/q2;
        Z=(t-(1-t)*x*q2/2)/q3-(1-t)*y/q2;}
    else {X=0;Y=0;Z=100.0;};
    if(Z==1){Z=1.01;};
    std::complex<double> ans(X/(1-Z),Y/(1-Z));
    return ans;
}
//For icosahedral symmetry, must average over rho5
inline std::complex<double> rho5(std::complex<double> in)
{    std::complex<double> tmpz(in.real(), in.imag());
    std::complex<double> ans1(1.0,0.0);
    //if((tmpz.real()==0)&&(tmpz.imag()==1.0)){ans1=1000;} else{
    ans1=(a5*tmpz+b5)*pow(q5*tmpz+a5,-1);//}
    return ans1;
}

//The blocks to be averaged must have tetrahedral sym, so use ave3, which includes the ave2 part
inline std::complex<double> ave5(std::complex<double> in,int nin,int min)
{    std::complex<double> ans(in.real(), in.imag());
    std::complex<double> tmpz=rho5(ans);
    std::complex<double> tmpz1=rho5(tmpz);
    std::complex<double> tmpz2=rho5(tmpz1);
    std::complex<double> tmpz3=rho5(tmpz2);
    std::complex<double> ans1=ave3(ans,nin,min);
    std::complex<double> ans2=ave3(tmpz,nin,min);
    std::complex<double> ans3=ave3(tmpz1,nin,min);
    std::complex<double> ans4=ave3(tmpz2,nin,min);
    std::complex<double> ans5=ave3(tmpz3,nin,min);
    return (ans1+ans2+ans3+ans4+ans5)/5.0;}

////////////////////Legendre polynomials and associated functions together with complex exp to make spher harmonics
/// This function takes a complex number and returns P_d^m(cos(phi), where d is degree and m goes in e^im*theta
/// We're using M from interface for HALF of degree and N from interface as HALF of Latitude frequency
inline std::complex<double> Leg(std::complex<double> in, int nin, int min)
{    double xv =in.real(); double yv= in.imag();double ans;
    double ra2=xv*xv+yv*yv;
    double ra=qSqrt(ra2);
    double phi=pi-qAtan2(2.0*ra, ra2-1.0);
    double theta=qAtan2(yv,xv);
    double zee=qCos(phi);
    double s=1-zee*zee;
    if (nin<0||nin>2){ans=2.0;} else{
        if(min<0||min<nin){ans=-1.0;} else{
            switch(nin){
                case 0:
                    switch(min){
                        case 0: ans=1.0;
                            break;
                        case 1: ans = (3.0*zee*zee-1.0)/2.0;
                            break;
                        case 2: ans= ( 35.0*pow(zee,4) -  30.0*pow(zee,2) +   3.0)/8.0;
                            break;
                        case 3: ans= ( 231.0*pow(zee,6) - 315.0*pow(zee,4)+ 105.0*pow(zee,2)-   5.0)/16.0;
                            break;
                            //    Pm(0,0,x) =    1
                            //    Pm(1,0,x) =    1 X
                            //    Pm(2,0,x) = (  3 X^2 -   1)/2
                            //    Pm(3,0,x) = (  5 X^3 -   3 X)/2
                            //    Pm(4,0,x) = ( 35 X^4 -  30 X^2 +   3)/8
                            //    Pm(5,0,x) = ( 63 X^5 -  70 X^3 +  15 X)/8
                            //    Pm(6,0,x) = (231 X^6 - 315 X^4 + 105 X^2 -  5)/16
                            //    Pm(7,0,x) = (429 X^7 - 693 X^5 + 315 X^3 - 35 X)/16
                    };break;
                    
                case 1:
                    switch(min){
                        case 0: ans=0.0;
                            break;
                        case 1: ans = 3.0*s;
                            break;
                        case 2: ans= 7.5*s*(7*pow(zee,2)-1);
                            break;
                        case 3: ans= 105.0*s*(33.0*pow(zee,4)-18*pow(zee,2)+1)/8.0;
                            break;
                            //    Pm(2,2,x) =   3 * (1-X^2)
                            //    Pm(3,2,x) =  15 * (1-X^2) * X
                            //    Pm(4,2,x) = 7.5 * (1-X^2) * (7*X^2-1)
                    };break;
                    
                case 2:
                    switch(min){
                        case 0: ans=0.0;
                            break;
                        case 1: ans = 0.0;
                            break;
                        case 2: ans= 105.0 * pow(s,2);
                            break;
                        case 3: ans= 945.0*pow(s,4)*(11.0*zee*zee-1)/2.0;
                            break;
                            // Pm(4,4,x) = 105 * (1-X^2)^2
                    };break;
            };}}
    std::complex<double> ansH = qCos(2*nin*theta)*ans;//remember that N was really 2N
    return ansH;}
//A function to create inversive symmetry
inline std::complex<double> ave2H(std::complex<double> in, int nin, int min)
{    std::complex<double> ans=Leg(in,nin,min);
    std::complex<double> ans1=Leg(pow(in,-1),nin,min);
    return (ans1+ans)/2.0;}
//A function to averageo over rho3
inline std::complex<double> ave3H(std::complex<double> in,int nin,int min)
{    std::complex<double> ans(in.real(), in.imag());
    std::complex<double> tmpz=rho3(ans);
    std::complex<double> tmpz1=rho3(tmpz);
    std::complex<double> ans1=ave2H(ans,nin,min);
    std::complex<double> ans2=ave2H(tmpz,nin,min);
    std::complex<double> ans3=ave2H(tmpz1,nin,min);
    return (ans1+ans2+ans3)/3.0;}

inline std::complex<double> ave5H(std::complex<double> in,int nin,int min)
{    std::complex<double> ans(in.real(), in.imag());
    std::complex<double> tmpz=rho5(ans);
    std::complex<double> tmpz1=rho5(tmpz);
    std::complex<double> tmpz2=rho5(tmpz1);
    std::complex<double> tmpz3=rho5(tmpz2);
    std::complex<double> ans1=ave3H(ans,nin,min);
    std::complex<double> ans2=ave3H(tmpz,nin,min);
    std::complex<double> ans3=ave3H(tmpz1,nin,min);
    std::complex<double> ans4=ave3H(tmpz2,nin,min);
    std::complex<double> ans5=ave3H(tmpz3,nin,min);
    return (ans1+ans2+ans3+ans4+ans5)/5.0;}

inline std::complex<double> Htest(std::complex<double> in,int nin,int min)
{    std::complex<double> ans(in.real(), in.imag());
    
    std::complex<double> ans1=Leg(ans,nin,min);//Some of the spherical harmonics seem to average
    
    return ans1;}





#endif // GEOMATH_H
