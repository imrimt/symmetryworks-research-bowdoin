#include "interface.h"

interface::interface(QWidget *parent) : QWidget(parent)
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
    settings->Width = DEFAULT_WIDTH;
    settings->Height = DEFAULT_HEIGHT;
    settings->XCorner = DEFAULT_XCORNER;
    settings->YCorner = DEFAULT_YCORNER;
    settings->OWidth = DEFAULT_OUTPUT_WIDTH;
    settings->OHeight = DEFAULT_OUTPUT_HEIGHT;
    
    // INITIALIZE LAYOUT
    initInterfaceLayout();
    
    functionSel->setCurrentIndex(0);
    colorwheelSel->setCurrentIndex(0);

    
}


void interface::initInterfaceLayout()
{
    
    // ORGANIZATIONAL ELEMENTS
    overallLayout = new QHBoxLayout();
    interfaceLayout = new QVBoxLayout();
    topbarLayout = new QHBoxLayout();
    leftbarLayout = new QVBoxLayout();
    
    imagePropsBox = new QGroupBox(tr("Image Properties"), this);
    displayWidget = new QWidget(this);
    patternTypeWidget = new QWidget(this);
    functionConstantsWidget = new QWidget(this);
    
    //toggleViewWidget = new QWidget(this);
    
    //leftbarLayout->addWidget(toggleViewWidget);
    leftbarLayout->addWidget(imagePropsBox);
    leftbarLayout->addWidget(patternTypeWidget);
    //leftbarLayout->setSizeConstraint(QLayout::SetFixedSize);

    topbarLayout->addLayout(leftbarLayout);
    topbarLayout->addWidget(displayWidget);
    
    historyDisplay = new HistoryDisplay(this);
    historyDisplay->hide();
    
    //historyDisplay->viewHistoryWidget->setGeometery(this->parent()->rect().topLeft().x()
    
    interfaceLayout->addLayout(topbarLayout);
    interfaceLayout->addWidget(functionConstantsWidget);
    
    
    overallLayout->addLayout(interfaceLayout);
    overallLayout->addWidget(historyDisplay->viewHistoryWidget);
    //overallLayout->setSizeConstraint(QLayout::SetFixedSize);
    overallLayout->activate();
    //overallLayout->addWidget(historyDisplay->dock);
   
    
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

//    toggleViewButton = new QPushButton(tr("Switch to Advanced View"), this);
//    toggleViewLayout = new QVBoxLayout(toggleViewWidget);
//    toggleViewLayout->addWidget(toggleViewButton);
    
    // INIT UI SUBELEMENTS
    initPreviewDisplay();
    initFunctionConstants();
    initPatternType();

    polarPlane = new PolarPlane(currFunction, &termIndex, this);
    polarPlaneMapper = new QSignalMapper(this);
    
    initImageProps();
    initImageExportPopUp();
    
    // SET DEFAULTS
    scaleREdit->setText(QString::number(currFunction->getScaleR()));
    scaleAEdit->setText(QString::number(currFunction->getScaleA()));

    connectAllSignals();

    refreshTableTerms();
    updatePreviewDisplay();

    //will reload
    //setFixedSize(1200,1000);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // FINALIZE WINDOW
    setFixedSize(sizeHint());
    setWindowTitle(tr("COOL WALLPAPER SOFTWARE"));

}



// INIT PREVIEW DISPLAY ELEMENTS
void interface::initPreviewDisplay()
{
    disp = new Display(DEFAULT_PREVIEW_SIZE, DEFAULT_IMAGE_SIZE, displayWidget);
    updatePreview = new QPushButton(tr("Snapshot"), this);
    exportImage = new QPushButton(tr("Export..."), this);
    resetImage = new QPushButton(tr("Reset"), this);
    dispLayout = new QVBoxLayout(displayWidget);
    //dispLayout->setSizeConstraint(QLayout::SetMinimumSize);
    buttonLayout = new QHBoxLayout();
    
    buttonLayout->addWidget(updatePreview);
    buttonLayout->addWidget(exportImage);
    buttonLayout->addWidget(resetImage);
    
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

    dispLayout->setAlignment(disp, Qt::AlignCenter);
    dispLayout->addWidget(disp);
    dispLayout->addLayout(displayProgressBar->layout);
    dispLayout->addLayout(buttonLayout);
}

// INIT FUNCTION CONSTANTS EDIT BOX
void interface::initFunctionConstants()
{

    functionConstantsBox = new QGroupBox(tr("Function Constants"), functionConstantsWidget);
    functionConstantsBoxLayout = new QVBoxLayout(functionConstantsBox);
    functionTermsGrid = new QGridLayout();
    
    currTermLabel = new QLabel(functionConstantsBox);
    currTermLabel->setText(tr("<b>Current Term: <\b>"));
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
    rEdit->setRange(-1000,1000);
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
    numTermsLayout = new QHBoxLayout();
    termsLayout = new QVBoxLayout();
    termEditLayout = new QHBoxLayout();
    functionConstantsFreqs = new QHBoxLayout();
    functionConstantsCoeffs = new QHBoxLayout();
    functionConstantsPairs = new QVBoxLayout();

    coeffPlaneEdit = new QPushButton(tr("Set on plane"), functionConstantsBox);
    
    numTermsLabel = new QLabel(tr("<b>Total Number of Terms: <\b>"), functionConstantsBox);
    numTermsEdit = new QSpinBox(functionConstantsBox);
    numTermsEdit->setRange(1, MAX_NUM_TERMS);
    
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

    functionTermsGrid->addWidget(numTermsLabel, 0, 0, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(numTermsEdit, 0, 1, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(termViewButton, 0, 2, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(currTermLabel, 1, 0, 2, 1, Qt::AlignLeft);
    functionTermsGrid->addWidget(currTermEdit, 1, 1, 2, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(freqpairLabel, 1, 2, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(nLabel, 1, 3, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(nEdit, 1, 4, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(mLabel, 1, 6, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(mEdit, 1, 7, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(coeffLabel, 2, 2, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(rLabel, 2, 3, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(rEdit, 2, 4, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(rValueLabel, 2, 5, 1, 1, Qt::AlignLeft);
    functionTermsGrid->addWidget(aLabel, 2, 6, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(aEdit, 2, 7, 1, 1, Qt::AlignCenter);
    functionTermsGrid->addWidget(aValueLabel, 2, 8, 1, 1, Qt::AlignLeft);
    functionTermsGrid->addWidget(coeffPlaneEdit, 2, 9, 1, 1, Qt::AlignCenter);
    
    functionConstantsBoxLayout->addLayout(functionTermsGrid);
    functionConstantsWidgetLayout->addWidget(functionConstantsBox);

}

// INIT PATTERN TYPE BOX
void interface::initPatternType()
{

    patternTypeBox = new QGroupBox(tr("Pattern Type"), patternTypeWidget);
    functionSel = new QComboBox(patternTypeBox);
    colorwheelSel = new QComboBox(patternTypeBox);

    gspacer1 = new QSpacerItem(0,20);
    gspacer2 = new QSpacerItem(0,10);
    gspacer3 = new QSpacerItem(0,10);
    gspacer4 = new QSpacerItem(0,50);
    gspacer5 = new QSpacerItem(0,10);

    patternTypeBoxOverallLayout = new QVBoxLayout(patternTypeWidget);
    patternTypeBoxLayout = new QVBoxLayout(patternTypeBox);
    functionLayout = new QVBoxLayout();
    colorwheelLayout = new QVBoxLayout();
    colorButtonLayout = new QHBoxLayout();
    globalConstantsLayout = new QVBoxLayout();
    globalConstantsGrid = new QGridLayout();

    setLoadedImage = new QPushButton(tr("Set/Change Image..."), patternTypeBox);
    fromImageButton = new QRadioButton(tr("Image"), patternTypeBox);
    fromColorWheelButton = new QRadioButton(tr("Color Wheel"), patternTypeBox);
    functionLabel = new QLabel(patternTypeBox);
    colorwheelLabel = new QLabel(patternTypeBox);
    imagePathLabel = new QLabel(patternTypeBox);
    globalConstantsLabel = new QLabel(tr("<b>Global Image Constants: <\b>"), patternTypeBox);
    scaleALabel = new QLabel(tr("Scaling Angle"), patternTypeBox);
    scaleRLabel = new QLabel(tr("Scaling Radius"), patternTypeBox); 
    scaleAEdit = new QLineEdit(patternTypeBox);
    scaleREdit = new QLineEdit(patternTypeBox);
    scaleAEdit->setFixedWidth(100);
    scaleREdit->setFixedWidth(100);
    scaleAEdit->setAlignment(Qt::AlignCenter);
    scaleREdit->setAlignment(Qt::AlignCenter);
    scalePlaneEdit = new QPushButton(tr("Set on plane"), patternTypeBox);

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
    
    
    functionIconsWindow = new QWidget(this, Qt::Window);
    functionIconsWindow->setWindowTitle(tr("Pattern Previews"));
    functionIconsWindow->move(70,160); // TODO this shouldn't be hardcoded!
    functionIconsWindowLayout = new QGridLayout(functionIconsWindow);

    functionIconsWindow->setWindowFlags(functionIconsWindow->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    
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
    
    colorwheelSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    colorwheelLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    // ASSEMBLE LAYOUT
    functionLayout->addWidget(functionLabel);
    functionLayout->addWidget(functionSel);
    functionLayout->addWidget(viewFunctionIconsButton);
    patternTypeBoxLayout->addLayout(functionLayout);
    patternTypeBoxLayout->addItem(gspacer1);

    colorwheelLayout->addItem(gspacer3);
    colorwheelLayout->addWidget(colorwheelLabel);
    colorwheelLayout->addWidget(fromColorWheelButton);
    colorwheelLayout->addWidget(colorwheelSel);
    colorwheelLayout->addWidget(fromImageButton);
    colorwheelLayout->addWidget(setLoadedImage);
    colorwheelLayout->addWidget(imagePathLabel);
    patternTypeBoxLayout->addLayout(colorwheelLayout);
    patternTypeBoxLayout->addItem(gspacer2);

    globalConstantsLayout->addItem(gspacer5);
    globalConstantsGrid->addWidget(scaleRLabel, 0, 0, 1, 1, Qt::AlignLeft);
    globalConstantsGrid->addWidget(scaleREdit, 0, 1, 1, 1, Qt::AlignCenter);
    globalConstantsGrid->addWidget(scaleALabel, 1, 0, 1, 1, Qt::AlignLeft);
    globalConstantsGrid->addWidget(scaleAEdit, 1, 1, 1, 1, Qt::AlignCenter);
    globalConstantsLayout->addWidget(globalConstantsLabel);
    globalConstantsLayout->addLayout(globalConstantsGrid);
    globalConstantsLayout->addWidget(scalePlaneEdit);
    patternTypeBoxLayout->addLayout(globalConstantsLayout);
    
    patternTypeBoxOverallLayout->addWidget(patternTypeBox);
    patternTypeBoxOverallLayout->addStretch();
    
}


// INIT IMAGE PROPERTIES BOX
void interface::initImageProps()
{
    imagePropsBoxLayout = new QVBoxLayout(imagePropsBox);
    
    imageShiftXLayout = new QHBoxLayout();
    imageShiftYLayout = new QHBoxLayout();

    imageStretchXLayout = new QHBoxLayout();
    imageStretchYLayout = new QHBoxLayout();
    
    XShiftLabel = new QLabel(tr("Horizontal Shift"), imagePropsBox);
    YShiftLabel = new QLabel(tr("Vertical Shift"), imagePropsBox);
    XShiftEdit = new QDoubleSlider(imagePropsBox);
    YShiftEdit = new QDoubleSlider(imagePropsBox);
    XShiftValueLabel = new QLineEdit(imagePropsBox);
    YShiftValueLabel = new QLineEdit(imagePropsBox);
    XShiftValueLabel->setValidator(doubleValidate);
    YShiftValueLabel->setValidator(doubleValidate);
    XShiftValueLabel->setFixedWidth(40);
    YShiftValueLabel->setFixedWidth(40);
    XShiftValueLabel->setAlignment(Qt::AlignCenter);
    YShiftValueLabel->setAlignment(Qt::AlignCenter);
    
    imageShiftXLayout->addWidget(XShiftLabel);
    imageShiftXLayout->addWidget(XShiftEdit);
    imageShiftXLayout->addWidget(XShiftValueLabel);
    
    imageShiftYLayout->addWidget(YShiftLabel);
    imageShiftYLayout->addWidget(YShiftEdit);
    imageShiftYLayout->addWidget(YShiftValueLabel);    
    
    worldWidthLabel = new QLabel(tr("Horizontal Stretch"), imagePropsBox);
    worldHeightLabel = new QLabel(tr("Vertical Stretch"), imagePropsBox);
    
    worldWidthEdit = new QDoubleSlider(imagePropsBox);
    worldHeightEdit = new QDoubleSlider(imagePropsBox);
    worldWidthValueLabel = new QLineEdit(imagePropsBox);
    worldHeightValueLabel = new QLineEdit(imagePropsBox);
    worldWidthValueLabel->setValidator(doubleValidate);
    worldHeightValueLabel->setValidator(doubleValidate);
    worldWidthValueLabel->setFixedWidth(40);
    worldHeightValueLabel->setFixedWidth(40);
    worldWidthValueLabel->setAlignment(Qt::AlignCenter);
    worldHeightValueLabel->setAlignment(Qt::AlignCenter);
    
    imageStretchXLayout->addWidget(worldWidthLabel);
    imageStretchXLayout->addWidget(worldWidthEdit);
    imageStretchXLayout->addWidget(worldWidthValueLabel);
    
    imageStretchYLayout->addWidget(worldHeightLabel);
    imageStretchYLayout->addWidget(worldHeightEdit);
    imageStretchYLayout->addWidget(worldHeightValueLabel);
    
    pspacer1 = new QSpacerItem(0, 20);
    
    XShiftEdit->setFixedWidth(100);
    YShiftEdit->setFixedWidth(100);
    XShiftEdit->setRange(-200, 150);
    XShiftEdit->setSingleStep(1);
    YShiftEdit->setRange(-200,150);
    YShiftEdit->setSingleStep(1);
    XShiftEdit->setOrientation(Qt::Horizontal);
    YShiftEdit->setOrientation(Qt::Horizontal);
    
    XShiftValueLabel->setText(QString::number(settings->XCorner));
    YShiftValueLabel->setText(QString::number(settings->YCorner));
    XShiftEdit->setValue(settings->XCorner * 100.0);
    YShiftEdit->setValue(settings->YCorner * 100.0);
    
    worldWidthEdit->setFixedWidth(100);
    worldHeightEdit->setFixedWidth(100);
    worldWidthEdit->setRange(0, 800);
    worldWidthEdit->setSingleStep(1);
    worldHeightEdit->setRange(0,800);
    worldHeightEdit->setSingleStep(1);
    worldWidthEdit->setOrientation(Qt::Horizontal);
    worldHeightEdit->setOrientation(Qt::Horizontal);
    
    worldWidthValueLabel->setText(QString::number(settings->Width));
    worldHeightValueLabel->setText(QString::number(settings->Height));
    worldWidthEdit->setValue(settings->Width * 100.0);
    worldHeightEdit->setValue(settings->Height * 100.0);
    
    // ASSEMBLE LAYOUT
    imagePropsBoxLayout->addLayout(imageShiftXLayout);
    imagePropsBoxLayout->addLayout(imageShiftYLayout);
    imagePropsBoxLayout->addItem(pspacer1);
    imagePropsBoxLayout->addLayout(imageStretchXLayout);
    imagePropsBoxLayout->addLayout(imageStretchYLayout);

}

void interface::initImageExportPopUp()
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
void interface::connectAllSignals()
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
    connect(resetImage, SIGNAL(clicked()), this, SLOT(resetImageFunction()));
    
    connect(fromColorWheelButton, SIGNAL(clicked()), this, SLOT(selectColorWheel()));
    connect(fromImageButton, SIGNAL(clicked()), this, SLOT(selectImage()));

    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), currColorWheel, SLOT(setCurrent(int)));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), this, SLOT(colorWheelChanged(int)));
    connect(setLoadedImage, SIGNAL(clicked()), this, SLOT(setImagePushed()));
    connect(functionSel, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFunction(int)));
    connect(numTermsEdit, SIGNAL(valueChanged(int)), this, SLOT(changeNumTerms(int)));
    connect(currTermEdit, SIGNAL(valueChanged(int)), this, SLOT(updateCurrTerm(int)));
    connect(termViewButton, SIGNAL(clicked()), this, SLOT(termViewPopUp()));
    connect(addTermButton, SIGNAL(clicked()), this, SLOT(addTermTable()));
    
    connect(nEdit, SIGNAL(valueChanged(int)), this, SLOT(changeN(int)));
    connect(mEdit, SIGNAL(valueChanged(int)), this, SLOT(changeM(int)));
    connect(rEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeR(double)));
    connect(aEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeA(double)));
    connect(scaleREdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleR(QString)));
    connect(scaleAEdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleA(QString)));
    
//    connect(outWidthEdit, SIGNAL(selectionChanged()), this, SLOT(changeOWidth(QString)));
//    connect(outHeightEdit, SIGNAL(selectionChanged()), this, SLOT(changeOHeight(QString)));
    connect(outWidthEdit, SIGNAL(editingFinished()), this, SLOT(changeOWidth()));
    connect(outHeightEdit, SIGNAL(editingFinished()), this, SLOT(changeOHeight()));
    
    
    connect(worldWidthEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeWorldWidth(double)));
    connect(worldHeightEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeWorldHeight(double)));
    connect(worldWidthValueLabel, SIGNAL(editingFinished()), this, SLOT(changeWorldWidth()));
    connect(worldHeightValueLabel, SIGNAL(editingFinished()), this, SLOT(changeWorldHeight()));
    
    
    connect(XShiftEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeXCorner(double)));
    connect(YShiftEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeYCorner(double)));
    connect(XShiftValueLabel, SIGNAL(editingFinished()), this, SLOT(changeXCorner()));
    connect(YShiftValueLabel, SIGNAL(editingFinished()), this, SLOT(changeYCorner()));
    
    connect(historyDisplay->viewMapper, SIGNAL(mapped(QString)), this, SLOT(loadSettings(QString)));
    connect(polarPlane, SIGNAL(setPolarCoordinates(int, QString, QString)), this, SLOT(setPolarCoordinates(int, QString, QString)));
    
    connect(updatePreviewShortcut, SIGNAL(activated()), this, SLOT(snapshotFunction()));
    
}


// concatenate constant name with current index number
QString interface::genLabel(const char *in)
{
    QString out;
    out.setNum(termIndex + 1);
    out.prepend(in);
    
    return out;
}

// update main window label names with appropriate text
void interface::refreshLabels()
{
    nLabel->setText(genLabel("n"));
    mLabel->setText(genLabel("m"));
    rLabel->setText(genLabel("r"));
    aLabel->setText(genLabel("a"));
}

// update term view table with appropriate values
void interface::refreshTableTerms()
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
void interface::refreshMainWindowTerms()
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
    
    mEdit->setValue(currFunction->getM(termIndex));
    nEdit->setValue(currFunction->getN(termIndex));
    aEdit->setValue(currFunction->getA(termIndex) * 100);
    rEdit->setValue(currFunction->getR(termIndex) * 100);

    aValueLabel->setText(QString::number(currFunction->getA(termIndex)));
    rValueLabel->setText(QString::number(currFunction->getR(termIndex)));
    
    mEdit->blockSignals(false);
    nEdit->blockSignals(false);
    aEdit->blockSignals(false);
    rEdit->blockSignals(false);
    
    refreshLabels(); // update labels with appropriate text
    
}

// removes a term
void interface::removeTerm(int row)
{
    
    qDebug() << "removing term" << row + 1;

    if (row <= 0 && termViewTable->rowCount() == 1) {
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
    currFunction->removeTerm(term);
    numTerms = currFunction->getNumTerms();
    
    currTermEdit->blockSignals(true);
    currTermEdit->setMaximum(currFunction->getNumTerms() + 1);
    currTermEdit->blockSignals(false);

}


// adds a new term
void interface::addTerm()
{
    unsigned int highestIndex = termViewTable->rowCount();
    termViewTable->setRowCount(highestIndex + 1);
    
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
void interface::resetImageFunction()
{

    termIndex = 0;

    currFunction->refresh();
    numTermsEdit->setValue(1);
    
    scaleREdit->setText(QString::number(currFunction->getScaleR()));
    scaleAEdit->setText(QString::number(currFunction->getScaleA()));
    
    worldWidthEdit->setValue(DEFAULT_WIDTH * 100.0);
    worldHeightEdit->setValue(DEFAULT_HEIGHT * 100.0);
    XShiftEdit->setValue(DEFAULT_XCORNER * 100.0);
    YShiftEdit->setValue(DEFAULT_YCORNER * 100.0);
}

//void interface::toggleViewMode()
//{
//    if (advancedMode) {
//        advancedMode = false;
//        toggleViewButton->setText(tr("Switch to Advanced View"));
//    } else {
//        advancedMode = true;
//        toggleViewButton->setText(tr("Switch to Default View"));
//    }
//}


void interface::termViewPopUp()
{
    if (!termViewWidget->isVisible()) {
        termViewWidget->show();
    }
    
}

// updates the term that is currently being edited
void interface::updateCurrTerm(int i)
{
    
    if (i > 0) termIndex = i - 1;
    
    refreshTableTerms();
    refreshMainWindowTerms();

}

// updates the number of terms of the current function
void interface::changeNumTerms(int i)
{

    numTermsEdit->setEnabled(false);

    int oldNumTerms = numTerms;

    qDebug() << "old:" << oldNumTerms << "| new:" << i;    

    if (i < oldNumTerms) {
        for (int k = oldNumTerms; k > i; --k) { removeTerm(k-1); }
    } else if (i > oldNumTerms) {
        currFunction->setNumTerms(i);
        numTerms = currFunction->getNumTerms();
        for (int k = oldNumTerms; k < i; ++k) addTerm();
    }
    
    currTermEdit->blockSignals(true);
    currTermEdit->setMaximum(i);
    currTermEdit->blockSignals(false);

    updateCurrTerm(i);
    updatePreviewDisplay();
}

// handles changing to a new color wheel
void interface::colorWheelChanged(int /* unused */ )
{
    // if(index == 9)
    //     setLoadedImage->show();
    // else
    //     setLoadedImage->hide();
    updatePreviewDisplay();
}

void interface::selectColorWheel() 
{
    colorwheelSel->setEnabled(true);
    setLoadedImage->setEnabled(false);
    imagePathLabel->setEnabled(false);

    currColorWheel->setCurrent(colorwheelSel->currentIndex());

    updatePreviewDisplay();
}

void interface::selectImage()
{
    colorwheelSel->setEnabled(false);
    setLoadedImage->setEnabled(true);
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
            updatePreviewDisplay();
        }
    }
}

// handles loading an image to use as a color wheel
void interface::setImagePushed()
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
void interface::changeFunction(int index)
{
    
    termIndex = 0;
    
    currFunction = functionVector[index];
    
    numTerms = currFunction->getNumTerms();

    termViewTable->setRowCount(0);

    currTermEdit->setMaximum(numTerms);
    
    previewDisplayPort->changeFunction(currFunction);
    imageExportPort->changeFunction(currFunction);
    
    refreshMainWindowTerms();
    refreshTableTerms();
    updatePreviewDisplay();

}


// SLOT function called when user attempts to save current settings into a wpr file
void interface::saveCurrSettings()
{
    
    qDebug() << "saving setting";

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
    out << currFunction->getNumTerms();
    
    unsigned int i;

    for(int index = 0; index < currFunction->getNumTerms(); index++)
    {
        i = index;
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
    
    refreshTableTerms();

    worldWidthEdit->setValue(settings->Width * 100.0);
    worldHeightEdit->setValue(settings->Height * 100.0);
    XShiftEdit->setValue(settings->XCorner * 100.0);
    YShiftEdit->setValue(settings->YCorner * 100.0);

    updatePreviewDisplay();
    
    QDir stickypath(fileName);
    stickypath.cdUp();
    return stickypath.path();
}

// updates the preview to reflect changes to the settings, function, and color wheel
void interface::updatePreviewDisplay()
{
    updatePreview->setEnabled(false);

    displayProgressBar->reset();
    
    previewDisplayPort->paintToDisplay(disp);
}

// slot function called when clicked "update preview" button to add to history and update the preview display to reflect current settings
void interface::snapshotFunction()
{
    historyDisplay->show();
    
    QDateTime savedTime = QDateTime::currentDateTimeUtc();
    QString newFile = savedTime.toString("MM.dd.yyyy.hh.mm.ss.zzz.t").append(".wpr");
    QString filePath = saveSettings(newFile).append("/" + newFile);

    historyDisplay->triggerAddToHistory(savedTime, filePath, currFunction, currColorWheel, settings);

    //updatePreviewDisplay();
    
}

// SLOT FUNCTIONS TO CHANGE OUTPUT IMAGE PROPERTIES
void interface::changeOHeight()
{
    
    int val = outHeightEdit->text().toInt();
    if (val < MIN_IMAGE_DIM || val > MAX_IMAGE_DIM) {
        errorHandler(INVALID_OUTPUT_IMAGE_DIM);
    }
    settings->OHeight = val;
    imageExportPort->changeSettings(settings);
    //updatePreviewDisplay();
}

void interface::changeOWidth()
{
    
    int val = outWidthEdit->text().toInt();
    if (val < MIN_IMAGE_DIM || val > MAX_IMAGE_DIM) {
        errorHandler(INVALID_OUTPUT_IMAGE_DIM);
    }
    settings->OWidth = val;
    imageExportPort->changeSettings(settings);
    //updatePreviewDisplay();
}

void interface::changeWorldHeight(double val)
{
    settings->Height = val;
    worldHeightValueLabel->setText(QString::number(val));
    updatePreviewDisplay();
}

void interface::changeWorldHeight()
{
    double val = worldHeightValueLabel->text().toDouble();
    settings->Height = val;
    worldHeightEdit->setValue(val * 100.0);
    //worldHeightValueLabel->setText(val);
    updatePreviewDisplay();
}

void interface::changeWorldWidth(double val)
{
    settings->Width = val;
    worldWidthValueLabel->setText(QString::number(val));
    updatePreviewDisplay();
}

void interface::changeWorldWidth()
{
    double val = worldWidthValueLabel->text().toDouble();
    settings->Width = val;
    worldWidthEdit->setValue(val * 100.0);
    updatePreviewDisplay();
}

void interface::changeXCorner(double val)
{
    settings->XCorner = val;
    XShiftValueLabel->setText(QString::number(val));
    updatePreviewDisplay();
}

void interface::changeXCorner()
{
    double val = XShiftValueLabel->text().toDouble();
    settings->XCorner = val;
    XShiftEdit->setValue(val * 100.0);
    updatePreviewDisplay();
}

void interface::changeYCorner(double val)
{
    settings->YCorner = val;
    YShiftValueLabel->setText(QString::number(val));
    updatePreviewDisplay();
}

void interface::changeYCorner()
{
    double val = YShiftValueLabel->text().toDouble();
    settings->YCorner = val;
    YShiftEdit->setValue(val * 100.0);
    updatePreviewDisplay();
}


// SLOT FUNCTIONS TO CHANGE FREQ AND COEFF PAIRS
void interface::changeN(int val)
{

    qDebug() << "change N";

    currFunction->setN(termIndex, val);
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void interface::changeM(int val)
{
    qDebug() << "change M";
    
    currFunction->setM(termIndex, val);
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void interface::changeR(double val)
{
    
    qDebug() << "change R";

    currFunction->setR(termIndex, val);
    rValueLabel->setText(QString::number(val));
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void interface::changeA(double val)
{

    qDebug() << "change A";

    currFunction->setA(termIndex, val);
    aValueLabel->setText(QString::number(val));
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void interface::changeScaleA(const QString &val)
{

    qDebug() << "change scale A";

    double passedval = val.toDouble();
    currFunction->setScaleA(passedval);
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}

void interface::changeScaleR(const QString &val)
{

    qDebug() << "change scale R";

    double passedval = val.toDouble();
    currFunction->setScaleR(passedval);
    refreshTableTerms();
    refreshMainWindowTerms();
    updatePreviewDisplay();
}



void interface::startImageExport() 
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
void interface::errorHandler(const int &flag)
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


// SLOT function to handle when a cell is clicked in the term table
void interface::termViewCellClicked(int row, int col)
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
void interface::updateTermTable(QObject *cell)
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

void interface::addTermTable() 
{
    addTermButton->blockSignals(true);
    int newNumTerms = currFunction->getNumTerms() + 1;
    currFunction->setNumTerms(newNumTerms);
    numTermsEdit->setValue(newNumTerms);
} 

// pop up window to appear when image file has finished exporting
void interface::popUpImageExportFinished(const QString &filePath)
{
    
    QMessageBox msgBox;
    msgBox.setText(tr("The file has been successfully saved to: ").append(filePath));
    msgBox.exec();

    saveloadPath = filePath;
    
    exportProgressBar->remove();
    if (!exportProgressBar) delete exportProgressBar;
}

// reset the table to receive signals - prevent updating too fast
void interface::resetTableButton() 
{
    addTermButton->blockSignals(false);
    termViewTable->blockSignals(false);
}

// reset the main window to receive signals - prevent updating too fast
void interface::resetMainWindowButton(const bool &status) 
{
    numTermsEdit->setEnabled(status);
    updatePreview->setEnabled(status);
}


