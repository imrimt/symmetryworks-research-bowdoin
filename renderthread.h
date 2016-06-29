#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
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

#include "functions.h"
#include "colorwheel.h"

#include "geomath.h"

const int DISPLAY_REPAINT_FLAG = 1;
const int HISTORY_ICON_REPAINT_FLAG = 2;
const int IMAGE_EXPORT_FLAG = 3;

class Controller : public QObject
{
    Q_OBJECT
public: 
    int getNumThreadsRunning() {return numThreadsRunning;}
    void setNumThreadsRunning(int value) {numThreadsRunning = value;}

private:
    int numThreadsRunning;
};

class RenderThread : public QThread
{
    Q_OBJECT
    
public:
    RenderThread(QObject *parent = 0);
    ~RenderThread();
    
    void render(AbstractFunction *function, ColorWheel *colorwheel, QPoint topLeft, QPoint bottomRight, Settings *settings, QImage *output, Controller *controllerObject, QWaitCondition *controllerCondition);
    
protected:
    void run() Q_DECL_OVERRIDE;
    
// signals:
//     void renderingFinished();
    
private:
    QMutex mutex;
    QWaitCondition condition;
    QWaitCondition *controllerCondition;
    
    int overallWidth, overallHeight;
    QPoint topLeft, bottomRight;
    double worldYStart1;
    double worldYStart2;
    double worldXStart;
    
    bool restart;
    bool abort;
    
    AbstractFunction *function;
    ColorWheel *colorwheel;
    Settings *settings;
    Controller *controllerObject;
    
    QImage *output;
    
};






#endif // RENDERTHREAD_H