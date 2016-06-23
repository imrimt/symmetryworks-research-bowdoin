//
//  port.cpp
//  
//
//  Created by Bridget E. Went on 6/21/16.
//
//

#include "port.h"


Port::Port(AbstractFunction *currFunction, ColorWheel *currColorWheel, int width, int height, double XCorner, double YCorner, double setWidth, double setHeight)
{
    this->width = width;
    this->height = height;
    this->XCorner = XCorner;
    this->YCorner = YCorner;
    this->setWidth = setWidth;
    this->setHeight = setHeight;
    
    this->currFunction = currFunction->clone();
    this->currColorWheel = currColorWheel->clone();
    
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
    
    double worldYStart1= setHeight + YCorner;
    double worldYStart2 = setHeight/height;
    double worldXStart = setWidth/width;
    
    double worldX, worldY;
    
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x <= (width - 1); x++)
        {
            // worldY= settings::Height-y*settings::Height/disp->dim()+settings::YCorner;
            // worldX= x*settings::Width/disp->dim()+settings::XCorner;
            
            worldY = worldYStart1 - y * worldYStart2;
            worldX = x * worldXStart + XCorner;
            
        
            //run the point through our mathematical function
            //...then convert that complex output to a color according to our color wheel
        
            std::complex<double> fout=(*currFunction)(worldX,worldY);
            QRgb color = (*currColorWheel)(fout);
            
            //finally push the determined color to the corresponding point on the display
            output->setPixel(x, y, color);
            
        }
    }
    
    // calc time elapsed to render all pixels
    end = clock();
    cpu_time_used = (double)(end - start)/CLOCKS_PER_SEC;
    qDebug() << "TIME TO RENDER ALL PIXELS: " << cpu_time_used << " sec";
    

}


