#ifndef CONTROLLERTHREAD_H
#define CONTROLLERTHREAD_H

#include "renderthread.h"

class ControllerThread : public QThread 
{
    Q_OBJECT

public:
    ControllerThread(QObject *parent = 0);
    ~ControllerThread();
    void prepareToRun(QVector<RenderThread *> workerThreads, AbstractFunction *function, 
        ColorWheel *colorwheel, Settings *settings, QImage *output, const int &actionFlag);
    // void prepareToRun(QVector<RenderThread *> workerThreads, AbstractFunction *function, 
    //     ColorWheel *colorwheel, Settings *settings, QImage *output, Display *display, const int &actionFlag);
    Controller* getControllerObject() {return controllerObject;}

protected:
    void run() Q_DECL_OVERRIDE;

signals:
    void resultReady(const int &actionFlag);

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

    AbstractFunction *function;
    ColorWheel *colorwheel;
    Settings *settings;
    Controller *controllerObject;
    Display *display;

    QImage *output;

	QVector<RenderThread *> threads;
};

#endif // CONTROLLERTHREAD_H