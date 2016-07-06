#include "interface.h"

interface::interface(QWidget *parent) : QWidget(parent)
{
    
    // FUNCTIONAL VARIABLES
    advancedMode = false;
    numTerms = 0;
    termIndex = 0;
    highestIndex = -1;
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
    
    // store defaults in settings struct
    settings = new Settings;
    settings->Width = DEFAULT_WIDTH;
    settings->Height = DEFAULT_HEIGHT;
    settings->XCorner = DEFAULT_XCORNER;
    settings->YCorner = DEFAULT_YCORNER;
    settings->OWidth = DEFAULT_OUTPUT_WIDTH;
    settings->OHeight = DEFAULT_OUTPUT_HEIGHT;
    
    // set up all layout elements for the interface
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
    
    historyDisplay = new HistoryDisplay(this);
    
    toggleViewWidget = new QWidget(this);
    
    leftbarLayout->addWidget(toggleViewWidget);
    leftbarLayout->addWidget(imagePropsBox);
    leftbarLayout->addWidget(patternTypeWidget);
    
    topbarLayout->addLayout(leftbarLayout);
    topbarLayout->addWidget(displayWidget);
    
    topbarLayout->addWidget(historyDisplay->viewHistoryWidget);
    
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

    toggleViewButton = new QPushButton(tr("Switch to Advanced View"), this);
    toggleViewLayout = new QVBoxLayout(toggleViewWidget);
    toggleViewLayout->addWidget(toggleViewButton);
        
    initPreviewDisplay();
    initFunctionConstants();
    initPatternType();
    //initViewHistory();
    //historyDisplay = new HistoryDisplay();
    coeffPlane = new CoeffPlane(currFunction, &termIndex);
    coeffMapper = new QSignalMapper(this);
    
    
    initImageProps();
    //initCoeffPlane();
    
    
    
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
    disp = new Display(DEFAULT_PREVIEW_SIZE, DEFAULT_IMAGE_SIZE, displayWidget);
    updatePreview = new QPushButton(tr("Update Preview"), this);
    exportImage = new QPushButton(tr("Export Image..."), this);
    resetImage = new QPushButton(tr("Reset"), this);
    dispLayout = new QVBoxLayout(displayWidget);
    buttonLayout = new QHBoxLayout();
    //progressBarLayout = new QHBoxLayout();
    
    buttonLayout->addWidget(updatePreview);
    buttonLayout->addWidget(exportImage);
    buttonLayout->addWidget(resetImage);
    

    
    //SET UP SHORTCUTS...add for save, open, undo?
    updatePreviewShortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
    
    imageExportPort = new Port(currFunction, currColorWheel, settings->OWidth, settings->OHeight, settings);
    previewDisplayPort = new Port(currFunction, currColorWheel, disp->width(), disp->height(), settings);
    
    displayProgressBar = new ProgressBar(tr("Preview"), true, previewDisplayPort);
    exportProgressBar = new ProgressBar(tr("Exporting"), false, imageExportPort);
    
    connect(previewDisplayPort->getControllerObject(), SIGNAL(progressChanged(int)), displayProgressBar, SLOT(update(int)));
    connect(imageExportPort->getControllerObject(), SIGNAL(progressChanged(int)), exportProgressBar, SLOT(update(int)));
    connect(imageExportPort, SIGNAL(finishedExport(QString)), this, SLOT(popUpImageExportFinished(QString)));
    
    dispLayout->setAlignment(disp, Qt::AlignCenter);
    dispLayout->addWidget(disp);
    dispLayout->addLayout(displayProgressBar->layout);
    dispLayout->addLayout(exportProgressBar->layout);
    dispLayout->addLayout(buttonLayout);
}



void interface::initFunctionConstants()
{
    functionConstantsBox = new QGroupBox(tr("Function Constants"), functionConstantsWidget);
    
    currTermLabel = new QLabel(functionConstantsBox);
    currTermLabel->setText(tr("Term:"));
    currTermEdit = new CustomSpinBox(functionConstantsBox);
    currTermEdit->setRange(1,numTerms);
    
    freqpairLabel = new QLabel(tr("Frequency Pair: "), functionConstantsBox);
    coeffLabel = new QLabel(tr("Coefficient Pair: "), functionConstantsBox);
    
    nLabel = new QLabel(functionConstantsBox);
    mLabel = new QLabel(functionConstantsBox);
    aLabel = new QLabel(functionConstantsBox);
    rLabel = new QLabel(functionConstantsBox);
    aValueLabel = new QLabel(functionConstantsBox);
    rValueLabel = new QLabel(functionConstantsBox);
    
    
    globalConsantsLabel = new QLabel(tr("<b>Global Function Constants: <\b>"), functionConstantsBox);
    scaleALabel = new QLabel(tr("Scaling Angle"), functionConstantsBox);
    scaleRLabel = new QLabel(tr("Scaling Radius"), functionConstantsBox);
    refreshLabels();
    
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
    rEdit->setRange(-1000,1000);
    rEdit->setSingleStep(1);
    
    nEdit->setAlignment(Qt::AlignCenter);
    mEdit->setAlignment(Qt::AlignCenter);
    
    
    //    scalePlaneEdit->setFixedWidth(120);
    nLabel->setFixedWidth(18);
    mLabel->setFixedWidth(18);
    rLabel->setFixedWidth(18);
    aLabel->setFixedWidth(18);
    aValueLabel->setFixedWidth(35);
    rValueLabel->setFixedWidth(35);
    // scaleALabel->setFixedWidth(50);
    // scaleRLabel->setFixedWidth(50);
    
    aEdit->setOrientation(Qt::Horizontal);
    rEdit->setOrientation(Qt::Horizontal);
    
    
    scaleAEdit = new QLineEdit(functionConstantsBox);
    scaleREdit = new QLineEdit(functionConstantsBox);
    scaleAEdit->setFixedWidth(100);
    scaleREdit->setFixedWidth(100);
    scaleAEdit->setAlignment(Qt::AlignCenter);
    scaleREdit->setAlignment(Qt::AlignCenter);
    scalePlaneEdit = new QPushButton(tr("Set on plane"), functionConstantsBox);
    coeffPlaneEdit = new QPushButton(tr("Set on plane"), functionConstantsBox);
    
    
    // ORGANIZATIONAL ELEMENTS
    functionConstantsWidgetLayout = new QVBoxLayout(functionConstantsWidget);
    functionConstantsBoxLayout = new QVBoxLayout(functionConstantsBox);
    
    functionConstantsScalingTerms = new QHBoxLayout();
    numTermsLayout = new QHBoxLayout();
    termsLayout = new QVBoxLayout();
    termEditLayout = new QHBoxLayout();
    functionConstantsFreqs = new QHBoxLayout();
    functionConstantsCoeffs = new QHBoxLayout();
    functionConstantsPairs = new QVBoxLayout();

    QSpacerItem *spacerItem1 = new QSpacerItem(80, 30);
    QSpacerItem *spacerItem2 = new QSpacerItem(30, 30);
    QSpacerItem *spacerItem3 = new QSpacerItem(100, 30);
    QSpacerItem *spacerItem4 = new QSpacerItem(30, 30);
    QSpacerItem *spacerItem5 = new QSpacerItem(87, 30);
    QSpacerItem *spacerItem6 = new QSpacerItem(30, 30);
    QSpacerItem *spacerItem7 = new QSpacerItem(50, 30);
    QSpacerItem *spacerItem8 = new QSpacerItem(50, 30);
    QSpacerItem *spacerItem9 = new QSpacerItem(50, 30);
    // QSpacerItem *spacerItem10 = new QSpacerItem(30, 30);
    // QSpacerItem *spacerItem11 = new QSpacerItem(30, 30);
    // QSpacerItem *spacerItem12 = new QSpacerItem(30, 30);
    // QSpacerItem *spacerItem13 = new QSpacerItem(30, 30);
    // QSpacerItem *spacerItem14 = new QSpacerItem(20, 30);
    
    functionConstantsScalingTerms->addWidget(globalConsantsLabel);
    functionConstantsScalingTerms->addItem(spacerItem1);
    functionConstantsScalingTerms->addWidget(scaleRLabel);
    functionConstantsScalingTerms->addItem(spacerItem9);
    functionConstantsScalingTerms->addWidget(scaleREdit);
    functionConstantsScalingTerms->addItem(spacerItem2);
    functionConstantsScalingTerms->addWidget(scaleALabel);
    functionConstantsScalingTerms->addWidget(scaleAEdit);
    functionConstantsScalingTerms->addItem(spacerItem3);
    functionConstantsScalingTerms->addWidget(scalePlaneEdit);
    functionConstantsScalingTerms->addStretch();
    functionConstantsScalingTerms->setAlignment(Qt::AlignLeft);
    
    functionConstantsFreqs->addWidget(freqpairLabel);
    functionConstantsFreqs->addItem(spacerItem4);
    functionConstantsFreqs->addWidget(nLabel);
    functionConstantsFreqs->addWidget(nEdit);
    functionConstantsFreqs->addItem(spacerItem5);
    functionConstantsFreqs->addWidget(mLabel);
    functionConstantsFreqs->addWidget(mEdit);
    functionConstantsFreqs->addStretch();
    functionConstantsFreqs->setAlignment(Qt::AlignLeft);
    
    functionConstantsCoeffs->addWidget(coeffLabel);
    functionConstantsCoeffs->addItem(spacerItem6);
    functionConstantsCoeffs->addWidget(rLabel);
    functionConstantsCoeffs->addWidget(rEdit);
    functionConstantsCoeffs->addWidget(rValueLabel);
    functionConstantsCoeffs->addItem(spacerItem7);
    functionConstantsCoeffs->addWidget(aLabel);
    functionConstantsCoeffs->addWidget(aEdit);
    functionConstantsCoeffs->addWidget(aValueLabel);
    functionConstantsCoeffs->addItem(spacerItem8);
    functionConstantsCoeffs->addWidget(coeffPlaneEdit);
    functionConstantsCoeffs->addStretch();
    functionConstantsCoeffs->setAlignment(Qt::AlignLeft);

    
    numTermsLabel = new QLabel(tr("Num Terms: "), functionConstantsBox);
    numTermsEdit = new QSpinBox(functionConstantsBox);
    numTermsEdit->setRange(1,MAX_NUM_TERMS);
    
    //numTermsEdit->setFixedWidth(50);
    
    termViewButton = new QPushButton(tr("View/Edit All Terms"), functionConstantsBox);
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
    
    
    numTermsLayout->addWidget(numTermsLabel);
    numTermsLayout->addWidget(numTermsEdit);
    termsLayout->addLayout(numTermsLayout);
    termsLayout->addWidget(termViewButton);
    termEditLayout->addLayout(termsLayout);
   
    termEditLayout->addWidget(currTermLabel);
    termEditLayout->addWidget(currTermEdit);
//    termEditLayout->setAlignment(Qt::AlignRight);
    
    functionConstantsPairs->addLayout(functionConstantsFreqs);
    functionConstantsPairs->addLayout(functionConstantsCoeffs);
    termEditLayout->addLayout(functionConstantsPairs);
    
    functionConstantsBoxLayout->addLayout(functionConstantsScalingTerms);
    functionConstantsBoxLayout->addLayout(termEditLayout);
    
    
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

    gspacer1 = new QSpacerItem(0,20);
    gspacer2 = new QSpacerItem(0,10);
    gspacer3 = new QSpacerItem(0,10);
    gspacer4 = new QSpacerItem(0,50);
    setLoadedImage = new QPushButton(tr("Set Image..."), patternTypeBox);
    patternTypeBoxOverallLayout = new QVBoxLayout(patternTypeWidget);
    functionLayout = new QVBoxLayout();
    colorwheelLayout = new QVBoxLayout();
    
    
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
    
    colorwheelSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    colorwheelLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    
    //numTermsEdit->setValidator(numtermsValidate);
    
    // ORGANIZATIONAL ELEMENTS
    patternTypeBoxLayout = new QVBoxLayout(patternTypeBox);
    functionLayout->addWidget(functionLabel);
    functionLayout->addWidget(functionSel);
   
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



//void interface::initViewHistory()
//{
//    
//    viewHistoryBox = new QGroupBox(tr("History"), viewHistoryWidget);
//    viewHistoryBoxOverallLayout = new QVBoxLayout(viewHistoryWidget);
//    
//    clearHistoryButton = new QPushButton(tr("Clear All History"), viewHistoryBox);
//    viewHistoryBoxLayout = new QVBoxLayout(viewHistoryBox);
//    
//    viewHistoryBoxOverallLayout->addWidget(viewHistoryBox);
//    viewHistoryBoxLayout->addWidget(clearHistoryButton);
//    viewHistoryBoxOverallLayout->addStretch();
//    
//    viewMapper = new QSignalMapper(this);
//    removeMapper = new QSignalMapper(this);
//    
//}


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




void interface::connectAllSignals()
{
    
    connect(termViewTable, SIGNAL(cellClicked(int, int)), this, SLOT(termViewCellClicked(int, int)));
    
    connect(coeffPlaneEdit, SIGNAL(clicked()), coeffMapper, SLOT(map()));
    connect(scalePlaneEdit, SIGNAL(clicked()), coeffMapper, SLOT(map()));
    coeffMapper->setMapping(coeffPlaneEdit, LOCAL_FLAG);
    coeffMapper->setMapping(scalePlaneEdit, GLOBAL_FLAG);
    connect(coeffMapper,SIGNAL(mapped(int)), coeffPlane, SLOT(showPlanePopUp(int)));

    connect(toggleViewButton, SIGNAL(clicked()), this, SLOT(toggleViewMode()));
    connect(updatePreview, SIGNAL(clicked()), this, SLOT(updateSavePreview()));
    connect(exportImage, SIGNAL(clicked()), this, SLOT(saveImageStart()));
    connect(resetImage, SIGNAL(clicked()), this, SLOT(resetImageFunction()));
    
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), currColorWheel, SLOT(setCurrent(int)));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), this, SLOT(colorWheelChanged(int)));
    connect(setLoadedImage, SIGNAL(clicked()), this, SLOT(setImagePushed()));
    connect(functionSel, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFunction(int)));
    connect(numTermsEdit, SIGNAL(valueChanged(int)), this, SLOT(changeNumTerms(int)));
    connect(currTermEdit, SIGNAL(valueChanged(int)), this, SLOT(updateCurrTerm(int)));
    connect(termViewButton, SIGNAL(clicked()), this, SLOT(termViewPopUp()));
    connect(addTermButton, SIGNAL(clicked()), this, SLOT(addTerm()));
    
    connect(nEdit, SIGNAL(valueChanged(int)), this, SLOT(changeN(int)));
    connect(mEdit, SIGNAL(valueChanged(int)), this, SLOT(changeM(int)));
    connect(rEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeR(double)));
//    connect(radiusEdit, SIGNAL(editingFinished()), this, SLOT(updatePolarCoordinates()));
    connect(aEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeA(double)));
//    connect(angleEdit, SIGNAL(editingFinished()), this, SLOT(updatePolarCoordinates()));
    connect(scaleREdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleR(QString)));
    connect(scaleAEdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleA(QString)));
    
    connect(outwidthEdit, SIGNAL(textChanged(QString)), this, SLOT(changeOWidth(QString)));
    connect(outheightEdit, SIGNAL(textChanged(QString)), this, SLOT(changeOHeight(QString)));
    connect(worldwidthEdit, SIGNAL(textChanged(QString)), this, SLOT(changeWorldWidth(QString)));
    connect(worldheightEdit, SIGNAL(textChanged(QString)), this, SLOT(changeWorldHeight(QString)));
    connect(XCornerEdit, SIGNAL(textChanged(QString)), this, SLOT(changeXCorner(QString)));
    connect(YCornerEdit, SIGNAL(textChanged(QString)), this, SLOT(changeYCorner(QString)));
    
    
    connect(historyDisplay->viewMapper, SIGNAL(mapped(QString)), this, SLOT(loadSettings(QString)));
    //connect(historyDisplay->removeMapper, SIGNAL(mapped(QObject*)), this, SLOT(removePreview(QObject*)));
    
    connect(coeffPlane, SIGNAL(setPolarCoordinates(int, QString, QString)), this, SLOT(setPolarCoordinates(int, QString, QString)));
    
    connect(updatePreviewShortcut, SIGNAL(activated()), this, SLOT(updateSavePreview()));
    
    
}



QString interface::genLabel(const char *in)     //concatenate the constant name
{                                               //with the current index number
    QString out;
    out.setNum(termIndex + 1);
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
        
        mEdit->setValue(currFunction->getM(index));
        nEdit->setValue(currFunction->getN(index));
        aEdit->setValue(currFunction->getA(index));
        rEdit->setValue(currFunction->getR(index));

        aValueLabel->setText(QString::number(currFunction->getA(index)));
        rValueLabel->setText(QString::number(currFunction->getR(index)));
        
        mEdit->blockSignals(false);
        nEdit->blockSignals(false);
        aEdit->blockSignals(false);
        rEdit->blockSignals(false);
        
    }
    
}

void interface::refreshMainWindowTerms()
{
    
    currTermEdit->blockSignals(true);
    numTermsEdit->blockSignals(true);
    mEdit->blockSignals(true);
    nEdit->blockSignals(true);
    aEdit->blockSignals(true);
    rEdit->blockSignals(true);

    currTermEdit->setValue(termIndex + 1);
    numTermsEdit->setValue(numTerms);
    
    mEdit->setValue(currFunction->getM(termIndex));
    nEdit->setValue(currFunction->getN(termIndex));
    aEdit->setValue(currFunction->getA(termIndex) * 100);
    rEdit->setValue(currFunction->getR(termIndex) * 100);

    currTermEdit->blockSignals(false);
    numTermsEdit->blockSignals(false);
    mEdit->blockSignals(false);
    nEdit->blockSignals(false);
    aEdit->blockSignals(false);
    rEdit->blockSignals(false);
    
    refreshLabels();
    
}

void interface::removeTerm(int row)
{
    
    if (row <= 0) {
        return;
    }
    
    for (int c = 0; c < termViewTable->columnCount(); ++c)
    {
        delete termViewTable->cellWidget(row, c);
    }
    
    termViewTable->removeRow(row);
    
    for (int r = row; r < termViewTable->rowCount(); ++r)
    {
        QLabel *term = static_cast<QLabel *>(termViewTable->cellWidget(r, 0));
        term->setText(QString::number(term->text().toInt() - 1));
    }
    
    
    highestIndex--;
    if (termIndex > highestIndex)
        termIndex = highestIndex;
    
    numTerms--;
    unsigned int term = row;
    currFunction->removeTerm(term);
    currFunction->setNumTerms(numTerms);
    currTermEdit->setMaximum(numTerms + 1);

    numTermsEdit->blockSignals(true);
    numTermsEdit->setValue(numTerms);
    numTermsEdit->blockSignals(false);
}


// function called when adding a new term to the termViewTable
void interface::addTerm()
{
    
    highestIndex++;
    numTerms++;
    
    termViewTable->setRowCount(numTerms);
    currTermEdit->setMaximum(numTerms + 1);
    currFunction->setNumTerms(numTerms);
    
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
    removeTermItem->setIcon(QIcon(":/Images/remove.png"));
    
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
    updatePreviewDisplay();
    
}


void interface::resetImageFunction()
{
    // delete currFunction;
    // delete previewDisplayPort;
    // delete imageExportPort;
    
    // currFunction = new hex3Function();
    // imageExportPort = new Port(currFunction, currColorWheel, settings->OWidth, settings->OHeight, settings);
    // previewDisplayPort = new Port(currFunction, currColorWheel, disp->getImage()->width(), disp->getImage()->height(), settings);

    // imageExportPort->changeFunction(currFunction);
    // previewDisplayPort->changeFunction(currFunction);

    // currFunction = functionVector[0];

    numTerms = 1;

    currFunction->reset();

    // functionSel->setCurrentIndex(0);
    // colorwheelSel->setCurrentIndex(0);
    
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


void interface::updateCurrTerm(int i)
{
    if (i > 0) termIndex = i - 1;
    
    refreshTerms();
    refreshLabels();
}

void interface::changeNumTerms(int i)
{
    
    currTermEdit->setMaximum(i);
    
    if (i < numTerms) {
        for (int k = numTerms - 1; k >= i; --k) removeTerm(k);
    } else if (i > numTerms) {
        for (int k = numTerms; k < i; ++k) addTerm();
    }
    
}

void interface::colorWheelChanged(int index)
{
    if(index == 9)
        setLoadedImage->show();
    else
        setLoadedImage->hide();
    updatePreviewDisplay();
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
    // delete currFunction;
    // delete previewDisplayPort;
    // delete imageExportPort;

    // switch(index)
    // {
    // case 0:
    //     currFunction = new hex3Function();
    //     colorwheelSel->setCurrentIndex(7);
    //     break;
    // case 1:
    //     currFunction = new hex6Function();
    //     colorwheelSel->setCurrentIndex(7);
    //     break;
    // case 2:
    //     currFunction = new squareFunction();
    //     colorwheelSel->setCurrentIndex(6);
    //     break;
    // case 3:
    //     currFunction = new generalpairedFunction();
    //     colorwheelSel->setCurrentIndex(3);
    //     break;
    // case 4:
    //     currFunction = new generalFunction();
    //     colorwheelSel->setCurrentIndex(7);
    //     break;
    // case 5:
    //     currFunction = new cmmFunction();
    //     colorwheelSel->setCurrentIndex(7);
    //     break;
    // case 6:
    //     currFunction = new p31mFunction();
    //     colorwheelSel->setCurrentIndex(1);
    //     break;
    // case 7:
    //     currFunction = new p3m1Function();
    //     colorwheelSel->setCurrentIndex(1);
    //     break;
    // case 8:
    //     currFunction = new p6mFunction();
    //     colorwheelSel->setCurrentIndex(6);
    //     break;
    // case 9:
    //     currFunction = new p4gFunction();
    //     colorwheelSel->setCurrentIndex(6);
    //     break;
    // case 10:
    //     currFunction = new p4mFunction();
    //     colorwheelSel->setCurrentIndex(6);
    //     break;
    // case 11:
    //     currFunction = new pmmFunction();
    //     colorwheelSel->setCurrentIndex(9);
    //     break;
    // case 12:
    //     currFunction = new pmgFunction();
    //     colorwheelSel->setCurrentIndex(9);
    //     break;
    // case 13:
    //     currFunction = new pggFunction();
    //     colorwheelSel->setCurrentIndex(9);
    //     break;
    // case 14:
    //     currFunction = new pmFunction();
    //     colorwheelSel->setCurrentIndex(9);
    //     break;
    // case 15:
    //     currFunction = new pgFunction();
    //     colorwheelSel->setCurrentIndex(9);
    //     break;
    // case 16:
    //     currFunction = new rhombicFunction();
    //     colorwheelSel->setCurrentIndex(9);
    //     break;
    // case 17:
    //     currFunction = new zzbarFunction();
    //     colorwheelSel->setCurrentIndex(9);
    //     break;
    // }

    currFunction = functionVector[index];

    // currFunction->setNumTerms(1);

    // previewDisplayPort = new Port(currFunction, currColorWheel, disp->getImage()->width(), disp->getImage()->height(), settings);
    // imageExportPort = new Port(currFunction, currColorWheel, settings->OWidth, settings->OHeight, settings);

    // numTerms = 1;

    previewDisplayPort->changeFunction(currFunction);
    imageExportPort->changeFunction(currFunction);

    refreshTerms();
    updatePreviewDisplay();
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


void interface::updatePreviewDisplay()
{
    displayProgressBar->reset();
    
    previewDisplayPort->paintToDisplay(disp);
}

void interface::updateSavePreview()
{
    
    QDateTime savedTime = QDateTime::currentDateTimeUtc();
    QString newFile = savedTime.toString("MM.dd.yyyy.hh.mm.ss.zzz.t").append(".wpr");
    QString filePath = saveSettings(newFile).append("/" + newFile);

    // qDebug() << "Updating Preview";
    historyDisplay->triggerAddToHistory(savedTime, filePath, currFunction, currColorWheel, settings);
    
    updatePreviewDisplay();
    
}


void interface::changeOHeight(const QString &val)
{
    settings->OHeight = val.toInt();
    updatePreviewDisplay();
}

void interface::changeOWidth(const QString &val)
{
    settings->OWidth = val.toInt();
    updatePreviewDisplay();
}

void interface::changeWorldHeight(const QString &val)
{
    settings->Height = val.toDouble();
    updatePreviewDisplay();
}

void interface::changeWorldWidth(const QString &val)
{
    settings->Width = val.toDouble();
    updatePreviewDisplay();
}

void interface::changeXCorner(const QString &val)
{
    settings->XCorner = val.toDouble();
    updatePreviewDisplay();
}

void interface::changeYCorner(const QString &val)
{
    settings->YCorner = val.toDouble();
    updatePreviewDisplay();
}

void interface::changeN(int val)
{
    // int passedval = val.toInt();
    currFunction->setN(termIndex, val);
    // nValueLabel->setText(QString::number(val));
    refreshTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void interface::changeM(int val)
{
    // int passedval = val.toInt();
    currFunction->setM(termIndex, val);
    // mValueLabel->setText(QString::number(val));
    refreshTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void interface::changeR(double val)
{
    // double passedval = val.toDouble();
    currFunction->setR(termIndex, val);
    rValueLabel->setText(QString::number(val));
    refreshTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void interface::changeA(double val)
{
    // double passedval = val.toDouble();
    currFunction->setA(termIndex, val);
    aValueLabel->setText(QString::number(val));
    refreshTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

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
    exportProgressBar->reset();
    
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    saveloadPath,
                                                    tr("PPM (*.ppm);;JPEG (*.jpg *.jpeg)"));
    
    QFile inFile(fileName);
    if (!inFile.open(QIODevice::WriteOnly))
        return;
    
    QImage *output = new QImage(settings->OWidth, settings->OHeight, QImage::Format_RGB32);
    imageExportPort->exportImage(output, fileName);
    
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


void interface::setPolarCoordinates(int coeffFlag, const QString &radius, const QString &angle)
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
        scaleAEdit->setText(angle);
    }
    
}



void interface::termViewCellClicked(int row, int col)
{
    if (col == 5) {
        removeTerm(row);
    } else if (col == 0) {
        termIndex = row;
        refreshMainWindowTerms();
    }

    updatePreviewDisplay();
}


void interface::updateTermTable(QObject *cell)
{
    
    int row = ((QPoint *)cell)->x();
    int col = ((QPoint *)cell)->y();
    
    float val;
    
    if (col > 0 && col < 3)
        val = static_cast<QSpinBox *>(termViewTable->cellWidget(row,col))->value();
    else
        val = static_cast<QDoubleSpinBox *>(termViewTable->cellWidget(row,col))->value();
    
    unsigned int index = row;
    int coeff = val;
    double freq = val;
    
    if (col == 1)
        currFunction->setM(index, coeff);
    else if (col == 2)
        currFunction->setN(index, coeff);
    else if (col == 3)
        currFunction->setA(index, freq);
    else if (col == 4)
        currFunction->setR(index, freq);
    
    refreshTerms();
    updatePreviewDisplay();
}



void interface::popUpImageExportFinished(const QString &filePath)
{
    
    QMessageBox msgBox;
    msgBox.setText(tr("The file has been successfully saved to: ").append(filePath));
    msgBox.exec();
    
    
}

