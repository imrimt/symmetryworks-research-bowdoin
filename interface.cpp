#include "Interface.h"

Interface::Interface(QWidget *parent) : QWidget(parent)
{
    
    // FUNCTIONAL VARIABLES

    termIndex = 0;
    saveloadPath = QDir::homePath();
    
    // FUNCTIONAL OBJECTS
    functionVector.push_back(new hex3Function());
    functionVector.push_back(new hex6Function());
    functionVector.push_back(new squareFunction());
    functionVector.push_back(new generalpairedFunction());
    functionVector.push_back(new generalFunction());
    functionVector.push_back(new cmmFunction());
    functionVector.push_back(new p31mFunction());
    functionVector.push_back(new p3m1Function());
    functionVector.push_back(new p6mFunction());
    functionVector.push_back(new p4gFunction());
    functionVector.push_back(new p4mFunction());
    functionVector.push_back(new pmmFunction());
    functionVector.push_back(new pmgFunction());
    functionVector.push_back(new pggFunction());
    functionVector.push_back(new pmFunction());
    functionVector.push_back(new pgFunction());
    functionVector.push_back(new rhombicFunction());
    functionVector.push_back(new zzbarFunction());
    currFunction = functionVector[0];
    currColorWheel = new ColorWheel();

    numTerms = currFunction->getNumTerms();
    
    // DEFAULT SETTINGS
    settings = new Settings;
    settings->Width = DEFAULT_WORLD_WIDTH;
    settings->Height = DEFAULT_WORLD_HEIGHT;
    settings->XCorner = DEFAULT_XCORNER;
    settings->YCorner = DEFAULT_YCORNER;
    settings->OWidth = DEFAULT_OUTPUT_WIDTH;
    settings->OHeight = DEFAULT_OUTPUT_HEIGHT;
    
    // INITIALIZE LAYOUT
    initInterfaceLayout();
    
    functionSel->setCurrentIndex(0);
    colorwheelSel->setCurrentIndex(0);

    
}


void Interface::initInterfaceLayout()
{
    
    // ORGANIZATIONAL ELEMENTS
    overallLayout = new QHBoxLayout();
    InterfaceLayout = new QVBoxLayout();
    topbarLayout = new QHBoxLayout();
    leftbarLayout = new QVBoxLayout();
    
    imagePropsBox = new QGroupBox(tr("Image Properties"), this);
    displayWidget = new QWidget(this);
    patternTypeBox = new QGroupBox(tr("Pattern Properties"),this);
    globalScalingBox = new QGroupBox(tr("Global Scaling Factors"), this);
    functionConstantsWidget = new QWidget(this);
    
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    spacerItem = new QSpacerItem(1, screenGeometry.height() * 0.02);
    
    leftbarLayout->addWidget(imagePropsBox);
    leftbarLayout->addItem(spacerItem);
    leftbarLayout->addWidget(patternTypeBox);
    leftbarLayout->addItem(spacerItem);
    leftbarLayout->addWidget(globalScalingBox);

    topbarLayout->addLayout(leftbarLayout);
    topbarLayout->addWidget(displayWidget);
    
    InterfaceLayout->addLayout(topbarLayout);
    InterfaceLayout->addWidget(functionConstantsWidget);
    
    
    overallLayout->addLayout(InterfaceLayout);
    overallLayout->activate();
    this->setLayout(overallLayout);
    
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

    // INIT UI SUBELEMENTS
    initPreviewDisplay();
    initPatternType();
    initGlobalScaling();
    initFunctionConstants();

    polarPlane = new PolarPlane(currFunction, &termIndex, this);
    polarPlaneMapper = new QSignalMapper(this);
    
    initImageProps();
    initImageExportPopUp();
    
    connectAllSignals();

    refreshTableTerms();
    updatePreviewDisplay();

}



// INIT PREVIEW DISPLAY ELEMENTS
void Interface::initPreviewDisplay()
{
    
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    previewWidth = screenGeometry.width() * PREVIEW_SCALING;
    previewHeight = screenGeometry.height() * PREVIEW_SCALING;
    previewSize = previewWidth > previewHeight ? previewWidth : previewHeight;

    disp = new Display(previewSize, previewSize, displayWidget);
    updatePreview = new QPushButton(tr("Snapshot"), this);
    exportImage = new QPushButton(tr("Export..."), this);
    resetButton = new QPushButton(tr("Reset"), this);
    dispLayout = new QVBoxLayout(displayWidget);
    //dispLayout->setSizeConstraint(QLayout::SetMinimumSize);
    buttonLayout = new QHBoxLayout();
    
    buttonLayout->addWidget(updatePreview);
    buttonLayout->addWidget(exportImage);
    buttonLayout->addWidget(resetButton);
    
    //SET UP SHORTCUTS...add for save, open, undo?
    updatePreviewShortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
    
    imageExportPort = new Port(currFunction, currColorWheel, settings->OWidth, settings->OHeight, settings);
    previewDisplayPort = new Port(currFunction, currColorWheel, disp->width(), disp->height(), settings);
    
    displayProgressBar = new ProgressBar(tr("Preview"), true, previewDisplayPort);
    exportProgressBar = new ProgressBar(tr("Exporting"), false, imageExportPort);
    // connect(previewDisplayPort->getControllerObject(), SIGNAL(progressChanged(int)), displayProgressBar, SLOT(update(int)));
    connect(previewDisplayPort->getControllerObject(), SIGNAL(partialProgressChanged(double)), displayProgressBar, SLOT(partialUpdate(double)));
    connect(previewDisplayPort, SIGNAL(paintingFinished(bool)), this, SLOT(resetMainWindowButton(bool)));
    connect(displayProgressBar, SIGNAL(renderFinished()), this, SLOT(resetTableButton()));
    connect(imageExportPort, SIGNAL(finishedExport(QString)), this, SLOT(popUpImageExportFinished(QString)));
    connect(imageExportPort->getControllerObject(), SIGNAL(partialProgressChanged(double)), exportProgressBar, SLOT(partialUpdate(double)));

    qRegisterMetaType<ComplexValue>("ComplexValue");
    connect(previewDisplayPort->getControllerObject(), SIGNAL(newImageDataPoint(ComplexValue)), this, SLOT(addNewImageDataPoint(ComplexValue)));

    dispLayout->setAlignment(disp, Qt::AlignCenter);
    dispLayout->addWidget(disp);
    dispLayout->addLayout(displayProgressBar->layout);
    dispLayout->addLayout(buttonLayout);
    dispLayout->addStretch();
}

// INIT FUNCTION CONSTANTS EDIT BOX
void Interface::initFunctionConstants()
{

    functionConstantsBox = new QGroupBox(tr("Function Parameters"), functionConstantsWidget);
    functionConstantsBoxLayout = new QVBoxLayout(functionConstantsBox);
    functionTermsGrid = new QGridLayout();
    
    QFrame* currTermLine = new QFrame(functionConstantsBox);
    currTermLine->setLineWidth(2);
    currTermLine->setMidLineWidth(1);
    currTermLine->setFrameShape(QFrame::VLine);
    currTermLine->setFrameShadow(QFrame::Raised);
    
    currTermLabel = new QLabel(functionConstantsBox);
    currTermLabel->setText(tr("<b>Current Term:<\b>"));
    currTermEdit = new CustomSpinBox(functionConstantsBox);
    currTermEdit->setRange(1, numTerms);
    
    freqpairLabel = new QLabel(tr("Frequency Pair: "), functionConstantsBox);
    coeffLabel = new QLabel(tr("Coefficient Pair: "), functionConstantsBox);
    
    nLabel = new QLabel(functionConstantsBox);
    mLabel = new QLabel(functionConstantsBox);
    aLabel = new QLabel(functionConstantsBox);
    rLabel = new QLabel(functionConstantsBox);
    aValueLabel = new QLabel(functionConstantsBox);
    rValueLabel = new QLabel(functionConstantsBox);
    
    nEdit = new QSpinBox(functionConstantsBox);
    mEdit = new QSpinBox(functionConstantsBox);
    aEdit = new QDoubleSlider(functionConstantsBox);
    rEdit = new QDoubleSlider(functionConstantsBox);
    
    nEdit->setFixedWidth(100);
    mEdit->setFixedWidth(100);
    rEdit->setFixedWidth(100);
    aEdit->setFixedWidth(100);
    nEdit->setRange(-10,10);
    nEdit->setSingleStep(1);
    mEdit->setRange(-10,10);
    mEdit->setSingleStep(1);
    aEdit->setRange(-314, 314);
    aEdit->setSingleStep(25);
    rEdit->setRange(0,1000);
    rEdit->setSingleStep(1);
    
    nEdit->setAlignment(Qt::AlignCenter);
    mEdit->setAlignment(Qt::AlignCenter);
    aEdit->setOrientation(Qt::Horizontal);
    rEdit->setOrientation(Qt::Horizontal);
    
    nLabel->setFixedWidth(18);
    mLabel->setFixedWidth(18);
    rLabel->setFixedWidth(18);
    aLabel->setFixedWidth(18);
    aValueLabel->setFixedWidth(35);
    rValueLabel->setFixedWidth(35);
    
    functionConstantsWidgetLayout = new QVBoxLayout(functionConstantsWidget);
    
    functionConstantsScalingTerms = new QHBoxLayout();
    termEditLayout = new QHBoxLayout();
    functionConstantsFreqs = new QHBoxLayout();
    functionConstantsCoeffs = new QHBoxLayout();
    functionConstantsPairs = new QVBoxLayout();

    coeffPlaneEdit = new QPushButton(tr("Set on plane"), functionConstantsBox);
    
    numTermsLabel = new QLabel(tr("<b>Total Number of Terms: <\b>"), functionConstantsBox);
    numTermsEdit = new QSpinBox(functionConstantsBox);
    numTermsEdit->setValue(1);
    numTermsEdit->setRange(1, MAX_NUM_TERMS);

    QFrame* line1 = new QFrame(functionConstantsBox);
    line1->setLineWidth(2);
    line1->setMidLineWidth(1);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Raised);

    QFrame* line2 = new QFrame(functionConstantsBox);
    line2->setLineWidth(2);
    line2->setMidLineWidth(1);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Raised);
    
    termViewButton = new QPushButton(tr("View/Edit All Terms"), functionConstantsBox);
    termViewWidget = new QWidget(this, Qt::Window);
    termViewWidget->setWindowTitle(tr("Edit Function Terms"));
    termViewWidget->setMinimumWidth(340);
    termViewLayout = new QVBoxLayout(termViewWidget);
    
    termViewWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    termViewWidget->hide();
    termViewTable = new QTableWidget(termViewWidget);
    termViewTable->setColumnCount(6);
    
    termViewHorizontalHeaders << tr("Term") << tr("m") << tr("n") << tr("a") << tr("r") << tr("");
    termViewTable->setHorizontalHeaderLabels(termViewHorizontalHeaders);
    termViewTable->verticalHeader()->hide();
    
    addTermButton = new QPushButton(tr("Add term..."));
    addTermButton->setIcon(*(new QIcon(":/Images/Icons/add.png")));
    
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
    
    //add initial term
    addTerm();
    
    termViewHeaderVertical= termViewTable->verticalHeader();
    termViewHeaderVertical->resizeSections(QHeaderView::Stretch);
    
    // ASSEMBLE LAYOUT
    termViewLayout->addWidget(termViewTable);
    termViewLayout->addWidget(addTermButton);
    termViewWidget->setLayout(termViewLayout);

    for (int r = 0; r < 5; r++) {
        switch(r) {
        case 0: 
            functionTermsGrid->addWidget(numTermsLabel, r, 0, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(numTermsEdit, r, 1, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(termViewButton, 0, 2, 1, 1, Qt::AlignCenter);
            // functionTermsGrid->setVerticalSpacing(20);
            break;
        case 1:
            functionTermsGrid->addWidget(line1, r, 0, 1, 10);
            break;
        case 2:
            functionTermsGrid->addWidget(currTermLabel, r, 0, 2, 1, Qt::AlignRight);
            functionTermsGrid->addWidget(currTermEdit, r, 1, 2, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(currTermLine, r, 2, 2, 1);
            functionTermsGrid->addWidget(freqpairLabel, r, 2, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(nLabel, r, 3, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(nEdit, r, 4, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(mLabel, r, 6, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(mEdit, r, 7, 1, 1, Qt::AlignCenter);
            break;
        case 3:
            functionTermsGrid->addWidget(coeffLabel, r, 2, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(rLabel, r, 3, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(rEdit, r, 4, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(rValueLabel, r, 5, 1, 1, Qt::AlignLeft);
            functionTermsGrid->addWidget(aLabel, r, 6, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(aEdit, r, 7, 1, 1, Qt::AlignCenter);
            functionTermsGrid->addWidget(aValueLabel, r, 8, 1, 1, Qt::AlignLeft);
            functionTermsGrid->addWidget(coeffPlaneEdit, r, 9, 1, 1, Qt::AlignCenter);
            break;
        case 4: 
            functionTermsGrid->addWidget(line2, r, 0, 1, 10);
        }
    }

    functionConstantsBoxLayout->addLayout(functionTermsGrid);
    functionConstantsWidgetLayout->addWidget(functionConstantsBox);

}

// INIT GLOBAL SCALING FACTORS BOX
void Interface::initGlobalScaling()
{
    
    globalScalingBoxLayout = new QVBoxLayout(globalScalingBox);

    scaleRLayout = new QHBoxLayout();
    scaleALayout = new QHBoxLayout();

    scaleALabel = new QLabel(tr("Scaling Angle"), globalScalingBox);
    scaleRLabel = new QLabel(tr("Scaling Radius"), globalScalingBox);
    scaleAEditSlider = new QDoubleSlider(globalScalingBox);
    scaleREditSlider = new QDoubleSlider(globalScalingBox);
    scaleAEdit = new QLineEdit(patternTypeBox);
    scaleREdit = new QLineEdit(patternTypeBox);
    scaleAEdit->setValidator(doubleValidate);
    scaleREdit->setValidator(doubleValidate);
    scaleAEdit->setFixedWidth(40);
    scaleREdit->setFixedWidth(40);
    scaleAEdit->setAlignment(Qt::AlignCenter);
    scaleREdit->setAlignment(Qt::AlignCenter);
    scaleAEditSlider->setOrientation(Qt::Horizontal);
    scaleREditSlider->setOrientation(Qt::Horizontal);
    scaleREditSlider->setRange(0, 500);
    scaleREditSlider->setSingleStep(1);
    scaleAEditSlider->setRange(-314,314);
    scaleAEditSlider->setSingleStep(1);
    scaleREditSlider->setFixedWidth(100);
    scaleAEditSlider->setFixedWidth(100);
    scalePlaneEdit = new QPushButton(tr("Set on plane"), globalScalingBox);
    
    
    scaleRLayout->addWidget(scaleRLabel);
    scaleRLayout->addWidget(scaleREditSlider);
    scaleRLayout->addWidget(scaleREdit);
    scaleALayout->addWidget(scaleALabel);
    scaleALayout->addWidget(scaleAEditSlider);
    scaleALayout->addWidget(scaleAEdit);

    globalScalingBoxLayout->addLayout(scaleRLayout);
    globalScalingBoxLayout->addLayout(scaleALayout);
    globalScalingBoxLayout->addWidget(scalePlaneEdit);
    
    scaleREdit->setText(QString::number(currFunction->getScaleR()));
    scaleAEdit->setText(QString::number(currFunction->getScaleA()));
    scaleREditSlider->setValue(currFunction->getScaleR() * 100.0);
    scaleAEditSlider->setValue(currFunction->getScaleA() * 100.0);
    
    
}

// INIT PATTERN TYPE BOX
void Interface::initPatternType()
{

    functionSel = new QComboBox(patternTypeBox);
    colorwheelSel = new QComboBox(patternTypeBox);

    gspacer1 = new QSpacerItem(0,20);
    gspacer2 = new QSpacerItem(0,10);
    gspacer3 = new QSpacerItem(0,10);
    gspacer4 = new QSpacerItem(0,50);
    gspacer5 = new QSpacerItem(0,10);

    patternTypeBoxLayout = new QVBoxLayout(patternTypeBox);
    functionLayout = new QHBoxLayout();
    colorwheelLayout = new QHBoxLayout();
    fromImageLayout = new QHBoxLayout();
    

    setLoadedImage = new QPushButton(tr("Set/Change Image..."), patternTypeBox);
    fromImageButton = new QRadioButton(tr("Image"), patternTypeBox);
    fromColorWheelButton = new QRadioButton(tr("Color Wheel"), patternTypeBox);
    // showImageDataGraphButton = new QPushButton(tr("Show Graph"), patternTypeBox);
    functionLabel = new QLabel(patternTypeBox);
    colorwheelLabel = new QLabel(patternTypeBox);
    imagePathLabel = new QLabel(patternTypeBox);
    
    QFrame* endPattern = new QFrame(patternTypeBox);
    endPattern->setLineWidth(2);
    endPattern->setMidLineWidth(1);
    endPattern->setFrameShape(QFrame::HLine);
    endPattern->setFrameShadow(QFrame::Raised);
    
    fromColorWheelButton->setChecked(true);
    setLoadedImage->setEnabled(false);

    if (imageSetPath == "") {
        imagePathLabel->setText("<i>(no image has been set)</i>");
    }
    
    // function selector
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
    functionSel->addItem("Original");
    
    // color wheel selector
    colorwheelSel->addItem("IcosColor");
    colorwheelSel->addItem("IcosColorC");
    colorwheelSel->addItem("StCol");
    colorwheelSel->addItem("StColC");
    colorwheelSel->addItem("StCol35");
    colorwheelSel->addItem("ZoneCol");
    colorwheelSel->addItem("SectCol");
    colorwheelSel->addItem("Sect6Col");
    colorwheelSel->addItem("WinCol");
    functionLabel->setText(tr("<b>Pattern<\b>"));
    colorwheelLabel->setText(tr("<b>Color<\b>"));
    
//    colorwheelSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
//    colorwheelLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    functionSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    functionLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    //initialize function previews window
    functionIconsWindow = new QWidget(this, Qt::Window);
    functionIconsWindow->setWindowTitle(tr("Pattern Previews"));
    functionIconsWindowLayout = new QGridLayout(functionIconsWindow);
    
    int row = 0;
    int col = 0;
    
    for (int i = 0; i < functionSel->count(); ++i) {
        QVBoxLayout *layout = new QVBoxLayout();
        
        QPixmap pixmap;
        pixmap.convertFromImage(*(new QImage(":/Images/function-previews/" + functionSel->itemText(i)+ ".png")));
        ClickableLabel *preview = new ClickableLabel(i, functionIconsWindow);
        
        connect(preview, SIGNAL(doubleClickFunctionIcon(int)), functionSel, SLOT(setCurrentIndex(int)));
        
        preview->setFixedWidth(100);
        preview->setPixmap(pixmap);
        
        QLabel *label = new QLabel(functionSel->itemText(i), functionIconsWindow);
        layout->addWidget(preview);
        layout->addWidget(label);
        
        functionIconsWindowLayout->addLayout(layout, row, col);
    
        col++;
        if (col % 5 == 0) {
            row++;
            col = 0;
        }

    }

    viewFunctionIconsButton = new QPushButton(tr("Previews..."), patternTypeBox);
    connect(viewFunctionIconsButton, SIGNAL(clicked()), this, SLOT(showFunctionIcons()));

    //initialize overflow color window
    setOverflowColorPopUp = new QColorDialog();
    // setOverflowColorButton = new QPushButton(tr("Set Overflow Color..."), patternTypeBox);

    //initialize image data window
    imageDataWindow = new QWidget(this, Qt::Window);
    imageDataWindowGraphLayout = new QVBoxLayout();
    imageDataWindowLayout = new QHBoxLayout(imageDataWindow);    
    imageDataSeries = new QScatterSeries(this);
    prevDataSeries = new QScatterSeries(this);
    imageDataGraphAxisX = new QValueAxis();
    imageDataGraphAxisY = new QValueAxis();
    imageDataGraph = new QChart();
    imageDataGraphView = new QChartView(imageDataGraph);
    updateImageDataGraphButton = new QPushButton(tr("Update Graph"), imageDataGraphView);
    imageLabel = new QLabel(imageDataWindow);

    imageDataGraph->legend()->hide();
    imageDataGraph->setTitle("SAMPLE POINTS ON IMAGE");

    //adjust data points size & color
    QPen pointPen(Qt::red);
    QBrush pointBrush(Qt::red);
    prevDataSeries->setPen(pointPen);
    prevDataSeries->setBrush(pointBrush);
    prevDataSeries->setMarkerSize(5.0);

    //adjust axis settings
    imageDataGraphAxisX->setRange(-2, 2);
    imageDataGraphAxisY->setRange(-2, 2);
    imageDataGraphAxisX->setTickCount(9);
    imageDataGraphAxisY->setTickCount(9);

    imageDataGraph->addSeries(prevDataSeries);
    imageDataGraph->addAxis(imageDataGraphAxisX, Qt::AlignBottom);
    imageDataGraph->addAxis(imageDataGraphAxisY, Qt::AlignLeft);
    prevDataSeries->attachAxis(imageDataGraphAxisX);
    prevDataSeries->attachAxis(imageDataGraphAxisY);

    //set size policies
    // imageDataGraphView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    // imageLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // imageDataWindowLayout->addStretch();

    imageDataWindowGraphLayout->addWidget(imageDataGraphView);
    imageDataWindowGraphLayout->addWidget(updateImageDataGraphButton);
    imageDataWindowLayout->addLayout(imageDataWindowGraphLayout);
    imageDataWindowLayout->addWidget(imageLabel);
    imageDataWindow->setLayout(imageDataWindowLayout);
    imageDataWindow->setFixedSize(previewSize * 2, previewSize);

    // imageDataWindow->show();

    // set image buttons false
    // setOverflowColorButton->setEnabled(false);
    // showImageDataGraphButton->setEnabled(false);
    // emit imageActionStatus(false);
    
    // ASSEMBLE LAYOUT

    functionLayout->addWidget(functionLabel);
    functionLayout->addWidget(functionSel);    
    functionLayout->addWidget(viewFunctionIconsButton);
    // functionLayout->addLayout(patternTitleLayout);
    // functionLayout->addWidget(viewFunctionIconsButton);
    patternTypeBoxLayout->addLayout(functionLayout);
    patternTypeBoxLayout->addWidget(endPattern);
    patternTypeBoxLayout->addWidget(colorwheelLabel);
    
    colorwheelLayout->addWidget(fromColorWheelButton);
    colorwheelLayout->addWidget(colorwheelSel);
    fromImageLayout->addWidget(fromImageButton);
    fromImageLayout->addWidget(setLoadedImage);
    
    patternTypeBoxLayout->addLayout(colorwheelLayout);
    patternTypeBoxLayout->addLayout(fromImageLayout);
    patternTypeBoxLayout->addWidget(imagePathLabel);
    
    
}


// INIT IMAGE PROPERTIES BOX
void Interface::initImageProps()
{
    imagePropsBoxLayout = new QVBoxLayout(imagePropsBox);
    
    imageShiftXLayout = new QHBoxLayout();
    imageShiftYLayout = new QHBoxLayout();
    
    XShiftLabel = new QLabel(tr("Horizontal Shift"), imagePropsBox);
    YShiftLabel = new QLabel(tr("Vertical Shift"), imagePropsBox);
    XShiftEditSlider = new QDoubleSlider(imagePropsBox);
    YShiftEditSlider = new QDoubleSlider(imagePropsBox);
    XShiftEdit = new QLineEdit(imagePropsBox);
    YShiftEdit = new QLineEdit(imagePropsBox);
    XShiftEdit->setValidator(doubleValidate);
    YShiftEdit->setValidator(doubleValidate);
    XShiftEdit->setFixedWidth(40);
    YShiftEdit->setFixedWidth(40);
    XShiftEdit->setAlignment(Qt::AlignCenter);
    YShiftEdit->setAlignment(Qt::AlignCenter);
    
    imageShiftXLayout->addWidget(XShiftLabel);
    imageShiftXLayout->addWidget(XShiftEditSlider);
    imageShiftXLayout->addWidget(XShiftEdit);
    
    imageShiftYLayout->addWidget(YShiftLabel);
    imageShiftYLayout->addWidget(YShiftEditSlider);
    imageShiftYLayout->addWidget(YShiftEdit);
    
    imageStretchXLayout = new QHBoxLayout();
    imageStretchYLayout = new QHBoxLayout();
    
    worldWidthLabel = new QLabel(tr("Horizontal Stretch"), imagePropsBox);
    worldHeightLabel = new QLabel(tr("Vertical Stretch"), imagePropsBox);
    
    worldWidthEditSlider = new QDoubleSlider(imagePropsBox);
    worldHeightEditSlider = new QDoubleSlider(imagePropsBox);
    worldWidthEdit = new QLineEdit(imagePropsBox);
    worldHeightEdit = new QLineEdit(imagePropsBox);
    worldWidthEdit->setValidator(doubleValidate);
    worldHeightEdit->setValidator(doubleValidate);
    worldWidthEdit->setFixedWidth(40);
    worldHeightEdit->setFixedWidth(40);
    worldWidthEdit->setAlignment(Qt::AlignCenter);
    worldHeightEdit->setAlignment(Qt::AlignCenter);
    
    imageStretchXLayout->addWidget(worldWidthLabel);
    imageStretchXLayout->addWidget(worldWidthEditSlider);
    imageStretchXLayout->addWidget(worldWidthEdit);
    
    imageStretchYLayout->addWidget(worldHeightLabel);
    imageStretchYLayout->addWidget(worldHeightEditSlider);
    imageStretchYLayout->addWidget(worldHeightEdit);
    
    XShiftEditSlider->setFixedWidth(100);
    YShiftEditSlider->setFixedWidth(100);
    XShiftEditSlider->setRange(-200, 150);
    XShiftEditSlider->setSingleStep(1);
    YShiftEditSlider->setRange(-200,150);
    YShiftEditSlider->setSingleStep(1);
    XShiftEditSlider->setOrientation(Qt::Horizontal);
    YShiftEditSlider->setOrientation(Qt::Horizontal);
    
    XShiftEdit->setText(QString::number(settings->XCorner));
    YShiftEdit->setText(QString::number(settings->YCorner));
    XShiftEditSlider->setValue(settings->XCorner * 100.0);
    YShiftEditSlider->setValue(settings->YCorner * 100.0);
    
    worldWidthEditSlider->setFixedWidth(100);
    worldHeightEditSlider->setFixedWidth(100);
    worldWidthEditSlider->setRange(0, 800);
    worldWidthEditSlider->setSingleStep(1);
    worldHeightEditSlider->setRange(0,800);
    worldHeightEditSlider->setSingleStep(1);
    worldWidthEditSlider->setOrientation(Qt::Horizontal);
    worldHeightEditSlider->setOrientation(Qt::Horizontal);
    
    worldWidthEdit->setText(QString::number(settings->Width));
    worldHeightEdit->setText(QString::number(settings->Height));
    worldWidthEditSlider->setValue(settings->Width * 100.0);
    worldHeightEditSlider->setValue(settings->Height * 100.0);
    
    // ASSEMBLE LAYOUT
    imagePropsBoxLayout->addLayout(imageShiftXLayout);
    imagePropsBoxLayout->addLayout(imageShiftYLayout);
    imagePropsBoxLayout->addLayout(imageStretchXLayout);
    imagePropsBoxLayout->addLayout(imageStretchYLayout);
    

}

void Interface::initImageExportPopUp()
{
    // IMAGE DIMENSIONS POP UP WINDOW
    settingsPopUp = new QWidget(this, Qt::Window);
    settingsPopUp->setWindowModality(Qt::WindowModal);
    settingsPopUp->setWindowTitle(tr("Image Settings"));
    settingsPopUpLayout = new QVBoxLayout(settingsPopUp);
   // settingsPopUp->setMinimumWidth(300);
    
    outHeightEdit = new QLineEdit(settingsPopUp);
    outWidthEdit = new QLineEdit(settingsPopUp);
    
    outWidthLayout = new QHBoxLayout();
    outWidthLabel = new QLabel(tr("Image Width"));
    outWidthLayout->addWidget(outWidthLabel);
    outWidthLayout->addWidget(outWidthEdit);
    outWidthLayout->addWidget(new QLabel(tr("pixels")));
    
    
    outHeightLayout = new QHBoxLayout();
    outHeightLabel = new QLabel(tr("Image Height"));
    outHeightLayout->addWidget(outHeightLabel);
    outHeightLayout->addWidget(outHeightEdit);
    outHeightLayout->addWidget(new QLabel(tr("pixels")));
    
    
    settingsPopUpLayout->addLayout(outWidthLayout);
    settingsPopUpLayout->addLayout(outHeightLayout);
    
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    settingsPopUpLayout->addWidget(buttonBox);
    
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(startImageExport()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancelImageExport()));
    
    outWidthEdit->setText(QString::number(DEFAULT_OUTPUT_WIDTH));
    outHeightEdit->setText(QString::number(DEFAULT_OUTPUT_HEIGHT));
   
    settingsPopUp->hide();

}

// CONNECT SIGNALS TO SLOTS
void Interface::connectAllSignals()
{
    
    connect(termViewTable, SIGNAL(cellClicked(int, int)), this, SLOT(termViewCellClicked(int, int)));
    
    connect(coeffPlaneEdit, SIGNAL(clicked()), polarPlaneMapper, SLOT(map()));
    connect(scalePlaneEdit, SIGNAL(clicked()), polarPlaneMapper, SLOT(map()));
    polarPlaneMapper->setMapping(coeffPlaneEdit, LOCAL_FLAG);
    polarPlaneMapper->setMapping(scalePlaneEdit, GLOBAL_FLAG);
    connect(polarPlaneMapper,SIGNAL(mapped(int)), polarPlane, SLOT(showPlanePopUp(int)));

   // connect(toggleViewButton, SIGNAL(clicked()), this, SLOT(toggleViewMode()));
    connect(updatePreview, SIGNAL(clicked()), this, SLOT(snapshotFunction()));
    connect(exportImage, SIGNAL(clicked()), this, SLOT(exportImageFunction()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetFunction()));
    
    connect(fromColorWheelButton, SIGNAL(clicked()), this, SLOT(selectColorWheel()));
    connect(fromImageButton, SIGNAL(clicked()), this, SLOT(selectImage()));
    
    // connect(setOverflowColorButton, SIGNAL(clicked()), this, SLOT(showOverflowColorPopUp()));
    connect(setOverflowColorPopUp, SIGNAL(colorSelected(QColor)), currColorWheel, SLOT(changeOverflowColor(QColor)));
    connect(setOverflowColorPopUp, SIGNAL(accepted()), this, SLOT(selectImage()));

    connect(functionSel, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFunction(int)));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), currColorWheel, SLOT(setCurrent(int)));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), this, SLOT(colorWheelChanged(int)));
    
    connect(setLoadedImage, SIGNAL(clicked()), this, SLOT(setImagePushed()));
    // connect(showImageDataGraphButton, SIGNAL(clicked()), this, SLOT(showImageDataGraph()));
    connect(updateImageDataGraphButton, SIGNAL(clicked()), this, SLOT(updateImageDataGraph()));
    connect(numTermsEdit, SIGNAL(valueChanged(int)), this, SLOT(changeNumTerms(int)));
    connect(currTermEdit, SIGNAL(valueChanged(int)), this, SLOT(updateCurrTerm(int)));
    connect(termViewButton, SIGNAL(clicked()), this, SLOT(termViewPopUp()));
    connect(addTermButton, SIGNAL(clicked()), this, SLOT(addTermTable()));
    
    connect(nEdit, SIGNAL(valueChanged(int)), this, SLOT(changeN(int)));
    connect(mEdit, SIGNAL(valueChanged(int)), this, SLOT(changeM(int)));
    connect(rEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeR(double)));
    connect(aEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeA(double)));
    connect(scaleREdit, SIGNAL(returnPressed()), this, SLOT(changeScaleR()));
    connect(scaleAEdit, SIGNAL(returnPressed()), this, SLOT(changeScaleA()));

    connect(scaleREditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeScaleR(double)));
    connect(scaleAEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeScaleA(double)));
    
    connect(outWidthEdit, SIGNAL(editingFinished()), this, SLOT(changeOWidth()));
    connect(outHeightEdit, SIGNAL(editingFinished()), this, SLOT(changeOHeight()));
    
    connect(worldWidthEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeWorldWidth(double)));
    connect(worldHeightEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeWorldHeight(double)));
    connect(worldWidthEdit, SIGNAL(returnPressed()), this, SLOT(changeWorldWidth()));
    connect(worldHeightEdit, SIGNAL(returnPressed()), this, SLOT(changeWorldHeight()));
    
    connect(XShiftEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeXCorner(double)));
    connect(YShiftEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeYCorner(double)));
    connect(XShiftEdit, SIGNAL(returnPressed()), this, SLOT(changeXCorner()));
    connect(YShiftEdit, SIGNAL(returnPressed()), this, SLOT(changeYCorner()));
    
    connect(polarPlane, SIGNAL(setPolarCoordinates(int, QString, QString)), this, SLOT(setPolarCoordinates(int, QString, QString)));
    
    connect(updatePreviewShortcut, SIGNAL(activated()), this, SLOT(snapshotFunction()));
    
}


// concatenate constant name with current index number
QString Interface::genLabel(const char *in)
{
    QString out;
    out.setNum(termIndex + 1);
    out.prepend(in);
    
    return out;
}

// update main window label names with appropriate text
void Interface::refreshLabels()
{
    nLabel->setText(genLabel("n"));
    mLabel->setText(genLabel("m"));
    rLabel->setText(genLabel("r"));
    aLabel->setText(genLabel("a"));
}

// update term view table with appropriate values
void Interface::refreshTableTerms()
{

    numTerms = currFunction->getNumTerms();
    

    if (termViewTable->rowCount() == 0) {
        for (int i = 0; i < numTerms; ++i) {     
            addTerm();
        }
    }    
    
    // refresh all terms in term table
    for (int r = 0; r < numTerms; ++r) {
        
        QSpinBox *mEdit = static_cast<QSpinBox *>(termViewTable->cellWidget(r,1));
        QSpinBox *nEdit = static_cast<QSpinBox *>(termViewTable->cellWidget(r,2));
        QDoubleSpinBox *aEdit = static_cast<QDoubleSpinBox *>(termViewTable->cellWidget(r,3));
        QDoubleSpinBox *rEdit = static_cast<QDoubleSpinBox *>(termViewTable->cellWidget(r,4));
        
        // block spinBox signals while setting the values internally
        mEdit->blockSignals(true);
        nEdit->blockSignals(true);
        aEdit->blockSignals(true);
        rEdit->blockSignals(true);
        
        unsigned int index = r;

        aValueLabel->setText(QString::number(currFunction->getA(index)));
        rValueLabel->setText(QString::number(currFunction->getR(index)));
        
        mEdit->setValue(currFunction->getM(index));
        nEdit->setValue(currFunction->getN(index));
        aEdit->setValue(currFunction->getA(index));
        rEdit->setValue(currFunction->getR(index));
        
        mEdit->blockSignals(false);
        nEdit->blockSignals(false);
        aEdit->blockSignals(false);
        rEdit->blockSignals(false);
        
    }
    
}

// update main window term editor with appropriate values
void Interface::refreshMainWindowTerms()
{

   // qDebug() << "in refreshMainWindowTerms";

    currTermEdit->blockSignals(true);
    numTermsEdit->blockSignals(true);

    currTermEdit->setValue(termIndex + 1);
    
    numTermsEdit->setValue(currFunction->getNumTerms());

    currTermEdit->blockSignals(false);
    numTermsEdit->blockSignals(false);
  
    mEdit->blockSignals(true);
    nEdit->blockSignals(true);
    aEdit->blockSignals(true);
    rEdit->blockSignals(true);
    scaleAEdit->blockSignals(true);
    scaleREdit->blockSignals(true);
    
    mEdit->setValue(currFunction->getM(termIndex));
    nEdit->setValue(currFunction->getN(termIndex));
    aEdit->setValue(currFunction->getA(termIndex) * 100);
    rEdit->setValue(currFunction->getR(termIndex) * 100);
    scaleAEdit->setText(QString::number(currFunction->getScaleA()));
    scaleREdit->setText(QString::number(currFunction->getScaleR()));
    
    mEdit->blockSignals(false);
    nEdit->blockSignals(false);
    aEdit->blockSignals(false);
    rEdit->blockSignals(false);
    scaleAEdit->blockSignals(false);
    scaleREdit->blockSignals(false);

    aValueLabel->setText(QString::number(currFunction->getA(termIndex)));
    rValueLabel->setText(QString::number(currFunction->getR(termIndex)));
    
    refreshLabels(); // update labels with appropriate text
    
}

// removes a term
void Interface::removeTerm(int row)
{

    if (termViewTable->rowCount() == 1) {
        return;
    }
    
    for (int c = 0; c < termViewTable->columnCount(); ++c) {
        delete termViewTable->cellWidget(row, c);
    }
    
    termViewTable->removeRow(row);
    
    for (int r = row; r < termViewTable->rowCount(); ++r) {
        QLabel *term = static_cast<QLabel *>(termViewTable->cellWidget(r, 0));
        term->setText(QString::number(term->text().toInt() - 1));
    }
    
    if (static_cast<int>(termIndex) > termViewTable->rowCount()) {
        termIndex = termViewTable->rowCount() - 1;
    }

    unsigned int term = row;
    //qDebug() << "removing term " << row << "from function";
    currFunction->removeTerm(term);
    numTerms = currFunction->getNumTerms();

    numTermsEdit->blockSignals(true);
    numTermsEdit->setValue(numTerms);
    numTermsEdit->blockSignals(false);
    
    // qDebug() << "num terms is now" << currFunction->getNumTerms() << "num terms" << numTerms;
    
    currTermEdit->blockSignals(true);
    currTermEdit->setMaximum(currFunction->getNumTerms());
    currTermEdit->blockSignals(false);

}


// adds a new term
void Interface::addTerm()
{
    unsigned int highestIndex = termViewTable->rowCount();
    termViewTable->setRowCount(highestIndex + 1);
    
    //qDebug() << "highest index" << highestIndex;

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
    
    termViewTable->verticalHeader()->setSectionResizeMode(highestIndex, QHeaderView::ResizeToContents); 
    
    QLabel *termLabel = new QLabel(QString::number(highestIndex + 1));
    termLabel->setAlignment(Qt::AlignCenter);
    termViewTable->setCellWidget(highestIndex, 0, termLabel);
    termViewTable->setCellWidget(highestIndex, 1, mEditTable);
    termViewTable->setCellWidget(highestIndex, 2, nEditTable);
    termViewTable->setCellWidget(highestIndex, 3, aEditTable);
    termViewTable->setCellWidget(highestIndex, 4, rEditTable);

    QTableWidgetItem *removeTermItem = new QTableWidgetItem();
    removeTermItem->setIcon(QIcon(":/Images/Icons/remove.png"));
    removeTermItem->setFlags(removeTermItem->flags() ^ Qt::ItemIsEditable);
    
    termViewTable->setItem(highestIndex, 5, removeTermItem);

    // set defaults
    mEditTable->setValue(currFunction->getM(highestIndex));
    nEditTable->setValue(currFunction->getN(highestIndex));
    aEditTable->setValue(currFunction->getA(highestIndex));
    rEditTable->setValue(currFunction->getR(highestIndex));
    
    // connect signals
    connect(mEditTable, SIGNAL(valueChanged(int)), termViewTableMapper, SLOT(map()));
    connect(nEditTable, SIGNAL(valueChanged(int)), termViewTableMapper, SLOT(map()));
    connect(rEditTable, SIGNAL(valueChanged(double)), termViewTableMapper, SLOT(map()));
    connect(aEditTable, SIGNAL(valueChanged(double)), termViewTableMapper, SLOT(map()));
    
    termViewTableMapper->setMapping(mEditTable, (QObject*)new QPoint(highestIndex, 1));
    termViewTableMapper->setMapping(nEditTable, (QObject*)new QPoint(highestIndex, 2));
    termViewTableMapper->setMapping(aEditTable, (QObject*)new QPoint(highestIndex, 3));
    termViewTableMapper->setMapping(rEditTable, (QObject*)new QPoint(highestIndex, 4));
    connect(termViewTableMapper, SIGNAL(mapped(QObject*)), this, SLOT(updateTermTable(QObject*)));

    refreshMainWindowTerms();
    // updatePreviewDisplay();
    // addTermButton->blockSignals(false);
    
}


// reset the image to its default settings with the current function and colorwheel
void Interface::resetFunction()
{
    termIndex = 0;

    currFunction->refresh();
    numTermsEdit->setValue(1);
    
    // scaleREdit->setText(QString::number(currFunction->getScaleR()));
    // scaleAEdit->setText(QString::number(currFunction->getScaleA()));
    // scaleREditSlider->setValue(currFunction->getScaleR() * 100.0);
    // scaleAEditSlider->setValue(currFunction->getScaleA() * 100.0);

    changeScaleR(currFunction->getScaleR());
    changeScaleA(currFunction->getScaleA());
    
    changeWorldHeight(DEFAULT_WORLD_HEIGHT);
    changeWorldWidth(DEFAULT_WORLD_WIDTH);
    changeXCorner(DEFAULT_XCORNER);
    changeYCorner(DEFAULT_YCORNER);
    
    currColorWheel->changeOverflowColor(Qt::black);
    
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
    
}


void Interface::termViewPopUp()
{
    termViewWidget->hide();
    termViewWidget->show();
}

// updates the term that is currently being edited
void Interface::updateCurrTerm(int i)
{
    
    if (i > 0) termIndex = i - 1;
    
    refreshTableTerms();
    refreshMainWindowTerms();

}

// updates the number of terms of the current function
void Interface::changeNumTerms(int i)
{

    if (!numTermsEdit->hasFocus()) numTermsEdit->setFocus();

    numTermsEdit->setEnabled(false);

    int oldNumTerms = numTerms;

    //qDebug() << "old:" << oldNumTerms << "| new:" << i;

    if (i < oldNumTerms) {
        for (int k = oldNumTerms; k > i; --k) { removeTerm(k-1); }
    } else if (i > oldNumTerms) {
        currFunction->setNumTerms(i);
        numTerms = currFunction->getNumTerms();
        for (int k = oldNumTerms; k < i; ++k) addTerm();
        currTermEdit->blockSignals(true);
        currTermEdit->setMaximum(i);
        currTermEdit->blockSignals(false);
    }    

    updateCurrTerm(i);
    updatePreviewDisplay();
}

// handles changing to a new color wheel
void Interface::colorWheelChanged(int /* unused */ )
{
    // if(index == 9)
    //     setLoadedImage->show();
    // else
    //     setLoadedImage->hide();
    updatePreviewDisplay();
}

void Interface::selectColorWheel() 
{
    colorwheelSel->setEnabled(true);
    setLoadedImage->setEnabled(false);
    // setOverflowColorButton->setEnabled(false);
    // showImageDataGraphButton->setEnabled(false);
    emit imageActionStatus(false);
    imagePathLabel->setEnabled(false);

    imageDataWindow->hide();

    currColorWheel->setCurrent(colorwheelSel->currentIndex());

    updatePreviewDisplay();
}

void Interface::selectImage()
{
    colorwheelSel->setEnabled(false);
    setLoadedImage->setEnabled(true);
    // setOverflowColorButton->setEnabled(true);
    // showImageDataGraphButton->setEnabled(true);
    emit imageActionStatus(true);
    imagePathLabel->setEnabled(true);

    if (imageSetPath == "") {
        setImagePushed();
    }
    else {
        QDir checkDir(imageSetPath);
        if (!checkDir.exists(openImageName)) {
            errorHandler(INVALID_FILE_ERROR);
        }
        else {
            currColorWheel->setCurrent(9);
            currColorWheel->loadImage(imageSetPath + "/" + openImageName);
            imagePathLabel->setText(openImageName);
            updatePreviewDisplay();
        }
    }
}

// handles loading an image to use as a color wheel
void Interface::setImagePushed()
{

    QString startingPath = imageSetPath == "" ? "/Documents" : imageSetPath;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    startingPath,
                                                    tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));

    QFile inFile(fileName);
    if (fileName == "") {
        return;
    }

    if (!inFile.exists()){
        errorHandler(INVALID_FILE_ERROR);
        return;
    }

    qDebug() << "opening file:" << fileName;
    if (!inFile.open(QIODevice::ReadOnly)) {
        return;
    }
    
    currColorWheel->setCurrent(9);
    currColorWheel->loadImage(fileName);
    
    QDir stickypath(fileName);
    stickypath.cdUp();
    imageSetPath = stickypath.path();

    openImageName = fileName.right(fileName.length() - fileName.lastIndexOf("/") - 1);

    imagePathLabel->setText(openImageName);

    updatePreviewDisplay();
}

// handles changing to a new function
void Interface::changeFunction(int index)
{

    //qDebug() << "change Function";
    
    termIndex = 0;
    
    currFunction = functionVector[index];
    
    numTerms = currFunction->getNumTerms();

    //qDebug() << "numTerms:" << numTerms;

    termViewTable->setRowCount(0);

    currTermEdit->setMaximum(numTerms);
    
    previewDisplayPort->changeFunction(currFunction);
    imageExportPort->changeFunction(currFunction);
    
    refreshMainWindowTerms();
    refreshTableTerms();
    updatePreviewDisplay();

}


// SLOT function called when user attempts to save current settings into a wpr file
void Interface::saveCurrSettings()
{
    
    qDebug() << "saving setting";

    QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                    saveloadPath,
                                                    tr("Wallpapers (*.wpr)"));
    
    saveloadPath = saveSettings(fileName) != nullptr ? saveSettings(fileName) : nullptr;
    
}

// internal function that handles saving settings
QString Interface::saveSettings(const QString &fileName) {
    
    QFile outFile(fileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return nullptr;
    }
    
    // QDataStream out(&outFile);
    // out << settings->Width << settings->Height;
    // out << settings->XCorner << settings->YCorner;
    // out << settings->OWidth << settings->OHeight;
    // out << functionSel->currentIndex();
    // if (fromColorWheelButton->isChecked()) {
    //     out << colorwheelSel->currentIndex();
    // }
    // else {
    //     out << 9;
    // }
    // out << currFunction->getScaleR() << currFunction->getScaleA();
    // out << currFunction->getNumTerms();
    
    // unsigned int i;

    // for(int index = 0; index < currFunction->getNumTerms(); index++)
    // {
    //     i = index;
    //     out << currFunction->getN(i) << currFunction->getM(i) << currFunction->getR(i) << currFunction->getA(i);
    // }

    // out << imageSetPath;
    // out << openImageName;

   	QTextStream out(&outFile);
   	out << "Horizontal Stretch: " << QString::number(settings->Width) << endl;
   	out << "Vertical StretcH: " << QString::number(settings->Height) << endl;
   	out << "Horizontal Shift: " << QString::number(settings->XCorner) << endl;
   	out << "Vertical Shift: " << QString::number(settings->YCorner) << endl;
    out << "Output Width: " << QString::number(settings->OWidth) << endl;
    out << "Output Height: " << QString::number(settings->OHeight) << endl;
   	out << "Function Type: " << "Wallpapers" << endl;
   	out << "Function: " << functionSel->currentText() << endl;

   	if (fromColorWheelButton->isChecked()) {
   		out << "Color Type: Colorwheel" << endl;
        out << "Colorwheel: " << colorwheelSel->currentText();
    }
    else {
        out << "Color Type: Image" << endl;
        out << "Image Path: " << imageSetPath;
        out << "Image Name: " << openImageName;
    }

    out << endl;

    out << "Scaling Radius: " << QString::number(currFunction->getScaleR()) << endl;
    out << "Scaling Angle: " << QString::number(currFunction->getScaleA()) << endl;

    unsigned int i;
    QString tabString(PARAMETER_SEPARATOR_LENGTH, ' ');

    out << "NUMBERS OF TERMS: " << currFunction->getNumTerms() << endl;

    for(int index = 0; index < currFunction->getNumTerms(); index++)
    {
        i = index;
        out << "Term " << i << " : " << tabString
        << "N: " << QString::number(currFunction->getN(i)) << tabString 
        << "M: " << QString::number(currFunction->getM(i)) << tabString 
        << "R: " << QString::number(currFunction->getR(i)) << tabString 
        << "A: " << QString::number(currFunction->getA(i)) << endl;
        // out << currFunction->getN(i) << currFunction->getM(i) << currFunction->getR(i) << currFunction->getA(i);
    }    

    outFile.setPermissions(QFile::ReadOther);

    outFile.close();
    
    QDir stickypath(fileName);
    stickypath.cdUp();
    
    return stickypath.absolutePath();
    
}

// SLOT function called only when user attempts to load from saved settings stored in a wpr file
void Interface::loadFromSettings()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    saveloadPath,
                                                    tr("Wallpapers (*.wpr)"));
    
    saveloadPath = loadSettings(fileName);
}

// internal function that handles loading settings from a specified file
QString Interface::loadSettings(const QString &fileName) {
    
    QFile inFile(fileName);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return nullptr;
    
    // QDataStream in(&inFile);

    QTextStream in(&inFile);
    QString skipString;
    QString functionType;
    QString functionName;
    QString colorType;
    QString colorName;

    QString line;

    QString imageLoadPath;
    QString loadImageName;
    int tempint, newFunctionIndex, newColorIndex, count;
    double tempdouble;

    in.readLineInto(&line);
    // qDebug() << "reading:" << line;
    settings->Width = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    in.readLineInto(&line);
    settings->Height = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    in.readLineInto(&line);
    settings->XCorner = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    in.readLineInto(&line);
    settings->YCorner = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    in.readLineInto(&line);
    settings->OWidth = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toInt();
    in.readLineInto(&line);
    settings->OHeight = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toInt();
    in.readLineInto(&line);
    functionType = (line.right(line.length() - line.lastIndexOf(" ") - 1));

    // in >> skipString >> settings->Width;
    // in >> skipString >> settings->Height;
    // in >> skipString >> settings->XCorner;
    // in >> skipString >> settings->YCorner;
    // in >> skipString >> settings->OWidth;
    // in >> skipString >> settings->OHeight;
    // in >> skipString >> functionType;

    // if (functionType == "Wallpapers") {
    	// in >> skipString >> functionName;
    	in.readLineInto(&line);
    	functionName = (line.right(line.length() - line.lastIndexOf(" ") - 1));
    	newFunctionIndex = functionSel->findText(functionName, Qt::MatchExactly);
    // }
    // else {
    // 	// deal with differnt types of functions
    // }

    in.readLineInto(&line);
    colorType = (line.right(line.length() - line.lastIndexOf(" ") - 1));

    // in >> skipString >> colorType;
    if (colorType == "Image") {
    	in.readLineInto(&line);
	    imageLoadPath = (line.right(line.length() - line.lastIndexOf(" ") - 1));
	    in.readLineInto(&line);
	    loadImageName = (line.right(line.length() - line.lastIndexOf(" ") - 1));
    	// in >> skipString >> imageLoadPath;
    	// in >> skipString >> loadImageName;
    }
    else {
    	in.readLineInto(&line);
	    colorName = (line.right(line.length() - line.lastIndexOf(" ") - 1));
    	// in >> skipString >> colorName;
    	newColorIndex = colorwheelSel->findText(colorName, Qt::MatchExactly);
    }
    
    // in >> settings->Width >> settings->Height;
    // in >> settings->XCorner >> settings->YCorner;
    // in >> settings->OWidth >> settings->OHeight;
    // in >> newFunctionIndex;
    currFunction = functionVector[newFunctionIndex];
    // in >> newColorIndex;

    // qDebug() << "new color index is:" << newColorIndex;

    in.readLineInto(&line);
	tempdouble = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    // in >> skipString >> tempDouble;
    currFunction->setScaleR(tempdouble);
    scaleREdit->blockSignals(true);
    scaleREdit->setText(QString::number(tempdouble));
    scaleREdit->blockSignals(false);

    in.readLineInto(&line);
	tempdouble = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    // in >> skipString >> tempdouble;
    scaleAEdit->blockSignals(true);
    currFunction->setScaleA(tempdouble);
    scaleAEdit->setText(QString::number(tempdouble));
    scaleAEdit->blockSignals(false);

    in.readLineInto(&line);
	count = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toInt();

    // in >> count;
    // numTermsEdit->blockSignals(true);
    // numTermsEdit->setValue(count);
    // numTermsEdit->blockSignals(false);

    // int newNumTerms = count;
    currFunction->setNumTerms(count);
    unsigned int unsignedCount = count;
    // currFunction->refresh();

    QString separator(PARAMETER_SEPARATOR_LENGTH, ' ');
    QStringList resultList;
    QString resultString;

    for(unsigned int i = 0; i < unsignedCount; i++)
    {
        in.readLineInto(&line);
        resultList = line.split(separator, QString::SkipEmptyParts);
        for (int j = 0; j < resultList.size(); j++) {
            resultString = resultList.at(j);
            if (j == 0) {
                tempint = resultString.right(resultString.length() - resultString.lastIndexOf(" ") - 1).toInt(); 
                currFunction->setN(i, tempint);
            }
            else if (j == 1) {
                tempint = resultString.right(resultString.length() - resultString.lastIndexOf(" ") - 1).toInt(); 
                currFunction->setM(i, tempint);
            }
            else if (j == 2) {
                tempdouble = resultString.right(resultString.length() - resultString.lastIndexOf(" ") - 1).toDouble(); 
                currFunction->setR(i, tempdouble);
            }
            else {
                tempdouble = resultString.right(resultString.length() - resultString.lastIndexOf(" ") - 1).toDouble(); 
                currFunction->setA(i, tempdouble);
            }
        }
    }

    // in >> imageLoadPath;
    // in >> loadImageName;

    inFile.close();    

    worldWidthEditSlider->setValue(settings->Width * 100.0);
    worldHeightEditSlider->setValue(settings->Height * 100.0);
    XShiftEditSlider->setValue(settings->XCorner * 100.0);
    YShiftEditSlider->setValue(settings->YCorner * 100.0);

    numTermsEdit->setValue(currFunction->getNumTerms());

    // functionSel->blockSignals(true);
    functionSel->setCurrentIndex(newFunctionIndex);
    // functionSel->blockSignals(false);

    if (newColorIndex == 9) {
        imageSetPath = imageLoadPath;
        openImageName = loadImageName;
        fromImageButton->setChecked(true);
        fromImageButton->clicked();
    }
    else {
        fromColorWheelButton->setChecked(true);
        colorwheelSel->setCurrentIndex(newColorIndex);
        fromColorWheelButton->clicked();
    }

    //refreshTableTerms();
    //updatePreviewDisplay();
    
    QDir stickypath(fileName);
    stickypath.cdUp();
    return stickypath.path();
}

// updates the preview to reflect changes to the settings, function, and color wheel
void Interface::updatePreviewDisplay()
{

    // if (!imageDataGraph->series().empty()) imageDataGraph->removeSeries(imageDataSeries);
	imageDataSeries->clear();

    updatePreview->setEnabled(false);

    displayProgressBar->reset();
    
    previewDisplayPort->paintToDisplay(disp);
}

// slot function called when clicked "update preview" button to add to history and update the preview display to reflect current settings
void Interface::snapshotFunction()
{
    historyDisplay->show();
    
    QDateTime savedTime = QDateTime::currentDateTimeUtc();
    QString newFile = savedTime.toString("MM.dd.yyyy.hh.mm.ss.zzz.t").append(".wpr");
    QString filePath = saveSettings(newFile).append("/" + newFile);

    historyDisplay->triggerAddToHistory(savedTime, filePath, currFunction, currColorWheel, settings);

    //updatePreviewDisplay();
    
}

// SLOT FUNCTIONS TO CHANGE OUTPUT IMAGE PROPERTIES
void Interface::changeOHeight()
{
    
    int val = outHeightEdit->text().toInt();
    if (val < MIN_IMAGE_DIM || val > MAX_IMAGE_DIM) {
        errorHandler(INVALID_OUTPUT_IMAGE_DIM);
    }
    settings->OHeight = val;
    imageExportPort->changeSettings(settings);
    // updatePreviewDisplay();
}

void Interface::changeOWidth()
{
    
    int val = outWidthEdit->text().toInt();
    if (val < MIN_IMAGE_DIM || val > MAX_IMAGE_DIM) {
        errorHandler(INVALID_OUTPUT_IMAGE_DIM);
    }
    settings->OWidth = val;
    imageExportPort->changeSettings(settings);
    // updatePreviewDisplay();
}

//changing slider values
void Interface::changeWorldHeight(double val)
{
    settings->Height = val;
    worldHeightEdit->setText(QString::number(val));
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeWorldHeight()
{
    double val = worldHeightEdit->text().toDouble();
    settings->Height = val;
    worldHeightEditSlider->blockSignals(true);
    worldHeightEditSlider->setValue(val * 100.0);
    worldHeightEditSlider->blockSignals(false);
    
    updatePreviewDisplay();
}

//changing slider values
void Interface::changeWorldWidth(double val)
{
    settings->Width = val;
    worldWidthEdit->setText(QString::number(val));
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeWorldWidth()
{
    double val = worldWidthEdit->text().toDouble();
    settings->Width = val;
    worldWidthEditSlider->blockSignals(true);
    worldWidthEditSlider->setValue(val * 100.0);
    worldWidthEditSlider->blockSignals(false);
    updatePreviewDisplay();
}

void Interface::changeXCorner(double val)
{
  
    settings->XCorner = val;
    XShiftEdit->setText(QString::number(val));
    updatePreviewDisplay();
}

void Interface::changeXCorner()
{
  
    double val = XShiftEdit->text().toDouble();
    settings->XCorner = val;
    
    XShiftEditSlider->blockSignals(true);
    XShiftEditSlider->setValue(val * 100.0);
    XShiftEditSlider->blockSignals(false);
    updatePreviewDisplay();
}

void Interface::changeYCorner(double val)
{
    settings->YCorner = val;
    YShiftEdit->setText(QString::number(val));
    updatePreviewDisplay();
}

void Interface::changeYCorner()
{
    double val = YShiftEdit->text().toDouble();
    settings->YCorner = val;
    YShiftEditSlider->blockSignals(true);
    YShiftEditSlider->setValue(val * 100.0);
    YShiftEditSlider->blockSignals(false);
    updatePreviewDisplay();
}


// SLOT FUNCTIONS TO CHANGE FREQ AND COEFF PAIRS
void Interface::changeN(int val)
{
    currFunction->setN(termIndex, val);
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void Interface::changeM(int val)
{
   
    currFunction->setM(termIndex, val);
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}


void Interface::changeR(double val)
{
    currFunction->setR(termIndex, val);
    rValueLabel->setText(QString::number(val));
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void Interface::changeA(double val)
{

    currFunction->setA(termIndex, val);
    aValueLabel->setText(QString::number(val));
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

//changing slider values
void Interface::changeScaleA(double val)
{
    currFunction->setScaleA(val);
    scaleAEdit->setText(QString::number(val));
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeScaleA()
{
    double val = scaleAEdit->text().toDouble();
    currFunction->setScaleA(val);
    scaleAEditSlider->blockSignals(true);
    scaleAEditSlider->setValue(val * 100.0);
    scaleAEditSlider->blockSignals(false);
    updatePreviewDisplay();
}


//changing slider values
void Interface::changeScaleR(double val)
{
    
    currFunction->setScaleR(val);
    scaleREdit->setText(QString::number(val));
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeScaleR()
{
    
    double val = scaleREdit->text().toDouble();
    currFunction->setScaleA(val);
    scaleREditSlider->blockSignals(true);
    scaleREditSlider->setValue(val * 100.0);
    scaleREditSlider->blockSignals(false);
    updatePreviewDisplay();
}



void Interface::startImageExport() 
{
    
    settingsPopUp->hide();
    
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    saveloadPath,
                                                    tr("JPEG (*.jpg *.jpeg);;TIFF (*.tiff);; PNG (*.png);;PPM (*.ppm)"));
    
    if (fileName == "") return;

    QFile inFile(fileName);
    if (!inFile.open(QIODevice::WriteOnly))
        return;

    exportProgressBar->resetBar(tr("Exporting"), true, imageExportPort);
    exportProgressBar->reset();

    dispLayout->insertLayout(2, exportProgressBar->layout);

    QImage *output = new QImage(settings->OWidth, settings->OHeight, QImage::Format_RGB32);
    
    imageExportPort->exportImage(output, fileName);
    
    
}

// function for error handling
void Interface::errorHandler(const int &flag)
{
    switch(flag)
    {
        case INVALID_FILE_ERROR:
            errorMessageBox->setText("Invalid file name/path");
            errorMessageBox->exec();
            break;
        case INVALID_OUTPUT_IMAGE_DIM:
            errorMessageBox->setText("Error: image dimensions must be between 20 and 10000");
            errorMessageBox->exec();
            break;
    }
    
}

// sets coefficient pair using data from polarPlane
void Interface::setPolarCoordinates(int coeffFlag, const QString &radius, const QString &angle)
{
    if (coeffFlag == LOCAL_FLAG)
    {
        rEdit->setValue(radius.toDouble() * 100);
        aEdit->setValue(angle.toDouble() * 100);
        rValueLabel->setText(radius);
        aValueLabel->setText(angle);
    }
    else if (coeffFlag == GLOBAL_FLAG)
    {
        scaleREdit->setText(radius);
        scaleREditSlider->setValue(radius.toDouble() * 100.0);
        emit scaleREdit->returnPressed();
        scaleAEdit->setText(angle);
        scaleAEditSlider->setValue(angle.toDouble() * 100.0);
        emit scaleAEdit->returnPressed();
    }
    
}


// SLOT function to handle when a cell is clicked in the term table
void Interface::termViewCellClicked(int row, int col)
{
    termViewTable->blockSignals(true);
    if (col == 5) {
        removeTerm(row);
        
    } else {
        termIndex = row;
        refreshMainWindowTerms();
    }

    updatePreviewDisplay();

}


// updates the tem table to changed values
void Interface::updateTermTable(QObject *cell)
{
    
    int row = ((QPoint *)cell)->x();
    int col = ((QPoint *)cell)->y();

    termIndex = row;
    
    float val;
    
    if (col > 0 && col < 3)
        val = static_cast<QSpinBox *>(termViewTable->cellWidget(row,col))->value();
    else
        val = static_cast<QDoubleSpinBox *>(termViewTable->cellWidget(row,col))->value();
    
    unsigned int index = row;
    int coeff = val;
    double freq = val;
    
    
    switch(col) {
        case 1:
            currFunction->setM(index, coeff);
            break;
        case 2:
            currFunction->setN(index, coeff);
            break;
        case 3:
            currFunction->setA(index, freq);
            break;
        case 4:
            currFunction->setR(index, freq);
            break;
    }
    
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void Interface::addTermTable() 
{
    addTermButton->blockSignals(true);
    int newNumTerms = currFunction->getNumTerms() + 1;
    currFunction->setNumTerms(newNumTerms);
    numTermsEdit->setValue(newNumTerms);
} 

// pop up window to appear when image file has finished exporting
void Interface::popUpImageExportFinished(const QString &filePath)
{
    
    QMessageBox msgBox;
    msgBox.setText(tr("The file has been successfully saved to: ").append(filePath));
    msgBox.exec();

    saveloadPath = filePath;
    
    exportProgressBar->remove();
    if (!exportProgressBar) delete exportProgressBar;
}

// reset the table to receive signals - prevent updating too fast
void Interface::resetTableButton() 
{
    addTermButton->blockSignals(false);
    termViewTable->blockSignals(false);

 	// imageDataWindow->show();   
}

// reset the main window to receive signals - prevent updating too fast
void Interface::resetMainWindowButton(const bool &status) 
{
    numTermsEdit->setEnabled(status);
    updatePreview->setEnabled(status);
}

void Interface::setSnapShotWindow(HistoryDisplay* window)
{
    historyDisplay = window;
    connect(historyDisplay->viewMapper, SIGNAL(mapped(QString)), this, SLOT(loadSettings(QString)));

}

// void Interface::addNewImageDataPoint(const std::complex<double> &data)
// {
// 	// qDebug() << "got here" << data.real() << data.imag();

// 	*imageDataSeries << QPointF(data.real(), data.imag());
// }

// void Interface::showImageDataGraph() 
// {
//     updateImageDataGraph();
//     imageDataWindow->show();
// }

void Interface::updateImageDataGraph() 
{ 
    // prevDataSeries->clear(); 
    // *prevDataSeries << imageDataSeries->points();

    imagePixmap.convertFromImage(QImage(imageSetPath + "/" + openImageName));
    imagePixmap = imagePixmap.scaledToHeight(previewSize);
    imageLabel->setPixmap(imagePixmap);

    prevDataSeries->replace(imageDataSeries->pointsVector());
}

