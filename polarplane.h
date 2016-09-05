#ifndef COEFFPLANE_H
#define COEFFPLANE_H

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QSplineSeries>
#include <QLineSeries>
#include <QScatterSeries>
#include <QMouseEvent>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QSlider>
#include <QKeyEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QValidator>

#include "shared.h"
#include "geomath.h"
#include "functions.h"

using namespace QtCharts;

const int LOCAL_FLAG = 0;
const int GLOBAL_FLAG = 1;


// object for creating a plane pop-up view for function constants
class PolarPlaneView : public QChartView {
    
    Q_OBJECT
    
public:
    
    PolarPlaneView(QChart *chart, QScatterSeries *coordinateSeries) : QChartView(chart) { this->chart = chart; this->coordinateSeries = coordinateSeries; setMouseTracking(false);}
    
signals:
    void newCoordinate();
    
protected:
    // MOUSE EVENTS
    
    void mouseReleaseEvent(QMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton)
        {
            // qDebug() << "Clicked on: " << event->pos();
            coordinateSeries->replace(0, chart->mapToValue(event->pos(), coordinateSeries));
            mouseMoving = false;
        }
    };
    
    void mousePressEvent(QMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton)
        {
            // qDebug() << "Clicked on: " << event->pos();
            // coordinateSeries->replace(0, chart->mapToValue(event->pos(), coordinateSeries));
            mouseMoving = true;
        }
        QChartView::mousePressEvent(event);
    }
    
    void mouseMoveEvent(QMouseEvent *event)
    {
        if(mouseMoving)
        {
            // qDebug() << "Clicked on: " << event->pos();
            coordinateSeries->replace(0, chart->mapToValue(event->pos(), coordinateSeries));
            emit newCoordinate();
        }
    };
    
private:
    QScatterSeries* coordinateSeries;
    QChart *chart;
    bool mouseMoving;
};



class PolarPlane : public QWidget
{
    Q_OBJECT
public:
    PolarPlane(QWidget *parent = 0) : QWidget(parent) { }
    explicit PolarPlane(AbstractFunction *currFunction, unsigned int *termIndex, QWidget *parent = 0);
    ~PolarPlane() { }
    
    void changeFunction(AbstractFunction* newFunction) { currFunction = newFunction; }
    void hidePolarPlane() { qDebug() << "hello"; polarPlanePopUp->close(); }
    
signals:
    void setPolarCoordinates(int coeffFlag, const QString &radius, const QString &angle);
    
private:
    // organizational elements
    QWidget *polarPlanePopUp;
    QGroupBox *polarCoordinatesBox;
    QHBoxLayout *polarPlanePopUpLayout;
    QVBoxLayout *polarCoordinatesLayout;
    QHBoxLayout *actionButtonLayout;
    QHBoxLayout *zoomButtonLayout;
    QVBoxLayout *polarPlaneWithZoomLayout;
    PolarPlaneView *graphDisplay;
    QChart *graph;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QLabel *radiusLabel;
    QLabel *angleLabel;
    QLineEdit *radiusEdit;
    QLineEdit *angleEdit;
    //sQPushButton *confirmButton;
    QPushButton *resetButton;
    QPushButton *cancelButton;
    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;
    QSpacerItem *planeSpacer1;
    QSpacerItem *planeSpacer2;
    QLineSeries *series;
    QLineSeries *series2;
    QScatterSeries *coordinateSeries;
    QLineSeries *xSeries;
    QLineSeries *ySeries;
    
    int coeffFlag; // stores type of function constant pair
    bool showActionFlag; //stores if the user trigger the "show" action
    double startingRadius; //stores the radius when first shown
    double startingAngle;  //stores the angle when first shown
    
    QDoubleValidator *doubleValidate;
    QDoubleValidator *angleValidate;
    
protected:
    
    AbstractFunction *currFunction;
    unsigned int *termIndex;
    
    void updatePolarCoordinates(QPointF point);
    
private slots:
    
    void showPlanePopUp(int flag);
    void updatePolarCoordinatesWithIndex(const int &index);
    void updatePolarCoordinates();
    void polarPlaneZoomIn();
    void polarPlaneZoomOut();
    
    void setPolarCoordinates();
    void resetPolarCoordinates();
    
    
};

#endif // COEFFPLANE_H