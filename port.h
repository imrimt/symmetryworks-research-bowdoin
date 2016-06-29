#ifndef PORT_H
#define PORT_H

#include "display.h"
#include "controllerthread.h"

#include <QThread>

// CONSTANT ACTION FLAGS
const int NUM_THREADS = 4;

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

protected:
    
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    Settings *currSettings;    
    int width, height;
    //double XCorner, YCorner, setWidth, setHeight;
    
private:
    void render(QImage *output, const int &actionFlag);
    void changeFunction(AbstractFunction *newFunction) { currFunction = newFunction; }

    // void render(QImage *output, Display *display, const int &actionFlag);s

    Display *display;
    QImage *output;
    QString filePathToExport;
    
    QVector<RenderThread *> threads;
    ControllerThread *controller;

    // int numThreadsFinished;

private slots:
    QString handleRenderedImage(const int &actionFlag);

};




#endif // PORT_H
