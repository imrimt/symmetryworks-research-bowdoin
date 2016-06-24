//
//  port.h
//
//
//  Created by Bridget E. Went on 6/21/16.
//
//

#ifndef PORT_H
#define PORT_H


#include "display.h"

#include "renderThread.h"


#define NUM_THREADS 2


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
    QStringList settings;
    
    QDateTime savedTime;
    
};


class Port
{
    
public:
    
    // CONSTRUCTOR
    Port(AbstractFunction *currFunction, ColorWheel *currColorWheel, int width, int height);
    
    virtual ~Port(){;}
    
    // ACTIONS
    QString exportImage(QImage *output, const QString &fileName);
    void paintToDisplay(Display *disp);
    void paintHistoryIcon(HistoryItem *item);
    
    
protected:
    
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    
    
    int width, height;
    //double XCorner, YCorner, setWidth, setHeight;
    
    
private:
    void render(QImage *output);
    
    
    
    QVector<RenderThread *> threads;
    
    
};




#endif // PORT_H
