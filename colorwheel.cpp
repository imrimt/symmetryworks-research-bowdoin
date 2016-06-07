#include "colorwheel.h"

ColorWheel::ColorWheel(QObject *parent) :
    QObject(parent)
{
    currentSel = 0;
    image = QImage(image_dim, image_dim, QImage::Format_RGB32);
    image.fill(255);
}

QRgb ColorWheel::operator() (std::complex<double> zin)
{
    QRgb col;

    switch(currentSel)
    {
    case 0:
        col=IcosColor(zin);
        break;
    case 1:
        col=IcosColorC(zin);
        break;
    case 2:
        col=StCol(zin);
        break;
    case 3:
        col=StColC(zin);
        break;
    case 4:
        col=StCol35(zin);
        break;
    case 5:
        col=ZoneCol(zin);
        break;
    case 6:
        col=SectCol(zin);
        break;
    case 7:
        col=Sect6Col(zin);
        break;
    case 8:
        col=WinCol(zin);
        break;
    case 9:
        col=FromImage(zin);
        break;
    }

    return col;
}

void ColorWheel::loadImage(QString filename)
{
    QImage raw(filename);
    image = raw.scaled(image_dim, image_dim, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QRgb ColorWheel::IcosColor(std::complex<double> zin)
{
    v3 V;
    int Tag,n;
    double test,compare;

    v3 icosFaces[] =
    {   initV3(1.0/q3,1.0/q3,1.0/q3),
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
        initV3(-1.0/q3,-1.0/q3,-1.0/q3) };

    V=tilt(stereo(zin));
    test=0.0;
    for(n=0; n<20; n++)
    {
       compare=dp(V,icosFaces[n]);
       if(compare > test)
       {
           Tag=n;
           test=compare;
       }
    }

    RGB out = Colors(icosFaces[Tag]);
    return toQRgb(out);

}

QRgb ColorWheel::IcosColorC(std::complex<double> zin)
{
    v3 V;
    int Tag,n;
    double test,compare;

    v3 icosFaces[] =
    {   initV3(1.0/q3,1.0/q3,1.0/q3),
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
        initV3(-1.0/q3,-1.0/q3,-1.0/q3) };

    V=tilt(stereo(zin));
    test=0.0;
    for(n=0;n<20;n++)
    {
       compare=dp(V,icosFaces[n]);
       if(compare > test)
       {
           Tag=n;
           test=compare;
       }
    }

    RGB out = Colors(crV(icosFaces[Tag]));
    return toQRgb(out);

}

QRgb ColorWheel::StCol(std::complex<double> zin)
{
    v3 V;
    V = tilt(stereo(zin));

    RGB out = Colors(V);
    return toQRgb(out);

}

QRgb ColorWheel::StColC(std::complex<double> zin)
{
    v3 V;
    V = crV(tilt(stereo(zin)));

    RGB out = Colors(V);
    return toQRgb(out);
}

QRgb ColorWheel::StCol35(std::complex<double> zin)
{
    v3 V;
    V = r35V(tilt(stereo(zin)));

    RGB out = Colors(V);
    return toQRgb(out);
}

QRgb ColorWheel::ZoneCol(std::complex<double> zin)
{
    v3 V;
    int Tag,i,j,n,m,znum;
    double test,compare;

    m=6;
    znum=2+6*(m-1);
    v3 zoneVect[znum];

    //initial zone vectors
    zoneVect[0]=tilt(initV3(0.0,0.0,1.0));
    zoneVect[1]=tilt(initV3(0.0,0.0,-1.0));

    for(i=1; i<m; i++)
    {
        for(j=0; j<6; j++)
        {
            zoneVect[2+j+6*(i-1)] = tilt(initV3(cos(2.0*pi*float(j)/6.0)*sin(pi*i/m),sin(2.0*pi*j/6.0)*sin(pi*i/m),cos(pi*i/m)));
        }
    }

    V=tilt(stereo(zin));
    test=0.0;
    Tag=0;

    for(n=0; n<znum; n++)
    {
       compare=dp(V,zoneVect[n]);
       if(compare>test)
       {
           Tag=n;
           test=compare;
       }
    }

    RGB out = Colors(crV(zoneVect[Tag]));
    return toQRgb(out);
}

QRgb ColorWheel::SectCol(std::complex<double> zin)
{

    int S1,r,g,b;
    double ra;
    double x = zin.real();
    double y = -zin.imag();

    ra = sqrt(x*x+y*y);

    if(ra<0.25)
    {
        r=255; g=255; b=255;
    }
    else
    {
        if(ra>2.0)
        {
            r=0;g=0;b=0;
        }
        else
        {
            if(y>0.0)
            {
                if(y<0.72654*x)
                {S1=1;}
                else
                {
                    if(y<-3.077*x)
                    {
                        S1=3;
                    }
                    else
                    {
                        S1=2;
                    }
                }
            }
            else
            {
                if(y>-0.72654*x)
                {
                    S1=1;
                }
                else
                {
                    if(y>3.077*x)
                    {
                        S1=4;
                    }
                    else
                    {
                        S1=5;
                    }
                }
            }
            switch(S1)
            {
            case 1:
                r=int(255-255*ra/2.0);
                g=0;
                b=0;
                break;
            case 2:
                r=int(255-255*ra/2.0);
                g=int(255-255*ra/2.0);
                b=0;
                break;
            case 3:
                r=0;
                g=int(255-255*ra/2.0);
                b=0;
                break;
            case 4:
                r=0;
                g=0;
                b=int(255-255*ra/2.0);
                break;
            case 5:
                r=int(255-255*ra/2.0);
                g=0;
                b=int(255-255*ra/2.0);
                break;
            }
        }
   }

    RGB out = initRGB(r,g,b);
    return toQRgb(out);
}

QRgb ColorWheel::Sect6Col(std::complex<double> zin)
{
    int S1,S2,r,g,b;
    double ra;
    double x = zin.real();
    double y = zin.imag();

    ra = sqrt(x*x+y*y);

    if(ra<0.25)
    {
        r=255;g=255;b=255;
    }
    else
    {
        if(ra>2.0)
        {
            r=0;g=0;b=0;
        }
        else
        {
            if (x > 0.0)
            {
                if(y < -x/1.732)
                {
                    S1=6;
                }
                else
                {
                    if(y > x/1.732)
                    {
                        S1=2;
                    }
                    else
                    {
                        S1=1;
                    }
                }
            }
            else
            {
                if(y<x/1.732)
                {
                    S1=5;
                }
                else
                {
                    if(y>-x/1.732)
                    {
                        S1=3;
                    }
                    else
                    {
                        S1=4;
                    }
                }
            }
            if(ra<0.75)
            {
                S2=1;
            }
            else
            {
                if(ra<1.25)
                {
                    S2=2;
                }
                else
                {
                    S2=3;
                }
            }
            switch(S2)
            {
            case 1:
                switch(S1)
                {
                case 1:
                    r=int(255);
                    g=int(255*(1.5-2*ra));
                    b=int(255*(1.5-2*ra));
                    break;
                case 2:
                    r=int(255);
                    g=int(255);
                    b=int(255*(1.5-2*ra));
                    break;
                case 3:
                    r=int(255*(1.5-2*ra));
                    g=255;
                    b=int(255*(1.5-2*ra));
                    break;
                case 4:
                    r=int(255*(1.5-2*ra));
                    g=int(255);
                    b=int(255);
                    break;
                case 5:
                    r=int(255*(1.5-2*ra));
                    g=int(255*(1.5-2*ra));
                    b=int(255);
                    break;
                case 6:
                    r=int(255);
                    g=int(255*(1.5-2*ra));
                    b=int(255);
                    break;
                }
                break;
            case 2:
                switch(S1)
                {
                case 1:
                    r=int(255);
                    g=int(0);
                    b=int(0);
                    break;
                case 2:
                    r=int(255);
                    g=int(255);
                    b=0;
                    break;
                case 3:
                    r=0;
                    g=255;
                    b=0;
                    break;
                case 4:
                    r=0;
                    g=255;
                    b=255;
                    break;
                case 5:
                    r=0;
                    g=0;
                    b=255;
                    break;
                case 6:
                    r=255;
                    g=0;
                    b=255;
                    break;
                }
                break;
            case 3:
                switch(S1)
                {
                case 1:
                    r=int(255-255*ra/2.0);
                    g=0;
                    b=0;
                    break;
                case 2:
                    r=int(255-255*ra/2.0);
                    g=int(255-255*ra/2.0);
                    b=0;
                    break;
                case 3:
                    r=0;
                    g=int(255-255*ra/2.0);
                    b=0;
                    break;
                case 4:
                    r=0;
                    g=int(255-255*ra/2.0);
                    b=int(255-255*ra/2.0);
                    break;
                case 5:
                    r=0;
                    g=0;
                    b=int(255-255*ra/2.0);
                    break;
                case 6:
                    r=int(255-255*ra/2.0);
                    g=0;
                    b=int(255-255*ra/2.0);
                    break;
                }
                break;
            }
        }
    }

    RGB out = initRGB(r,g,b);
    return toQRgb(out);
}

QRgb ColorWheel::WinCol(std::complex<double> zin)
{
      double X,Y,Z,xa,ya,xtm,ytm,ztm,Xf,Yf,Zf;
      vect E1,E2,E3, B1,B2;
      int Rv,Gv,Bv;

      E1=initVect(1.0/ma,c2/ma,c4/ma,c6/ma,c8/ma);
      E2=initVect(0.0,s2/ma,s4/ma,s6/ma,s8/ma);
      E3=initVect(1.0/sqrt(5.0),1.0/sqrt(5.0),1.0/sqrt(5.0),1.0/sqrt(5.0),1.0/sqrt(5.0));

      xa=zin.real();
      ya=zin.imag();

      //1st do stereographic proj
      xtm=2.0*xa/(1.0+xa*xa+ya*ya);
      ytm=2.0*ya/(1.0+xa*xa+ya*ya);
      ztm= 2.0/(1.0+xa*xa+ya*ya) -1.0;

      //put sphere up in R5
      B1=Bvect(xtm,ytm,ztm);

      //take cube roots
      B2=initVect(crt(B1.xv),crt(B1.yv),crt(B1.zv),crt(B1.uv),crt(B1.vv));
      Xf=B2.xv*E1.xv+B2.yv*E1.yv+B2.zv*E1.zv+B2.uv*E1.uv+B2.vv*E1.vv;
      Yf=B2.xv*E2.xv+B2.yv*E2.yv+B2.zv*E2.zv+B2.uv*E2.uv+B2.vv*E2.vv;
      Zf=B2.xv*E3.xv+B2.yv*E3.yv+B2.zv*E3.zv+B2.uv*E3.uv+B2.vv*E3.vv;

      //following accomplishes "tilt," but it was already written out, so fine.
      X=(Xf*q2+Zf)/q3;
      Z=0.0-Xf/q2/q3-Yf/q2+Zf/q3;
      Y=0.0-Xf/q2/q3+Yf/q2+Zf/q3;
      Rv = int(255.0*(1.0+X)/2.0);  if(Rv>255){Rv=255;}
      Gv = int(255.0*(1.0+Z)/2.0);  if(Gv>255){Gv=255;}
      Bv= int(255.0*(1.0+Y)/2.0);   if(Bv>255){Bv=255;}

      RGB out = initRGB(Rv,Gv,Bv);
      return toQRgb(out);

}

QRgb ColorWheel::FromImage(std::complex<double> zin)
{
    double x = zin.real();
    double y = zin.imag();
    QRgb color;

    if(x >= -2.0 && x < 2.0 && y >= -2.0 && y < 2.0)      //our image is defined within the Cartesian coordinates
    {                                                       // -2 <= x <= 2  and -2 <= y <= 2
        int translated_x = (int) ((x + 2.0) * (image_dim / 4.0));
        int translated_y = (int) image_dim - ((y + 2.0) * (image_dim / 4.0));

        color = image.pixel(translated_x, translated_y);
    }
    else
        //color = qRgb(210,231,255);
        color = qRgb(0,0,0);

    return color;
}

v3 ColorWheel::initV3(double x1, double y1, double z1)
{
    v3 out;
    out.xv = x1;
    out.yv = y1;
    out.zv = z1;
    return out;
}

v3 ColorWheel::tilt(v3 Vin)
{
    v3 out;
    out = initV3((q2*Vin.xv+Vin.zv)/q3,-Vin.xv/q2/q3+Vin.yv/q2+Vin.zv/q3,-Vin.xv/q2/q3-Vin.yv/q2+Vin.zv/q3);
    return out;
}

v3 ColorWheel::stereo(std::complex<double> Zin)
{
    double r2;
    double x = Zin.real();
    double y = Zin.imag();

    v3 out;
    r2 = 1+x*x+y*y;
    out = initV3(2.0*x/r2,2.0*y/r2,2.0/r2-1.0);
    return out;
}

v3 ColorWheel::crV(v3 Vin)  //cube root of vector entries
{
    v3 out;
    out = initV3(crt(Vin.xv),crt(Vin.yv),crt(Vin.zv));
    return out;
}

v3 ColorWheel::r35V(v3 Vin) // 3/5ths power of vector entries
{
    v3 out;
    out = initV3(r35(Vin.xv),r35(Vin.yv),r35(Vin.zv));
    return out;
}

vect ColorWheel::initVect(double x1, double y1, double z1, double u1, double v1)
{
    vect out;
    out.xv=x1;
    out.yv=y1;
    out.zv=z1;
    out.uv=u1;
    out.vv=v1;
    return out;
}

vect ColorWheel::Bvect(double a, double b, double c)
{
    vect out;
    double x5,y5,z5,u5,v5;
    x5=a/ma+c/sqrt(5.0);
    y5=a*c2/ma+b*s2/ma+c/sqrt(5.0);
    z5=a*c4/ma+b*s4/ma+c/sqrt(5.0);
    u5=a*c6/ma+b*s6/ma+c/sqrt(5.0);
    v5=a*c8/ma+b*s8/ma+c/sqrt(5.0);
    out=initVect(x5,y5,z5,u5,v5);
    return out;
}

RGB ColorWheel::initRGB(int Rin, int Gin, int Bin)
{
    RGB out;
    out.Rv=Rin;
    out.Gv=Gin;
    out.Bv=Bin;
    return out;
}

RGB ColorWheel::Colors(v3 Vin)
{
    RGB out;
    out = initRGB(int(255.0*(1.0+Vin.xv)/2.0),int(255.0*(1.0+Vin.yv)/2.0),int(255.0*(1.0+Vin.zv)/2.0));
    return out;
}

double ColorWheel::dp(v3 A1, v3 A2) //dot product
{
    return A1.xv*A2.xv + A1.yv*A2.yv + A1.zv*A2.zv;
}

double ColorWheel::crt(double nu)   //cube root function
{
    static double xt;

    if(nu==0.0)
        return 0.0;

    if (nu>0.0)
        xt=exp(log(nu)/3.0);

    else
        xt=0.0-exp(log(0.0-nu)/3.0);

    return xt;
}

double ColorWheel::r35(double nu)
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

void ColorWheel::setCurrent(int index)
{
    if(index >= 0 && index <= 9)
        currentSel = index;
}
