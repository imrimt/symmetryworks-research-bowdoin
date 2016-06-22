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
#include <complex>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QComboBox>
#include <QString>
#include <QDebug>
#include <QPushButton>
#include <QImage>
#include <QDir>
#include <QInputDialog>
#include <QFileDialog>
#include <QSignalMapper>
#include <QDateTime>
#include <QShortCut>
#include <QAction>
#include <QMessageBox>

#include <time.h>

#include "display.h"
#include "functions.h"
#include "colorwheel.h"



//#include "interface.h"

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
    
private:
    
};

class Port
{
   
public:

    // CONSTRUCTOR
    Port(AbstractFunction *currFunction, ColorWheel *currColorWheel, int width, int height, double XCorner, double YCorner, double setWidth, double setHeight);

    
    virtual ~Port(){;}
    
    // ACTIONS
    QString exportImage(QImage *output, const QString &fileName);
    void paintToDisplay(Display *disp);
    void paintHistoryIcon(HistoryItem *item);

    
protected:
    
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    
    
    int width, height;
    double XCorner, YCorner, setWidth, setHeight;
    
    
private:
    
    void render(QImage *output);
    
};




#endif // PORT_H
