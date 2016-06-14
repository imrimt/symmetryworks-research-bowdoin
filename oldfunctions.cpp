#include "functions.h"

AbstractFunction::AbstractFunction(QVector<coeffpair> &in_coeffs, QVector<freqpair> &in_freqs)
{
    initWithVectors(in_coeffs, in_freqs);
}

int AbstractFunction::N(unsigned int &i) const
{
    if(i < terms)
        return freqs[i].N();
    else
    {
        qDebug() << "AbstractFunction.cpp error: freqs[" << i << "] is out of bounds";
        return 0;
    }
}

int AbstractFunction::M(unsigned int &i) const
{
    if(i < terms)
        return freqs[i].M();
    else
    {
        qDebug() << "AbstractFunction.cpp error: freqs[" << i << "] is out of bounds";
        return 0;
    }
}

double AbstractFunction::R(unsigned int &i) const
{
    if(i < terms)
        return coeffs[i].R();
    else
    {
        qDebug() << "AbstractFunction.cpp error: coeffs[" << i << "] is out of bounds";
        return 0.0;
    }
}

double AbstractFunction::A(unsigned int &i) const
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
    if(val > 0 && val <= 99 && val != (int) terms)
    {
        terms = val;
        coeffs.resize(terms);
        freqs.resize(terms);
    }
}

void AbstractFunction::refresh()
{
    coeffs.clear();
    coeffs.fill(coeffpair::coeffpair(), terms);

    freqs.clear();
    freqs.fill(freqpair::freqpair(), terms);

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
    int N,M;
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

std::complex<double> rhombicFunction::bundle(double &x, double &y, unsigned int &i) const
{
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
    std::complex<double> part1 = ei(N*Xrhombic + M*Yrhombic);

    return part1;

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
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
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
    int N,M;
    N = freqs[i].N();
    M = freqs[i].M();
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

