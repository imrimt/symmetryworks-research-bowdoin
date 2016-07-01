#ifndef PORT_H
#define PORT_H

#include "controllerthread.h"
#include "iothread.h"

class Port : public QObject
{
    Q_OBJECT
    
public:
    
    // CONSTRUCTOR
    Port(AbstractFunction *currFunction, ColorWheel *currColorWheel, int width, int height, Settings *currSettings);
    
    virtual ~Port(){;}
    
    // ACTIONS
    void exportImage(QImage *output, const QString &fileName);
    void paintToDisplay(Display *display);
    void paintHistoryIcon(HistoryItem *item);

    // SETTERS
    void changeFunction(AbstractFunction *newFunction) { 
        currFunction = newFunction;
        controller->changeFunction(newFunction); 
    }
    void changeColorWheel(ColorWheel *newColorWheel) { 
        currColorWheel = newColorWheel;
        controller->changeColorWheel(newColorWheel); 
    }
    void changeSettings(Settings *newSettings) {
        currSettings = newSettings;
        controller->changeSettings(newSettings);
    }

    Controller *getControllerObject() { return controllerObject; }

protected:
    
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    Settings *currSettings;    
    int overallWidth, overallHeight;
    //double XCorner, YCorner, setWidth, setHeight;
    
private:
    void render(QImage *output, const int &actionFlag);
    void render(Display *display, const int &actionFlag);

    // void render(QImage *output, Display *display, const int &actionFlag);s

    Display *display;
    QImage *output;
    QString filePathToExport;
    
    // QVector<RenderThread *> threads;
    ControllerThread *controller;
    Controller *controllerObject;
    
    IOThread *ioThread;

    // int numThreadsFinished;

signals:
    void threadFinished(const int &numThreadsCompleted);
    void finishedExport(const QString &filePath);

private slots:
    void handleRenderedImage(const int &actionFlag);
    
    void handleFinishedExport(const QString &filePath) { qDebug() << "HI"; emit finishedExport(filePath); }

    // void combineRenderedImageParts(QPoint startPoint, QVector<QVector<QRgb>> result);
    // void combineRenderedImageParts(const QPoint &startPoint, const Q2DArray &result);
};




#endif // PORT_H
