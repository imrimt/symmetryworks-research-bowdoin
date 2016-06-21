//
//  port.h
//  
//
//  Created by Bridget E. Went on 6/21/16.
//
//

#ifndef PORT_H
#define PORT_H

#include <complex>
#include <QVector>
#include <QDebug>
#include <QtCore/qmath.h>


#include "interface.h"

class HistoryItem : public QObject
{
    Q_OBJECT
public:
    HistoryItem(QObject *parent = 0) : QObject(parent) { }
    
    QImage getImage() { return preview->getImage(); }
    
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
    
private:
    
    
};

class Port
{
   
public:

    // CONSTRUCTOR
    Port(AbstractFunction *currFunction, ColorWheel *currColorWheel, int width, int height);
    
    virtual ~Port(){;}
    
    // ACTIONS
    QString exportImage();
    void paintToDisplay(Display *disp);
    void paintHistoryIcon(HistoryItem *item);

    
protected:
    
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    int width, height;
    
    
private:
    
    void render();
    
};

#endif // PORT_H
