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
#include <QtWidgets>

#include <QScrollArea>
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
#include <QAbstractButton>
#include <QImage>
#include <QDir>
#include <QInputDialog>
#include <QFileDialog>
#include <QSignalMapper>
#include <QDateTime>
#include <QShortCut>
#include <QAction>
#include <QMessageBox>
#include <QTableWidget>
#include <QStringList>

#include <time.h>

#include "display.h"
#include "functions.h"
#include "colorwheel.h"
#include "renderThread.h"


#define NUM_THREADS 2

const double DEFAULT_WIDTH = 2.5;
const double DEFAULT_HEIGHT = 2.0;
const double DEFAULT_XCORNER = -0.5;
const double DEFAULT_YCORNER = -0.5;
const int DEFAULT_OUTPUT_WIDTH = 6000; //6000 width 4800 height standard for art prints
const int DEFAULT_OUTPUT_HEIGHT = 4800;
const int DEFAULT_PREVIEW_SIZE = 600;

namespace settings
{
    double Width = DEFAULT_WIDTH;
    double Height = DEFAULT_HEIGHT;
    double XCorner = DEFAULT_XCORNER;
    double YCorner = DEFAULT_YCORNER;
    int OWidth = DEFAULT_OUTPUT_WIDTH;
    int OHeight = DEFAULT_OUTPUT_HEIGHT;
}

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
    
    
    
    QVector<RenderThread *> threads;
    
    
};




#endif // PORT_H
