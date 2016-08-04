#include "functions.h"

AbstractFunction::AbstractFunction(QVector<coeffpair> &in_coeffs, QVector<freqpair> &in_freqs)
{
    terms = 99;
    initWithVectors(in_coeffs, in_freqs);
}

int AbstractFunction::getN(unsigned int &i) const
{
    if(i < terms)
        return freqs[i].N();
    else
    {
        qDebug() << "AbstractFunction.cpp error: freqs[" << i << "] is out of bounds";
        return 0;
    }
}

int AbstractFunction::getM(unsigned int &i) const
{
    if(i < terms)
        return freqs[i].M();
    else
    {
        qDebug() << "AbstractFunction.cpp error: freqs[" << i << "] is out of bounds";
        return 0;
    }
}

double AbstractFunction::getR(unsigned int &i) const
{
    if(i < terms)
        return coeffs[i].R();
    else
    {
        qDebug() << "AbstractFunction.cpp error: coeffs[" << i << "] is out of bounds";
        return 0.0;
    }
}

double AbstractFunction::getA(unsigned int &i) const
{
    if(i < terms)
        return coeffs[i].A();
    else
    {
        qDebug() << "AbstractFunction.cpp error: coeffs[" << i << "] is out of bounds";
        return 0.0;
    }
}

void AbstractFunction::setN(unsigned int &i, int &val)
{
    if(i < terms)
        freqs[i].setN(val);
    else
        qDebug() << "AbstractFunction.cpp error: freqs[" << i << "] is out of bounds";
}

void AbstractFunction::setM(unsigned int &i, int &val)
{
    if(i < terms)
        freqs[i].setM(val);
    else
        qDebug() << "AbstractFunction.cpp error: freqs[" << i << "] is out of bounds";
}

void AbstractFunction::setR(unsigned int &i, double &val)
{
    if(i < terms)
        coeffs[i].setR(val);
    else
        qDebug() << "AbstractFunction.cpp error: coeffs[" << i << "] is out of bounds";
}

void AbstractFunction::setA(unsigned int &i, double &val)
{
    if(i < terms)
        coeffs[i].setA(val);
    else
        qDebug() << "AbstractFunction.cpp error: coeffs[" << i << "] is out of bounds";
}

void AbstractFunction::setScaleR(double &val)
{
    scale.setR(val);
}

void AbstractFunction::setScaleA(double &val)
{
    scale.setA(val);
}

void AbstractFunction::setNumTerms(int &val)
{
    
    if(val > 0 && val <= 99 && val != (int) terms) {
        terms = val;
        coeffs.resize(terms);
        freqs.resize(terms);
    }
}

void AbstractFunction::removeTerm(unsigned int &i)
{
    if (coeffs.size() >= 1 && freqs.size() >= 1) {
        int it = i;
        
        coeffs.erase(coeffs.begin() + it);
        freqs.erase(freqs.begin() + it);
        
        terms--;
    }
    
}

void AbstractFunction::refresh()
{
    
    coeffs.clear();
    coeffs.fill(coeffpair(), terms);
    
    freqs.clear();
    freqs.fill(freqpair(), terms);
    
    scale.setR(1.0);
    scale.setA(0.0);
}

void AbstractFunction::initWithVectors(QVector<coeffpair> &in_coeffs, QVector<freqpair> &in_freqs)
{
    terms = in_coeffs.size() < in_freqs.size() ? in_coeffs.size() : in_freqs.size();
    
    if(in_coeffs.size() > (int) terms)
        in_coeffs.resize(terms);
    else if(in_freqs.size() > (int) terms)
        in_freqs.resize(terms);
    
    coeffs = in_coeffs;
    freqs = in_freqs;
    scale.setR(1.0);
    scale.setA(0.0);
}


////////////////////////////////////////////////////////////

std::complex<double> generalFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N, M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xgen + M*Ygen);
    
    return part1;
}

std::complex<double> generalFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> locSymFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    std::complex<double> part1;
    N = freqs[i].N();
    M = freqs[i].M();
    if(N==0 and M==0)
        part1 = qCos(Xloc) * qCos(Yloc);
    else
        part1 = (ei((2*N+M)*Xloc + (-2*M)*Yloc)+ ei((2*M+N)*Xloc + (-2*N)*Yloc))/2.0;
    
    return part1;
}

std::complex<double> locSymFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> locSym2Function::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    std::complex<double> part1;
    N = freqs[i].N();
    M = freqs[i].M();
    if(N==0 and M==0){part1=qCos(Xloc)*qCos(Yloc);}else{
        part1 = (ei((2*N+M)*Xloc + (-2*M)*Yloc)+ ei((2*M+N)*Xloc + (-2*N)*Yloc)+ei((-2*N-M)*Xloc + (2*M)*Yloc)+ ei((-2*M-N)*Xloc + (2*N)*Yloc))/4.0;};
    
    return part1;
}

std::complex<double> locSym2Function::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> locSymCTFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    std::complex<double> part1;
    N = freqs[i].N();
    M = freqs[i].M();
    
    part1 = (ei((2*N+M+1)*Xloc + (-2*M+1)*Yloc));
    
    return part1;
}

std::complex<double> locSymCTFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k = 0; k < terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}
////////////////////////////////////////////////////////////

std::complex<double> generalpairedFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xgen2 + M*Ygen2);
    std::complex<double> part2 = ei(-N*Xgen2 - M*Ygen2);
    
    return (part1 + part2) / 2.0;
    
}

std::complex<double> generalpairedFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k = 0; k < terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans += thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> hex3Function::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xhex3 + M*Yhex3);
    std::complex<double> part2 = ei((M)*Xhex3 - (N+M)*Yhex3);
    std::complex<double> part3 = ei(-(N+M)*Xhex3 + (N)*Yhex3);
    
    return (part1 + part2 + part3)/3.0;
    
}

std::complex<double> hex3Function::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    
    for(unsigned int k = 0; k < terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans += thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> p31mFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xhex3 + M*Yhex3)+ei(M*Xhex3 + N*Yhex3);
    std::complex<double> part2 = ei((M)*Xhex3 - (N+M)*Yhex3)+ei(-(N+M)*Xhex3 + M*Yhex3);
    std::complex<double> part3 = ei(-(N+M)*Xhex3 + (N)*Yhex3)+ei(N*Xhex3 - (N+M)*Yhex3);
    
    return (part1 + part2 + part3)/6.0;
    
}

std::complex<double> p31mFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}
////////////////////////////////////////////////////////////

std::complex<double> p3m1Function::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xhex3 + M*Yhex3)+ei(-M*Xhex3 - N*Yhex3);
    std::complex<double> part2 = ei((M)*Xhex3 - (N+M)*Yhex3)+ei((N+M)*Xhex3 - (M)*Yhex3);
    std::complex<double> part3 = ei(-(N+M)*Xhex3 + (N)*Yhex3)+ei(-(N)*Xhex3 + (N+M)*Yhex3);
    
    return (part1 + part2 + part3)/6.0;
    
}

std::complex<double> p3m1Function::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> hex3CTFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xhex3 + M*Yhex3);
    std::complex<double> part2 = ei((M)*Xhex3 - (N+M)*Yhex3-2.0*pi/3.0);
    std::complex<double> part3 = ei(-(N+M)*Xhex3 + (N)*Yhex3-4.0*pi/3.0);
    
    return (part1 + part2 + part3)/3.0;
    
}

std::complex<double> hex3CTFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////


std::complex<double> hex6Function::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = qCos(N*Xhex6 + M*Yhex6);
    std::complex<double> part2 = qCos((M)*Xhex6 - (N+M)*Yhex6);
    std::complex<double> part3 = qCos(-(N+M)*Xhex6 + (N)*Yhex6);
    return (part1 + part2 + part3)/3.0;
    
}

std::complex<double> hex6Function::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

///////////////////////////

std::complex<double> p6mFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = qCos(N*Xhex6 + M*Yhex6);
    std::complex<double> part2 = qCos((M)*Xhex6 - (N+M)*Yhex6);
    std::complex<double> part3 = qCos(-(N+M)*Xhex6 + (N)*Yhex6);
    std::complex<double> part4 = qCos(M*Xhex6 +N*Yhex6);
    std::complex<double> part5 = qCos((N)*Xhex6 - (N+M)*Yhex6);
    std::complex<double> part6 = qCos(-(N+M)*Xhex6 + (M)*Yhex6);
    return (part1 + part2 + part3+part4 + part5 + part6)/6.0;
    
}

std::complex<double> p6mFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> rectangularFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xrect + M*Yrect);
    
    return part1;
}

std::complex<double> rectangularFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> pmFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = (ei(N*Xrect + M*Yrect)+ei(-N*Xrect + M*Yrect))/2.0;
    
    return part1;
}

std::complex<double> pmFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> pmmFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = (ei(N*Xrect + M*Yrect)+ei(-N*Xrect + M*Yrect))/4.0;
    std::complex<double> part2 = (ei(-N*Xrect - M*Yrect)+ei(N*Xrect - M*Yrect))/4.0;
    return part1+part2;
}

std::complex<double> pmmFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> pggFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M, nega;
    N = freqs[i].N();
    M = freqs[i].M();
    nega = -1;
    if((N+M) % 2==0){nega=1;};
    std::complex<double> part1 = (ei(N*Xrect + M*Yrect)+ei(-N*Xrect -M*Yrect))/4.0;
    std::complex<double> part2 = (ei(-N*Xrect + M*Yrect)+ei(N*Xrect - M*Yrect))/4.0;
    part2 *=nega;
    return part1+part2;
}

std::complex<double> pggFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> pmgFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M, nega;
    N = freqs[i].N();
    M = freqs[i].M();
    nega = -1;
    if((M) % 2==0){nega=1;};
    std::complex<double> part1 = (ei(N*Xrect + M*Yrect)+ei(-N*Xrect -M*Yrect))/4.0;
    std::complex<double> part2 = (ei(-N*Xrect + M*Yrect)+ei(N*Xrect - M*Yrect))/4.0;
    part2 *=nega;
    return part1+part2;
}

std::complex<double> pmgFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> pgFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M, parity;
    N = freqs[i].N();
    M = freqs[i].M();
    parity = M%2;
    std::complex<double> part1 = (ei(N*Xrect + M*Yrect)+pow(-1,parity)*ei(-N*Xrect + M*Yrect))/2.0;
    
    return part1;
}

std::complex<double> pgFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> pmgpgFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M,nega;
    N = freqs[i].N();
    M = freqs[i].M();
    nega = -1;
    if(M % 2==0){nega=1;};
    std::complex<double> part1 = ei(N*Xrect + M*Yrect);
    std::complex<double> part2 = ei(-N*Xrect - M*Yrect);
    std::complex<double> part3 = ei(-N*Xrect + M*Yrect);
    std::complex<double> part4 = ei(N*Xrect - M*Yrect);
    part3 *=nega;
    part4 *=nega;
    return (part1-part2+ (part3)-(part4))/ 4.0;
}
//Note: as a hack, I made part2 and part4 positive to create a pmg fcn.Changed back9/9/13
std::complex<double> pmgpgFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> rectangularpairedFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xrect2 + M*Yrect2);
    std::complex<double> part2 = ei(-N*Xrect2 - M*Yrect2);
    
    return (part1 + part2) / 2.0;
    
}

std::complex<double> rectangularpairedFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////
//Note: Original rhombic function had no mirrors turned on. This is now a cm function. And I've switched to vertical stripes

std::complex<double> rhombicFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xrhombic + M*Yrhombic);
    std::complex<double> part2 = ei(M*Xrhombic + N*Yrhombic);
    return (part1+part2)/2.0;
    
}

std::complex<double> rhombicFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> rhombicpairedFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xrhombic2 + M*Yrhombic2);
    std::complex<double> part2 = ei(-N*Xrhombic2 - M*Yrhombic2);
    
    return (part1 + part2) / 2.0;
    
}

std::complex<double> rhombicpairedFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}
////////////////////////////////////////////////////////////

std::complex<double> cmmFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xrhombic2 + M*Yrhombic2)+ei(M*Xrhombic2 + N*Yrhombic2);
    std::complex<double> part2 = ei(-N*Xrhombic2 - M*Yrhombic2)+ei(-M*Xrhombic2 - N*Yrhombic2);
    
    return (part1 + part2) / 4.0;
    
}

std::complex<double> cmmFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}
////////////////////////////////////////////////////////////

std::complex<double> squareFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xsquare + M*Ysquare);
    std::complex<double> part2 = ei(-M*Xsquare + N*Ysquare);
    std::complex<double> part3 = ei(-N*Xsquare - M*Ysquare);
    std::complex<double> part4 = ei(M*Xsquare - N*Ysquare);
    
    
    return (part1 + part2 + part3 + part4)/4.0;
    
}

std::complex<double> squareFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}
////////////////////////////////////////////////////////////

std::complex<double> p4mFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xsquare + M*Ysquare);
    std::complex<double> part2 = ei(-M*Xsquare + N*Ysquare);
    std::complex<double> part3 = ei(-N*Xsquare - M*Ysquare);
    std::complex<double> part4 = ei(M*Xsquare - N*Ysquare);
    
    std::complex<double> part5 = ei(M*Xsquare + N*Ysquare);
    std::complex<double> part6 = ei(-N*Xsquare + M*Ysquare);
    std::complex<double> part7 = ei(-M*Xsquare - N*Ysquare);
    std::complex<double> part8 = ei(N*Xsquare - M*Ysquare);
    return (part1 + part2 + part3 + part4+part5 + part6 + part7 + part8)/4.0;
    
}

std::complex<double>  p4mFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> p4gFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    double G=pow(-1.0,N+M);
    std::complex<double> part1 = ei(N*Xsquare + M*Ysquare);
    std::complex<double> part2 = ei(-M*Xsquare + N*Ysquare);
    std::complex<double> part3 = ei(-N*Xsquare - M*Ysquare);
    std::complex<double> part4 = ei(M*Xsquare - N*Ysquare);
    
    std::complex<double> part5 = G*ei(M*Xsquare + N*Ysquare);
    std::complex<double> part6 = G*ei(-N*Xsquare + M*Ysquare);
    std::complex<double> part7 = G*ei(-M*Xsquare - N*Ysquare);
    std::complex<double> part8 = G*ei(N*Xsquare - M*Ysquare);
    return (part1 + part2 + part3 + part4+part5 + part6 + part7 + part8)/4.0;
    
}

std::complex<double>  p4gFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> squareMFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xsquare + M*Ysquare);
    std::complex<double> part2 = ei(-M*Xsquare + N*Ysquare);
    std::complex<double> part3 = ei(-N*Xsquare - M*Ysquare);
    std::complex<double> part4 = ei(M*Xsquare - N*Ysquare);
    
    
    return (part1 - part2 + part3 - part4)/4.0;
    
}

std::complex<double> squareMFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

std::complex<double> squareTFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xsquare + M*Ysquare);
    std::complex<double> part2 = ei(-M*Xsquare + N*Ysquare+3.0*pi/2.0);
    std::complex<double> part3 = ei(-N*Xsquare - M*Ysquare+pi);
    std::complex<double> part4 = ei(M*Xsquare - N*Ysquare+ pi/2.0);
    
    
    return (part1 + part2 + part3 + part4)/4.0;
    
}

std::complex<double> squareTFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    
    ans *= scale.combined();
    return ans;
}
////////////////////////////////////////////////////////////

std::complex<double> holoFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N;
    N = freqs[i].N();
    std::complex<double> ans(x , y);
    ans=pow(ans,N);
    return ans;
    
}

std::complex<double> holoFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    ans *= scale.combined();
    return ans;
}


////////////////////////////////////////////////////////////

std::complex<double> contFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N;
    N = freqs[i].N();
    std::complex<double> ans(x , y);
    if (N>0) {ans=pow(ans,N);} else
    {ans=conj(ans);ans=pow(ans,0-N);};
    return ans;
    
}

std::complex<double> contFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    ans *= scale.combined();
    return ans;
}

////////////////////////////////////////////////////////////

std::complex<double> zzbarFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> ans(x , y);
    std::complex<double> ans2(x , y);
    ans=pow(ans,N);
    ans*=conj(pow(ans2,M));
    return ans;
    
}

std::complex<double> zzbarFunction::operator ()(double i, double j)
{
    std::complex<double> ans(0,0);
    for(unsigned int k=0; k<terms; k++)
    {
        std::complex<double> thisterm = bundle(i, j, k);
        thisterm *= coeffs[k].combined();
        ans+= thisterm;
    }
    ans *= scale.combined();
    return ans;
}

