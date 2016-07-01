#ifndef CONTROLLERTHREAD_H
#define CONTROLLERTHREAD_H

#include "renderthread.h"
#include <time.h>
#include <stdio.h>

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(QObject *parent = 0) : QObject(parent) { } 
    explicit Controller(Display *display, QImage *output, QObject *parent = 0) : QObject(parent) { 
        this->display = display;
        this->output = output;
        restart = false;
    } 
    int getNumThreadsRunning() { return numThreadsRunning; }
    int getNumThreadsActive() { return numThreadsActive; }
    void setNumThreadsRunning(int value) { numThreadsRunning = value; }
    void setNumThreadsActive(int value) { numThreadsActive = value; }
    void setActionFlag(int flag) { actionFlag = flag; }
    void setDisplay(Display *display) { this->display = display; }
    void setOutput(QImage *output) { this->output = output; }
    void setRestart(bool status) { restart = status; }    
    
signals:
    void workFinished(const int &actionFlag);
    void allThreadsFinished();
    void progressChanged(const int &actionFlag);

private:
    int numThreadsRunning;
    int numThreadsActive;
    int actionFlag;
    QImage *output;     //for file writing
    Display *display;   //for display repainting
    bool restart;
    void repaintWork(Display *display, const QPoint &startPoint, const Q2DArray &result) {
        int width = result.size();
        int height = result[0].size();
        int translatedX = startPoint.x();
        int translatedY = startPoint.y();
        for (int x = 0; x < width; x++) {
            if (restart) return;
            for (int y = 0; y < height; y++) {
                if (restart) return;
                display->setPixel(translatedX + x, translatedY + y, result[x][y]);
            }
        }
    }
    void repaintWork(QImage *output, const QPoint &startPoint, const Q2DArray &result) {
        int width = result.size();
        int height = result[0].size();
        int translatedX = startPoint.x();
        int translatedY = startPoint.y();
        for (int x = 0; x < width; x++) {
            if (restart) return;
            for (int y = 0; y < height; y++) {
                if (restart) return;
                output->setPixel(translatedX + x, translatedY + y, result[x][y]);
            }
        }
    }

private slots: 
    void handleRenderedImageParts(const QPoint &startPoint, const Q2DArray &result) {
        if (restart) return;
        
        switch (actionFlag) {
        case DISPLAY_REPAINT_FLAG:
        case HISTORY_ICON_REPAINT_FLAG:
            repaintWork(display, startPoint, result);
            break;
        case IMAGE_EXPORT_FLAG:
            repaintWork(output, startPoint, result);
            break;
        } 

        --numThreadsRunning;
        // qDebug() << "numThreadsRunning = " << numThreadsRunning;

        emit progressChanged(numThreadsActive - numThreadsRunning);

        if (numThreadsRunning == 0) {
            //qDebug() << "emit workFinished signal";
            // restart = false;
            emit allThreadsFinished();
            emit workFinished(actionFlag);
        }
    }
};

class ControllerThread : public QThread 
{
    Q_OBJECT

public:
    ControllerThread(QObject *parent = 0) : QThread(parent) { } 
    explicit ControllerThread(AbstractFunction *function, ColorWheel *colorwheel, Settings *settings, Controller *controllerObject, const QSize &outputSize, QObject *parent = 0);
    ~ControllerThread();
    void prepareToRun(QImage *output, const int &actionFlag);
    void prepareToRun(Display *display, const int &actionFlag);

    // GETTERS
    Controller* getControllerObject() {return controllerObject;}

    // SETTERS
    void changeFunction(AbstractFunction *newFunction) { 
        currFunction = newFunction;
        for (int i = 0; i < threads.size(); i++) {
            threads[i]->changeFunction(newFunction);
        } 
    }
    void changeColorWheel(ColorWheel *newColorWheel) { 
        currColorWheel = newColorWheel;
        for (int i = 0; i < threads.size(); i++) {
            threads[i]->changeColorWheel(newColorWheel);
        } 
    }
    void changeSettings(Settings *newSettings) { 
        currSettings = newSettings; 
        for (int i = 0; i < threads.size(); i++) {
            threads[i]->changeSettings(newSettings);
        }
    }

    // CONSTANTS (SET VALUE ONCE)
    int NUM_THREADS;

protected:
    void run() Q_DECL_OVERRIDE;

signals:
    void resultReady(const int &actionFlag);
    void newWork();

// private slots:
//     void combineRenderedImageParts(const QPoint &startPoint, const Q2DArray &result);

private:
	QMutex mutex;
	QWaitCondition allWorkersFinishedCondition;
    QWaitCondition restartCondition;

	bool restart, abort;

	int numThreadsActive;
	int overallWidth, overallHeight;
    int actionFlag;

    // QPoint topLeft;
    // QPoint bottomRight;

    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    Settings *currSettings;
    Controller *controllerObject;
    Display *display;

    QImage *output;

	QVector<RenderThread *> threads;
};

#endif // CONTROLLERTHREAD_H