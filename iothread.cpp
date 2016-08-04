#include "iothread.h"

IOThread::IOThread(QObject *parent) : QThread(parent)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

IOThread::~IOThread()
{
    wait();
}


void IOThread::prepareToWrite(QImage *output, const QString &filePathToExport)
{
    
    QMutexLocker locker(&mutex);
    this->output = output;
    this->filePathToExport = filePathToExport;
    
    start(InheritPriority);
    
}


void IOThread::run()
{
    
    output->save(filePathToExport);
    QDir stickypath(filePathToExport);
    stickypath.cdUp();
    result = stickypath.path();
    
    emit finishedExport(result);
    
}

