//
//  port.cpp
//  
//
//  Created by Bridget E. Went on 6/21/16.
//
//

#include "port.h"



Port::Port(AbstractFunction *currFunction, ColorWheel *currColorWheel, int width, int height, Settings *currSettings)
{
    this->width = width;
    this->height = height;
    
    this->currFunction = currFunction->clone();
    this->currColorWheel = currColorWheel->clone();
    
    this->currSettings = currSettings;
    
    for (int i = 0; i < NUM_THREADS; i++) {
        RenderThread *nextThread = new RenderThread();
        threads.push_back(nextThread);
        QThread::connect(nextThread, SIGNAL(finished()), nextThread, SLOT(deleteLater()));
    }
    

}


QString Port::exportImage(QImage *output, const QString &fileName)
{
    
    render(output);
    
    output->save(fileName);
    
    QDir stickypath(fileName);
    stickypath.cdUp();
    return stickypath.path();
    
}


void Port::paintToDisplay(Display *display)
{
    
    
    render(display->getImage());
    
    display->repaint();
    
}


void Port::paintHistoryIcon(HistoryItem *item)
{
    
    render(item->getImage());
    
    //item->getImage()->repaint();
}

void Port::render(QImage *output)
{
    
    // temporary timing elements used for code profiling
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i]->render(currFunction, currColorWheel, QPoint(i * width/NUM_THREADS,0), QPoint((i + 1) * width/NUM_THREADS, height), width, height, currSettings, output);
    }
    
//    double worldYStart1= setHeight + YCorner;
//    double worldYStart2 = setHeight/height;
//    double worldXStart = setWidth/width;
//    
//    double worldX, worldY;
//    
//    
//    for (int y = 0; y < height; y++)
//    {
//        for (int x = 0; x < width; x++)
//        {
//            // worldY= settings->Height-y*settings->Height/disp->dim()+settings->YCorner;
//            // worldX= x*settings->Width/disp->dim()+settings->XCorner;
//            
//            worldY = worldYStart1 - y * worldYStart2;
//            worldX = x * worldXStart + XCorner;
//            
//        
//            //run the point through our mathematical function
//            //...then convert that complex output to a color according to our color wheel
//        
//            std::complex<double> fout=(*currFunction)(worldX,worldY);
//            QRgb color = (*currColorWheel)(fout);
//            
//            //finally push the determined color to the corresponding point on the display
//            output->setPixel(x, y, color);
//            
//        }
//    }
    
    // calc time elapsed to render all pixels
    end = clock();
    cpu_time_used = (double)(end - start)/CLOCKS_PER_SEC;
    qDebug() << "TIME TO RENDER ALL PIXELS: " << cpu_time_used << " sec";
    

}


