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

    // qDebug() << "new work with " << threads.size() << "numThreadsRunning";

    controllerObject->setNumThreadsRunning(numThreadsActive);

    if (!isRunning()) {
        
        start(InheritPriority);
    }
    else {
        // controllerObject->setRestart(true);
        restart = true;
        emit newWork();
        restartCondition.wakeOne();
    }
}

// void ControllerThread::combineRenderedImageParts(const QPoint &startPoint, const Q2DArray &result)
// {
//     int width = result.size();
//     int height = result[0].size();
//     int translatedX = startPoint.x();
//     int translatedY = startPoint.y();
//     for (int x = 0; x < width; x++) {
//         for (int y = 0; y < height; y++) {
//             display->setPixel(translatedX + x, translatedY + y, result[x][y]);
//         }
//     }
// }

void ControllerThread::run() 
{
    forever {
    	mutex.lock();
        
        // qDebug() << currentThread() << "starts running";
        
        int counter = overallWidth/numThreadsActive;

    	for (int i = 0; i < numThreadsActive; i++)
    	{
    		threads[i]->render(QPoint(i * counter, 0), QPoint((i + 1) * counter, overallHeight), &allWorkersFinishedCondition);
            // qDebug() << "thread" << i << "starts";
            
    	}

        mutex.unlock();

        time_t timer;
        struct tm y2k;
        double seconds;
        y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
        y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;
        time(&timer);  /* get current time; same as: timer = time(NULL)  */
        seconds = difftime(timer,mktime(&y2k));
        // printf ("%.f seconds: ", seconds);

        QEventLoop q;
        connect(this, SIGNAL(newWork()), &q, SLOT(quit()));
        connect(controllerObject, SIGNAL(allThreadsFinished()), &q, SLOT(quit()));
        q.exec();

        time_t timer2;
        struct tm y2k2;
        double seconds2;
        y2k2.tm_hour = 0;   y2k2.tm_min = 0; y2k2.tm_sec = 0;
        y2k2.tm_year = 100; y2k2.tm_mon = 0; y2k2.tm_mday = 1;
        time(&timer2);  /* get current time; same as: timer = time(NULL)  */
        seconds2 = difftime(timer2,mktime(&y2k2));
        // printf ("%.f seconds2: ", seconds2);

        qDebug() << "TIME TO RENDER ALL PIXELS: " << seconds2 - seconds << " secs";

        mutex.lock();
        if (!restart) {
            //qDebug() << "controller goes to wait for restart";

            restartCondition.wait(&mutex);
        }
        
        restart = false;
        mutex.unlock();
    }
}

