#include "polarplane.h"


// CONSTRUCTOR sets up layout for plane pop-up window
PolarPlane::PolarPlane(AbstractFunction *currFunction, unsigned int *termIndex, QWidget *parent)
{
    this->currFunction = currFunction;
    this->termIndex = termIndex;
    
    // INPUT VALIDATORS (NUMERICAL)
    doubleValidate = new QDoubleValidator(-9999999.0, 9999999.0, 5, this);
    angleValidate = new QDoubleValidator(-pi, pi, 5, this);
    
    
    //SET UP COEFFICIENT PLANE
    polarPlanePopUp = new QWidget(parent, Qt::Window);
    polarPlanePopUpLayout = new QHBoxLayout();
    polarPlaneWithZoomLayout = new QVBoxLayout();
    polarCoordinatesBox = new QGroupBox(tr("Polar Cooridnates"), polarPlanePopUp);
    polarCoordinatesLayout = new QVBoxLayout(polarCoordinatesBox);
    actionButtonLayout = new QHBoxLayout();
    zoomButtonLayout = new QHBoxLayout();
    
    polarPlanePopUp->setWindowModality(Qt::WindowModal);
    // GRAPH ELEMENTS
    graph = new QChart();
    
    axisX = new QValueAxis();
    axisY = new QValueAxis();
    
    series = new QSplineSeries();
    series2 = new QSplineSeries();
    xSeries = new QLineSeries();
    ySeries = new QLineSeries();
    coordinateSeries = new QScatterSeries();
    
    radiusLabel = new QLabel(tr("Radius: "), polarCoordinatesBox);
    angleLabel = new QLabel(tr("Angle: "), polarCoordinatesBox);
    
    radiusEdit = new QLineEdit();
    angleEdit = new QLineEdit();
    
    // UI ELEMENTS
    confirmButton = new QPushButton(tr("OK"));
    resetButton = new QPushButton(tr("Reset"));
    cancelButton = new QPushButton(tr("Cancel"));
    zoomInButton = new QPushButton(QIcon(":/Images/Icons/zoomin.png"), "Zoom In");
    zoomInButton->setStyleSheet("QPushButton { text-align:center; padding:5px}");
    zoomOutButton = new QPushButton(QIcon(":/Images/Icons/zoomout.png"), "Zoom Out");
    zoomOutButton->setStyleSheet("QPushButton { text-align:center; padding:5px}");

    // cancelButton->setStyleSheet("background-color: red");
    
    planeSpacer1 = new QSpacerItem(15,15);
    planeSpacer2 = new QSpacerItem(5,5);
    
    *series << QPointF(10, 0) << QPointF(2,0) << QPointF(-2,0) << QPointF(-10,0);
    *series2 << QPointF(0, 10) << QPointF(0,2) << QPointF(0,-2) << QPointF(0, -10);
    *xSeries << QPointF(0,0) << QPointF(1,0);
    *ySeries << QPointF(0,0) << QPointF(1,0);
    *coordinateSeries << QPointF(1, 0);
    
    radiusEdit->setValidator(doubleValidate);
    radiusEdit->setText("1.00");
    angleEdit->setValidator(angleValidate);
    angleEdit->setText("0.00");
    
    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    series->setPen(axisPen);
    series2->setPen(axisPen);
    
    QPen linePen(Qt::green);
    linePen.setWidth(2);
    xSeries->setPen(linePen);
    ySeries->setPen(linePen);
    
    QPen pointPen(Qt::red);
    QBrush pointBrush(Qt::red);
    coordinateSeries->setPen(pointPen);
    coordinateSeries->setBrush(pointBrush);
    coordinateSeries->setMarkerSize(10.0);
    
    axisX->setTickCount(9);
    axisX->setRange(-2.5, 2.5);
    axisY->setTickCount(8);
    axisY->setRange(-2.5, 2.5);
    
    graph->addSeries(series);
    graph->addSeries(series2);
    graph->addSeries(xSeries);
    graph->addSeries(ySeries);
    graph->addSeries(coordinateSeries);
    
    axisX->setGridLineVisible(false);
    axisY->setGridLineVisible(false);
    
    graph->addAxis(axisX, Qt::AlignBottom);
    graph->addAxis(axisY, Qt::AlignLeft);
    
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    series2->attachAxis(axisX);
    series2->attachAxis(axisY);
    xSeries->attachAxis(axisX);
    xSeries->attachAxis(axisY);
    ySeries->attachAxis(axisX);
    ySeries->attachAxis(axisY);
    coordinateSeries->attachAxis(axisX);
    coordinateSeries->attachAxis(axisY);
    
    graphDisplay = new PolarPlaneView(graph, coordinateSeries);
    graphDisplay->setRenderHint(QPainter::Antialiasing);
    
    graph->legend()->hide();
    graph->setTitle("PLANE FOR COEFFICIENT PAIR");
    
    // coeffPlanePopUpLayout->setFixedSize(600,600);
    
    radiusLabel->setFixedWidth(200);
    radiusEdit->setFixedWidth(200);
    angleLabel->setFixedWidth(200);
    angleEdit->setFixedWidth(200);
    
    actionButtonLayout->addWidget(confirmButton);
    actionButtonLayout->addWidget(resetButton);
    
    polarCoordinatesLayout->addWidget(radiusLabel);
    polarCoordinatesLayout->addWidget(radiusEdit);
    polarCoordinatesLayout->addWidget(angleLabel);
    polarCoordinatesLayout->addWidget(angleEdit);
    polarCoordinatesLayout->addItem(planeSpacer1);
    polarCoordinatesLayout->addLayout(actionButtonLayout);
    polarCoordinatesLayout->addWidget(cancelButton);
    polarCoordinatesLayout->addStretch(0);
    polarCoordinatesLayout->setSizeConstraint(QLayout::SetFixedSize);
    
    zoomButtonLayout->addWidget(zoomInButton);
    zoomButtonLayout->addWidget(zoomOutButton);
    
    polarPlaneWithZoomLayout->addWidget(graphDisplay);
    polarPlaneWithZoomLayout->addLayout(zoomButtonLayout);
    
    polarPlanePopUpLayout->addLayout(polarPlaneWithZoomLayout);
    polarPlanePopUpLayout->addWidget(polarCoordinatesBox);
    
    polarPlanePopUp->setLayout(polarPlanePopUpLayout);
    polarPlanePopUp->setFixedSize(1000, 600);
    
    // CONNECT ALL SIGNALS
    connect(radiusEdit, SIGNAL(editingFinished()), this, SLOT(updatePolarCoordinates()));
    connect(angleEdit, SIGNAL(editingFinished()), this, SLOT(updatePolarCoordinates()));
    connect(coordinateSeries, SIGNAL(pointReplaced(int)), this, SLOT(updatePolarCoordinatesWithIndex(int)));
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(setPolarCoordinates()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(hidePolarPlane()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetPolarCoordinates()));
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(polarPlaneZoomIn()));
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(polarPlaneZoomOut()));
    
    
}


// show plane in new window
void PolarPlane::showPlanePopUp(int flag)
{
    
    coeffFlag = flag;
    
    double tempA, tempR = 0;
    
    if (coeffFlag == LOCAL_FLAG)
    {
        tempA = currFunction->getA(*termIndex);
        tempR = currFunction->getR(*termIndex);
        
    }
    else if (coeffFlag == GLOBAL_FLAG)
    {
        tempA = currFunction->getScaleA();
        tempR = currFunction->getScaleR();
    }
    else {
        return;
    }
    
    QPointF point(tempR * cos(tempA), tempR * sin(tempA));
    
    coordinateSeries->replace(0, point);
    // updatePolarCoordinates(QPointF(tempR * cos(tempA), tempR * sin(tempA)));
    polarPlanePopUp->show();
    
}


void PolarPlane::updatePolarCoordinatesWithIndex(const int &index)
{
    QPointF point = coordinateSeries->at(index);
    // qDebug() << "update info " << point;
    updatePolarCoordinates(point);
}

void PolarPlane::updatePolarCoordinates()
{
    double tempA = angleEdit->text().toDouble();
    double tempR = radiusEdit->text().toDouble();
    
    coordinateSeries->replace(0, QPointF(tempR * cos(tempA), tempR * sin(tempA)));
}

void PolarPlane::setPolarCoordinates()
{
    emit setPolarCoordinates(coeffFlag, radiusEdit->text(), angleEdit->text());
    
    hidePolarPlane();
}

void PolarPlane::resetPolarCoordinates()
{
    QPointF point(1, 0);
    coordinateSeries->replace(0, point);
    
    QVector<QPointF> list1;
    list1 << QPointF(0,0) << point;
    QVector<QPointF> list2;
    list2 << QPointF(point.x(), 0) << point;
    
    xSeries->replace(list1);
    ySeries->replace(list2);
    
    radiusEdit->setText("1.00");
    angleEdit->setText("0.00");
}

void PolarPlane::updatePolarCoordinates(QPointF point)
{
    QVector<QPointF> list1;
    list1 << QPointF(0,0) << point;
    QVector<QPointF> list2;
    list2 << QPointF(point.x(), 0) << point;
    
    xSeries->replace(list1);
    ySeries->replace(list2);
    
    radiusEdit->setText(QString::number(sqrt(pow(point.x(), 2) + pow(point.y(), 2)), 'f', 2));
    angleEdit->setText(QString::number(atan(point.y() / point.x()), 'f', 2));
}

void PolarPlane::polarPlaneZoomOut()
{
    series->insert(0, QPointF(series->at(0).x() * 2, series->at(0).y() * 2));
    series->append(QPointF(series->at(series->count() - 1).x() * 2, series->at(series->count() - 1).y() * 2));
    
    series2->insert(0, QPointF(series2->at(0).x() * 2, series2->at(0).y() * 2));
    series2->append(QPointF(series2->at(series2->count() - 1).x() * 2, series2->at(series2->count() - 1).y() * 2));
    
    graph->zoomOut();
}

void PolarPlane::polarPlaneZoomIn()
{
    series->replace(0, QPointF(series->at(0).x() / 2, series->at(0).y() / 2));
    series->replace(series->count() - 1, QPointF(series->at(series->count() - 1).x() / 2, series->at(series->count() - 1).y() / 2));
    
    series2->replace(0, QPointF(series2->at(0).x() / 2, series2->at(0).y() / 2));
    series2->replace(series->count() - 1, QPointF(series2->at(series2->count() - 1).x() / 2, series2->at(series2->count() - 1).y() / 2));
    
    graph->zoomIn();
}

