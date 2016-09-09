#ifndef shared_h
#define shared_h

#include <QVector>
#include <QDebug>
#include <QDateTime>
#include <complex>
#include <QLayout>
#include <QPushButton>
#include <QDir>
#include <time.h>

#include "display.h"

//constants
const double DEFAULT_WORLD_WIDTH = 2.0;
const double DEFAULT_WORLD_HEIGHT = 2.0;
const double DEFAULT_XCORNER = 0.0;
const double DEFAULT_YCORNER = 0.0;
const int DEFAULT_OUTPUT_WIDTH = 6000; //6000 width 4800 height standard for art prints
const int DEFAULT_OUTPUT_HEIGHT = 4800;
const double PREVIEW_SCALING = 0.4;
const int PARAMETER_SEPARATOR_LENGTH = 10;

//struct that holds information about image and output properties
struct Settings
{
    double Width = DEFAULT_WORLD_WIDTH;
    double Height = DEFAULT_WORLD_HEIGHT;
    double XCorner = DEFAULT_XCORNER;
    double YCorner = DEFAULT_YCORNER;
    int OWidth = DEFAULT_OUTPUT_WIDTH;
    int OHeight = DEFAULT_OUTPUT_HEIGHT;
    
    Settings* clone() {
        
        Settings* newSettings = new Settings;
        newSettings->Width = this->Width;
        newSettings->Height = this->Height;
        newSettings->XCorner = this->XCorner;
        newSettings->YCorner = this->YCorner;
        newSettings->OWidth = this->OWidth;
        newSettings->OHeight = this->OHeight;
        
        return newSettings;
    }
};

//object that represents a snapshot
class HistoryItem : public QObject
{
    Q_OBJECT
    
public:
    
    HistoryItem(QObject *parent = 0) : QObject(parent) { }
    
    QImage *getImage() { return preview->getImage(); }
    Display *getDisplay() { return preview; }
    
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

#endif /* shared_h */
