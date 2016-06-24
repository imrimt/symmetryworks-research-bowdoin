#include "renderthread.h"



RenderThread::RenderThread(QObject *parent)
: QThread(parent)
{
    
    restart = false;
    abort = false;
    
}

RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();
    
    wait();
    
}


void RenderThread::render(AbstractFunction *function, ColorWheel *colorwheel, QPoint topLeft, QPoint bottomRight, int imageWidth, int imageHeight, Settings *settings, QImage *output)
{
    QMutexLocker locker(&mutex);
    
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
    overallWidth = imageWidth;
    overallHeight = imageHeight;
    this->output = output;
    
    this->function = function->clone();
    this->colorwheel = colorwheel->clone();
    this->settings = settings;
    
    worldYStart1= settings->Height + settings->YCorner;
    worldYStart2 = settings->Height/overallHeight;
    worldXStart = settings->Width/overallWidth;
    
    if (!isRunning()) {
        start(LowPriority);
    } else {
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
        
        mutex.unlock();

        // qDebug() << "drawing at " << topLeft << " and " << bottomRight;
        
        for (int x = topLeft.x(); x < bottomRight.x(); x++)
        {
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
         
        // create output image
        // process all pixels
        // qDebug() << "Rendering pixels...";
        
        mutex.lock();
        if (!restart) {
            condition.wait(&mutex);
        }
        restart = false;
        mutex.unlock();
        
    }
    
    
    
    
}