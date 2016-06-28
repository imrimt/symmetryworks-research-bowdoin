#include "renderthread.h"

RenderThread::RenderThread(QObject *parent)
: QThread(parent)
{
    restart = false;
    abort = false;

    controllerCondition = new QWaitCondition();
}

RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();
    
    wait();
}


void RenderThread::render(AbstractFunction *function, ColorWheel *colorwheel, QPoint topLeft, QPoint bottomRight,  
    Settings *settings, QImage *output, Controller *controllerObject, QWaitCondition *controllerCondition)
{

    QMutexLocker locker(&mutex);
    
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
    this->output = output;
    overallWidth = output->width();
    overallHeight = output->height();
    this->function = function;
    this->colorwheel = colorwheel;
    this->settings = settings;
    this->controllerCondition = controllerCondition;
    this->controllerObject = controllerObject;
    // this->mutex = mutex;
    
    worldYStart1 = settings->Height + settings->YCorner;
    worldYStart2 = settings->Height/overallHeight;
    worldXStart = settings->Width/overallWidth;

    // qDebug() << QThread::currentThread() << "starts running as controller thread";
    
    if (!isRunning()) {
        // qDebug() << QThread::currentThread() << "starts running as render thread";
        start(InheritPriority);
    } else {
        // qDebug() << QThread::currentThread() << " is running already, so wake one";
        restart = true;
        condition.wakeOne();
    }
        
}


void RenderThread::run()
{
    forever {
        // qDebug() << QThread::currentThread() << "begins rendering and tries to obtain lock" << &mutex;
        // if (mutex.tryLock()) qDebug() << QThread::currentThread() << "obtains lock" << &mutex;
        mutex.lock();
        
        double worldX, worldY;
        double worldYStart1 = this->worldYStart1;
        double worldYStart2 = this->worldYStart2;
        double worldXStart = this->worldXStart;
        double XCorner = settings->XCorner;
        
        mutex.unlock();
        // qDebug() << "lock unlocked";

        // controllerObject->setNumThreadsRunning(controllerObject->getNumThreadsRunning() + 1);
        // qDebug() << "drawing at" << topLeft << "and" << bottomRight;
        
        for (int x = topLeft.x(); x < bottomRight.x(); x++)
        {   
            if (restart) break;
            if (abort) return;
            for (int y = topLeft.y(); y < bottomRight.y(); y++)
            {
                
                // worldY= settings->Height-y*settings->Height/disp->dim()+settings->YCorner;
                // worldX= x*settings->Width/disp->dim()+settings->XCorner;
                
                worldY = worldYStart1 - y * worldYStart2;
                worldX = x * worldXStart + XCorner;
                
                //run the point through our mathematical function
                //...then convert that complex output to a color according to our color wheel
                
                std::complex<double> fout=(*function)(worldX,worldY);
                // std::complex<double> fout = (*function)(x,y);
                QRgb color = (*colorwheel)(fout);
                
                //finally push the determined color to the corresponding point on the display
                output->setPixel(x, y, color);
                
            }
        }  

        // emit renderingFinished();
        
        mutex.lock();
        controllerObject->setNumThreadsRunning(controllerObject->getNumThreadsRunning() - 1);
        // qDebug() << "thread" << QThread::currentThread() << "finishes rendering";
        if (!restart) {
            // qDebug() << "thread" << QThread::currentThread() << "goes to wait before restarting";
            // emit renderingFinished();
            controllerCondition->wakeOne();
            condition.wait(&mutex);
        }
        // qDebug() << "thread" << QThread::currentThread() << "wakes up from restarting";
        restart = false;
        mutex.unlock();        
    }   
}