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
    void changeDimensions(double newWidth, double newHeight)
    {
        overallWidth = newWidth;
        overallHeight = newHeight;
        
        controller->changeDimensions(newWidth, newHeight);
    }
    
    
    Controller *getControllerObject() { return controllerObject; }
    
    void deleteMembers() {
        if (!currFunction) delete currFunction;
        if (!currColorWheel) delete currColorWheel;
        if (!currSettings) delete currSettings;
        if (!display) delete display;
        if (!output) delete output;
        if (!controller) delete controller;
        if (!controllerObject) delete controllerObject;
    }
    
protected:
    
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    Settings *currSettings;
    int overallWidth, overallHeight;
    
private:
    void render(QImage *output, const int &actionFlag);
    void render(Display *display, const int &actionFlag);
    
    Display *display;
    QImage *output;
    QString filePathToExport;
    
    ControllerThread *controller;
    Controller *controllerObject;
    
signals:
    void finishedExport(const QString &filePath);
    void paintingFinished(const bool &status);
    
    private slots:
    void handleRenderedImage(const int &actionFlag);
    void handleFinishedExport(const QString &filePath) { emit finishedExport(filePath); }

};




#endif // PORT_H
