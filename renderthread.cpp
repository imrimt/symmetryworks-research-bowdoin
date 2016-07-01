#include "renderthread.h"

RenderThread::RenderThread(AbstractFunction *function, ColorWheel *colorwheel, Settings *settings, QSize outputSize, QObject *parent) : QThread(parent)
{
    restart = false;
    abort = false;

    currFunction = function;
    currColorWheel = colorwheel;
    currSettings = settings;

    overallWidth = outputSize.width();
    overallHeight = outputSize.height();

    worldYStart1 = currSettings->Height + currSettings->YCorner;
    worldYStart2 = currSettings->Height/overallHeight;
    worldXStart = currSettings->Width/overallWidth;

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

void RenderThread::render(QPoint topLeft, QPoint bottomRight, QWaitCondition *controllerCondition)
{
    QMutexLocker locker(&mutex);
    
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
    topLeftXValue = topLeft.x();
    topLeftYValue = topLeft.y();
    bottomRightXValue = bottomRight.x();
    bottomRightYValue = bottomRight.y();
    this->controllerCondition = controllerCondition;
    
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
        double XCorner = currSettings->XCorner;
        // int topLeftXValue = this->topLeftXValue;
        // int topLeftYValue = this->topLeftYValue;
        // int bottomRightXValue = this->bottomRightXValue;
        // int bottomRightYValue = this->bottomRightYValue; 
        double outputWidth = bottomRightXValue - topLeftXValue;
        double outputHeight = bottomRightYValue - topLeftYValue;
        QPoint topLeft = this->topLeft;
        int translated = bottomRightXValue;
        // QImage *output = new QImage(outputWidth, outputHeight);
        QVector<QVector<QRgb>> colorMap(outputWidth, QVector<QRgb>(outputHeight));

        std::complex<double> fout;
        
        mutex.unlock();

        // qDebug() << "drawing from" << topLeft << "to" << bottomRight;

        for (int x = 0; x < outputWidth; x++)
        {   
            if (restart) break;
            if (abort) return;
            for (int y = 0; y < outputHeight; y++)
            {
                if (restart) break;
                if (abort) return;

                worldX = (x + translated) * worldXStart + XCorner;
                worldY = worldYStart1 - y * worldYStart2;
                
                //run the point through our mathematical function
                //...then convert that complex output to a color according to our color wheel
                
                fout = (*currFunction)(worldX,worldY);
                QRgb color = (*currColorWheel)(fout);
                
                //finally push the determined color to the corresponding point on the display
                colorMap[x][y] = color;
            }
        }  
        
        mutex.lock();
        
        // qDebug() << "thread" << QThread::currentThread() << "finishes rendering";
        if (!restart) {
            // qDebug() << "thread" << QThread::currentThread() << "goes to wait before restarting";
            emit renderingFinished(topLeft, colorMap);
            // qDebug() << "emit successfully";
            //controllerCondition->wakeOne();
            condition.wait(&mutex);
        }
        // qDebug() << "thread" << QThread::currentThread() << "wakes up from restarting";
        restart = false;
        mutex.unlock();        
    }   
}