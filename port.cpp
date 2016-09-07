#include "port.h"

Port::Port(AbstractFunction *currFunction, ColorWheel *currColorWheel, int width, int height, Settings *currSettings)
{
    
    overallWidth = width;
    overallHeight = height;
    this->currFunction = currFunction;
    this->currColorWheel = currColorWheel;
    this->currSettings = currSettings;
    
    output = new QImage();
    display = new Display();
    
    controllerObject = new Controller(display, output);
    controller = new ControllerThread(this->currFunction, this->currColorWheel, this->currSettings, controllerObject, QSize(overallWidth, overallHeight), this);
    
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
    //qDebug() << "in paint to display";
    this->display = display;
    render(display, DISPLAY_REPAINT_FLAG);
}


void Port::paintHistoryIcon(HistoryItem *item)
{
    this->display = item->getDisplay();
    render(display, HISTORY_ICON_REPAINT_FLAG);
}

void Port::handleRenderedImage(const int &actionFlag)
{
    QString result = "";
    switch (actionFlag) {
        case DISPLAY_REPAINT_FLAG:
            display->repaint();
            break;
        case HISTORY_ICON_REPAINT_FLAG:
            display->repaint();
            break;
        case IMAGE_EXPORT_FLAG:
            IOThread *ioThread = new IOThread();
            connect(ioThread, SIGNAL(finishedExport(QString)), this, SLOT(handleFinishedExport(QString)));
            ioThread->prepareToWrite(output, filePathToExport);
            break;
    }
    
   // qDebug() << "painting finished";
    emit paintingFinished(true);
    
}

void Port::render(QImage *output, const int &actionFlag)
{
    controller->prepareToRun(output, actionFlag);
}

void Port::render(Display *display, const int &actionFlag)
{
    controller->prepareToRun(display, actionFlag);
}

