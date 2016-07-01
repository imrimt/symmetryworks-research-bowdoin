#include "port.h"

Port::Port(AbstractFunction *currFunction, ColorWheel *currColorWheel, int width, int height, Settings *currSettings)
{
    
    overallWidth = width;
    overallHeight = height;    
    this->currFunction = currFunction;
    this->currColorWheel = currColorWheel;
    this->currSettings = currSettings;

    // display = new Display(DEFAULT_PREVIEW_SIZE, DEFAULT_IMAGE_SIZE);
    output = new QImage();
    display = new Display();

    controllerObject = new Controller(display, output);    
    controller = new ControllerThread(this->currFunction, this->currColorWheel, this->currSettings, controllerObject, QSize(overallWidth, overallHeight));
    controllerObject->moveToThread(controller);
    connect(controllerObject, SIGNAL(workFinished(int)), this, SLOT(handleRenderedImage(int)));

}


void Port::exportImage(QImage *output, const QString &fileName)
{
    filePathToExport = fileName;
    this->output = output;
    render(output, IMAGE_EXPORT_FLAG);
}


void Port::paintToDisplay(Display *display)
{ 
    this->display = display;
    render(display, DISPLAY_REPAINT_FLAG);
}


void Port::paintHistoryIcon(HistoryItem *item)
{
    this->display = item->preview;
    render(display, HISTORY_ICON_REPAINT_FLAG);
    
    //item->getImage()->repaint();
}

QString Port::handleRenderedImage(const int &actionFlag)
{
    QString result = "";
    // qDebug() << "print out result";
    switch (actionFlag) {
    case DISPLAY_REPAINT_FLAG:
        display->repaint();
        break;
    case HISTORY_ICON_REPAINT_FLAG:
        display->repaint();
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

    controller->prepareToRun(output, actionFlag);

    // ControllerThread *controller = new ControllerThread();
    // controller->prepareToRun(output, actionFlag);
    // controller->wait();
    
    // calc time elapsed to render all pixels
    end = clock();
    cpu_time_used = (double)(end - start)/CLOCKS_PER_SEC;
    // qDebug() << "TIME TO RENDER ALL PIXELS: " << cpu_time_used << " sec";
}

void Port::render(Display *display, const int &actionFlag)
{
        // temporary timing elements used for code profiling
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    controller->prepareToRun(display, actionFlag);

    // ControllerThread *controller = new ControllerThread();
    // controller->prepareToRun(output, actionFlag);
    // controller->wait();
    
    // calc time elapsed to render all pixels
    end = clock();
    cpu_time_used = (double)(end - start)/CLOCKS_PER_SEC;
    // qDebug() << "TIME TO RENDER ALL PIXELS: " << cpu_time_used << " sec";
}

