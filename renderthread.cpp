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


// void RenderThread::render(AbstractFunction *function, ColorWheel *colorwheel, QPoint topLeft, QPoint bottomRight,  
//     Settings *settings, QImage *output, Controller *controllerObject, QWaitCondition *controllerCondition)
// {
void RenderThread::render(AbstractFunction *function, ColorWheel *colorwheel, QPoint topLeft, QPoint bottomRight,  
    Settings *settings, Controller *controllerObject, QWaitCondition *controllerCondition)
{
    QMutexLocker locker(&mutex);
    
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
    topLeftXValue = topLeft.x();
    topLeftYValue = topLeft.y();
    bottomRightXValue = bottomRight.x();
    bottomRightYValue = bottomRight.y();
    // this->output = output;
    overallWidth = bottomRightXValue - topLeftXValue;
    overallHeight = bottomRightYValue - topLeftYValue;
    this->function = function;
    this->colorwheel = colorwheel;
    this->settings = settings;
    this->controllerCondition = controllerCondition;
    this->controllerObject = controllerObject;
    
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
        mutex.lock();
        
        double worldX, worldY;
        double worldYStart1 = this->worldYStart1;
        double worldYStart2 = this->worldYStart2;
        double worldXStart = this->worldXStart;
        double XCorner = settings->XCorner;
        // int topLeftXValue = this->topLeftXValue;
        // int topLeftYValue = this->topLeftYValue;
        // int bottomRightXValue = this->bottomRightXValue;
        // int bottomRightYValue = this->bottomRightYValue; 
        double outputWidth = overallWidth;
        double outputHeight = overallHeight;
        QPoint topLeft = this->topLeft;
        int translated = topLeft.x();
        // QImage *output = new QImage(outputWidth, outputHeight);
        QVector<QVector<QRgb>> colorMap(outputWidth, QVector<QRgb>(outputHeight));

        std::complex<double> fout;
        
        mutex.unlock();
        // qDebug() << "lock unlocked";

        // controllerObject->setNumThreadsRunning(controllerObject->getNumThreadsRunning() + 1);

        qDebug() << "drawing from" << topLeft << "to" << bottomRight;
        
        // for (int x = topLeftXValue; x < bottomRightXValue; x++)
        for (int x = 0; x < outputWidth; x++)
        {   
            if (restart) break;
            if (abort) return;
            // for (int y = topLeftYValue; y < bottomRightYValue; y++)
            for (int y = 0; y < outputHeight; y++)
            {
                worldX = (x + translated) * worldXStart + XCorner;
                worldY = worldYStart1 - y * worldYStart2;
                
                //run the point through our mathematical function
                //...then convert that complex output to a color according to our color wheel
                
                fout = (*function)(worldX,worldY);
                // std::complex<double> fout = (*function)(x,y);
                QRgb color = (*colorwheel)(fout);
                
                //finally push the determined color to the corresponding point on the display
                colorMap[x][y] = color;
            }
        }  
        
        mutex.lock();
        controllerObject->setNumThreadsRunning(controllerObject->getNumThreadsRunning() - 1);
        // qDebug() << "thread" << QThread::currentThread() << "finishes rendering";
        if (!restart) {
            qDebug() << "thread" << QThread::currentThread() << "goes to wait before restarting";
            emit renderingFinished(topLeft, colorMap);
            controllerCondition->wakeOne();
            condition.wait(&mutex);
        }
        // qDebug() << "thread" << QThread::currentThread() << "wakes up from restarting";
        restart = false;
        mutex.unlock();        
    }   
}