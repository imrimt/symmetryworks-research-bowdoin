#include "colorwheel.h"

ColorWheel::ColorWheel(QObject *parent) :
QObject(parent)
{
    currentSel = 0;
    image = QImage(image_dim, image_dim, QImage::Format_RGB32);
    image.fill(MAX_RGB);
    
    //initialize zoneVect
    zoneVect[0] = tilt(QVector3D(0.0,0.0,1.0));
    zoneVect[1] = tilt(QVector3D(0.0,0.0,-1.0));
    
    int m = 6;
    
    for(int i = 1; i < m; i++)
    {
        for(int j = 0; j < m; j++)
        {
            zoneVect[2+j+6*(i-1)] = tilt(QVector3D(cos(2.0*pi*float(j)/6.0)*sin(pi*i/m),sin(2.0*pi*j/6.0)*sin(pi*i/m),cos(pi*i/m)));
        }
    }
}

ColorWheel* ColorWheel::clone()
{
    
    ColorWheel *c = new ColorWheel();
    
    c->setCurrent(this->currentSel);
    c->changeOverflowColor(this->getOverflowColor());
    
    if (!(this->image.isNull())) {
        c->image = this->image.copy();
    } else {
        
    }
    
    return c;
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
    image = raw.scaled(image_dim, image_dim, Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

QRgb ColorWheel::IcosColor(std::complex<double> zin)
{
    QVector3D V;
    int Tag;
    double test,compare;
    
    
    V = tilt(stereo(zin));
    test = 0.0;
    for(unsigned int n = 0; n < ICOS_FACES_SIZE; n++)
    {
        compare = dotProduct(V, icosFaces[n]);
        if(compare > test)
        {
            Tag = n;
            test = compare;
        }
    }
    
    return RgbFromVec3(icosFaces[Tag]);
}

QRgb ColorWheel::IcosColorC(std::complex<double> zin)
{
    QVector3D V;
    int Tag;
    double test,compare;
    
    V = tilt(stereo(zin));
    test = 0.0;
    for(unsigned int n = 0; n < ICOS_FACES_SIZE; n++)
    {
        compare = dotProduct(V, icosFaces[n]);
        if(compare > test)
        {
            Tag = n;
            test = compare;
        }
    }
    
    return RgbFromVec3(cubeRootVec(icosFaces[Tag]));
}

QRgb ColorWheel::StCol(std::complex<double> zin)
{
    QVector3D V;
    V = tilt(stereo(zin));
    
    return RgbFromVec3(V);
    
}

QRgb ColorWheel::StColC(std::complex<double> zin)
{
    QVector3D V;
    V = cubeRootVec(tilt(stereo(zin)));
    
    return RgbFromVec3(V);
}

QRgb ColorWheel::StCol35(std::complex<double> zin)
{
    QVector3D V;
    V = pow35Vec(tilt(stereo(zin)));
    
    return RgbFromVec3(V);
}

QRgb ColorWheel::ZoneCol(std::complex<double> zin)
{
    QVector3D V;
    int Tag;
    double test,compare;
    
    V = tilt(stereo(zin));
    test = 0.0;
    Tag = 0;
    
    for(unsigned int n = 0; n < ZONE_VECT_SIZE; n++)
    {
        compare = dotProduct(V, zoneVect[n]);
        if(compare > test)
        {
            Tag = n;
            test = compare;
        }
    }
    
    return RgbFromVec3(cubeRootVec(zoneVect[Tag]));;
}

QRgb ColorWheel::SectCol(std::complex<double> zin)
{
    
    int S1,r,g,b;
    double ra;
    double x = zin.real();
    double y = -zin.imag();
    
    ra = qSqrt(x*x+y*y);
    
    int extremeValue = int(MAX_RGB - (MAX_RGB * ra)/2.0);
    
    if(ra < 0.25)
    {
        r = MAX_RGB;
        g = MAX_RGB;
        b = MAX_RGB;
    }
    else
    {
        if(ra > 2.0)
        {
            r = MIN_RGB;
            g = MIN_RGB;
            b = MIN_RGB;
        }
        else
        {
            if(y > 0.0)
            {
                if (y < 0.72654 * x)
                {
                    S1 = 1;
                }
                else
                {
                    if(y < -3.077 * x)
                    {
                        S1 = 3;
                    }
                    else
                    {
                        S1 = 2;
                    }
                }
            }
            else
            {
                if(y > -0.72654 * x)
                {
                    S1 = 1;
                }
                else
                {
                    if(y > 3.077 * x)
                    {
                        S1 = 4;
                    }
                    else
                    {
                        S1 = 5;
                    }
                }
            }
            switch(S1)
            {
                case 1:
                    r = extremeValue;
                    g = MIN_RGB;
                    b = MIN_RGB;
                    break;
                case 2:
                    r = extremeValue;
                    g = extremeValue;
                    b = MIN_RGB;
                    break;
                case 3:
                    r = MIN_RGB;
                    g = extremeValue;
                    b = MIN_RGB;
                    break;
                case 4:
                    r = MIN_RGB;
                    g = MIN_RGB;
                    b = extremeValue;
                    break;
                case 5:
                    r = extremeValue;
                    g = MIN_RGB;
                    b = extremeValue;
                    break;
            }
        }
    }
    
    return QColor(r, g, b).rgb();
}

QRgb ColorWheel::Sect6Col(std::complex<double> zin)
{
    int S1,S2,r,g,b;
    
    double x = zin.real();
    double y = zin.imag();
    
    double ra = qSqrt(x * x + y * y);
    
    int specialValue1 = int(MAX_RGB * (1.5 - 2 * ra));
    int specialValue2 = int(MAX_RGB - (MAX_RGB * ra)/2.0);
    
    if(ra < 0.25)
    {
        r = MAX_RGB;
        g = MAX_RGB;
        b = MAX_RGB;
    }
    else
    {
        if(ra > 2.0)
        {
            r = MIN_RGB;
            g = MIN_RGB;
            b = MIN_RGB;
        }
        else
        {
            if (x > 0.0)
            {
                if(y < -x/1.732)
                {
                    S1 = 6;
                }
                else
                {
                    if(y > x/1.732)
                    {
                        S1 = 2;
                    }
                    else
                    {
                        S1 = 1;
                    }
                }
            }
            else
            {
                if(y < x/1.732)
                {
                    S1 = 5;
                }
                else
                {
                    if(y > -x/1.732)
                    {
                        S1 = 3;
                    }
                    else
                    {
                        S1 = 4;
                    }
                }
            }
            if(ra < 0.75)
            {
                S2 = 1;
            }
            else
            {
                if(ra < 1.25)
                {
                    S2 = 2;
                }
                else
                {
                    S2 = 3;
                }
            }
            switch(S2)
            {
                case 1:
                    switch(S1)
                {
                    case 1:
                        r = int(MAX_RGB);
                        g = specialValue1;
                        b = specialValue1;
                        break;
                    case 2:
                        r = int(MAX_RGB);
                        g = int(MAX_RGB);
                        b = specialValue1;
                        break;
                    case 3:
                        r = specialValue1;
                        g = MAX_RGB;
                        b = specialValue1;
                        break;
                    case 4:
                        r = specialValue1;
                        g = int(MAX_RGB);
                        b = int(MAX_RGB);
                        break;
                    case 5:
                        r = specialValue1;
                        g = specialValue1;
                        b = int(MAX_RGB);
                        break;
                    case 6:
                        r = int(MAX_RGB);
                        g = specialValue1;
                        b = int(MAX_RGB);
                        break;
                }
                    break;
                case 2:
                    switch(S1)
                {
                    case 1:
                        r = int(MAX_RGB);
                        g = int(0);
                        b = int(0);
                        break;
                    case 2:
                        r = int(MAX_RGB);
                        g = int(MAX_RGB);
                        b = 0;
                        break;
                    case 3:
                        r = 0;
                        g = MAX_RGB;
                        b = 0;
                        break;
                    case 4:
                        r = 0;
                        g = MAX_RGB;
                        b = MAX_RGB;
                        break;
                    case 5:
                        r = 0;
                        g = 0;
                        b = MAX_RGB;
                        break;
                    case 6:
                        r = MAX_RGB;
                        g = 0;
                        b = MAX_RGB;
                        break;
                }
                    break;
                case 3:
                    switch(S1)
                {
                    case 1:
                        r = specialValue2;
                        g = 0;
                        b = 0;
                        break;
                    case 2:
                        r = specialValue2;
                        g = specialValue2;
                        b = 0;
                        break;
                    case 3:
                        r = 0;
                        g = specialValue2;
                        b = 0;
                        break;
                    case 4:
                        r = 0;
                        g = specialValue2;
                        b = specialValue2;
                        break;
                    case 5:
                        r = 0;
                        g = 0;
                        b = specialValue2;
                        break;
                    case 6:
                        r = specialValue2;
                        g = 0;
                        b = specialValue2;
                        break;
                }
                    break;
            }
        }
    }
    
    return QColor(r, g, b).rgb();
}

QRgb ColorWheel::WinCol(std::complex<double> zin)
{
    double X,Y,Z,xa,ya,xtm,ytm,ztm,Xf,Yf,Zf;
    vect5 E1, E2, E3, B1, B2;
    int r, g, b;
    
    E1 = initVect5(1.0/ma,c2/ma,c4/ma,c6/ma,c8/ma);
    E2 = initVect5(0.0,s2/ma,s4/ma,s6/ma,s8/ma);
    E3 = initVect5(1.0/qSqrt(5.0),1.0/qSqrt(5.0),1.0/qSqrt(5.0),1.0/qSqrt(5.0),1.0/qSqrt(5.0));
    
    xa = zin.real();
    ya = zin.imag();
    
    //1st do stereographic proj
    xtm = 2.0*xa/(1.0+xa*xa+ya*ya);
    ytm = 2.0*ya/(1.0+xa*xa+ya*ya);
    ztm = 2.0/(1.0+xa*xa+ya*ya) -1.0;
    
    //put sphere up in R5
    B1 = Bvect5(xtm,ytm,ztm);
    
    //take cube roots
    B2 = initVect5(cubeRoot(B1.xv),cubeRoot(B1.yv),cubeRoot(B1.zv),cubeRoot(B1.uv),cubeRoot(B1.vv));
    Xf = B2.xv * E1.xv + B2.yv * E1.yv + B2.zv * E1.zv + B2.uv * E1.uv + B2.vv * E1.vv;
    Yf = B2.xv * E2.xv + B2.yv * E2.yv + B2.zv * E2.zv + B2.uv * E2.uv + B2.vv * E2.vv;
    Zf = B2.xv * E3.xv + B2.yv * E3.yv + B2.zv * E3.zv + B2.uv * E3.uv + B2.vv * E3.vv;
    
    //following accomplishes "tilt," but it was already written out, so fine.
    X = (Xf*q2+Zf)/q3;
    Z = 0.0 - Xf/q2/q3-Yf/q2+Zf/q3;
    Y = 0.0 - Xf/q2/q3+Yf/q2+Zf/q3;
    r = int(double(MAX_RGB) * (1.0 + X)/2.0);
    g = int(double(MAX_RGB) * (1.0 + Z)/2.0);
    b = int(double(MAX_RGB) * (1.0 + Y)/2.0);
    if(r > MAX_RGB){ r = MAX_RGB; }
    if(g > MAX_RGB){ g = MAX_RGB; }
    if(b > MAX_RGB){ b = MAX_RGB; }
    if(r < MIN_RGB){ r = MIN_RGB; }
    if(g < MIN_RGB){ g = MIN_RGB; }
    if(b < MIN_RGB){ b = MIN_RGB; }
    
    return QColor(r, g, b).rgb();
    
    
}

QRgb ColorWheel::FromImage(std::complex<double> zin)
{
    double x = zin.real();
    double y = zin.imag();
    QColor color;
    
    if(x >= -2.0 && x < 2.0 && y >= -2.0 && y < 2.0)      //our image is defined within the Cartesian coordinates
    {                                                       // -2 <= x <= 2  and -2 <= y <= 2
        int translated_x = (int) ((x + 2.0) * (image_dim / 4.0));
        int translated_y = (int) image_dim - ((y + 2.0) * (image_dim / 4.0));
        
        color = image.pixel(translated_x, translated_y);
    }
    else {
        color = overflowColor;
    }
    
    return color.rgb();
}

void ColorWheel::setCurrent(int index)
{
    if(index >= 0 && index <= 9)
        currentSel = index;
}

