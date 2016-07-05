#include "controllerthread.h"

ControllerThread::ControllerThread(AbstractFunction *function, ColorWheel *colorwheel, Settings *settings, Controller *controllerObject, const QSize &outputSize, QObject *parent) : QThread(parent)
{
    numThreadsActive = 0;

    NUM_THREADS = idealThreadCount() != -1 ? idealThreadCount() : 8;

    restart = false;
    abort = false;

    currFunction = function;
    currColorWheel = colorwheel;
    currSettings = settings;

    this->controllerObject = controllerObject;
    this->controllerObject->setNumThreadsActive(NUM_THREADS);

    qRegisterMetaType<Q2DArray>("Q2DArray");

    for (int i = 0; i < NUM_THREADS; i++) {
        RenderThread *nextThread = new RenderThread(currFunction, currColorWheel, currSettings, outputSize);
        // if (i < NUM_THREADS) {
        //     displayThreads.push_back(nextThread);
        // }
        // if (NUM_THREADS >= i && i < 2 * NUM_THREADS) {
        //     exportThreads.push_back(nextThread);
        // }
        // if (i >= 2 * NUM_THREADS) {
        //     historyThreads.push_back(nextThread);
        // }
        threads.push_back(nextThread);
        connect(nextThread, SIGNAL(renderingFinished(QPoint, Q2DArray)), controllerObject, SLOT(handleRenderedImageParts(QPoint, Q2DArray)));
    }

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

void ControllerThread::prepareToRun(QImage *output, const int &actionFlag)
{

	QMutexLocker locker(&mutex);

    //delete display;
    qDebug() << "cloning" << currFunction;
    currFunction = currFunction->clone();
    currColorWheel = currColorWheel->clone();
    qDebug() << "after cloning:" << currFunction;

    //change exportThreads functions & colorwheel
    for (int i = 0; i < threads.size(); i++) {
        threads[i]->changeFunction(currFunction);
        threads[i]->changeColorWheel(currColorWheel);
    }

    this->output = output;
    overallWidth = output->width();
    overallHeight = output->height();
    this->actionFlag = actionFlag;
    controllerObject->setActionFlag(this->actionFlag);
    controllerObject->setOutput(this->output);

	numThreadsActive = threads.size();

	controllerObject->setNumThreadsRunning(numThreadsActive);

    if (!isRunning()) {
        start(InheritPriority);
    }
    else {
        restart = true;
        emit newWork();
        restartCondition.wakeOne();
    }
}

void ControllerThread::prepareToRun(Display *display, const int &actionFlag)
{

    QMutexLocker locker(&mutex);

    //delete output;
    
    this->display = display;
    overallWidth = display->width();
    overallHeight = display->height();
    this->actionFlag = actionFlag;
    controllerObject->setActionFlag(this->actionFlag);
    controllerObject->setDisplay(this->display);

    numThreadsActive = threads.size();

    // qDebug() << "new work with " << displayThreads.size() << "numThreadsRunning";

    controllerObject->setNumThreadsRunning(numThreadsActive);

    if (!isRunning()) {
        start(InheritPriority);
    }
    else {
        restart = true;
        emit newWork();
        restartCondition.wakeOne();
    }
}

void ControllerThread::run() 
{
    forever {
    	mutex.lock();
        
        // qDebug() << currentThread() << "starts running";
        
        int counter = overallWidth/numThreadsActive;

    	for (int i = 0; i < numThreadsActive; i++)
    	{
            if (i == numThreadsActive - 1) {
                threads[i]->render(QPoint(i * counter, 0), QPoint(overallWidth, overallHeight), &allWorkersFinishedCondition);
            }
            else {
                threads[i]->render(QPoint(i * counter, 0), QPoint((i + 1) * counter, overallHeight), &allWorkersFinishedCondition);
            }
    	}

        mutex.unlock();

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        QEventLoop q;
        connect(this, SIGNAL(newWork()), &q, SLOT(quit()));
        connect(controllerObject, SIGNAL(allThreadsFinished()), &q, SLOT(quit()));
        q.exec();

        std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
        qDebug() << "TIME TO RENDER ALL PIXELS:" << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / pow(10, 6) << "seconds";

        mutex.lock();
        if (!restart) {
            //qDebug() << "controller goes to wait for restart";

            restartCondition.wait(&mutex);
        }
        
        restart = false;
        mutex.unlock();
    }
}

