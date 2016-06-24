#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H




class RenderThread : public QThread
{
    Q_OBJECT
    
public:
    RenderThread(QObject *parent = 0);
    ~RenderThread();
    
    void render(AbstractFunction *function, ColorWheel *colorwheel, QPoint &topLeft, QPoint &bottomRight, int imageWidth, int imageHeight, QImage *output);
    
signals:
    void renderedImage(const QImage &image);
    
protected:
    void run() Q_DECL_OVERRIDE;
    
private:
    QMutex mutex;
    QWaitCondition condition;
    
    int overallWidth, overallHeight;
    QPoint topLeft, bottomRight;
    double worldYStart1;
    double worldYStart2;
    double worldXStart;
    
    bool restart;
    bool abort;
    
    AbstractFunction *function;
    ColorWheel *colorwheel;
    
    QImage *output;
    
};






#endif // RENDERTHREAD_H