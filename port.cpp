//
//  port.cpp
//  
//
//  Created by Bridget E. Went on 6/21/16.
//
//

#include "port.h"


Port::Port(AbstractFunction *currFunction, ColorWheel *currFunction, int width, int height)
{
    this->width = width;
    this->height = height;
    
    this->currFunction = currFunction->copy();
    this->currColorWheel = currFunction->copy();
    
}


QString Port::exportImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    saveloadPath,
                                                    tr("PPM (*.ppm);;JPEG (*.jpg *.jpeg)"));
    
    QFile inFile(fileName);
    if (!inFile.open(QIODevice::WriteOnly))
        return;
    
    double worldY, worldX ;
    QImage output = new QImage(width, height, QImage::Format_RGB32);
    
    render(output);
    
    output->save(fileName);
    
    QDir stickypath(fileName);
    stickypath.cdUp();
    return stickypath.path();
    
}

void Port::paintToDisplay(Display *display)
{
    render(display->getImage());
    
    display->getImage()->repaint();
}


void Port::paintHistoryIcon(HistoryItem *item)
{
    
    render(item->getImage());
    
    item->getImage()->repaint();
}

void Port::render(QImage &output)
{
    
    double worldYStart1= settings::Height + settings::YCorner;
    double worldYStart2 = settings::Height/height;
    double worldXStart = settings::Width/width;
    
    double worldX, worldY;
    
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x <= (width - 1); x++)
        {
            // worldY= settings::Height-y*settings::Height/disp->dim()+settings::YCorner;
            // worldX= x*settings::Width/disp->dim()+settings::XCorner;
            
            worldY = worldYStart1 - y * worldYStart2;
            worldX = x * worldXStart + settings::XCorner;
            
            //run the point through our mathematical function
            //...then convert that complex output to a color according to our color wheel
        
            std::complex<double> fout=(*currFunction)(worldX,worldY);
            QRgb color = (*currColorWheel)(fout);
            
            //finally push the determined color to the corresponding point on the display
            output->setPixel(x, y, color);
            
        }
    }
    

}


