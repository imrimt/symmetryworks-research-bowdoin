#include "port.h"

Port::Port(AbstractFunction *currFunction, ColorWheel *currColorWheel, int width, int height, Settings *currSettings)
{
    this->width = width;
    this->height = height;    
    this->currFunction = currFunction;
    this->currColorWheel = currColorWheel;
    this->currSettings = currSettings;

    // display = new Display(DEFAULT_PREVIEW_SIZE, DEFAULT_IMAGE_SIZE);
    output = new QImage();
    
    // qDebug() << "creating 2 threads: " << QThread::currentThread();

    controller = new ControllerThread();
    connect(controller, SIGNAL(resultReady(int)), this, SLOT(handleRenderedImage(int)));
    for (int i = 0; i < NUM_THREADS; i++) {
        RenderThread *nextThread = new RenderThread();
        threads.push_back(nextThread);
    }

}


void Port::exportImage(QImage *output, const QString &fileName)
{
    filePathToExport = fileName;
    this->output = output;
    render(output, IMAGE_EXPORT_FLAG);
    
    // output->save(fileName);
    
    // QDir stickypath(fileName);
    // stickypath.cdUp();
    // return stickypath.path();
}


void Port::paintToDisplay(Display *display)
{ 
    this->display = display;
    render(display->getImage(), DISPLAY_REPAINT_FLAG);

    //overloading with display
    // render(this->display->getImage(), display, DISPLAY_REPAINT_FLAG);

    // qDebug() << "repainting display";
    
    // display->repaint();
    
}


void Port::paintHistoryIcon(HistoryItem *item)
{
    
    render(item->getImage(), HISTORY_ICON_REPAINT_FLAG);
    
    //item->getImage()->repaint();
}

QString Port::handleRenderedImage(const int &actionFlag)
{
    QString result = "";
    qDebug() << "print out result";
    switch (actionFlag) {
    case DISPLAY_REPAINT_FLAG:
        display->repaint();
        break;
    case HISTORY_ICON_REPAINT_FLAG:
        break;
    case IMAGE_EXPORT_FLAG:
        output->save(filePathToExport);
        QDir stickypath(filePathToExport);
        stickypath.cdUp();
        result = stickypath.path();
        break;
    }
    return result;
}

void Port::render(QImage *output, const int &actionFlag)
{
    
    // temporary timing elements used for code profiling
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // ControllerThread *controller = new ControllerThread();
    controller->prepareToRun(threads, currFunction, currColorWheel, currSettings, output, actionFlag);
    // controller->wait();
    
    // calc time elapsed to render all pixels
    end = clock();
    cpu_time_used = (double)(end - start)/CLOCKS_PER_SEC;
    // qDebug() << "TIME TO RENDER ALL PIXELS: " << cpu_time_used << " sec";
}

//overloading with display
// void Port::render(QImage *output, Display *display, const int &actionFlag)
// {
//     controller->prepareToRun(threads, currFunction, currColorWheel, currSettings, output, display, actionFlag);
// }

// void Port::checkProgress()
// {

//     ++numThreadsFinished;

//     if (numThreadsFinished < NUM_THREADS)
//     {
//         QThread::wait();
//     }
// }


