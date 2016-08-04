#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <complex>

#include <QVector>
#include <QDebug>
#include <QtCore/qmath.h>

#include "pairs.h"
#include "geomath.h"
#include "display.h"

class AbstractFunction      //this is the base class for all other classes that follow in this file;
{                           //it defines many of the member functions that we needed for all of the
public:                   //derived classes
    
    //CONSTRUCTORS
    AbstractFunction() { terms = 1; refresh(); }
    AbstractFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    AbstractFunction(QVector<coeffpair> &in_coeffs, QVector<freqpair> &in_freqs);
    virtual ~AbstractFunction(){;}
    
    // CONST MEMBER FUNCTIONS
    int getNumTerms() { return terms; }
    virtual std::complex<double> bundle(double &x, double &y, unsigned int &i) const = 0;
    virtual std::complex<double> operator() (double i, double j) = 0;
    int getN(unsigned int &i) const;
    int getM(unsigned int &i) const;
    double getR(unsigned int &i) const;
    double getA(unsigned int &i) const;
    double getScaleR() const { return scale.R(); }
    double getScaleA() const { return scale.A(); }
    
    virtual AbstractFunction* clone() const = 0;
    
    // MUTATORS
    void setN(unsigned int &i, int &val);
    void setM(unsigned int &i, int &val);
    void setR(unsigned int &i, double &val);
    void setA(unsigned int &i, double &val);
    void setScaleR(double &val);
    void setScaleA(double &val);
    void setNumTerms(int &val);
    void removeTerm(unsigned int &i);
    void reset() {terms = 1; refresh();}
    void refresh();
    
protected:
    
    // DATA
    QVector<coeffpair> coeffs;
    QVector<freqpair> freqs;
    unsigned int terms;
    coeffpair scale;
    
    // PRIVATE MEMBER FUNCTIONS
    void initWithVectors(QVector<coeffpair> &in_coeffs, QVector<freqpair> &in_freqs);
};




////////////////////////////////////////////////////////////////

#define Kgen 1.6
#define Lgen 1
// hoping for top/bottom matches in fabric

//#define Xgen ( 2*pi*x ) original defaults
//#define Ygen ( 2*pi*(x/Kgen + y/Lgen) )

#define Xgen ( 2*pi*x+2*pi*y/q3 ) //for hex lattice without rotation
#define Ygen ( 4*pi*y/q3 )


class generalFunction : public AbstractFunction
{
public:
    generalFunction() { terms = 1; refresh(); }
    generalFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    generalFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new generalFunction(*this); };
    
};

////////////////////////////////////////////////////////////////

//#define Kgen 1.6
//#define Lgen 1
// locSym function uses a special lattice with qSqrt 7

//#define Xgen ( 2*pi*x ) original defaults
//#define Ygen ( 2*pi*(x/Kgen + y/Lgen) )

#define Xloc ( 2*pi*x-2*pi*y/q7 ) //for hex lattice without rotation
#define Yloc ( 4*pi*y/q7 )


class locSymFunction : public AbstractFunction
{
public:
    locSymFunction() { terms = 1; refresh(); }
    locSymFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    locSymFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new locSymFunction(*this); };
};
////////////////////////////////////////////////////////////////
class locSym2Function : public AbstractFunction
{
public:
    locSym2Function() { terms = 1; refresh(); }
    locSym2Function(unsigned int in_terms) { terms = in_terms; refresh(); }
    locSym2Function(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new locSym2Function(*this); };
};
////////////////////////////////////////////////////////////////
class locSymCTFunction : public AbstractFunction
{
public:
    locSymCTFunction() { terms = 1; refresh(); }
    locSymCTFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    locSymCTFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new locSymCTFunction(*this); };
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
    generalpairedFunction() { terms = 1; refresh(); }
    generalpairedFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    generalpairedFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new generalpairedFunction(*this); };
};

////////////////////////////////////////////////////////////////


#define Xhex3 (2*pi*x+2*pi*y/q3)
#define Yhex3 (4*pi*y/q3)

class hex3Function : public AbstractFunction
{
public:
    hex3Function() { terms = 1; refresh(); }
    hex3Function(unsigned int in_terms) { terms = in_terms; refresh(); }
    hex3Function(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new hex3Function(*this); };
    
    
};

////////////////////////////////////////////////////////////////


class p31mFunction : public AbstractFunction
{
public:
    p31mFunction() { terms = 1; refresh(); }
    p31mFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    p31mFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new p31mFunction(*this); };
    
    
};

////////////////////////////////////////////////////////////////


class p3m1Function : public AbstractFunction
{
public:
    p3m1Function() { terms = 1; refresh(); }
    p3m1Function(unsigned int in_terms) { terms = in_terms; refresh(); }
    p3m1Function(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new p3m1Function(*this); };
    
};
////////////////////////////////////////////////////////////////

class hex3CTFunction : public AbstractFunction
{
public:
    hex3CTFunction() { terms = 1; refresh(); }
    hex3CTFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    hex3CTFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new hex3CTFunction(*this); };
    
    
};
////////////////////////////////////////////////////////////////

#define Xhex6 (2*pi*x+2*pi*y/q3)
#define Yhex6 (4*pi*y/q3)

class hex6Function : public AbstractFunction
{
public:
    hex6Function() { terms = 1; refresh(); }
    hex6Function(unsigned int in_terms) { terms = in_terms; refresh(); }
    hex6Function(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new hex6Function(*this); };
    
    
};

////////////////////////////////////////////////////////////////

class p6mFunction : public AbstractFunction
{
public:
    p6mFunction() { terms = 1; refresh(); }
    p6mFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    p6mFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new p6mFunction(*this); };
    
    
};

////////////////////////////////////////////////////////////////

#define Krect 1
#define Lrect .5

#define Xrect ( (2*pi*x) / Krect)
#define Yrect ( (2*pi*y) / Lrect)

class rectangularFunction : public AbstractFunction
{
public:
    rectangularFunction() { terms = 1; refresh(); }
    rectangularFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    rectangularFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new rectangularFunction(*this); };
};
///////////////////////////
class pmFunction : public AbstractFunction
{
public:
    pmFunction() { terms = 1; refresh(); }
    pmFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    pmFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new pmFunction(*this); };
};
///////////////////////////
class pmmFunction : public AbstractFunction
{
public:
    pmmFunction() { terms = 1; refresh(); }
    pmmFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    pmmFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new pmmFunction(*this); };
};

///////////////////////////
class pggFunction : public AbstractFunction
{
public:
    pggFunction() { terms = 1; refresh(); }
    pggFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    pggFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new pggFunction(*this); };
};
///////////////////////////
class pmgFunction : public AbstractFunction
{
public:
    pmgFunction() { terms = 1; refresh(); }
    pmgFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    pmgFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new pmgFunction(*this); };
};
///////////////////////////
class pgFunction : public AbstractFunction
{
public:
    pgFunction() { terms = 1; refresh(); }
    pgFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    pgFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new pgFunction(*this); };
};
////////////////////////////////////////////////////////////////
class pmgpgFunction : public AbstractFunction
{
public:
    pmgpgFunction() { terms = 1; refresh(); }
    pmgpgFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    pmgpgFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new pmgpgFunction(*this); };
};

////////////////////////////////////////////////////////////////
#define Krect2 1.6
#define Lrect2 1

#define Xrect2 ( (2*pi*x) / Krect2)
#define Yrect2 ( (2*pi*y) / Lrect2)

class rectangularpairedFunction : public AbstractFunction
{
public:
    rectangularpairedFunction() { terms = 1; refresh(); }
    rectangularpairedFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    rectangularpairedFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new rectangularpairedFunction(*this); };
};

////////////////////////////////////////////////////////////////
//Note: Switched x and y to make vertical stripes 7/31/15

#define Krhombic 1.6
#define Lrhombic 1

#define Xrhombic (pi * (y/Krhombic + x/Lrhombic) )
#define Yrhombic (pi * (y/Krhombic - x/Lrhombic) )

class rhombicFunction : public AbstractFunction
{
public:
    rhombicFunction() { terms = 1; refresh(); }
    rhombicFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    rhombicFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new rhombicFunction(*this); };
    
    
};

////////////////////////////////////////////////////////////////

#define Krhombic2 1.6
#define Lrhombic2 1

#define Xrhombic2 (pi * (x/Krhombic2 + y/Lrhombic2) )
#define Yrhombic2 (pi * (x/Krhombic2 - y/Lrhombic2) )

class rhombicpairedFunction : public AbstractFunction
{
public:
    rhombicpairedFunction() { terms = 1; refresh(); }
    rhombicpairedFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    rhombicpairedFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new rhombicpairedFunction(*this); };
    
};

////////////////////////////////////////////////////////////////
class cmmFunction : public AbstractFunction
{
public:
    cmmFunction() { terms = 1; refresh(); }
    cmmFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    cmmFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new cmmFunction(*this); };
    
};

////////////////////////////////////////////////////////////////

#define Xsquare (2*pi*x)
#define Ysquare (2*pi*y)

class squareFunction : public AbstractFunction
{
public:
    squareFunction() { terms = 1; refresh(); }
    squareFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    squareFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new squareFunction(*this); };
    
    
};
/////////////////////////////////
////////////////////////////////////////////////////////////////
class p4mFunction : public AbstractFunction
{
public:
    p4mFunction() { terms = 1; refresh(); }
    p4mFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    p4mFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new p4mFunction(*this); };
    
    
};

////////////////////////////////////////////////////////////////
class p4gFunction : public AbstractFunction
{
public:
    p4gFunction() { terms = 1; refresh(); }
    p4gFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    p4gFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new p4gFunction(*this); };
    
    
};
///////////////////////////////////////////////////////////////


class squareMFunction : public AbstractFunction
{
public:
    squareMFunction() { terms = 1; refresh(); }
    squareMFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    squareMFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new squareMFunction(*this); };
    
    
};
///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////



class squareTFunction : public AbstractFunction
{
public:
    squareTFunction() { terms = 1; refresh(); }
    squareTFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    squareTFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new squareTFunction(*this); };
    
    
};
///////////////////////////////////////////////////////////////

class holoFunction : public AbstractFunction
{
public:
    holoFunction() { terms = 1; refresh(); }
    holoFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    holoFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new holoFunction(*this); };
    
};

///////////////////////////////////////////////////////////////

class contFunction : public AbstractFunction
{
public:
    contFunction() { terms = 1; refresh(); }
    contFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    contFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new contFunction(*this); };
    
};
///////////////////////////////////////////////////////////////

class zzbarFunction : public AbstractFunction
{
public:
    zzbarFunction() { terms = 1; refresh(); }
    zzbarFunction(unsigned int in_terms) { terms = in_terms; refresh(); }
    zzbarFunction(QVector<coeffpair> in_coeffs, QVector<freqpair> in_freqs) {initWithVectors(in_coeffs, in_freqs);}
    
    std::complex<double> bundle(double &x, double &y, unsigned int &i) const;
    std::complex<double> operator() (double i, double j);
    
    virtual AbstractFunction* clone() const { return new zzbarFunction(*this); };
    
};

///////////////////////////////////////////////////////////////

#endif // FUNCTIONS_H
