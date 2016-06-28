#ifndef CONTROLLERTHREAD_H
#define CONTROLLERTHREAD_H

#include "renderthread.h"

// class Controller : public QObject
// {
// 	Q_OBJECT
// public:	
// 	int getNumThreadsRunning() {return numThreadsRunning;}
// 	void setNumThreadsRunning(int value) {numThreadsRunning = value;}

// private slots:
// 	void checkProgress() 
// 	{
// 		--numThreadsRunning;
// 		qDebug() << "reducing numThreadsRunning to" << numThreadsRunning;
// 	}

// private:
// 	int numThreadsRunning;

// };

class ControllerThread : public QThread 
{
    Q_OBJECT

public:
    ControllerThread(QObject *parent = 0);
    ~ControllerThread();
    void prepareToRun(QVector<RenderThread *> workerThreads, AbstractFunction *function, ColorWheel *colorwheel, Settings *settings, QImage *output);
    Controller* getControllerObject() {return controllerObject;}

protected:
    void run() Q_DECL_OVERRIDE;

private:
	QMutex mutex;
	QWaitCondition condition;

	bool notFinished, restart, abort;

	int numThreadsRunning;
	int numThreadsActive;

	int overallWidth, overallHeight;

    AbstractFunction *function;
    ColorWheel *colorwheel;
    Settings *settings;
    Controller *controllerObject;
    
    QImage *output;

	QVector<RenderThread *> threads;

// private slots:
// 	void checkProgress();
// 	void updateProgress();
};

#endif // CONTROLLERTHREAD_H