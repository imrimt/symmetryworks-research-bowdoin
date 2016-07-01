#ifndef PORT_H
#define PORT_H

#include "display.h"
#include "controllerthread.h"
#include <QThread>

// TODO: move this to its own source file?
class HistoryItem : public QObject
{
    Q_OBJECT

public:
    
    HistoryItem(QObject *parent = 0) : QObject(parent) { }
    
    QImage *getImage() { return preview->getImage(); }
    
    QVBoxLayout *layoutWithLabelItem;
    QHBoxLayout *layoutItem;
    QVBoxLayout *buttonLayoutItem;
    Display *preview;
    QPushButton *viewButton;
    QPushButton *removeButton;
    QLabel *labelItem;
    QString filePathName;
    QDateTime savedTime;
};

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
    void changeFunction(AbstractFunction *newFunction) { currFunction = newFunction; }
    void changeColorWheel(ColorWheel *newColorWheel) { currColorWheel = newColorWheel; }

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

    // int numThreadsFinished;

private slots:
    QString handleRenderedImage(const int &actionFlag);
    // void combineRenderedImageParts(QPoint startPoint, QVector<QVector<QRgb>> result);
    // void combineRenderedImageParts(const QPoint &startPoint, const Q2DArray &result);
};




#endif // PORT_H
