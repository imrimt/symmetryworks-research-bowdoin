#include "controllerthread.h"

ControllerThread::ControllerThread(AbstractFunction *function, ColorWheel *colorwheel, Settings *settings, Controller *controllerObject, const QSize &outputSize, QObject *parent) : QThread(parent)
{
    numThreadsActive = 0;

    restart = false;
    abort = false;

    this->function = function;
    this->colorwheel = colorwheel;
    this->settings = settings;

//    display = new Display();
//    output = new QImage();

    // controllerObject = new Controller(display, output);
    // controllerObject->moveToThread(this);

    this->controllerObject = controllerObject;

    qRegisterMetaType<Q2DArray>("Q2DArray");

    for (int i = 0; i < NUM_THREADS; i++) {
        RenderThread *nextThread = new RenderThread(this->function, this->colorwheel, this->settings = settings, outputSize);
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
    
    qDebug() << "prepare to run, is it a history item? " << (bool)(actionFlag == HISTORY_ICON_REPAINT_FLAG);

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

        clock_t start, end;
        double cpu_time_used;
        start = clock();

        mutex.unlock();

        QEventLoop q;
        connect(this, SIGNAL(newWork()), &q, SLOT(quit()));
        connect(controllerObject, SIGNAL(allThreadsFinished()), &q, SLOT(quit()));
        q.exec();

        // qDebug() << "this is what happens after exec";

        end = clock();
        cpu_time_used = (double)(end - start)/CLOCKS_PER_SEC;
        qDebug() << "TIME TO RENDER ALL PIXELS: " << cpu_time_used << " sec";

        mutex.lock();
        if (!restart) {
            //qDebug() << "controller goes to wait for restart";
            restartCondition.wait(&mutex);
        }
        restart = false;
        mutex.unlock();
    }
}

