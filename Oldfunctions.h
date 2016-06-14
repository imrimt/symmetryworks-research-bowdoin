#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <complex>

#include <QVector>
#include <QDebug>
#include <QtCore/qmath.h>

#include "pairs.h"
#include "geomath.h"

class AbstractFunction      //this is the base class for all other classes that follow in this file;
{                           //it defines many of the member functions that we needed for all of the
  public:                   //derived classes

    //CONSTRUCTORS
    AbstractFunction() { terms = 6; refresh(); }
    AbstractFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    AbstractFunction(QVector<coeffpair> &in_coeffs, QVector<freqpair> &in_freqs);


    // CONST MEMBER FUNCTIONS
    unsigned int numterms() const { return terms; }
    virtual std::complex<double> bundle(double &x, double &y, unsigned int &i) const = 0;
    virtual std::complex<double> operator() (double i, double j) = 0;
    int N(unsigned int &i) const;
    int M(unsigned int &i) const;
    double R(unsigned int &i) const;
    double A(unsigned int &i) const;
    double scaleR() const { return scale.R(); }
    double scaleA() const { return scale.A(); }

    // MUTATORS
    void setN(unsigned int &i, int &val);
    void setM(unsigned int &i, int &val);
    void setR(unsigned int &i, double &val);
    void setA(unsigned int &i, double &val);
    void setScaleR(double &val);
    void setScaleA(double &val);
    void setNumTerms(int &val);


  protected:

    // DATA
    QVector<coeffpair> coeffs;
    QVector<freqpair> freqs;
    unsigned int terms;
    coeffpair scale;

    // PRIVATE MEMBER FUNCTIONS
    void refresh();
    void initWithVectors(QVector<coeffpair> &in_coeffs, QVector<freqpair> &in_freqs);
};

////////////////////////////////////////////////////////////////

#define Kgen 1.6
#define Lgen 1

//#define Xgen ( 2*pi*x ) original defaults
//#define Ygen ( 2*pi*(x/Kgen + y/Lgen) )

#define Xgen ( 2*pi*x+2*pi*y/q3 )
#define Ygen ( 4*pi*y/q3 )

class generalFunction : public AbstractFunction
{
  public:
    generalFunction() { terms = 4; refresh(); }
    generalFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    generalFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
};

////////////////////////////////////////////////////////////////

#define Kgen2 1.6
#define Lgen2 1

//#define Xgen2 ( 2*pi*x ) original defaults
//#define Ygen2 ( 2*pi*(x/Kgen2 + y/Lgen2) )

#define Xgen2 ( 2*pi*x+2*pi*y/q3 )
#define Ygen2 ( 4*pi*y/q3 )
class generalpairedFunction : public AbstractFunction
{
  public:
    generalpairedFunction() { terms = 4; refresh(); }
    generalpairedFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    generalpairedFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
};

////////////////////////////////////////////////////////////////

#define Xhex3 (2*pi*x+2*pi*y/q3)
#define Yhex3 (4*pi*y/q3)

class hex3Function : public AbstractFunction
{
  public:
    hex3Function() { terms = 4; refresh(); }
    hex3Function(unsigned int in_terms) { terms = in_terms; refresh(); }
    hex3Function(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);


};
////////////////////////////////////////////////////////////////

class hex3CTFunction : public AbstractFunction
{
  public:
    hex3CTFunction() { terms = 6; refresh(); }
    hex3CTFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    hex3CTFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);


};
////////////////////////////////////////////////////////////////

#define Xhex6 (2*pi*x+2*pi*y/q3)
#define Yhex6 (4*pi*y/q3)

class hex6Function : public AbstractFunction
{
  public:
    hex6Function() { terms = 4; refresh(); }
    hex6Function(unsigned int in_terms) { terms = in_terms; refresh(); }
    hex6Function(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);


};

////////////////////////////////////////////////////////////////

#define Krect 1.6
#define Lrect 1

#define Xrect ( (2*pi*x) / Krect)
#define Yrect ( (2*pi*y) / Lrect)

class rectangularFunction : public AbstractFunction
{
  public:
    rectangularFunction() { terms = 4; refresh(); }
    rectangularFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    rectangularFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
};

////////////////////////////////////////////////////////////////

#define Krect2 1.6
#define Lrect2 1

#define Xrect2 ( (2*pi*x) / Krect2)
#define Yrect2 ( (2*pi*y) / Lrect2)

class rectangularpairedFunction : public AbstractFunction
{
  public:
    rectangularpairedFunction() { terms = 4; refresh(); }
    rectangularpairedFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    rectangularpairedFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
};

////////////////////////////////////////////////////////////////

#define Krhombic 1.6
#define Lrhombic 1

#define Xrhombic (pi * (x/Krhombic + y/Lrhombic) )
#define Yrhombic (pi * (x/Krhombic - y/Lrhombic) )

class rhombicFunction : public AbstractFunction
{
  public:
    rhombicFunction() { terms = 4; refresh(); }
    rhombicFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    rhombicFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);


};

////////////////////////////////////////////////////////////////

#define Krhombic2 1.6
#define Lrhombic2 1

#define Xrhombic2 (pi * (x/Krhombic2 + y/Lrhombic2) )
#define Yrhombic2 (pi * (x/Krhombic2 - y/Lrhombic2) )

class rhombicpairedFunction : public AbstractFunction
{
  public:
    rhombicpairedFunction() { terms = 4; refresh(); }
    rhombicpairedFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    rhombicpairedFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);

};

////////////////////////////////////////////////////////////////

#define Xsquare (2*pi*x)
#define Ysquare (2*pi*y)

class squareFunction : public AbstractFunction
{
  public:
    squareFunction() { terms = 4; refresh(); }
    squareFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    squareFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);


};
///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////



class squareMFunction : public AbstractFunction
{
  public:
    squareMFunction() { terms = 4; refresh(); }
    squareMFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    squareMFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);


};
///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////



class squareTFunction : public AbstractFunction
{
  public:
    squareTFunction() { terms = 4; refresh(); }
    squareTFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    squareTFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);


};
///////////////////////////////////////////////////////////////

class holoFunction : public AbstractFunction
{
  public:
    holoFunction() { terms = 4; refresh(); }
    holoFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    holoFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);

};

///////////////////////////////////////////////////////////////

class contFunction : public AbstractFunction
{
  public:
    contFunction() { terms = 4; refresh(); }
    contFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    contFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
};
///////////////////////////////////////////////////////////////

class zzbarFunction : public AbstractFunction
{
  public:
    zzbarFunction() { terms = 4; refresh(); }
    zzbarFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    zzbarFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}

    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);

};

///////////////////////////////////////////////////////////////

#endif // FUNCTIONS_H
