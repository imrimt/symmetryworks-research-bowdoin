#include "interface.h"


//the settings namespace stores a few variables used to compute the
//image via the mathematical function and the color wheel, as well as
//the output width and height.



interface::interface(QWidget *parent) : QWidget(parent)
{

    // FUNCTIONAL VARIABLES
    advancedMode = false;
    numTerms = 0;
    termIndex = -1;
    saveloadPath = QDir::homePath();

    // FUNCTIONAL OBJECTS
    currFunction = new hex3Function();
    currColorWheel = new ColorWheel();
    
    settings = new Settings;
    settings->Width = DEFAULT_WIDTH;
    settings->Height = DEFAULT_HEIGHT;
    settings->XCorner = DEFAULT_XCORNER;
    settings->YCorner = DEFAULT_YCORNER;
    settings->OWidth = DEFAULT_OUTPUT_WIDTH;
    settings->OHeight = DEFAULT_OUTPUT_HEIGHT;
    
    initInterfaceLayout();
    
    functionSel->setCurrentIndex(0);
    colorwheelSel->setCurrentIndex(0);
    
}


void interface::initInterfaceLayout()
{
    
    // ORGANIZATIONAL ELEMENTS
    interfaceLayout = new QVBoxLayout(this);
    topbarLayout = new QHBoxLayout();
    leftbarLayout = new QVBoxLayout();
    
    imagePropsBox = new QGroupBox(tr("Image Properties"), this);
    displayWidget = new QWidget(this);
    patternTypeWidget = new QWidget(this);
    functionConstantsWidget = new QWidget(this);
    viewHistoryWidget = new QWidget(this);
    toggleViewWidget = new QWidget(this);
    
    leftbarLayout->addWidget(toggleViewWidget);
    leftbarLayout->addWidget(imagePropsBox);
    leftbarLayout->addWidget(patternTypeWidget);
    
    topbarLayout->addLayout(leftbarLayout);
    topbarLayout->addWidget(displayWidget);
    topbarLayout->addWidget(viewHistoryWidget);
    interfaceLayout->addLayout(topbarLayout);
    interfaceLayout->addWidget(functionConstantsWidget);
    setLayout(interfaceLayout);
    
    errorMessageBox = new QMessageBox(this);
    errorMessageBox->setIcon(QMessageBox::Critical);
    
    // INPUT VALIDATORS (NUMERICAL)
    doubleValidate = new QDoubleValidator(-9999999.0, 9999999.0, 5, this);
    angleValidate = new QDoubleValidator(-pi, pi, 5, this);
    intValidate = new QIntValidator(-9999999, 9999999, this);
    posdoubleValidate = new QDoubleValidator(1.0, 9999999.0, 5, this);
    posintValidate = new QIntValidator(1, 9999999, this);
    numtermsValidate = new QIntValidator(1, 99, this);
    dimValidate = new QIntValidator(1, 10000, this);
    
    
    // modeToggle SUBELEMENTS...remove?
    toggleViewButton = new QPushButton(tr("Switch to Advanced View"), this);
    toggleViewLayout = new QVBoxLayout(toggleViewWidget);
    toggleViewLayout->addWidget(toggleViewButton);
    
    
    
    initPreviewDisplay();
    initFunctionConstants();
    initPatternType();
    initViewHistory();
    initImageProps();
    initCoeffPlane();
    connectAllSignals();
    
    // SET DEFAULTS
    refreshTerms();
    scaleREdit->setText(QString::number(currFunction->getScaleR()));
    scaleAEdit->setText(QString::number(currFunction->getScaleA()));
    
    outwidthEdit->setText(QString::number(DEFAULT_OUTPUT_WIDTH));
    outheightEdit->setText(QString::number(DEFAULT_OUTPUT_HEIGHT));
    worldwidthEdit->setText(QString::number(DEFAULT_WIDTH));
    worldheightEdit->setText(QString::number(DEFAULT_HEIGHT));
    XCornerEdit->setText(QString::number(DEFAULT_XCORNER));
    YCornerEdit->setText(QString::number(DEFAULT_YCORNER));
    
    // FINALIZE WINDOW
    setFixedSize(sizeHint());
    setWindowTitle(tr("Wallpaper Generation"));
    setTabOrder(nEdit, mEdit);
    setTabOrder(mEdit, rEdit);
    setTabOrder(rEdit, aEdit);
    setTabOrder(aEdit, scaleREdit);
    setTabOrder(scaleREdit, scaleAEdit);
    setTabOrder(scaleAEdit, XCornerEdit);
    setTabOrder(XCornerEdit, YCornerEdit);
    setTabOrder(YCornerEdit, worldwidthEdit);
    setTabOrder(worldwidthEdit, worldheightEdit);
    setTabOrder(worldheightEdit, outwidthEdit);
    setTabOrder(outwidthEdit, outheightEdit);
    

}




void interface::initPreviewDisplay()
{
    disp = new Display(DEFAULT_PREVIEW_SIZE, displayWidget);
    updatePreview = new QPushButton(tr("Update Preview"), this);
    exportImage = new QPushButton(tr("Export Image..."), this);
    resetImage = new QPushButton(tr("Reset"), this);
    dispLayout = new QVBoxLayout(displayWidget);
    buttonLayout = new QHBoxLayout();
    
    buttonLayout->addWidget(updatePreview);
    buttonLayout->addWidget(exportImage);
    buttonLayout->addWidget(resetImage);
    
    dispLayout->addWidget(disp);
    dispLayout->setAlignment(disp, Qt::AlignCenter);
    dispLayout->addLayout(buttonLayout);
    
    //SET UP SHORTCUTS...add for save, open, undo?
    updatePreviewShortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
    
}



void interface::initFunctionConstants()
{
    functionConstantsBox = new QGroupBox(tr("Function Constants"), functionConstantsWidget);
    
    
    
    currTermLabel = new QLabel(functionConstantsBox);
    currTermLabel->setText(tr("Term"));
    
    
    //currTermEdit = new CustomSpinBox(functionConstantsBox);
    
    freqpairLabel = new QLabel(tr("Frequency Pair: "), functionConstantsBox);
    coeffLabel = new QLabel(tr("Coefficient Pair: "), functionConstantsBox);
    
    nLabel = new QLabel(functionConstantsBox);
    mLabel = new QLabel(functionConstantsBox);
    aLabel = new QLabel(functionConstantsBox);
    rLabel = new QLabel(functionConstantsBox);
    aValueLabel = new QLabel(functionConstantsBox);
    rValueLabel = new QLabel(functionConstantsBox);
    
    
    globalConsantsLabel = new QLabel(tr("<b>Global Function Constants:       <\b>"), functionConstantsBox);
    scaleALabel = new QLabel(tr("Scaling Angle"), functionConstantsBox);
    scaleRLabel = new QLabel(tr("Scaling Radius"), functionConstantsBox);
    refreshLabels();
    
    nEdit = new QSpinBox(functionConstantsBox);
    mEdit = new QSpinBox(functionConstantsBox);
    aEdit = new QDoubleSlider();
    rEdit = new QDoubleSlider();
    
    
    nEdit->setFixedWidth(100);
    mEdit->setFixedWidth(100);
    rEdit->setFixedWidth(100);
    aEdit->setFixedWidth(100);
    
//    scalePlaneEdit->setFixedWidth(120);
    nLabel->setFixedWidth(18);
    mLabel->setFixedWidth(18);
    rLabel->setFixedWidth(18);
    aLabel->setFixedWidth(18);
//    scaleALabel->setFixedWidth(100);
//    scaleRLabel->setFixedWidth(100);

    aEdit->setOrientation(Qt::Horizontal);
    rEdit->setOrientation(Qt::Horizontal);
    
    
    scaleAEdit = new QLineEdit(functionConstantsBox);
    scaleREdit = new QLineEdit(functionConstantsBox);
    scaleAEdit->setFixedWidth(20);
    scaleREdit->setFixedWidth(20);
    scalePlaneEdit = new QPushButton(tr("Set on plane"), functionConstantsBox);
    
    coeffPlaneEdit = new QPushButton(tr("Set on plane"), functionConstantsBox);
    coeffPlanePopUp = new QWidget(this, Qt::Window);
    currTermEdit = new CustomSpinBox(functionConstantsBox);
    
    currTermEdit->setRange(1,1);
    
    // ORGANIZATIONAL ELEMENTS
    functionConstantsWidgetLayout = new QVBoxLayout(functionConstantsWidget);
    functionConstantsBoxLayout = new QVBoxLayout(functionConstantsBox);
    
    functionConstantsScalingTerms = new QHBoxLayout();
    functionConstantsTerm1 = new QHBoxLayout();
    functionConstantsFreqs = new QHBoxLayout();
    functionConstantsCoeffs = new QHBoxLayout();
    functionConstantsPairs = new QVBoxLayout();
    
    functionConstantsScalingTerms->addWidget(globalConsantsLabel);
    functionConstantsScalingTerms->addWidget(scaleRLabel);
    functionConstantsScalingTerms->addWidget(scaleREdit);
    functionConstantsScalingTerms->addWidget(scaleALabel);
    functionConstantsScalingTerms->addWidget(scaleAEdit);
    functionConstantsScalingTerms->addWidget(scalePlaneEdit);
    functionConstantsScalingTerms->setAlignment(Qt::AlignLeft);
    functionConstantsScalingTerms->addStretch();
    
    functionConstantsFreqs->addWidget(freqpairLabel);
    functionConstantsFreqs->addWidget(nLabel);
    functionConstantsFreqs->addWidget(nEdit);
    functionConstantsFreqs->addWidget(mLabel);
    functionConstantsFreqs->addWidget(mEdit);
    functionConstantsFreqs->setAlignment(Qt::AlignLeft);
    
    functionConstantsCoeffs->addWidget(coeffLabel);
    functionConstantsCoeffs->addWidget(rLabel);
    functionConstantsCoeffs->addWidget(rEdit);
    functionConstantsCoeffs->addWidget(rValueLabel);
    functionConstantsCoeffs->addWidget(aLabel);
    functionConstantsCoeffs->addWidget(aEdit);
    functionConstantsCoeffs->addWidget(aValueLabel);
    functionConstantsCoeffs->addWidget(coeffPlaneEdit);
    functionConstantsCoeffs->setAlignment(Qt::AlignLeft);
    
    termViewButton = new QPushButton(tr("View All Terms"), functionConstantsBox);
    termViewWidget = new QWidget(this, Qt::Window);
    termViewWidget->setWindowTitle(tr("Edit Function Terms"));
    termViewWidget->setMinimumWidth(340);
    termViewLayout = new QVBoxLayout(termViewWidget);
    
    termViewWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    termViewWidget->hide();
    termViewTable = new QTableWidget(termViewWidget);
    termViewTable->setRowCount(numTerms);
    termViewTable->setColumnCount(6);
    termViewLayout->addWidget(termViewTable);
    
    termViewHorizontalHeaders << tr("Term") << tr("m") << tr("n") << tr("a") << tr("r") << tr("");
    termViewTable->setHorizontalHeaderLabels(termViewHorizontalHeaders);
    termViewTable->verticalHeader()->hide();
    
    addTermButton = new QPushButton(tr("Add term..."));
    addTermButton->setIcon(*(new QIcon(":/Images/add.png")));
    termViewLayout->addWidget(addTermButton);
    
    // resize all columns to maximum stretch
    for (int c = 0; c < termViewTable->horizontalHeader()->count(); ++c)
    {
        if (c == 5)
            termViewTable->horizontalHeader()->resizeSection(5, 30);
        else
            termViewTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
        
    }
    
    // SIGNAL MAPPERS
    
    termViewTableMapper = new QSignalMapper(this);
    removeTermMapper = new QSignalMapper(this);
    
    addTerm();
    
    termViewHeaderVertical= termViewTable->verticalHeader();
    termViewHeaderVertical->resizeSections(QHeaderView::Stretch);
    
    termViewWidget->setLayout(termViewLayout);
    
   
    functionConstantsTerm1->addWidget(termViewButton);
    functionConstantsTerm1->addWidget(currTermLabel);
    functionConstantsTerm1->addWidget(currTermEdit);
    functionConstantsTerm1->setAlignment(Qt::AlignLeft);
    
    functionConstantsPairs->addLayout(functionConstantsFreqs);
    functionConstantsPairs->addLayout(functionConstantsCoeffs);
    functionConstantsTerm1->addLayout(functionConstantsPairs);
    
    functionConstantsBoxLayout->addLayout(functionConstantsScalingTerms);
    functionConstantsBoxLayout->addLayout(functionConstantsTerm1);
    
    
    functionConstantsWidgetLayout->addWidget(functionConstantsBox);
    
  
}

void interface::initPatternType()
{
    
    // patternType SUBELEMENTS
    patternTypeBox = new QGroupBox(tr("Pattern Type"), patternTypeWidget);
    functionSel = new QComboBox(patternTypeBox);        //initialize elements
    colorwheelSel = new QComboBox(patternTypeBox);
    functionLabel = new QLabel(patternTypeBox);
    colorwheelLabel = new QLabel(patternTypeBox);
    numTermsLabel = new QLabel(tr("<b>Number of Terms</b>"), patternTypeBox);
    numTermsEdit = new CustomSpinBox(patternTypeBox);
    numTermsEdit->setRange(1,4);
    gspacer1 = new QSpacerItem(0,20);
    gspacer2 = new QSpacerItem(0,10);
    gspacer3 = new QSpacerItem(0,10);
    gspacer4 = new QSpacerItem(0,50);
    setLoadedImage = new QPushButton(tr("Set Image..."), patternTypeBox);
    patternTypeBoxOverallLayout = new QVBoxLayout(patternTypeWidget);
    functionLayout = new QVBoxLayout();
    colorwheelLayout = new QVBoxLayout();
    numtermsLayout = new QHBoxLayout();
    
    functionSel->addItem("Hex3");
    functionSel->addItem("Hex6");
    functionSel->addItem("p4");
    functionSel->addItem("p2");
    functionSel->addItem("p1");
    functionSel->addItem("cmm");
    functionSel->addItem("p31m");
    functionSel->addItem("p3m1");
    functionSel->addItem("p6m");
    functionSel->addItem("p4g");
    functionSel->addItem("p4m");
    functionSel->addItem("pmm");
    functionSel->addItem("pmg");
    functionSel->addItem("pgg");
    functionSel->addItem("pm");
    functionSel->addItem("pg");
    functionSel->addItem("cm");
    functionSel->addItem("Cx Polyn");
    colorwheelSel->addItem("IcosColor");
    colorwheelSel->addItem("IcosColorC");
    colorwheelSel->addItem("StCol");
    colorwheelSel->addItem("StColC");
    colorwheelSel->addItem("StCol35");
    colorwheelSel->addItem("ZoneCol");
    colorwheelSel->addItem("SectCol");
    colorwheelSel->addItem("Sect6Col");
    colorwheelSel->addItem("WinCol");
    colorwheelSel->addItem("FromImage");
    functionLabel->setText(tr("Function"));
    colorwheelLabel->setText(tr("Color Wheel"));
    numTermsEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    numTermsLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    colorwheelSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    colorwheelLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    numTermsEdit->setFixedWidth(30);
    //numTermsEdit->setValidator(numtermsValidate);
    
    // ORGANIZATIONAL ELEMENTS
    patternTypeBoxLayout = new QVBoxLayout(patternTypeBox);
    functionLayout->addWidget(functionLabel);
    functionLayout->addWidget(functionSel);
    numtermsLayout->addWidget(numTermsLabel);
    numtermsLayout->addWidget(numTermsEdit);
    functionLayout->addLayout(numtermsLayout);
    patternTypeBoxLayout->addLayout(functionLayout);
    patternTypeBoxLayout->addItem(gspacer1);
    colorwheelLayout->addItem(gspacer3);
    colorwheelLayout->addWidget(colorwheelLabel);
    colorwheelLayout->addWidget(colorwheelSel);
    colorwheelLayout->addWidget(setLoadedImage);
    setLoadedImage->hide();
    patternTypeBoxLayout->addLayout(colorwheelLayout);
    patternTypeBoxLayout->addItem(gspacer2);
    
    patternTypeBoxOverallLayout->addWidget(patternTypeBox);
    patternTypeBoxOverallLayout->addStretch();
    
}



void interface::initViewHistory()
{
    
    viewHistoryBox = new QGroupBox(tr("History"), viewHistoryWidget);
    viewHistoryBoxOverallLayout = new QVBoxLayout(viewHistoryWidget);
    
    clearHistoryButton = new QPushButton(tr("Clear All History"), viewHistoryBox);
    viewHistoryBoxLayout = new QVBoxLayout(viewHistoryBox);

    viewHistoryBoxOverallLayout->addWidget(viewHistoryBox);
    viewHistoryBoxLayout->addWidget(clearHistoryButton);
    viewHistoryBoxOverallLayout->addStretch();
    
    viewMapper = new QSignalMapper(this);
    removeMapper = new QSignalMapper(this);
    
}


// TODO compress this...
void interface::initImageProps()
{
    outheightLabel = new QLabel(tr("Output Height"), imagePropsBox);
    outwidthLabel = new QLabel(tr("Output Width"), imagePropsBox);
    XCornerLabel = new QLabel(tr("XCorner"), imagePropsBox);
    YCornerLabel = new QLabel(tr("YCorner"), imagePropsBox);
    worldwidthLabel = new QLabel(tr("World Width"), imagePropsBox);
    worldheightLabel = new QLabel(tr("World Height"), imagePropsBox);
    outheightEdit = new QLineEdit(imagePropsBox);
    outwidthEdit = new QLineEdit(imagePropsBox);
    XCornerEdit = new QLineEdit(imagePropsBox);
    YCornerEdit = new QLineEdit(imagePropsBox);
    worldwidthEdit = new QLineEdit(imagePropsBox);
    worldheightEdit = new QLineEdit(imagePropsBox);
    pspacer1 = new QSpacerItem(0, 12);
    pspacer2 = new QSpacerItem(0, 14);
    pspacer3 = new QSpacerItem(15, 30);
    pspacer4 = new QSpacerItem(20, 40);
    pspacer5 = new QSpacerItem(20, 15);
    
    imagePropsBoxOverallLayout = new QVBoxLayout(imagePropsBox);
    imagePropsBoxLayout = new QHBoxLayout();
    imagePropsEditStack = new QVBoxLayout();
    imagePropsBoxStack = new QVBoxLayout();
    savePushLayout = new QHBoxLayout();
    
    outheightEdit->setValidator(dimValidate);
    outwidthEdit->setValidator(dimValidate);
    XCornerEdit->setValidator(doubleValidate);
    YCornerEdit->setValidator(doubleValidate);
    worldwidthEdit->setValidator(doubleValidate);
    worldheightEdit->setValidator(doubleValidate);
    
    outheightEdit->setFixedWidth(100);
    outwidthEdit->setFixedWidth(100);
    XCornerEdit->setFixedWidth(100);
    YCornerEdit->setFixedWidth(100);
    worldwidthEdit->setFixedWidth(100);
    worldheightEdit->setFixedWidth(100);
    
    imagePropsBoxStack->addWidget(XCornerLabel);
    imagePropsBoxStack->addWidget(YCornerLabel);
    imagePropsBoxStack->addWidget(worldwidthLabel);
    imagePropsBoxStack->addWidget(worldheightLabel);
    imagePropsBoxStack->addItem(pspacer1);
    imagePropsBoxStack->addWidget(outwidthLabel);
    imagePropsBoxStack->addWidget(outheightLabel);
    
    imagePropsBoxStack->setAlignment(XCornerLabel, Qt::AlignLeft);
    imagePropsBoxStack->setAlignment(YCornerLabel, Qt::AlignLeft);
    imagePropsBoxStack->setAlignment(worldwidthLabel, Qt::AlignLeft);
    imagePropsBoxStack->setAlignment(worldheightLabel, Qt::AlignLeft);
    imagePropsBoxStack->setAlignment(outwidthLabel, Qt::AlignLeft);
    imagePropsBoxStack->setAlignment(outheightLabel, Qt::AlignLeft);
    
    imagePropsEditStack->addWidget(XCornerEdit);
    imagePropsEditStack->addWidget(YCornerEdit);
    imagePropsEditStack->addWidget(worldwidthEdit);
    imagePropsEditStack->addWidget(worldheightEdit);
    imagePropsEditStack->addItem(pspacer2);
    imagePropsEditStack->addWidget(outwidthEdit);
    imagePropsEditStack->addWidget(outheightEdit);
    
    imagePropsBoxLayout->addLayout(imagePropsBoxStack);
    imagePropsBoxLayout->addLayout(imagePropsEditStack);
    imagePropsBoxLayout->addItem(pspacer4);
    
    imagePropsBoxOverallLayout->addLayout(imagePropsBoxLayout);
    
}

void interface::initCoeffPlane()
{
    //SET UP COEFFICIENT PLANE
    coeffPlanePopUpLayout = new QHBoxLayout();
    polarPlaneWithZoomLayout = new QVBoxLayout();
    polarCoordinatesBox = new QGroupBox(tr("Polar Cooridnates"), coeffPlanePopUp);
    polarCoordinatesLayout = new QVBoxLayout(polarCoordinatesBox);
    actionButtonLayout = new QHBoxLayout();
    zoomButtonLayout = new QHBoxLayout();
    
    coeffMapper = new QSignalMapper(this);
    
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
    
    confirmButton = new QPushButton(tr("OK"));
    resetButton = new QPushButton(tr("Reset"));
    zoomInButton = new QPushButton(QIcon(":/Images/zoomin.png"), "Zoom In");
    zoomInButton->setStyleSheet("QPushButton { text-align:center; padding:5px}");
    zoomOutButton = new QPushButton(QIcon(":/Images/zoomout.png"), "Zoom Out");
    zoomOutButton->setStyleSheet("QPushButton { text-align:center; padding:5px}");
    
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
    
    graphDisplay = new CoeffPlaneView(graph, coordinateSeries);
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
    polarCoordinatesLayout->addStretch(0);
    
    // coeffPlanePopUpLayout->addWidget(polarPlaneBox);
    
    polarCoordinatesLayout->setSizeConstraint(QLayout::SetFixedSize);
    
    zoomButtonLayout->addWidget(zoomInButton);
    zoomButtonLayout->addWidget(zoomOutButton);
    
    polarPlaneWithZoomLayout->addWidget(graphDisplay);
    polarPlaneWithZoomLayout->addLayout(zoomButtonLayout);
    
    coeffPlanePopUpLayout->addLayout(polarPlaneWithZoomLayout);
    coeffPlanePopUpLayout->addWidget(polarCoordinatesBox);
    
    coeffPlanePopUp->setLayout(coeffPlanePopUpLayout);
    
    coeffPlanePopUp->setFixedSize(1000, 600);
    
    
}

void interface::connectAllSignals()
{
    
    connect(termViewTable, SIGNAL(cellClicked(int, int)), this, SLOT(handleTermViewCellClicked(int, int)));
    
    connect(coeffPlaneEdit, SIGNAL(clicked()), coeffMapper, SLOT(map()));
    connect(scalePlaneEdit, SIGNAL(clicked()), coeffMapper, SLOT(map()));
    
    coeffMapper->setMapping(coeffPlaneEdit, LOCAL_FLAG);
    coeffMapper->setMapping(scalePlaneEdit, GLOBAL_FLAG);
    
    // CONNECT SIGNALS & SLOTS
    connect(toggleViewButton, SIGNAL(clicked()), this, SLOT(toggleViewMode()));
    connect(updatePreview, SIGNAL(clicked()), this, SLOT(updateSavePreview()));
    connect(exportImage, SIGNAL(clicked()), this, SLOT(saveImageStart()));
    connect(resetImage, SIGNAL(clicked()), this, SLOT(resetImageFunction()));
    
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), currColorWheel, SLOT(setCurrent(int)));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), this, SLOT(colorWheelChanged(int)));
    connect(setLoadedImage, SIGNAL(clicked()), this, SLOT(setImagePushed()));
    connect(functionSel, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFunction(int)));
    connect(numTermsEdit, SIGNAL(valueChanged(int)), this, SLOT(changeMaxTerms(int)));
    //connect(currTermEdit, SIGNAL(valueChanged(int)), this, SLOT(updateTerms(int)));
    connect(termViewButton, SIGNAL(clicked()), this, SLOT(termViewPopUp()));
    connect(addTermButton, SIGNAL(clicked()), this, SLOT(addNewTerm()));
    
    //    connect(nEdit, SIGNAL(valueChanged(int)), this, SLOT(changeN(int)));
    //    connect(mEdit, SIGNAL(valueChanged(int)), this, SLOT(changeM(int)));
    //    connect(rEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeR(double)));
    connect(radiusEdit, SIGNAL(editingFinished()), this, SLOT(updatePolarCoordinates()));
    //    connect(aEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeA(double)));
    connect(angleEdit, SIGNAL(editingFinished()), this, SLOT(updatePolarCoordinates()));
    connect(scaleREdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleR(QString)));
    connect(scaleAEdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleA(QString)));
    
    connect(outwidthEdit, SIGNAL(textChanged(QString)), this, SLOT(changeOWidth(QString)));
    connect(outheightEdit, SIGNAL(textChanged(QString)), this, SLOT(changeOHeight(QString)));
    connect(worldwidthEdit, SIGNAL(textChanged(QString)), this, SLOT(changeWorldWidth(QString)));
    connect(worldheightEdit, SIGNAL(textChanged(QString)), this, SLOT(changeWorldHeight(QString)));
    connect(XCornerEdit, SIGNAL(textChanged(QString)), this, SLOT(changeXCorner(QString)));
    connect(YCornerEdit, SIGNAL(textChanged(QString)), this, SLOT(changeYCorner(QString)));
    
     //connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromSettings()));
     //connect(saveButton, SIGNAL(clicked()), this, SLOT(saveCurrSettings()));
    
    connect(clearHistoryButton, SIGNAL(clicked()), this, SLOT(clearAllHistory()));
    
    connect(viewMapper, SIGNAL(mapped(QString)), this, SLOT(loadSettings(QString)));
    connect(removeMapper, SIGNAL(mapped(QObject*)), this, SLOT(removePreview(QObject*)));
    connect(coeffMapper,SIGNAL(mapped(int)), this, SLOT(showPlanePopUp(int)));
    
    connect(updatePreviewShortcut, SIGNAL(activated()), this, SLOT(updateSavePreview()));
    
    connect(coordinateSeries, SIGNAL(pointReplaced(int)), this, SLOT(updatePolarCoordinatesWithIndex(int)));
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(setPolarCoordinates()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetPolarCoordinates()));
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(polarPlaneZoomIn()));
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(polarPlaneZoomOut()));

}






QString interface::genLabel(const char *in)     //concatenate the constant name
{                                               //with the current index number
    QString out;
    out.setNum(termIndex+1);
    out.prepend(in);

    return out;
}

void interface::refreshLabels()                 //for updating our label names
{                                               //with the current term #
    nLabel->setText(genLabel("n"));
    mLabel->setText(genLabel("m"));
    rLabel->setText(genLabel("r"));
    aLabel->setText(genLabel("a"));
}

void interface::refreshTerms()
{
    for (int r = 0; r < numTerms; ++r) {
        
            
        QSpinBox *mEdit = static_cast<QSpinBox *>(termViewTable->cellWidget(r,1));
        QSpinBox *nEdit = static_cast<QSpinBox *>(termViewTable->cellWidget(r,2));
    
        
        QDoubleSpinBox *aEdit = static_cast<QDoubleSpinBox *>(termViewTable->cellWidget(r,3));
        QDoubleSpinBox *rEdit = static_cast<QDoubleSpinBox *>(termViewTable->cellWidget(r,4));
        
        mEdit->blockSignals(true);
        nEdit->blockSignals(true);
        aEdit->blockSignals(true);
        rEdit->blockSignals(true);
            
        unsigned int index = r;

        
        mEdit->setValue(currFunction->getN(index));
        
        nEdit->setValue(currFunction->getM(index));
                    
        aEdit->setValue(currFunction->getA(index));
                    
        rEdit->setValue(currFunction->getR(index));
        
        
        
        mEdit->blockSignals(false);
        nEdit->blockSignals(false);
        aEdit->blockSignals(false);
        rEdit->blockSignals(false);
            
    }
    
}

void interface::removeTerm(int row)
{
    
    for (int c = 0; c < termViewTable->columnCount(); ++c)
    {
        
        delete termViewTable->cellWidget(row, c);
        //        if (c < 1)
        //            termViewTable->setCellWidget(row, c, static_cast<QLabel *>(items[c]));
        //        else if (c < 3)
        //            termViewTable->setCellWidget(row, c, static_cast<QSpinBox *>(items[c]));
        //        else
        //            termViewTable->setCellWidget(row, c, static_cast<QDoubleSpinBox *>(items[c]));
    }
    
    termViewTable->removeRow(row);
    
    for (int r = row; r < termViewTable->rowCount(); ++r)
    {
        QLabel *term = static_cast<QLabel *>(termViewTable->cellWidget(r, 0));
        term->setText(QString::number(term->text().toInt() - 1));
    }
    
    
    qDebug() << "num rows: " << termViewTable->rowCount();
    
    
    termIndex--;
    numTerms--;
    unsigned int term = row;
    currFunction->removeTerm(term);
    currFunction->setNumTerms(numTerms);
    
}

void interface::addTerm()
{
    termIndex++;
    numTerms++;
    unsigned int highestIndex = termIndex;
    
    termViewTable->setRowCount(numTerms);
   
    currFunction->setNumTerms(numTerms);
    
    qDebug() << "adding term " << numTerms;
 
    QSpinBox *nEditTable = new QSpinBox();
    QSpinBox *mEditTable = new QSpinBox();
    QDoubleSpinBox *aEditTable = new QDoubleSpinBox();
    QDoubleSpinBox *rEditTable = new QDoubleSpinBox();
    
    nEditTable->setRange(-10,10);
    nEditTable->setSingleStep(1);
    mEditTable->setRange(-10,10);
    mEditTable->setSingleStep(1);
    aEditTable->setRange(-3.14, 3.14);
    aEditTable->setSingleStep(0.25);
    rEditTable->setRange(-10,10);
    rEditTable->setSingleStep(0.1);
    
    nEditTable->setAlignment(Qt::AlignCenter);
    mEditTable->setAlignment(Qt::AlignCenter);
    aEditTable->setAlignment(Qt::AlignCenter);
    rEditTable->setAlignment(Qt::AlignCenter);

    
    
    // for each term, add its components
    
    termViewTable->verticalHeader()->setSectionResizeMode(termIndex, QHeaderView::ResizeToContents);
    
    
    QLabel *termLabel = new QLabel(QString::number(termIndex + 1));
    termLabel->setAlignment(Qt::AlignCenter);
    termViewTable->setCellWidget(termIndex, 0, termLabel);
    
    termViewTable->setCellWidget(termIndex, 1, mEditTable);
    termViewTable->setCellWidget(termIndex, 2, nEditTable);
    termViewTable->setCellWidget(termIndex, 3, aEditTable);
    termViewTable->setCellWidget(termIndex, 4, rEditTable);
    
    QTableWidgetItem *removeTermItem = new QTableWidgetItem();
    removeTermItem->setIcon(QIcon(":/Images/remove.png"));
                                                       
    termViewTable->setItem(termIndex, 5, removeTermItem);
    qDebug() << "highest index: " << highestIndex;
    
  
    // set defaults
    
    mEditTable->setValue(currFunction->getN(highestIndex));
    nEditTable->setValue(currFunction->getM(highestIndex));
    aEditTable->setValue(currFunction->getA(highestIndex));
    rEditTable->setValue(currFunction->getR(highestIndex));
    
    connect(mEditTable, SIGNAL(valueChanged(int)), termViewTableMapper, SLOT(map()));
    connect(nEditTable, SIGNAL(valueChanged(int)), termViewTableMapper, SLOT(map()));
    connect(rEditTable, SIGNAL(valueChanged(double)), termViewTableMapper, SLOT(map()));
    connect(aEditTable, SIGNAL(valueChanged(double)), termViewTableMapper, SLOT(map()));
    
    
    //termViewTableMapper->setMapping(removeTerm, (QObject*)new QPoint(termIndex, 1));
    termViewTableMapper->setMapping(mEditTable, (QObject*)new QPoint(numTerms, 1));
    termViewTableMapper->setMapping(nEditTable, (QObject*)new QPoint(numTerms, 2));
    termViewTableMapper->setMapping(aEditTable, (QObject*)new QPoint(numTerms, 3));
    termViewTableMapper->setMapping(rEditTable, (QObject*)new QPoint(numTerms, 4));
    connect(termViewTableMapper, SIGNAL(mapped(QObject*)), this, SLOT(updateTermTable(QObject*)));
    
    qDebug() << "num terms: " << numTerms << "\nterm index: " << termIndex;
    
    currFunction->setNumTerms(numTerms);
    updatePreviewDisplay();
    
}


void interface::resetImageFunction()
{
    delete currFunction;

    currFunction = new hex3Function();
    functionSel->setCurrentIndex(0);
    colorwheelSel->setCurrentIndex(0);

    //termIndex = 0;

    refreshTerms();
    scaleREdit->setText(QString::number(currFunction->getScaleR()));
    scaleAEdit->setText(QString::number(currFunction->getScaleA()));

    outwidthEdit->setText(QString::number(DEFAULT_OUTPUT_WIDTH));
    outheightEdit->setText(QString::number(DEFAULT_OUTPUT_HEIGHT));
    worldwidthEdit->setText(QString::number(DEFAULT_WIDTH));
    worldheightEdit->setText(QString::number(DEFAULT_HEIGHT));
    XCornerEdit->setText(QString::number(DEFAULT_XCORNER));
    YCornerEdit->setText(QString::number(DEFAULT_YCORNER));

    updatePreviewDisplay();
}

void interface::toggleViewMode()
{
    if (advancedMode) {
        advancedMode = false;
        toggleViewButton->setText(tr("Switch to Advanced View"));
    } else {
        advancedMode = true;
        toggleViewButton->setText(tr("Switch to Default View"));
    }
}

void interface::termViewPopUp()
{
    if (!termViewWidget->isVisible()) {
        termViewWidget->show();
    }
    
}

void interface::addNewTerm() {
    // handles adding a new term to the termViewTable

    addTerm();
    
}

void interface::updateTerms(int i)
{
    termIndex = i;
    
    refreshTerms();
    refreshLabels();
}

void interface::changeMaxTerms(int i)
{
    
    if (i >= 1) {
        currTermEdit->setMaximum(i);
        //numTerms = i;
        for (int c = numTerms; c < i; ++c) {
            addTerm();
        }
        //currFunction->setNumTerms(i);
    }
    
}

void interface::colorWheelChanged(int index)
{
    if(index == 9)
        setLoadedImage->show();
    else
        setLoadedImage->hide();
}

void interface::setImagePushed()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    saveloadPath,
                               tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));

    QFile inFile(fileName);
    if (!inFile.open(QIODevice::ReadOnly))
             return;

    currColorWheel->loadImage(fileName);

    QDir stickypath(fileName);
    stickypath.cdUp();
    saveloadPath = stickypath.path();
}

void interface::changeFunction(int index)
{
    delete currFunction;

    switch(index)
    {
    case 0:
        currFunction = new hex3Function();
        colorwheelSel->setCurrentIndex(7);
        break;
    case 1:
        currFunction = new hex6Function();
        colorwheelSel->setCurrentIndex(7);
        break;
    case 2:
        currFunction = new squareFunction();
        colorwheelSel->setCurrentIndex(6);
        break;
    case 3:
        currFunction = new generalpairedFunction();
        colorwheelSel->setCurrentIndex(3);
        break;
    case 4:
        currFunction = new generalFunction();
        colorwheelSel->setCurrentIndex(7);
        break;
    case 5:
        currFunction = new cmmFunction();
        colorwheelSel->setCurrentIndex(7);
        break;
    case 6:
        currFunction = new p31mFunction();
        colorwheelSel->setCurrentIndex(1);
        break;
    case 7:
        currFunction = new p3m1Function();
        colorwheelSel->setCurrentIndex(1);
        break;
    case 8:
        currFunction = new p6mFunction();
        colorwheelSel->setCurrentIndex(6);
        break;
    case 9:
        currFunction = new p4gFunction();
        colorwheelSel->setCurrentIndex(6);
        break;
    case 10:
        currFunction = new p4mFunction();
        colorwheelSel->setCurrentIndex(6);
        break;
    case 11:
        currFunction = new pmmFunction();
        colorwheelSel->setCurrentIndex(9);
        break;
    case 12:
        currFunction = new pmgFunction();
        colorwheelSel->setCurrentIndex(9);
        break;
    case 13:
        currFunction = new pggFunction();
        colorwheelSel->setCurrentIndex(9);
        break;
    case 14:
        currFunction = new pmFunction();
        colorwheelSel->setCurrentIndex(9);
        break;
    case 15:
        currFunction = new pgFunction();
        colorwheelSel->setCurrentIndex(9);
        break;
    case 16:
        currFunction = new rhombicFunction();
        colorwheelSel->setCurrentIndex(9);
        break;
    case 17:
        currFunction = new zzbarFunction();
        colorwheelSel->setCurrentIndex(9);
        break;

    }

    refreshTerms();
}

// SLOT function called only when user attempts to save current settings into a wpr file
void interface::saveCurrSettings()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                saveloadPath,
                                                tr("Wallpapers (*.wpr)"));
    
    saveloadPath = saveSettings(fileName) != nullptr ? saveSettings(fileName) : nullptr;

}

// internal function that handles saving settings
QString interface::saveSettings(const QString &fileName) {
    
    QFile outFile(fileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return nullptr;
    }
    QDataStream out(&outFile);
    out << settings->Width << settings->Height;
    out << settings->XCorner << settings->YCorner;
    out << settings->OWidth << settings->OHeight;
    out << functionSel->currentIndex();
    out << colorwheelSel->currentIndex();
    out << currFunction->getScaleR() << currFunction->getScaleA();
    out << currFunction->numterms();
    
    for(unsigned int i=0; i<currFunction->numterms(); i++) 
    {
        out << currFunction->getN(i) << currFunction->getM(i) << currFunction->getR(i) << currFunction->getA(i);
    }
    outFile.close();
    
    QDir stickypath(fileName);
    stickypath.cdUp();

    return stickypath.absolutePath();
    
}

// SLOT function called only when user attempts to load from saved settings stored in a wpr file
void interface::loadFromSettings()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                               saveloadPath,
                                               tr("Wallpapers (*.wpr)"));
    
    saveloadPath = loadSettings(fileName) != nullptr ? saveSettings(fileName) : nullptr;
}

// internal function that handles loading settings from a specified file
QString interface::loadSettings(const QString &fileName) {
    
    QFile inFile(fileName);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return nullptr;
    
    QDataStream in(&inFile);
    int tempint;
    unsigned int count;
    double tempdouble;
    
    in >> settings->Width >> settings->Height;
    in >> settings->XCorner >> settings->YCorner;
    in >> settings->OWidth >> settings->OHeight;
    in >> tempint;
    functionSel->setCurrentIndex(tempint);
    in >> tempint;
    colorwheelSel->setCurrentIndex(tempint);
    
    in >> tempdouble;
    currFunction->setScaleR(tempdouble);
    scaleREdit->setText(QString::number(tempdouble));
    in >> tempdouble;
    currFunction->setScaleA(tempdouble);
    scaleAEdit->setText(QString::number(tempdouble));
    in >> count;
    numTermsEdit->setValue(count);
    for(unsigned int i=0; i<count; i++)
    {
        in >> tempint; currFunction->setN(i, tempint);
        in >> tempint; currFunction->setM(i, tempint);
        in >> tempdouble; currFunction->setR(i, tempdouble);
        in >> tempdouble; currFunction->setA(i, tempdouble);
    }
    
    inFile.close();
    
    refreshTerms();
    outwidthEdit->setText(QString::number(settings->OWidth));
    outheightEdit->setText(QString::number(settings->OHeight));
    worldwidthEdit->setText(QString::number(settings->Width));
    worldheightEdit->setText(QString::number(settings->Height));
    XCornerEdit->setText(QString::number(settings->XCorner));
    YCornerEdit->setText(QString::number(settings->YCorner));
    updatePreviewDisplay();
    
    QDir stickypath(fileName);
    stickypath.cdUp();
    return stickypath.path();
}


void interface::addToHistory()
{    
    HistoryItem *item = new HistoryItem();

    QVBoxLayout *historyItemsWithLabelLayout = new QVBoxLayout();
    QHBoxLayout *historyItemsLayout = new QHBoxLayout();
    QVBoxLayout *historyItemsButtonsLayout = new QVBoxLayout();
    Display *d = new Display(60, viewHistoryBox);
    QPushButton *viewButton = new QPushButton(tr("View"), viewHistoryBox);
    QPushButton *removeButton = new QPushButton(tr("Remove"), viewHistoryBox);
    QLabel *timeStampLabel = new QLabel(viewHistoryBox);
      
    QDateTime savedTime = QDateTime::currentDateTimeUtc();

    QString newFile = savedTime.toString("MM.dd.yyyy.hh.mm.ss.zzz.t").append(".wpr");
    
    QString savedTimeLabel = "Saved: " + savedTime.toString("MM/dd/yyyy") + " at " + savedTime.toString("hh:mm:ss");

    timeStampLabel->setText(savedTimeLabel);

    historyItemsLayout->addWidget(d);
    historyItemsButtonsLayout->addWidget(viewButton);
    historyItemsButtonsLayout->addWidget(removeButton);
    historyItemsLayout->addLayout(historyItemsButtonsLayout);
    historyItemsWithLabelLayout->addLayout(historyItemsLayout);
    historyItemsWithLabelLayout->addWidget(timeStampLabel);
    viewHistoryBoxLayout->insertLayout(1, historyItemsWithLabelLayout);

    //saving all values to history item
    item->preview = d;
    item->savedTime = savedTime;
    item->layoutWithLabelItem = historyItemsWithLabelLayout;
    item->layoutItem = historyItemsLayout;
    item->buttonLayoutItem = historyItemsButtonsLayout;
    item->viewButton = viewButton;
    item->removeButton = removeButton;
    item->labelItem = timeStampLabel;
    item->filePathName = saveSettings(newFile).append("/" + newFile);

    qDebug() << item->filePathName;    

    connect(viewButton, SIGNAL(clicked()), viewMapper, SLOT(map()));
    connect(removeButton, SIGNAL(clicked()), removeMapper, SLOT(map()));

    viewMapper->setMapping(viewButton, newFile);
    removeMapper->setMapping(removeButton, item);
    
    // this handles the painting of the preview icon
    Port *historyDisplay = new Port(currFunction, currColorWheel, item->preview->dim(), item->preview->dim(), settings);
    historyDisplay->paintHistoryIcon(item);
    
//    double worldY, worldX;
//    
//    for (int y = 0; y < item->preview->dim(); y++)
//    {
//        for (int x = 0; x <= ((item->preview->dim())-1); x++)
//        {
//            worldY= settings->Height-y*settings->Height/item->preview->dim()+settings->YCorner;
//            worldX= x*settings->Width/item->preview->dim()+settings->XCorner;
//            
//            //run the point through our mathematical function
//            //then convert that complex output to a color according to our color wheel
//            std::complex<double> fout = (*currFunction)(worldX,worldY);  
//            QRgb color = (*currColorWheel)(fout);                          
//            
//            //finally push the determined color to the corresponding point on the display
//            item->preview->setPixel(x, y, color);                    
//        }
//    }
//    item->preview->repaint();    

    historyItemsMap.insert(savedTime, item);
    
}

void interface::removePreview(QObject *item)
{
    HistoryItem *historyItemToRemove = qobject_cast<HistoryItem*>(item);
    
    historyItemToRemove->buttonLayoutItem->removeWidget(historyItemToRemove->viewButton);
    delete historyItemToRemove->viewButton;

    historyItemToRemove->buttonLayoutItem->removeWidget(historyItemToRemove->removeButton);
    delete historyItemToRemove->removeButton;

    historyItemToRemove->layoutItem->removeItem(historyItemToRemove->buttonLayoutItem);
    delete historyItemToRemove->buttonLayoutItem;
    
    historyItemToRemove->layoutItem->removeWidget(historyItemToRemove->preview);
    delete historyItemToRemove->preview;

    historyItemToRemove->layoutWithLabelItem->removeItem(historyItemToRemove->layoutItem);
    delete historyItemToRemove->layoutItem;

    historyItemToRemove->layoutWithLabelItem->removeWidget(historyItemToRemove->labelItem);
    delete historyItemToRemove->labelItem;
    
    viewHistoryBoxLayout->removeItem(historyItemToRemove->layoutWithLabelItem);
    delete historyItemToRemove->layoutWithLabelItem;

    historyItemsMap.erase(historyItemsMap.find(historyItemToRemove->savedTime));

    QFile::remove(historyItemToRemove->filePathName);
}

void interface::updatePreviewDisplay()
{
    
    Port *previewDisplay = new Port(currFunction, currColorWheel, disp->dim(), disp->dim(), settings);
    
    previewDisplay->paintToDisplay(disp);
}

void interface::updateSavePreview()
{

    // qDebug() << "Updating Preview";

    if (historyItemsMap.size() < MAX_HISTORY_ITEMS) {
        addToHistory();
    } else {
        removePreview(*(historyItemsMap.begin()));
        addToHistory();      
    }
    
    updatePreviewDisplay();
    
}

void interface::clearAllHistory()
{
    QMap<QDateTime, HistoryItem*>::iterator it;
    
    for (it = historyItemsMap.begin(); it != historyItemsMap.end(); ++it) {
        removePreview(it.value());
    }
}

void interface::changeOHeight(const QString &val)
{
    settings->OHeight = val.toInt();
}

void interface::changeOWidth(const QString &val)
{
    settings->OWidth = val.toInt();
}

void interface::changeWorldHeight(const QString &val)
{
    settings->Height = val.toDouble();
}

void interface::changeWorldWidth(const QString &val)
{
    settings->Width = val.toDouble();
}

void interface::changeXCorner(const QString &val)
{
    settings->XCorner = val.toDouble();
}

void interface::changeYCorner(const QString &val)
{
    settings->YCorner = val.toDouble();
}

//void interface::changeN(int val)
//{
//    // int passedval = val.toInt();
//    currFunction->setN(termIndex, val);
//    // nValueLabel->setText(QString::number(val));
//    updatePreviewDisplay();
//}
//
//void interface::changeM(int val)
//{
//    // int passedval = val.toInt();
//    currFunction->setM(termIndex, val);
//    // mValueLabel->setText(QString::number(val));
//    updatePreviewDisplay();
//}
//
//void interface::changeR(double val)
//{
//    // double passedval = val.toDouble();
//    currFunction->setR(termIndex, val);
//    rValueLabel->setText(QString::number(val));
//    updatePreviewDisplay();
//}
//
//void interface::changeA(double val)
//{
//    // double passedval = val.toDouble();
//    currFunction->setA(termIndex, val);
//    aValueLabel->setText(QString::number(val));
//    updatePreviewDisplay();
//}

void interface::changeScaleA(const QString &val)
{
    double passedval = val.toDouble();
    currFunction->setScaleA(passedval);
}

void interface::changeScaleR(const QString &val)
{
    double passedval = val.toDouble();
    currFunction->setScaleR(passedval);
}

void interface::saveImageStart()
{
    
    
    Port *imageExport = new Port(currFunction, currColorWheel, settings->OWidth, settings->OHeight, settings);
    
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    saveloadPath,
                                                    tr("PPM (*.ppm);;JPEG (*.jpg *.jpeg)"));
    
    QFile inFile(fileName);
    if (!inFile.open(QIODevice::WriteOnly))
        return;

    double worldY, worldX ;
    QImage outputImage(settings->OWidth, settings->OHeight, QImage::Format_RGB32);

    double worldYPreCalculations1 = settings->Height + settings->YCorner;
    double worldYPreCalculations2 = settings->Height/settings->OHeight;
    double worldXPreCalculations = settings->Width/settings->OWidth;

    for (int y = 0; y < settings->OHeight; y++)
    {
        for (int x = 0; x <= ((settings->OWidth)-1); x++)
        {   
            // worldY = settings->Height - y*settings->Height/settings->OHeight + settings->YCorner;
            // worldX = x*settings->Width/settings->OWidth + settings->XCorner;

            worldY = worldYPreCalculations1 - y * worldYPreCalculations2;
            worldX = x * worldXPreCalculations + settings->XCorner;

            //run the point through our mathematical function
            //...then turn that complex output into a color per our color wheel
            clock_t start, end;
            double cpu_time_used;

            start = clock();
            std::complex<double> fout=(*currFunction)(worldX,worldY);
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            // qDebug() << "function: " << cpu_time_used;

            start = clock();
            QRgb color = (*currColorWheel)(fout);
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            // qDebug() << "function: " << cpu_time_used;  

            outputImage.setPixel(x, y, color);
        }
    }

    outputImage.save(fileName);

    QDir stickypath(fileName);
    stickypath.cdUp();
    saveloadPath = stickypath.path();
        return;
    
    QImage *output = new QImage(settings->OWidth, settings->OHeight, QImage::Format_RGB32);
    
    imageExport->exportImage(output, fileName);
}

void interface::errorHandler(const int &flag) 
{   
    switch(flag) 
    {
    case INVALID_FILE_ERROR:
        errorMessageBox->setText("Invalid file name/path");
        errorMessageBox->exec();
            break;
    }
}

void interface::showPlanePopUp(int flag)
{
    
    coeffFlag = flag;

    double tempA, tempR = 0;

    if (coeffFlag == LOCAL_FLAG)
    {
        tempA = currFunction->getA(termIndex);
        tempR = currFunction->getR(termIndex);
        
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
    coeffPlanePopUp->show();
    // hide();
}

void interface::updatePolarCoordinatesWithIndex(const int &index)
{
    QPointF point = coordinateSeries->at(index);
    // qDebug() << "update info " << point;
    updatePolarCoordinates(point);
}

void interface::updatePolarCoordinates()
{
    double tempA = angleEdit->text().toDouble();
    double tempR = radiusEdit->text().toDouble();

    coordinateSeries->replace(0, QPointF(tempR * cos(tempA), tempR * sin(tempA)));
}

void interface::setPolarCoordinates()
{
    if (coeffFlag == LOCAL_FLAG)
    {
        rEdit->setValue(radiusEdit->text().toDouble() * 100);
        aEdit->setValue(angleEdit->text().toDouble() * 100);
        rValueLabel->setText(radiusEdit->text());
        aValueLabel->setText(angleEdit->text());
    }
    else if (coeffFlag == GLOBAL_FLAG)
    {
        scaleREdit->setText(radiusEdit->text());
        scaleAEdit->setText(angleEdit->text());
    }
    coeffPlanePopUp->hide();
}

void interface::resetPolarCoordinates()
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

void interface::updatePolarCoordinates(QPointF point)
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

void interface::polarPlaneZoomOut()
{
    series->insert(0, QPointF(series->at(0).x() * 2, series->at(0).y() * 2));
    series->append(QPointF(series->at(series->count() - 1).x() * 2, series->at(series->count() - 1).y() * 2));

    series2->insert(0, QPointF(series2->at(0).x() * 2, series2->at(0).y() * 2));
    series2->append(QPointF(series2->at(series2->count() - 1).x() * 2, series2->at(series2->count() - 1).y() * 2));

    graph->zoomOut();
}

void interface::polarPlaneZoomIn()
{
    series->replace(0, QPointF(series->at(0).x() / 2, series->at(0).y() / 2));
    series->replace(series->count() - 1, QPointF(series->at(series->count() - 1).x() / 2, series->at(series->count() - 1).y() / 2));

    series2->replace(0, QPointF(series2->at(0).x() / 2, series2->at(0).y() / 2));
    series2->replace(series->count() - 1, QPointF(series2->at(series2->count() - 1).x() / 2, series2->at(series2->count() - 1).y() / 2));

    graph->zoomIn();
}


void interface::handleTermViewCellClicked(int row, int col)
{
    
    qDebug() << "got the signal";
    
    if (col == 5)
        removeTerm(row);
//    else if (col == 1)
//        termIndex = row;
//        refreshTerms();
    
  
}


void interface::updateTermTable(QObject *cell)
{
    
    
    int row = ((QPoint *)cell)->x();
    int col = ((QPoint *)cell)->y();
    termIndex = row;

    float val;
    
    if (col < 3)
        val = static_cast<QSpinBox *>(termViewTable->cellWidget(row,col))->value();
    else
        val = static_cast<QDoubleSpinBox *>(termViewTable->cellWidget(row,col))->value();
    
    unsigned int index = row;
    int coeff = val;
    double freq = val;
    
    if (col == 1)
            currFunction->setN(index, coeff);
    else if (col == 2)
            currFunction->setM(index, coeff);
    else if (col == 3)
            currFunction->setA(index, freq);
    else if (col == 4)
            currFunction->setR(index, freq);
            

    updatePreviewDisplay();
}


