#include "controllerthread.h"

ControllerThread::ControllerThread(QObject *parent)
: QThread(parent)
{
    numThreadsActive = 0;

    restart = false;
    abort = false;

    controllerObject = new Controller();

    controllerObject->moveToThread(this);

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

ControllerThread::~ControllerThread()
{
    mutex.lock();
    abort = true;
    delete controllerObject;
    restartCondition.wakeOne();
    mutex.unlock();
    
    wait();
}

void ControllerThread::prepareToRun(QVector<RenderThread *> workerThreads, AbstractFunction *function, 
    ColorWheel *colorwheel, Settings *settings, QImage *output, const int &actionFlag)
{

	QMutexLocker locker(&mutex);

	threads = workerThreads;
    this->output = output;
    overallWidth = output->width();
    overallHeight = output->height();
    this->function = function;
    this->colorwheel = colorwheel;
    this->settings = settings;
    this->actionFlag = actionFlag;

	numThreadsActive = workerThreads.size();

	controllerObject->setNumThreadsRunning(numThreadsActive);

    if (!isRunning()) {
        start(InheritPriority);
    }
    else {
        restart = true;
        restartCondition.wakeOne();
    }
}

// overloading for display
// void ControllerThread::prepareToRun(QVector<RenderThread *> workerThreads, AbstractFunction *function, 
//     ColorWheel *colorwheel, Settings *settings, QImage *output, Display *display, const int &actionFlag)
// {

//     QMutexLocker locker(&mutex);

//     threads = workerThreads;
//     this->output = output;
//     overallWidth = output->width();
//     overallHeight = output->height();
//     this->function = function;
//     this->colorwheel = colorwheel;
//     this->settings = settings;
//     this->display = display;
//     this->actionFlag = actionFlag;

//     numThreadsActive = workerThreads.size();

//     controllerObject->setNumThreadsRunning(numThreadsActive);

//     if (!isRunning()) {
//         start(InheritPriority);
//     }
//     else {
//         restart = true;
//         restartCondition.wakeOne();
//     }
// }

void ControllerThread::run() 
{
    forever {
    	mutex.lock();

    	for (int i = 0; i < numThreadsActive; i++)
    	{
    		threads[i]->render(function, colorwheel, QPoint(i * overallWidth/numThreadsActive, 0), QPoint((i + 1) * overallWidth/numThreadsActive, 
    			overallHeight), settings, output, controllerObject, &allWorkersFinishedCondition);
            qDebug() << "thread" << i << "starts";
    	}

        clock_t start, end;
        double cpu_time_used;
        start = clock();

        while (controllerObject->getNumThreadsRunning() > 0) 
        {
            if (restart) break;
            if (abort) return;
            allWorkersFinishedCondition.wait(&mutex);
            qDebug() << "wake up in while loop";
        }

        end = clock();
        cpu_time_used = (double)(end - start)/CLOCKS_PER_SEC;
        qDebug() << "TIME TO RENDER ALL PIXELS: " << cpu_time_used << " sec";

        if (!restart) {
            emit resultReady(actionFlag);
            restartCondition.wait(&mutex);
            qDebug() << "wake up after finishing one loop";
        }
        restart = false;

        mutex.unlock();
    }
}

