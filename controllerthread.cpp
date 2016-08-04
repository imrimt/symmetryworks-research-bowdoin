#include "controllerthread.h"

ControllerThread::ControllerThread(AbstractFunction *function, ColorWheel *colorwheel, Settings *settings, Controller *controllerObject, const QSize &outputSize, QObject *parent) : QThread(parent)
{
    QMutexLocker locker(&mutex);
    
    numThreadsActive = 0;
    
    NUM_THREADS = idealThreadCount() != -1 ? idealThreadCount() : 8;
    
    //NUM_THREADS = 1;         //for testing
    
    restart = false;
    abort = false;
    
    currFunction = function;
    currColorWheel = colorwheel;
    currSettings = settings;
    
    this->controllerObject = controllerObject;
    this->controllerObject->setNumThreadsActive(NUM_THREADS);
    
    qRegisterMetaType<Q2DArray>("Q2DArray");
    qRegisterMetaType<ComplexValue>("ComplexValue");
    
    for (int i = 0; i < NUM_THREADS; i++) {
        RenderThread *nextThread = new RenderThread(currFunction, currColorWheel, currSettings, outputSize);
        threads.push_back(nextThread);
        connect(nextThread, SIGNAL(renderingFinished(QPoint, Q2DArray)), controllerObject, SLOT(handleRenderedImageParts(QPoint, Q2DArray)));
        connect(nextThread, SIGNAL(newProgress(double)), controllerObject, SLOT(handleNewProgress(double)));
        connect(nextThread, SIGNAL(newImageDataPoint(ComplexValue)), controllerObject, SLOT(addNewImageDataPoint(ComplexValue)));
    }
    
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

ControllerThread::~ControllerThread()
{
    mutex.lock();
    restart = false;
    abort = true;
    
    restartCondition.wakeOne();
    mutex.unlock();
    
    wait();
}

void ControllerThread::prepareToRun(QImage *output, const int &actionFlag)
{
    
    QMutexLocker locker(&mutex);
    
    AbstractFunction *imageFunction = currFunction->clone();
    ColorWheel *imageColorWheel = currColorWheel->clone();
    Settings *imageSettings = currSettings->clone();
    
    //clone when dealing with image (usually I/O)
    for (int i = 0; i < threads.size(); i++) {
        threads[i]->changeFunction(imageFunction);
        threads[i]->changeColorWheel(imageColorWheel);
        threads[i]->changeSettings(imageSettings);
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
        controllerObject->setRestart(true);
        restart = true;
        emit newWork();
        restartCondition.wakeOne();
    }
    
}

void ControllerThread::prepareToRun(Display *display, const int &actionFlag)
{
    
    QMutexLocker locker(&mutex);
    
    
    this->display = display;
    overallWidth = display->getWidth();
    overallHeight = display->getHeight();
    this->actionFlag = actionFlag;
    controllerObject->setActionFlag(this->actionFlag);
    controllerObject->setDisplay(this->display);
    
    numThreadsActive = threads.size();
    
    controllerObject->setNumThreadsRunning(numThreadsActive);
    
    if (!isRunning()) {
        start(InheritPriority);
    }
    else {
        controllerObject->setRestart(true);
        restart = true;
        emit newWork();
        restartCondition.wakeOne();
    }
}

void ControllerThread::run()
{
    forever {
        if (abort) return;
        
        mutex.lock();
        
        int width = overallWidth;
        int height = overallHeight;
        int counter = overallWidth/numThreadsActive;
        
        for (int i = 0; i < numThreadsActive; i++)
        {
            if (restart) break;
            if (abort) return;
            if (i == numThreadsActive - 1) {
                threads[i]->render(QPoint(i * counter, 0), QPoint(width, height), &allWorkersFinishedCondition);
            }
            else {
                threads[i]->render(QPoint(i * counter, 0), QPoint((i + 1) * counter, height), &allWorkersFinishedCondition);
            }
        }
        
        mutex.unlock();
        
        // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        
        if (!restart) {
            QEventLoop q;
            connect(this, SIGNAL(newWork()), &q, SLOT(quit()));
            connect(controllerObject, SIGNAL(allThreadsFinished()), &q, SLOT(quit()));
            q.exec();
        }
        
        // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        // qDebug() << "TIME TO RENDER ALL PIXELS:" << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / pow(10, 6) << "seconds";
        
        mutex.lock();
        if (!restart) {
            restartCondition.wait(&mutex);
        }
        
        restart = false;
        controllerObject->setRestart(false);
        mutex.unlock();
    }
}

