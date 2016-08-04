////////////////
//Cameron Wong//
//Frank Farris//
//  07-12-11  //
////////////////
// pairs.h
//
// inline class defintions for our coeff- and freq- pair classes.
// our coefficient constants always come in r,a pairs, and our
// frequency constants always come in n,m pairs; these
// classes allows us to index our storage vectors
// by pairs (instead of by each individual constant).

#ifndef PAIRS_H
#define PAIRS_H

#include <complex>
#include "geomath.h"

class coeffpair
{
public:
    
    // CONSTRUCTORS
    coeffpair()
    {
        r = 1.0;
        a = 0.0;
    }
    coeffpair(double rval, double aval)
    {
        r = rval;
        a = aval;
    }
    
    // CONST-MEMBER FUNCTIONS
    double R() const {return r;}
    double A() const {return a;}
    std::complex<double> combined()
    {
        std::complex<double> ans = ei(a);
        return ans * r;
    }
    
    // MUTATORS
    void setR(double val) {r = val;}
    void setA(double val) {a = val;}
    
    
private:
    
    // DATA
    double r;
    double a;
};


/////////////////////////////////////////////////////

class freqpair
{
public:
    
    // CONSTRUCTORS
    freqpair()
    {
        n = 1;
        m = 0;
    }
    freqpair(int nval, int mval)
    {
        n = nval;
        m = mval;
    }
    
    // CONST-MEMBER FUNCTIONS
    int N() const {return n;}
    int M() const {return m;}
    
    // MUTATORS
    void setN(int val) {n = val;}
    void setM(int val) {m = val;}
    
    
private:
    
    // DATA
    int n;
    int m;
};

#endif // PAIRS_H
