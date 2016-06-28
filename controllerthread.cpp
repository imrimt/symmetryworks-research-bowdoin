#include "controllerthread.h"

ControllerThread::ControllerThread(QObject *parent)
: QThread(parent)
{
    numThreadsRunning = 0;
    numThreadsActive = 0;

    notFinished = true;

    restart = false;
    abort = false;

    controllerObject = new Controller();

    controllerObject->moveToThread(this);

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

ControllerThread::~ControllerThread()
{
    mutex.lock();
    abort = true;
    delete controllerObject;
    condition.wakeOne();
    mutex.unlock();
    
    wait();
}

void ControllerThread::prepareToRun(QVector<RenderThread *> workerThreads, AbstractFunction *function, ColorWheel *colorwheel, Settings *settings, QImage *output)
{

	QMutexLocker locker(&mutex);

	threads = workerThreads;
    this->output = output;
    overallWidth = output->width();
    overallHeight = output->height();
    this->function = function;
    this->colorwheel = colorwheel;
    this->settings = settings;

	numThreadsActive = workerThreads.size();

	controllerObject->setNumThreadsRunning(numThreadsActive);

    // if (!isRunning()) {
        
    // } else {
    //     // qDebug() << QThread::currentThread() << " is running already, so wake one";
    //     condition.wakeOne();
    // }

    // qDebug() << QThread::currentThread() << "starts running after preparing with" << numThreadsActive << "threads active";

    // while (controllerObject->getNumThreadsRunning() > 0) 
    // {
    //     // condition.wakeOne();
    //     qDebug() << QThread::currentThread() << "goes to wait";
    //     condition.wait(&mutex);
    //     qDebug() << QThread::currentThread() << "wake up from wait";
    //     // condition.wakeOne();
    //     // wait();
    //     if (mutex.tryLock()) qDebug() << QThread::currentThread() << "obtains lock";
    //     // mutex.lock();
    // }
    if (!isRunning()) {
        start(InheritPriority);
    }
    else {
        restart = true;
        condition.wakeOne();
    }

    // wait();

    // while (controllerObject->getNumThreadsRunning() > 0) { 
    //     qDebug() << QThread::currentThread() << "yields since there are" << controllerObject->getNumThreadsRunning() << "threads running";
    //     yieldCurrentThread();
    // }

    // qDebug() << QThread::currentThread() << "finishes preparing, returning to main thread";

    // mutex.unlock(); 
}

// void ControllerThread::checkProgress()
// {
// 	mutex.lock();
	
//  	-- numThreadsRunning;
//  	qDebug() << "reduce and numThreadsRuning is now " << numThreadsRunning;

//  	if (numThreadsRunning > 0)
// 	{
// 		// condition.wait(&mutex);
// 		qDebug() << "all threads have not finished. Waiting";
// 		// mutex.unlock();
// 		condition.wait(&mutex);
// 		mutex.lock();
// 	}
// 	else {
// 		notFinished = false;
// 	}
 	
// 	mutex.unlock();
// }

// void ControllerThread::updateProgress()
// {
// 	mutex.lock();
// 	qDebug() << "update numThreads";
// 	// ++numThreadsRunning;
// 	mutex.unlock();
// }

void ControllerThread::run() 
{
	// while (controllerObject->getNumThreadsRunning() > 0) 
	// {
    // forever
    // {
		mutex.lock();

		// qDebug() << QThread::currentThread() << "start running";

		for (int i = 0; i < numThreadsActive; i++)
		{
			// ++numThreadsRunning;
			threads[i]->render(function, colorwheel, QPoint(i * overallWidth/numThreadsActive, 0), QPoint((i + 1) * overallWidth/numThreadsActive, 
				overallHeight), settings, output, controllerObject, &condition);
		}

        // for (int i = 0; i < this->numThreadsActive; i++)
        // {
        //     qDebug() << QThread::currentThread() << "waiting for thread" << threads[i] << "to finish and release lock" << &mutex;
        //     threads[i]->wait();
        // }

		// qDebug() << QThread::currentThread() << ": current number of thread running:" << controllerObject->getNumThreadsRunning();

        while (controllerObject->getNumThreadsRunning() > 0) 
        {
            if (restart) break;
            if (abort) return;

            condition.wait(&mutex);
        }

        mutex.unlock();

        // mutex.lock();
        // if (!restart) {
        //     condition.wait(&mutex);
        // }
        // restart = false;
        // mutex.unlock();



		// qDebug() << QThread::currentThread() << "wake up after 4 threads finishes";

		// condition.wakeAll();

		// while (numThreadsRunning > 0)
		// {
		// 	// condition.wait(&mutex);
		// 	qDebug() << "all threads have not finished. Waiting";
		// 	// mutex.unlock();
		// 	condition.wait(&mutex);
		// 	mutex.lock();
		// }

		// notFinished = false;
		// condition.wait(&mutex);

		// if (!restart) {
		// 	qDebug() << QThread::currentThread() << "goes to wait";
  //           condition.wait(&mutex);
  //           mutex.lock();
  //           qDebug() << QThread::currentThread() << "wakes up from wait";
  //           // wait();
  //       }
  //       restart = false;
		// mutex.unlock();
	// }
}

