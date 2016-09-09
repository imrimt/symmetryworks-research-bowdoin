#include "Interface.h"
/*
 *
 *  Interface class manages the UI elements for wallgen.
 *
 *
 *  Last Modified: 08/03/2016 by Bridget E. Went and Son D. Ngo
 *
 *
 *
 */
Interface::Interface(QWidget *parent) : QWidget(parent)
{

    // FUNCTIONAL VARIABLES
    newUpdate = true;
    newAction = true;
    termIndex = 0;
    saveloadPath = QDir::homePath();
    errPrint = false;
    
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
    
    
    initInterfaceLayout();
    
    updatePreviewShortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
    undoStack = new QUndoStack(this);
    connectAllSignals();
    
    functionSel->setCurrentIndex(0);
    colorwheelSel->setCurrentIndex(0);
    
    functionSel->setFocus();

    //CREATE SNAPSHOT FOLDER
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    dir.mkdir("Wallgen Snapshots");

    snapshotFolderPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/Wallgen Snapshots";
    
    infoPopUp = new QMessageBox(this);
//    infoPopUp->setIcon(QMessageBox::Information);
    infoPopUp->setText(tr("Wallgen Software Overview"));
    infoPopUp->setInformativeText(tr("Welcome to wallgen!"));
    infoPopUp->hide();
    
    // Set Up Tab Orders
    setTabOrder(functionSel, colorwheelSel);
    setTabOrder(colorwheelSel, scaleREdit);
    setTabOrder(scaleREdit, scaleAEdit);
    setTabOrder(scaleAEdit, XShiftEdit);
    setTabOrder(XShiftEdit, YShiftEdit);
    setTabOrder(YShiftEdit, worldWidthEdit);
    setTabOrder(worldWidthEdit, worldHeightEdit);
    setTabOrder(worldHeightEdit, numTermsEdit);
    setTabOrder(numTermsEdit, currTermEdit);
    setTabOrder(currTermEdit, mEdit);
    setTabOrder(nEdit, mEdit);
    setTabOrder(mEdit, rEdit);
    setTabOrder(rEdit, aEdit);
    
    refreshTableTerms();
    updatePreviewDisplay();
    
    
}

// INIT INTERFACE LAYOUT
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
    spacerItem2 = new QSpacerItem(1, screenGeometry.height() * 0.02);
    
    
    leftbarLayout->addWidget(patternTypeBox);
    leftbarLayout->addItem(spacerItem);
    leftbarLayout->addWidget(globalScalingBox);
    leftbarLayout->addItem(spacerItem2);
    leftbarLayout->addWidget(imagePropsBox);
    
    topbarLayout->addLayout(leftbarLayout);
    topbarLayout->addWidget(displayWidget);
    
    InterfaceLayout->addLayout(topbarLayout);
    InterfaceLayout->addWidget(functionConstantsWidget);
    
    overallLayout->addLayout(InterfaceLayout);
    overallLayout->activate();
    this->setLayout(overallLayout);
    
    errorMessageBox = new QMessageBox(this);
    errorMessageBox->setIcon(QMessageBox::Critical);
    errorMessageBox->addButton(QMessageBox::Close);
    //connect(errorMessageBox, SIGNAL(accepted()), errorMessageBox, SLOT(accept()));
    
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
    initToolTips();

    // globalSnapshotFolder = QStandPaths::writableLocation(QStandPaths::DesktopLocation) + "/Wallgen Snapshots";
}



// INIT WALLPAPER PREVIEW DISPLAY
void Interface::initPreviewDisplay()
{
    
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    previewWidth = screenGeometry.width() * PREVIEW_SCALING;
    previewHeight = screenGeometry.height() * PREVIEW_SCALING;
    previewSize = previewWidth > previewHeight ? previewWidth : previewHeight;
    
    disp = new Display(previewSize, previewSize, displayWidget);
    snapshotButton= new QPushButton(tr("Snapshot"), this);
    dispLayout = new QVBoxLayout(displayWidget);
    //dispLayout->setSizeConstraint(QLayout::SetMinimumSize);
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(snapshotButton);
    
    imageExportPort = new Port(currFunction, currColorWheel, settings->OWidth, settings->OHeight, settings);
    previewDisplayPort = new Port(currFunction, currColorWheel, disp->getWidth(), disp->getHeight(), settings);
    
    displayProgressBar = new ProgressBar(tr("Preview"), previewDisplayPort);
    exportProgressBar = new ProgressBar(tr("Exporting"), imageExportPort);
    
    displayProgressBar->setPermanentVisibility(false);
    exportProgressBar->setPermanentVisibility(true);
    
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
    currTermEdit->installEventFilter(this);
    
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
    nEdit->installEventFilter(this);
    mEdit->installEventFilter(this);
    
    nEdit->setFixedWidth(100);
    mEdit->setFixedWidth(100);
    rEdit->setFixedWidth(100);
    aEdit->setFixedWidth(100);
    nEdit->setRange(MIN_FREQ_VALUE, MAX_FREQ_VALUE);
    nEdit->setSingleStep(FREQ_SPINBOX_STEP);
    mEdit->setRange(MIN_FREQ_VALUE, MAX_FREQ_VALUE);
    mEdit->setSingleStep(FREQ_SPINBOX_STEP);
    // aEdit->setRange((-314, 314);
    aEdit->setRange((int)(-pi*100), (int)(pi*100));
    aEdit->setSingleStep(ANGLE_SPINBOX_STEP * 100);
    rEdit->setRange(0,(int)MAX_RADIUS * 100);
    rEdit->setSingleStep(RADIUS_SPINBOX_STEP * 10);
    
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
    
    coeffPlaneEdit = new QPushButton(tr("Set on Plane"), functionConstantsBox);
    
    numTermsLabel = new QLabel(tr("<b>Total Number of Terms: <\b>"), functionConstantsBox);
    numTermsEdit = new QSpinBox(functionConstantsBox);
    numTermsEdit->setValue(1);
    numTermsEdit->setRange(1, MAX_NUM_TERMS);
    numTermsEdit->installEventFilter(this);
    
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
    addTermButton->setIcon(*(new QIcon(":/Images/Icons/plus.png")));
    
    // resize all columns to maximum stretch
    for (int c = 0; c < termViewTable->horizontalHeader()->count(); ++c)
    {
        if (c == 5)
            termViewTable->horizontalHeader()->resizeSection(5, 30);
        else
            termViewTable->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    
    // signal mappers
    termViewTableMapper = new QSignalMapper(this);
    removeTermMapper = new QSignalMapper(this);
    
    //add initial term
    addTerm();
    
    termViewHeaderVertical= termViewTable->verticalHeader();
    termViewHeaderVertical->resizeSections(QHeaderView::Stretch);
    
    termViewLayout->addWidget(termViewTable);
    termViewLayout->addWidget(addTermButton);
    termViewWidget->setLayout(termViewLayout);
    
    
    // set up the grid layout
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
    scaleAEdit = new CustomLineEdit(patternTypeBox);
    scaleREdit = new CustomLineEdit(patternTypeBox);
    
    
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
    scalePlaneEdit = new QPushButton(tr("Set on Plane"), globalScalingBox);
    
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
    
    functionSel->setFocusPolicy(Qt::StrongFocus);
    colorwheelSel->setFocusPolicy(Qt::StrongFocus);
    
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
    
    //initialize function previews window
    functionIconsWindow = new QWidget(this, Qt::Window);
    functionIconsWindow->setWindowTitle(tr("Pattern Previews"));
    functionIconsWindowLayout = new QGridLayout(functionIconsWindow);
    
    int row = 0;
    int col = 0;
    
    // init pattern preview grid layout
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
    QPen pointPen(Qt::black);
    QBrush pointBrush(Qt::gray);
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
    
    imageDataWindowGraphLayout->addWidget(imageDataGraphView);
    imageDataWindowGraphLayout->addWidget(updateImageDataGraphButton);
    imageDataWindowLayout->addLayout(imageDataWindowGraphLayout);
    imageDataWindowLayout->addWidget(imageLabel);
    imageDataWindow->setLayout(imageDataWindowLayout);
    imageDataWindow->setFixedSize(previewSize * 2, previewSize);
    
    functionLayout->addWidget(functionLabel);
    functionLayout->addWidget(functionSel);
    functionLayout->addWidget(viewFunctionIconsButton);
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
    XShiftEdit = new CustomLineEdit(imagePropsBox);
    YShiftEdit = new CustomLineEdit(imagePropsBox);
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
    worldWidthEdit = new CustomLineEdit(imagePropsBox);
    worldHeightEdit = new CustomLineEdit(imagePropsBox);
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
    XShiftEditSlider->setRange(-1000, 1000);
    XShiftEditSlider->setSingleStep(1);
    YShiftEditSlider->setRange(-1000, 1000);
    YShiftEditSlider->setSingleStep(1);
    XShiftEditSlider->setOrientation(Qt::Horizontal);
    YShiftEditSlider->setOrientation(Qt::Horizontal);
    
    XShiftEdit->setText(QString::number(settings->XCorner));
    YShiftEdit->setText(QString::number(settings->YCorner));
    XShiftEditSlider->setValue(settings->XCorner * 100.0);
    YShiftEditSlider->setValue(settings->YCorner * 100.0);
    
    worldWidthEditSlider->setFixedWidth(100);
    worldHeightEditSlider->setFixedWidth(100);
    worldWidthEditSlider->setRange(10, 1000);
    worldWidthEditSlider->setSingleStep(1);
    worldHeightEditSlider->setRange(10, 1000);
    worldHeightEditSlider->setSingleStep(1);
    worldWidthEditSlider->setOrientation(Qt::Horizontal);
    worldHeightEditSlider->setOrientation(Qt::Horizontal);
    
    worldWidthEdit->setText(QString::number(settings->Width));
    worldHeightEdit->setText(QString::number(settings->Height));
    worldWidthEditSlider->setValue(settings->Width * 100.0);
    worldHeightEditSlider->setValue(settings->Height * 100.0);
    
    imagePropsBoxLayout->addLayout(imageShiftXLayout);
    imagePropsBoxLayout->addLayout(imageShiftYLayout);
    imagePropsBoxLayout->addLayout(imageStretchXLayout);
    imagePropsBoxLayout->addLayout(imageStretchYLayout);
    
    
}

void Interface::initImageExportPopUp()
{
    // IMAGE DIMENSIONS POP UP WINDOW
    imageDimensionsPopUp = new QWidget(this, Qt::Window);
    imageDimensionsPopUp->setWindowModality(Qt::WindowModal);
    imageDimensionsPopUp->setWindowTitle(tr("Output Image Dimensions"));
    imageDimensionsPopUpLayout = new QVBoxLayout(imageDimensionsPopUp);
    
    outHeightEdit = new QLineEdit(imageDimensionsPopUp);
    outWidthEdit = new QLineEdit(imageDimensionsPopUp);
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
    
    outWidthEdit->setText(QString::number(DEFAULT_OUTPUT_WIDTH));
    outHeightEdit->setText(QString::number(DEFAULT_OUTPUT_HEIGHT));
    
    double width = outWidthEdit->text().toDouble() * ASPECT_RATIO_SCALE;
    double height = outHeightEdit->text().toDouble() * ASPECT_RATIO_SCALE;
    aspectRatio = (double)(width / height);
    
    aspectRatioEditLayout = new QHBoxLayout();
    aspectRatioPreviewLayout = new QHBoxLayout();
    aspectRatioLabel = new QLabel(tr("Aspect Ratio: "));
    aspectRatioEdit = new QLineEdit();
    aspectRatioEdit->setFixedWidth(100);
    aspectRatioEdit->setMaxLength(4);
    aspectRatioEdit->setAlignment(Qt::AlignCenter);
    aspectRatioEdit->setText(QString::number(aspectRatio));
    
    aspectRatioWidget = new QWidget();
    aspectRatioWidget->setFixedSize(MAX_IMAGE_DIM * 0.25 * ASPECT_RATIO_SCALE, MAX_IMAGE_DIM * 0.25 * ASPECT_RATIO_SCALE);
    aspectRatioPreview = new Display(MAX_IMAGE_DIM * 0.25 * ASPECT_RATIO_SCALE, MAX_IMAGE_DIM * 0.25 * ASPECT_RATIO_SCALE, aspectRatioWidget);
    QSize size = aspectRatioPreview->changeDisplayDimensions(width, height);
    aspectRatioPreviewLayout->addWidget(aspectRatioWidget);
    aspectRatioPreviewLayout->setAlignment(Qt::AlignCenter);
    aspectRatioPreviewDisplayPort = new Port(currFunction, currColorWheel, size.width(), size.height(), settings);
    aspectRatioPreviewDisplayPort->paintToDisplay(aspectRatioPreview);
    
    aspectRatioEditLayout->addWidget(aspectRatioLabel);
    aspectRatioEditLayout->addWidget(aspectRatioEdit);
    
    imageDimensionsPopUpLayout->addLayout(outWidthLayout);
    imageDimensionsPopUpLayout->addLayout(outHeightLayout);
    imageDimensionsPopUpLayout->addItem(new QSpacerItem(10, 20));
    imageDimensionsPopUpLayout->addLayout(aspectRatioEditLayout);
    imageDimensionsPopUpLayout->addLayout(aspectRatioPreviewLayout);
    
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    imageDimensionsPopUpLayout->addWidget(buttonBox);
    
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(startImageExport()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancelImageExport()));
    
    
    imageDimensionsPopUp->hide();
    
}

void Interface::initToolTips()
{
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(14);
    QToolTip::setFont(font);
    
    QString lineEditText = "Press <i> Enter </i> when finish editing to update preview.";
    
    //patern properties
    functionLabel->setToolTip("Select among 17 different wallpaper patterns.");
    colorwheelLabel->setToolTip("Select among different color wheels or load in an image.");
    
    scaleRLabel->setToolTip("Changes which points on the color wheel\n will be called up by the wallpaper function.");
    scaleALabel->setToolTip("Changes which points on the color wheel\n will be called up by the wallpaper function.");
    
    QString freqToolTip = "Larger values of <b>n</b> and <b>m</b> will make your wallpaper pattern more 'wiggly.' \nThese represent directional frequencies of waves.";
    freqpairLabel->setToolTip(freqToolTip);
    mLabel->setToolTip(freqToolTip);
    nLabel->setToolTip(freqToolTip);
    
    QString coeffToolTip = "These represent the radius and angle of the wave function.";
    QString rToolTip = "Increasing <b>r</b> reflects in larger 'nuclei.'";
    QString aToolTip = "Adjusting <b>a</b> will rotate about the nuclei of your pattern.";
    coeffLabel->setToolTip(coeffToolTip);
    rLabel->setToolTip(rToolTip);
    aLabel->setToolTip(aToolTip);
}





// CONNECT SIGNALS TO SLOTS
void Interface::connectAllSignals()
{

    connect(disp, SIGNAL(displayPressed(QPoint)), this, SLOT(startShifting(QPoint)));
    connect(disp, SIGNAL(displayReleased(QPoint)), this, SLOT(finishShifting(QPoint)));
    connect(disp, SIGNAL(displayMoved(QPoint)), this, SLOT(updateShifting(QPoint)));

    connect(snapshotButton, SIGNAL(clicked()), this, SLOT(snapshotFunction()));
    
    connect(functionSel, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFunction(int)));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), currColorWheel, SLOT(setCurrent(int)));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), this, SLOT(colorWheelChanged(int)));
    connect(fromColorWheelButton, SIGNAL(clicked()), this, SLOT(selectColorWheel()));
    connect(fromImageButton, SIGNAL(clicked()), this, SLOT(selectImage()));
    connect(setLoadedImage, SIGNAL(clicked()), this, SLOT(setImagePushed()));
    connect(setOverflowColorPopUp, SIGNAL(colorSelected(QColor)), this, SLOT(changeOverflowColor(QColor)));
    connect(setOverflowColorPopUp, SIGNAL(accepted()), this, SLOT(selectImage()));
    connect(updateImageDataGraphButton, SIGNAL(clicked()), this, SLOT(updateImageDataGraph()));
    
    connect(scaleREdit, SIGNAL(returnPressed()), this, SLOT(changeScaleR()));
    connect(scaleAEdit, SIGNAL(returnPressed()), this, SLOT(changeScaleA()));
    connect(scaleREditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeScaleR(double)));
    connect(scaleREditSlider, SIGNAL(newSliderAction(QObject*, double, double)), this, SLOT(createUndoAction(QObject*, double, double)));
    connect(scaleAEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeScaleA(double)));
    connect(scaleAEditSlider, SIGNAL(newSliderAction(QObject*, double, double)), this, SLOT(createUndoAction(QObject*, double, double)));
    
    connect(numTermsEdit, SIGNAL(valueChanged(int)), this, SLOT(changeNumTerms(int)));
    connect(currTermEdit, SIGNAL(valueChanged(int)), this, SLOT(updateCurrTerm(int)));
    
    connect(termViewTable, SIGNAL(cellClicked(int, int)), this, SLOT(termViewCellClicked(int, int)));
    connect(termViewButton, SIGNAL(clicked()), this, SLOT(termViewPopUp()));
    connect(termViewTableMapper, SIGNAL(mapped(QObject*)), this, SLOT(updateTermTable(QObject*)));
    connect(addTermButton, SIGNAL(clicked()), this, SLOT(addTermTable()));
    connect(nEdit, SIGNAL(valueChanged(int)), this, SLOT(changeN(int)));
    connect(mEdit, SIGNAL(valueChanged(int)), this, SLOT(changeM(int)));
    connect(rEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeR(double)));
    connect(aEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeA(double)));
    
    connect(worldWidthEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeWorldWidth(double)));
    connect(worldWidthEditSlider, SIGNAL(newSliderAction(QObject*, double, double)), this, SLOT(createUndoAction(QObject*, double, double)));
    connect(worldHeightEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeWorldHeight(double)));
    connect(worldHeightEditSlider, SIGNAL(newSliderAction(QObject*, double, double)), this, SLOT(createUndoAction(QObject*, double, double)));
    connect(worldWidthEdit, SIGNAL(returnPressed()), this, SLOT(changeWorldWidth()));
    connect(worldHeightEdit, SIGNAL(returnPressed()), this, SLOT(changeWorldHeight()));
    connect(XShiftEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeXCorner(double)));
    connect(XShiftEditSlider, SIGNAL(newSliderAction(QObject*, double, double)), this, SLOT(createUndoAction(QObject*, double, double)));
    connect(YShiftEditSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeYCorner(double)));
    connect(YShiftEditSlider, SIGNAL(newSliderAction(QObject*, double, double)), this, SLOT(createUndoAction(QObject*, double, double)));
    connect(XShiftEdit, SIGNAL(returnPressed()), this, SLOT(changeXCorner()));
    connect(YShiftEdit, SIGNAL(returnPressed()), this, SLOT(changeYCorner()));
    
    connect(coeffPlaneEdit, SIGNAL(clicked()), polarPlaneMapper, SLOT(map()));
    connect(scalePlaneEdit, SIGNAL(clicked()), polarPlaneMapper, SLOT(map()));
    polarPlaneMapper->setMapping(coeffPlaneEdit, LOCAL_FLAG);
    polarPlaneMapper->setMapping(scalePlaneEdit, GLOBAL_FLAG);
    connect(polarPlaneMapper,SIGNAL(mapped(int)), polarPlane, SLOT(showPlanePopUp(int)));
    connect(polarPlane, SIGNAL(setPolarCoordinates(int, QString, QString)), this, SLOT(setPolarCoordinates(int, QString, QString)));
    
    connect(outWidthEdit, SIGNAL(returnPressed()), this, SLOT(changeOWidth()));
    connect(outHeightEdit, SIGNAL(returnPressed()), this, SLOT(changeOHeight()));
    connect(aspectRatioEdit, SIGNAL(returnPressed()), this, SLOT(changeAspectRatio()));
    //connect(aspectRatioEdit, SIGNAL(editingFinished()), this, SLOT(changeAspectRatio()));
    
    connect(previewDisplayPort->getControllerObject(), SIGNAL(partialProgressChanged(double)), displayProgressBar, SLOT(partialUpdate(double)));
    connect(previewDisplayPort, SIGNAL(paintingFinished(bool)), this, SLOT(resetMainWindowButton(bool)));
    connect(displayProgressBar, SIGNAL(renderFinished()), this, SLOT(resetTableButton()));
    connect(imageExportPort, SIGNAL(finishedExport(QString)), this, SLOT(popUpImageExportFinished(QString)));
    connect(imageExportPort->getControllerObject(), SIGNAL(partialProgressChanged(double)), exportProgressBar, SLOT(partialUpdate(double)));
    qRegisterMetaType<ComplexValue>("ComplexValue");
    connect(previewDisplayPort->getControllerObject(), SIGNAL(newImageDataPoint(ComplexValue)), this, SLOT(addNewImageDataPoint(ComplexValue)));
    
    //shortcut
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
    scaleAEditSlider->blockSignals(true);
    scaleREdit->blockSignals(true);
    scaleREditSlider->blockSignals(true);
    
    oldM = currFunction->getM(termIndex);
    oldN = currFunction->getN(termIndex);
    mEdit->setValue(oldM);
    nEdit->setValue(oldN);
    aEdit->setValue(currFunction->getA(termIndex) * 100);
    rEdit->setValue(currFunction->getR(termIndex) * 100);
    scaleAEdit->setText(QString::number(currFunction->getScaleA()));
    scaleAEditSlider->setValue(currFunction->getScaleA() * 100);
    scaleREdit->setText(QString::number(currFunction->getScaleR()));
    scaleREditSlider->setValue(currFunction->getScaleR() * 100);
    
    mEdit->blockSignals(false);
    nEdit->blockSignals(false);
    aEdit->blockSignals(false);
    rEdit->blockSignals(false);
    scaleAEdit->blockSignals(false);
    scaleAEditSlider->blockSignals(false);
    scaleREdit->blockSignals(false);
    scaleREditSlider->blockSignals(false);
    
    aValueLabel->setText(QString::number(currFunction->getA(termIndex)));
    rValueLabel->setText(QString::number(currFunction->getR(termIndex)));
    
    refreshLabels(); // update labels with appropriate text
    
}

// removes a term
void Interface::removeTerm(int row)
{
    
    removeTableTerm(row);
    
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

void Interface::removeTableTerm(int row)
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
    
    if (static_cast<int>(termIndex) >= termViewTable->rowCount()) {
        termIndex = termViewTable->rowCount() - 1;
    }
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
    mEditTable->installEventFilter(this);
    nEditTable->installEventFilter(this);
    aEditTable->installEventFilter(this);
    rEditTable->installEventFilter(this);
    
    nEditTable->setRange(MIN_FREQ_VALUE,MAX_FREQ_VALUE);
    nEditTable->setSingleStep(FREQ_SPINBOX_STEP);
    mEditTable->setRange(MIN_FREQ_VALUE,MAX_FREQ_VALUE);
    mEditTable->setSingleStep(FREQ_SPINBOX_STEP);
    aEditTable->setRange(-pi, pi);
    aEditTable->setSingleStep(ANGLE_SPINBOX_STEP);
    rEditTable->setRange(MIN_FREQ_VALUE,MAX_FREQ_VALUE);
    rEditTable->setSingleStep(RADIUS_SPINBOX_STEP);
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
    
    oldMTable.append(mEditTable->value());
    oldNTable.append(nEditTable->value());
    oldATable.append(aEditTable->value());
    oldRTable.append(rEditTable->value());
    
    // connect signals
    connect(mEditTable, SIGNAL(valueChanged(int)), termViewTableMapper, SLOT(map()));
    connect(nEditTable, SIGNAL(valueChanged(int)), termViewTableMapper, SLOT(map()));
    connect(rEditTable, SIGNAL(valueChanged(double)), termViewTableMapper, SLOT(map()));
    connect(aEditTable, SIGNAL(valueChanged(double)), termViewTableMapper, SLOT(map()));
    
    termViewTableMapper->setMapping(mEditTable, (QObject*)new QPoint(highestIndex, 1));
    termViewTableMapper->setMapping(nEditTable, (QObject*)new QPoint(highestIndex, 2));
    termViewTableMapper->setMapping(aEditTable, (QObject*)new QPoint(highestIndex, 3));
    termViewTableMapper->setMapping(rEditTable, (QObject*)new QPoint(highestIndex, 4));
    
    refreshMainWindowTerms();
    // updatePreviewDisplay();
    // addTermButton->blockSignals(false);
    
}


// reset the image to its default settings with the current function and colorwheel
void Interface::resetFunction()
{
    newUpdate = false;
    termIndex = 0;
    
    currFunction->refresh();
    numTermsEdit->setValue(1);
    
    // scaleREdit->setText(QString::number(currFunction->getScaleR()));
    // scaleAEdit->setText(QString::number(currFunction->getScaleA()));
    // scaleREditSlider->setValue(currFunction->getScaleR() * 100.0);
    // scaleAEditSlider->setValue(currFunction->getScaleA() * 100.0);
    
    changeScaleR(currFunction->getScaleR());
    changeScaleA(currFunction->getScaleA());
    
    // changeWorldHeight(DEFAULT_WORLD_HEIGHT);
    // changeWorldWidth(DEFAULT_WORLD_WIDTH);
    // changeXCorner(DEFAULT_XCORNER);
    // changeYCorner(DEFAULT_YCORNER);
    
    worldWidthEditSlider->setValue(DEFAULT_WORLD_HEIGHT * 100.0);
    worldHeightEditSlider->setValue(DEFAULT_WORLD_WIDTH * 100.0);
    XShiftEditSlider->setValue(DEFAULT_XCORNER * 100.0);
    YShiftEditSlider->setValue(DEFAULT_YCORNER * 100.0);
    
    currColorWheel->changeOverflowColor(Qt::black);
    
    refreshTableTerms();
    refreshMainWindowTerms();
    
    newUpdate = true;
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
    int oldTermIndex = termIndex;
    if (i > 0) termIndex = i - 1;
    
    createUndoAction(currTermEdit, oldTermIndex + 1, termIndex + 1);
    
    refreshTableTerms();
    refreshMainWindowTerms();    
}

// updates the number of terms of the current function
void Interface::changeNumTerms(int i)
{
    
    // qDebug() << "size of stack" << undoStack->count();
    
    newUpdate = false;
    if (!numTermsEdit->hasFocus()) numTermsEdit->setFocus();
    
    numTermsEdit->setEnabled(false);
    
    int oldNumTerms = numTerms;
    
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
    
    createUndoAction(numTermsEdit, oldNumTerms, numTerms);
    
    //updateCurrTerm(i);
    
    refreshMainWindowTerms();
    refreshTableTerms();
    newUpdate = true;
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
    imagePathLabel->setEnabled(false);
    
    emit imageActionStatus(false);
    
    imageDataWindow->hide();
    
    currColorWheel->setCurrent(colorwheelSel->currentIndex());
    
    updatePreviewDisplay();
}

void Interface::selectImage()
{
    colorwheelSel->setEnabled(false);
    setLoadedImage->setEnabled(true);
    imagePathLabel->setEnabled(true);
    
    emit imageActionStatus(true);
    
    if (openImageName == "") {
        setImagePushed();
    }
    else {
        QDir checkDir(imageSetPath);
        if (!checkDir.exists(openImageName)) {
            errorHandler(INVALID_IMAGE_FILE_ERROR);
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
    
    QString startingPath = openImageName == "" ? "/Documents" : imageSetPath;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    startingPath,
                                                    tr("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
    
    QFile inFile(fileName);
    if (fileName == "") {
        return;
    }
    
    if (!inFile.exists()){
        errorHandler(INVALID_IMAGE_FILE_ERROR);
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
    newUpdate = false;
    
    termIndex = 0;
    //resetFunction();
    
    currFunction = functionVector[index];
    numTerms = currFunction->getNumTerms();
    termViewTable->setRowCount(0);
    currTermEdit->setMaximum(numTerms);
    
    polarPlane->changeFunction(currFunction);
    previewDisplayPort->changeFunction(currFunction);
    imageExportPort->changeFunction(currFunction);
    aspectRatioPreviewDisplayPort->changeFunction(currFunction);
    
    refreshMainWindowTerms();
    refreshTableTerms();
    
    newUpdate = true;
    updatePreviewDisplay();
    
    resetFunction();
}


// SLOT function called when user attempts to save current settings into a wpr file
void Interface::saveCurrWorkspace()
{
    
    
    QString currFileName = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                    saveloadPath,
                                                    tr("Wallpapers (*.wpr)"));
    
    if (currFileName.isEmpty()) {
        return;
    }
    // qDebug() << "saving";
    saveloadPath = saveSettings(currFileName, WORKSPACE_ACTION);
    
    QMessageBox msgBox;
    msgBox.setText(tr("The file has been successfully saved to: ").append(saveloadPath));
    msgBox.exec();
    
}

void Interface::saveCurrWorkspaceAs()
{
    QString newFileName = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                       saveloadPath,
                                                       tr("Wallpapers (*.wpr)"));
    
    currFileName = newFileName;
    if (currFileName.isEmpty()) {
        return;
    }
    
    saveloadPath = saveSettings(currFileName, WORKSPACE_ACTION);
    
    QMessageBox msgBox;
    msgBox.setText(tr("The file has been successfully saved to: ").append(saveloadPath));
    msgBox.exec();
    
}


// internal function that handles saving settings
QString Interface::saveSettings(const QString &fileName, const int &actionFlag) {
    
    // QDir::setCurrent(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    if (actionFlag == SNAPSHOT_ACTION) {
        QDir::setCurrent(snapshotFolderPath);
    }
    QFile outFile(fileName);    

    // qDebug() << QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + fileName;

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return "";
    }
    
    
   	QTextStream out(&outFile);
    out << "Horizontal Shift: " << QString::number(settings->XCorner) << endl;
    out << "Vertical Shift: " << QString::number(settings->YCorner) << endl;
   	out << "Horizontal Stretch: " << QString::number(settings->Width) << endl;
   	out << "Vertical Stretch: " << QString::number(settings->Height) << endl;
    out << "Output Width: " << QString::number(settings->OWidth) << endl;
    out << "Output Height: " << QString::number(settings->OHeight) << endl;
   	out << "Function Type: " << "Wallpapers" << endl;
   	out << "Function: " << functionSel->currentText() << endl;
    
   	if (fromColorWheelButton->isChecked()) {
        out << "Color Type: Colorwheel" << endl;
        out << "Colorwheel: " << colorwheelSel->currentText() << endl;
    }
    else {
        out << "Color Type: Image" << endl;
        out << "Image Path: " << imageSetPath << endl;
        out << "Image Name: " << openImageName << endl;
        out << "Overflow Color: " << currColorWheel->getOverflowColor().name() << endl;
    }
    
    out << "Scaling Radius: " << QString::number(currFunction->getScaleR()) << endl;
    out << "Scaling Angle: " << QString::number(currFunction->getScaleA()) << endl;
    
    unsigned int i;
    QString tabString(PARAMETER_SEPARATOR_LENGTH, ' ');
    
    out << "NUMBERS OF TERMS: " << currFunction->getNumTerms() << endl;
    
    for(int index = 0; index < currFunction->getNumTerms(); index++)
    {
        i = index;
        out << "Term " << i << " :" << tabString
        << "N: " << QString::number(currFunction->getN(i)) << tabString
        << "M: " << QString::number(currFunction->getM(i)) << tabString
        << "R: " << QString::number(currFunction->getR(i)) << tabString
        << "A: " << QString::number(currFunction->getA(i)) << endl;
        // out << currFunction->getN(i) << currFunction->getM(i) << currFunction->getR(i) << currFunction->getA(i);
    }
    
    // outFile.setPermissions(QFile::ReadOther);
    
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
    
    //qDebug() << "load" << fileName;
//    QMessageBox msgBox;
//    msgBox.setText(tr("Loading from: ").append(fileName));
//    msgBox.exec();
    
    QFile inFile(fileName);
    
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setText(tr("ERROR LOADING FILE"));
        msgBox.exec();
        // return "";
    } else {
        qDebug() << "SUCCEEDED LOADING FILE";
    }

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
    QColor overflowColor;
    
   // newColorIndex = 0;
    
    in.readLineInto(&line);
    settings->XCorner = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    in.readLineInto(&line);
    settings->YCorner = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    in.readLineInto(&line);
    settings->Width = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    in.readLineInto(&line);
    settings->Height = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    in.readLineInto(&line);
    settings->OWidth = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toInt();
    in.readLineInto(&line);
    settings->OHeight = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toInt();
    in.readLineInto(&line);
    functionType = (line.right(line.length() - line.lastIndexOf(" ") - 1));
    
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
        newColorIndex = -1;
        in.readLineInto(&line);
        imageLoadPath = (line.right(line.length() - line.lastIndexOf(" ") - 1));
        in.readLineInto(&line);
        loadImageName = (line.right(line.length() - line.lastIndexOf(" ") - 1));
        in.readLineInto(&line);
        overflowColor = QColor(line.right(line.length() - line.lastIndexOf(" ") - 1));
    }
    else {
        in.readLineInto(&line);
        colorName = (line.right(line.length() - line.lastIndexOf(" ") - 1));
        newColorIndex = colorwheelSel->findText(colorName, Qt::MatchExactly);
    }
    
    currFunction = functionVector[newFunctionIndex];
    
    in.readLineInto(&line);
    tempdouble = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
   // qDebug() << "scale R: " << tempdouble;
    scaleREdit->blockSignals(true);
    currFunction->setScaleR(tempdouble);
    scaleREdit->setText(QString::number(tempdouble));
    scaleREdit->blockSignals(false);
    
    in.readLineInto(&line);
    tempdouble = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toDouble();
    scaleAEdit->blockSignals(true);
    currFunction->setScaleA(tempdouble);
    scaleAEdit->setText(QString::number(tempdouble));
    scaleAEdit->blockSignals(false);
    
    in.readLineInto(&line);
    count = (line.right(line.length() - line.lastIndexOf(" ") - 1)).toInt();
    
    currFunction->setNumTerms(count);
    unsigned int unsignedCount = count;
    //currFunction->refresh();
    
    QString separator(PARAMETER_SEPARATOR_LENGTH, ' ');
    QStringList resultList;
    QString resultString;
    
    
    for(unsigned int i = 0; i < unsignedCount; i++)
    {
        in.readLineInto(&line);
        resultList = line.split(separator, QString::SkipEmptyParts);
        for (int j = 1; j < resultList.size(); j++) {
            
            resultString = resultList.at(j);
            
            if (j == 1) {
                tempint = resultString.right(resultString.length() - resultString.lastIndexOf(" ") - 1).toInt();
                currFunction->setN(i, tempint);
            }
            else if (j == 2) {
                tempint = resultString.right(resultString.length() - resultString.lastIndexOf(" ") - 1).toInt();
                currFunction->setM(i, tempint);
            }
            else if (j == 3) {
                tempdouble = resultString.right(resultString.length() - resultString.lastIndexOf(" ") - 1).toDouble();
                currFunction->setR(i, tempdouble);
            }
            else {
                tempdouble = resultString.right(resultString.length() - resultString.lastIndexOf(" ") - 1).toDouble();
                currFunction->setA(i, tempdouble);
            }
        }
    }
 
    inFile.close();
    //qDebug() << scaleREdit->text();
    
    worldWidthEditSlider->setValue(settings->Width * 100.0);
    worldHeightEditSlider->setValue(settings->Height * 100.0);
    XShiftEditSlider->setValue(settings->XCorner * 100.0);
    YShiftEditSlider->setValue(settings->YCorner * 100.0);
    
    if (functionSel->currentIndex() == newFunctionIndex) {
        //qDebug() << scaleREdit->text();
       // changeFunction(newFunctionIndex);
        
    }
    else {
        functionSel->setCurrentIndex(newFunctionIndex);
         //qDebug() << scaleREdit->text();
    }
//
   // qDebug() << currFunction->getScaleR();
    refreshMainWindowTerms();
    refreshTableTerms();
    
    if (newColorIndex == -1) {
     //   qDebug() << "no color wheel: image";
        imageSetPath = imageLoadPath;
        openImageName = loadImageName;
        currColorWheel->changeOverflowColor(overflowColor);
        fromImageButton->setChecked(true);
        fromImageButton->clicked();
        
    }
    else {
        
        fromColorWheelButton->setChecked(true);
        colorwheelSel->setCurrentIndex(newColorIndex);
        fromColorWheelButton->clicked();
    }
    
    newUpdate = true;
    updatePreviewDisplay();
    
    QDir stickypath(fileName);
    stickypath.cdUp();
    saveloadPath = stickypath.path();
    currFileName = saveloadPath + "/" + openImageName;
    return saveloadPath;
    
}

// updates the preview to reflect changes to the settings, function, and color wheel
void Interface::updatePreviewDisplay()
{
    if (!newUpdate) {
        return;
    }

   // qDebug() << "updates";
    
    imageDataSeries->clear();
    
    snapshotButton->setEnabled(false);
    
    displayProgressBar->reset();
    previewDisplayPort->paintToDisplay(disp);
    updateAspectRatio();
    
}

// slot function called when clicked "update preview" button to add to history and update the preview display to reflect current settings
void Interface::snapshotFunction()
{
    historyDisplay->show();
    
    QDateTime savedTime = QDateTime::currentDateTimeUtc();
    QString newFile = savedTime.toString("MM.dd.yyyy.hh.mm.ss.zzz.t").append(".wpr");
    QString filePath = saveSettings(newFile, SNAPSHOT_ACTION).append("/" + newFile);
    
    qDebug() << "save" << filePath;
//    QMessageBox msgBox;
//    msgBox.setText(tr("Saving from: ").append(filePath));
//    msgBox.exec();
    
    historyDisplay->triggerAddToHistory(savedTime, filePath, currFunction, currColorWheel, settings);
    
    
}

// SLOT FUNCTIONS TO CHANGE OUTPUT IMAGE PROPERTIES
void Interface::changeOHeight()
{
    heightChanged = true;
    int val = outHeightEdit->text().toInt();
    
    
    if (val < MIN_IMAGE_DIM) {
        errorHandler(INVALID_OUTPUT_IMAGE_DIM);
    } else if (val > MAX_IMAGE_DIM) {
        errorHandler(INVALID_OUTPUT_IMAGE_DIM);
    }
    
    settings->OHeight = val;
    if (!widthChanged) changeOWidth();
    imageExportPort->changeSettings(settings);
    
    updateAspectRatio();
    
    
}

void Interface::changeOWidth()
{
    widthChanged = true;
    int val = outWidthEdit->text().toInt();
    
    if (val < MIN_IMAGE_DIM) {
        errorHandler(INVALID_OUTPUT_IMAGE_DIM);
    } else if (val > MAX_IMAGE_DIM) {
        errorHandler(INVALID_OUTPUT_IMAGE_DIM);
    }
    
    
    settings->OWidth = val;
    if (!heightChanged) changeOHeight();
    
    imageExportPort->changeSettings(settings);
    
    updateAspectRatio();
    
    
}

void Interface::updateAspectRatio()
{
   
    double width = outWidthEdit->text().toInt() * ASPECT_RATIO_SCALE;
    double height = outHeightEdit->text().toInt() * ASPECT_RATIO_SCALE;
   // qDebug() << "width: " << width << "height: " << height;
    
    double temp = (double)(width / height);
    
    if (temp < 0.1 || temp > 10) {
        errorHandler(INVALID_ASPECT_RATIO);
    } else {
        aspectRatio = temp;
        
        QSize size = aspectRatioPreview->changeDisplayDimensions(width, height);
        aspectRatioPreviewDisplayPort->changeDimensions(size.width(), size.height());
        //aspectRatioPreviewDisplayPort->changeSettings(settings);
        
        aspectRatioEdit->setText(QString::number(aspectRatio));
        aspectRatioPreviewDisplayPort->paintToDisplay(aspectRatioPreview);
    }
    
    heightChanged = false;
    widthChanged = false;
    
}

void Interface::changeAspectRatio()
{
    double temp = aspectRatioEdit->text().toDouble();
    double width, height, val;
   
    if (temp < MIN_ASPECT_RATIO) {
        errorHandler(INVALID_ASPECT_RATIO);
    } else if (temp > MAX_ASPECT_RATIO) {
        errorHandler(INVALID_ASPECT_RATIO);
    } else {
        aspectRatio = temp;
    }
    
    if (aspectRatio > 1.0) {
        
        width = outWidthEdit->text().toDouble() * ASPECT_RATIO_SCALE;
        height = width / aspectRatio;
        
        val = (int)(height / ASPECT_RATIO_SCALE);
        outHeightEdit->blockSignals(true);
        outHeightEdit->setText(QString::number(val));
        outHeightEdit->blockSignals(false);
        settings->OHeight = val;
        
    } else {
        
        height = outHeightEdit->text().toDouble() * ASPECT_RATIO_SCALE;
        width = height * aspectRatio;
        
        val = (int)(width / ASPECT_RATIO_SCALE);
        outWidthEdit->blockSignals(true);
        outWidthEdit->setText(QString::number(val));
        outWidthEdit->blockSignals(false);
        settings->OWidth = val;
        
    }
    
    imageExportPort->changeSettings(settings);
    QSize size = aspectRatioPreview->changeDisplayDimensions(width, height);
    aspectRatioPreviewDisplayPort->changeDimensions(size.width(), size.height());
    aspectRatioPreviewDisplayPort->paintToDisplay(aspectRatioPreview);
    
}

//changing slider values
void Interface::changeWorldHeight(double val)
{
    settings->Height = val;
    
    worldHeightEdit->setText(QString::number(val));
    worldHeightEdit->setModified(false);
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeWorldHeight()
{
    double val = worldHeightEdit->text().toDouble();
    settings->Height = val;
    
    createUndoAction(worldHeightEdit, worldHeightEditSlider->value() / 100.0, val);
    
    worldHeightEditSlider->blockSignals(true);
    worldHeightEditSlider->setValue(val * 100.0);
    worldHeightEditSlider->blockSignals(false);
    worldHeightEdit->setModified(false);
    
    updatePreviewDisplay();
}

//changing slider values
void Interface::changeWorldWidth(double val)
{
    settings->Width = val;
    worldWidthEdit->setText(QString::number(val));
    worldWidthEdit->setModified(false);
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeWorldWidth()
{
    double val = worldWidthEdit->text().toDouble();
    settings->Width = val;
    
    createUndoAction(worldWidthEdit, worldWidthEditSlider->value() / 100.0, val);
    
    worldWidthEditSlider->blockSignals(true);
    worldWidthEditSlider->setValue(val * 100.0);
    worldWidthEditSlider->blockSignals(false);
    worldWidthEdit->setModified(false);
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeXCorner(double val)
{
    
    settings->XCorner = val;
    XShiftEdit->setText(QString::number(val));
    XShiftEdit->setModified(false);
    updatePreviewDisplay();
}


//changing slider values
void Interface::changeXCorner()
{
    
    double val = XShiftEdit->text().toDouble();
    settings->XCorner = val;
    
    createUndoAction(XShiftEdit, XShiftEditSlider->value() / 100.0, val);
    
    XShiftEditSlider->blockSignals(true);
    XShiftEditSlider->setValue(val * 100.0);
    XShiftEditSlider->blockSignals(false);
    XShiftEdit->setModified(false);
    
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeYCorner(double val)
{
    settings->YCorner = val;
    YShiftEdit->setText(QString::number(val));
    YShiftEdit->setModified(false);
    updatePreviewDisplay();
}

//changing sliders value
void Interface::changeYCorner()
{
    double val = YShiftEdit->text().toDouble();
    settings->YCorner = val;
    
    createUndoAction(YShiftEdit, YShiftEditSlider->value() / 100.0, val);
    
    YShiftEditSlider->blockSignals(true);
    YShiftEditSlider->setValue(val * 100.0);
    YShiftEditSlider->blockSignals(false);
    YShiftEdit->setModified(false);
    updatePreviewDisplay();
}


//changing slider values
void Interface::changeScaleA(double val)
{
    currFunction->setScaleA(val);
    scaleAEdit->setText(QString::number(val));
    scaleAEdit->setModified(false);
    // createUndoAction(scaleAEditSlider, scaleAEditSlider->value() / 100.0, val);
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeScaleA()
{
    
    double val = scaleAEdit->text().toDouble();
    currFunction->setScaleA(val);
    
    createUndoAction(scaleAEdit, scaleAEditSlider->value(), val);
    
    scaleAEditSlider->blockSignals(true);
    scaleAEditSlider->setValue(val * 100.0);
    scaleAEditSlider->blockSignals(false);
    scaleAEdit->setModified(false);
    updatePreviewDisplay();
}


//changing slider values
void Interface::changeScaleR(double val)
{
    currFunction->setScaleR(val);
    scaleREdit->setText(QString::number(val));
    scaleREdit->setModified(false);
    // createUndoAction(scaleREditSlider, scaleREditSlider->value() / 100.0, val);
    updatePreviewDisplay();
}

//changing edit box values
void Interface::changeScaleR()
{
    
    double val = scaleREdit->text().toDouble();
    currFunction->setScaleR(val);
    
    createUndoAction(scaleREdit, scaleREditSlider->value(), val);
    
    scaleREditSlider->blockSignals(true);
    scaleREditSlider->setValue(val * 100.0);
    scaleREditSlider->blockSignals(false);
    scaleREdit->setModified(false);
    updatePreviewDisplay();
}


// SLOT FUNCTIONS TO CHANGE FREQ AND COEFF PAIRS
void Interface::changeN(int val)
{
    
    currFunction->setN(termIndex, val);
    
    createUndoAction(nEdit, oldN, val);
    
    refreshTableTerms();
    // refreshMainWindowTerms();
    updatePreviewDisplay();
    
    oldN = val;
}

void Interface::changeM(int val)
{
    
    currFunction->setM(termIndex, val);
    
    createUndoAction(mEdit, oldM, val);

    refreshTableTerms();
    // refreshMainWindowTerms();
    updatePreviewDisplay();
    
    oldM = val;
}


void Interface::changeR(double val)
{
    currFunction->setR(termIndex, val);
    createUndoAction(rEdit, rValueLabel->text().toDouble(), val);
    rValueLabel->setText(QString::number(val));
    
    refreshTableTerms();
    // refreshMainWindowTerms();
    updatePreviewDisplay();
}

void Interface::changeA(double val)
{
    currFunction->setA(termIndex, val);
    createUndoAction(aEdit, aValueLabel->text().toDouble(), val);
    aValueLabel->setText(QString::number(val));
    
    refreshTableTerms();
    // refreshMainWindowTerms();
    updatePreviewDisplay();
}

void Interface::startImageExport()
{
    emit outWidthEdit->returnPressed();
    emit outHeightEdit->returnPressed();
    
    aspectRatio = (double)settings->Width/settings->Height;
    
    
    imageDimensionsPopUp->hide();
    
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    saveloadPath,
                                                    tr("JPEG (*.jpg *.jpeg);;TIFF (*.tiff);; PNG (*.png);;PPM (*.ppm)"));
    
    if (fileName == "") return;
    
    QFile inFile(fileName);
    if (!inFile.open(QIODevice::WriteOnly))
        return;
    
    exportProgressBar->resetBar(tr("Exporting"), imageExportPort);
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
        case INVALID_IMAGE_FILE_ERROR:
            errorMessageBox->setText("Unable to load image from path: " + imageSetPath + "/" + openImageName);
            imageSetPath = saveloadPath;
            openImageName = "";
            imagePathLabel->setText("<i>(no image has been set)</i>");
            qDebug() << errorMessageBox->exec();
            break;
        case INVALID_OUTPUT_IMAGE_DIM:
            if (errPrint) {
                errPrint = false;
                return;
            }
            errorMessageBox->setText("Error: image dimensions must be between 20 and 9999");
            qDebug() << errorMessageBox->exec();
            //errPrint = true;
            break;
        case INVALID_ASPECT_RATIO:
            if (errPrint) {
                errPrint = false;
                return;
            }
            QString msg = QString("Error: aspect ratio must be between %1 and %2. Please correct the input accordingly.").arg(MIN_ASPECT_RATIO).arg(MAX_ASPECT_RATIO);
            errorMessageBox->setText(msg);
            errorMessageBox->exec();
            errPrint = true;
            return;
        
    }
    
}

// sets coefficient pair using data from polarPlane
void Interface::setPolarCoordinates(int coeffFlag, const QString &radius, const QString &angle)
{
    if (coeffFlag == LOCAL_FLAG)
    {
        rEdit->setValue(radius.toDouble() * 100);
        aEdit->setValue(angle.toDouble() * 100);
//        rValueLabel->setText(radius);
//        aValueLabel->setText(angle);
    }
    else if (coeffFlag == GLOBAL_FLAG)
    {
        newUpdate = false;
        scaleREdit->setText(radius);
        emit scaleREdit->returnPressed();
        // scaleREditSlider->setValue(radius.toDouble() * 100.0);
        newUpdate = true;
        scaleAEdit->setText(angle);
        emit scaleAEdit->returnPressed();
        // scaleAEditSlider->setValue(angle.toDouble() * 100.0);
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
    
    QSpinBox *editor;
    QDoubleSpinBox *doubleEditor;
    
    if (col > 0 && col < 3) {
        
        editor = static_cast<QSpinBox *>(termViewTable->cellWidget(row,col));
        val = editor->value();
    } else {
        doubleEditor = static_cast<QDoubleSpinBox *>(termViewTable->cellWidget(row,col));
        val = doubleEditor->value();
    }
    
    
    unsigned int index = row;
    int coeff = val;
    double freq = val;
    
    
    switch(col) {
        case 1:
            currFunction->setM(index, coeff);
            createUndoAction(editor, oldMTable.at(index), coeff);
            oldMTable.insert(index, coeff);
            break;
        case 2:
            currFunction->setN(index, coeff);
            createUndoAction(editor, oldNTable.at(index), coeff);
            oldNTable.insert(index, coeff);
            break;
        case 3:
            currFunction->setA(index, freq);
            createUndoAction(doubleEditor, oldATable.at(index), freq);
            oldATable.insert(index, freq);
            break;
        case 4:
            currFunction->setR(index, freq);
            createUndoAction(doubleEditor, oldRTable.at(index), freq);
            oldRTable.insert(index, freq);
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
    
    //undoStack->push(new ChangeCommand(numTermsEdit, currFunction->getNumTerms(), newNumTerms));
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
}

// reset the main window to receive signals - prevent updating too fast
void Interface::resetMainWindowButton(const bool &status)
{
    numTermsEdit->setEnabled(status);
    snapshotButton->setEnabled(status);
}

void Interface::setSnapShotWindow(HistoryDisplay* window)
{
    historyDisplay = window;
    connect(historyDisplay->viewMapper, SIGNAL(mapped(QString)), this, SLOT(loadSettings(QString)));
    
}

void Interface::mousePressEvent(QMouseEvent* event)
{
    QWidget *widget = QApplication::focusWidget();
    
    if (event->button() == Qt::LeftButton) {
        if (!widget->underMouse()) {
            if (QLineEdit* lineEditBox = dynamic_cast<QLineEdit*>(widget)) {
                if (lineEditBox->isModified()) {
                    lineEditBox->undo();
                }
                lineEditBox->deselect();
            }
        }
    } else if (event->button() == Qt::RightButton) {
        
        QLabel* lbl = static_cast<QLabel*>(QApplication::widgetAt(QCursor::pos()));
        
        QToolTip::showText(QCursor::pos(), lbl->toolTip());
    }
}

void Interface::startShifting(const QPoint &point)
{
    prevMousePos = point;
    mouseMoving = true;
    newAction = false;
    oldXShift = XShiftEdit->text().toDouble();
    oldYShift = YShiftEdit->text().toDouble();
}

void Interface::finishShifting(const QPoint& /* unused */ )
{
    newAction = true;
    mouseMoving = false;
    createUndoAction(XShiftEdit, oldXShift, XShiftEdit->text().toDouble());
    createUndoAction(YShiftEdit, oldYShift, YShiftEdit->text().toDouble());
}

void Interface::updateShifting(const QPoint &point)
{
    if (mouseMoving) {
        double xTranslation = (double)(point.x() - prevMousePos.x()) / 100;
        double yTranslation = (double)(point.y() - prevMousePos.y()) / 100;
        newUpdate = false;
        XShiftEdit->setText(QString::number(XShiftEdit->text().toDouble() - xTranslation));
        YShiftEdit->setText(QString::number(YShiftEdit->text().toDouble() + yTranslation));
        emit XShiftEdit->returnPressed();
        newUpdate = true;
        emit YShiftEdit->returnPressed();
        prevMousePos = point;
    }
}

void Interface::updateImageDataGraph()
{
    
    imagePixmap.convertFromImage(QImage(imageSetPath + "/" + openImageName));
    imagePixmap = imagePixmap.scaledToHeight(previewSize);
    imageLabel->setPixmap(imagePixmap);
    
    prevDataSeries->replace(imageDataSeries->pointsVector());
}

void Interface::handleUndo()
{
    
   // if(!undoStack->canUndo()) { return; }
    
    // qDebug() << "in Undo";

    newAction = false;
    polarPlane->hidePolarPlane();
    undoStack->undo();
    newAction = true;
    //emit redoEnabled();
    //qDebug() << "undo stack size" << undoStack->count();
    
    // item->setValue(undoVal);
    
}

void Interface::handleRedo()
{
    //if(!undoStack->canRedo()) { return; }
    newAction = false;
    qDebug() << "starting redo";
    polarPlane->hidePolarPlane();
    undoStack->redo();
    newAction = true;
    // item->setValue(redoVal);
}

void Interface::createUndoAction(QObject *item, double oldVal, double newVal)
{
    if (newAction) {
        ChangeCommand *command = new ChangeCommand(item, oldVal, newVal);
        qDebug() << "pushing command: " << "old val:" << oldVal << "newVal:" << newVal;
        undoStack->push(command);
    }
}


bool Interface::eventFilter(QObject* object, QEvent* event)
{
    // qDebug() << "FILTER" << object->metaObject()->className();
    
    if(event->type() == QEvent::KeyPress)
    {
       // qDebug() << event->type();
        
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        QKeySequence keySequence(keyEvent->key() | keyEvent->modifiers());
        
        if(keySequence.matches(QKeySequence::Undo)) {
            // qDebug() << "here2";
            handleUndo();
            return true;
        } else if (keySequence.matches(QKeySequence::Redo)) {
            // qDebug() << "here";
            handleRedo();
            return true;
        }
        
        return false;
    }
    else
    {
        
        return QObject::eventFilter(object,event);
    }
}

void Interface::closing()
{
    QDir dir(snapshotFolderPath);
    dir.removeRecursively();
}


