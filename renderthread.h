#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QMetaType>
#include <QEventLoop>
#include <QMutex>

#include "functions.h"
#include "colorwheel.h"

#include "geomath.h"
#include "shared.h"

const int DISPLAY_REPAINT_FLAG = 1;
const int HISTORY_ICON_REPAINT_FLAG = 2;
const int IMAGE_EXPORT_FLAG = 3;

typedef QVector<QVector<QRgb> > Q2DArray;
typedef std::complex<double> ComplexValue;

class RenderThread : public QThread
{
    Q_OBJECT
    
public:
    RenderThread(QObject *parent = 0) : QThread(parent) { }
    explicit RenderThread(AbstractFunction *function, ColorWheel *colorwheel, Settings *settings, QSize outputSize, QObject *parent = 0);
    ~RenderThread();
    
    void render(QPoint topLeft, QPoint bottomRight, QWaitCondition *controllerCondition);
    
    // SETTERS
    void changeFunction(AbstractFunction *newFunction) { currFunction = newFunction; }
    void changeColorWheel(ColorWheel *newColorWheel) { currColorWheel = newColorWheel; }
    void changeSettings(Settings *newSettings) {
        currSettings = newSettings;
        worldYStart1 = currSettings->Height + currSettings->YCorner;
        worldYStart2 = currSettings->Height/overallHeight;
        worldXStart = currSettings->Width/overallWidth;
        overallWidth = currSettings->OWidth;
        overallHeight = currSettings->OHeight;
    }
    void changeDimensions(double newWidth, double newHeight) {
        overallWidth = newWidth;
        overallHeight = newHeight;
    }
    
protected:
    void run() Q_DECL_OVERRIDE;
    
signals:
    void renderingFinished(const QPoint &startPoint, const Q2DArray &result);
    void newProgress(const double &progress);
    void newImageDataPoint(const ComplexValue &data);
    
private:
    QMutex mutex;
    QWaitCondition condition;
    QWaitCondition *controllerCondition;
    
    int overallWidth, overallHeight;
    int topLeftXValue, topLeftYValue, bottomRightXValue, bottomRightYValue;
    QPoint topLeft, bottomRight;
    double worldYStart1, worldYStart2, worldXStart;
    
    bool restart;
    bool abort;
    
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    Settings *currSettings;

};






#endif // RENDERTHREAD_H