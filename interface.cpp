#include "interface.h"



interface::interface(QWidget *parent) : QWidget(parent)
{

    // FUNCTIONAL VARIABLES
    advancedMode = false;
    termIndex = 0;
    saveloadPath = QDir::homePath();

    // FUNCTIONAL OBJECTS
    currFunction = new hex3Function();
    currColorWheel = new ColorWheel();

    viewMapper = new QSignalMapper(this);
    removeMapper = new QSignalMapper(this);

    // ORGANIZATIONAL ELEMENTS
    
    //create elements
    functionConstantsBox = new QGroupBox(tr("Function Constants"), this);
    interfaceLayout = new QVBoxLayout(this);
    topbarLayout = new QHBoxLayout();
    leftbarLayout = new QVBoxLayout();
    imagePropsBox = new QGroupBox(tr("Image Properties"), this);
    displayWidget = new QWidget(this);
    patternTypeWidget = new QWidget(this);
    viewHistoryWidget = new QWidget(this);
    toggleViewWidget = new QWidget(this);
    leftbarLayout->addWidget(toggleViewWidget);
    leftbarLayout->addWidget(imagePropsBox);
    leftbarLayout->addWidget(patternTypeWidget);
    
    
    topbarLayout->addLayout(leftbarLayout);
    topbarLayout->addWidget(displayWidget);
    topbarLayout->addWidget(viewHistoryWidget);
    interfaceLayout->addLayout(topbarLayout);                       //lay out vertically
    interfaceLayout->addWidget(functionConstantsBox);
    setLayout(interfaceLayout);

    errorMessageBox = new QMessageBox(this);
    errorMessageBox->setIcon(QMessageBox::Critical);

    // INPUT VALIDATORS (NUMERICAL)
    doubleValidate = new QDoubleValidator(-9999999.0, 9999999.0, 5, this);
    intValidate = new QIntValidator(-9999999, 9999999, this);
    posdoubleValidate = new QDoubleValidator(1.0, 9999999.0, 5, this);
    posintValidate = new QIntValidator(1, 9999999, this);
    numtermsValidate = new QIntValidator(1, 99, this);
    dimValidate = new QIntValidator(1, 10000, this);

    
    //modeToggle SUBELEMENTS
    
    toggleViewButton = new QPushButton(tr("Switch to Advanced View"), this);
    toggleViewLayout = new QVBoxLayout(toggleViewWidget);
    toggleViewLayout->addWidget(toggleViewButton);
    

    
    //functionConstantsBox SUBELEMENTS
    //create labels
    currTermLabel = new QLabel(functionConstantsBox);
    currTermLabel->setText(tr("Term"));

    freqpairLabel = new QLabel(tr("Frequency Pair: "), functionConstantsBox);
    coeffLabel = new QLabel(tr("Coefficient Pair: "), functionConstantsBox);

    nLabel = new QLabel(functionConstantsBox);
    mLabel = new QLabel(functionConstantsBox);
    aLabel = new QLabel(functionConstantsBox);
    rLabel = new QLabel(functionConstantsBox);

    // nValueLabel = new QLabel(functionConstantsBox);
    // mValueLabel = new QLabel(functionConstantsBox);
    aValueLabel = new QLabel(functionConstantsBox);
    rValueLabel = new QLabel(functionConstantsBox);

    scaleALabel = new QLabel(tr("Scaling Angle"), functionConstantsBox);
    scaleRLabel = new QLabel(tr("Scaling Radius"), functionConstantsBox);
    refreshLabels();

    // create input
    nEdit = new QSpinBox(functionConstantsBox);
    mEdit = new QSpinBox(functionConstantsBox);
    // aEdit = new QDoubleSpinBox(functionConstantsBox);
    // rEdit = new QDoubleSpinBox(functionConstantsBox);

    // nEdit = new QSlider(Qt::Horizontal);
    // mEdit = new QSlider(Qt::Horizontal);
    aEdit = new QDoubleSlider();
    rEdit = new QDoubleSlider();
    coeffPlaneEdit = new QPushButton(tr("Set on plane"), functionConstantsBox);
    // coeffPlanePopUp = new QMessageBox(this);
    // coeffPlanePopUp->setIcon(QMessageBox::Information);
    // coeffPlanePopUp->setText("Hello");
    coeffPlanePopUp = new QWidget(this, Qt::Window);

    aEdit->setOrientation(Qt::Horizontal);
    rEdit->setOrientation(Qt::Horizontal);

    scaleAEdit = new QLineEdit(functionConstantsBox);
    scaleREdit = new QLineEdit(functionConstantsBox);
    scalePlaneEdit = new QPushButton(tr("Set on plane"), functionConstantsBox);
    // scalePlanePopUp = new QMessageBox(this);
    // scalePlanePopUp->setIcon(QMessageBox::Information);
    // scalePlanePopUp->setText("It's me");

    scalePlanePopUp = new QWidget(this, Qt::Window);

    currTermEdit = new CustomSpinBox(functionConstantsBox);
    espacer1 = new QSpacerItem(5, 5);
    espacer2 = new QSpacerItem(5, 5);
    espacer3 = new QSpacerItem(5, 5);
    espacer4 = new QSpacerItem(5, 5);
    espacer5 = new QSpacerItem(130, 15);
    espacer6 = new QSpacerItem(2, 0);
    espacer7 = new QSpacerItem(15, 15);

    loadButton = new QPushButton(tr("Load Setting..."), functionConstantsBox);
    saveButton = new QPushButton(tr("Save Current Setting"), functionConstantsBox);
    currTermEdit->setRange(1,1);
    nEdit->setFixedWidth(200);
    mEdit->setFixedWidth(200);
    rEdit->setFixedWidth(200);
    aEdit->setFixedWidth(200);
    scaleAEdit->setFixedWidth(50);
    scaleREdit->setFixedWidth(50);
    nLabel->setFixedWidth(18);
    mLabel->setFixedWidth(18);
    rLabel->setFixedWidth(18);
    aLabel->setFixedWidth(18);
    scaleALabel->setFixedWidth(100);
    scaleRLabel->setFixedWidth(100);

    nEdit->setRange(-10,10);
    nEdit->setSingleStep(1);
    mEdit->setRange(-10,10);
    mEdit->setSingleStep(1);
    rEdit->setRange(-250,250);
    rEdit->setSingleStep(1);
    aEdit->setRange(-314, 314);
    aEdit->setSingleStep(25);

    scaleAEdit->setValidator(doubleValidate);
    scaleREdit->setValidator(doubleValidate);

    functionConstantsBoxLayoutStack = new QVBoxLayout(functionConstantsBox);  //initialize layout
    
    functionConstantsBoxLayoutLower = new QHBoxLayout();
    functionConstantsBoxLayoutFirstLevel = new QHBoxLayout();
    functionConstantsBoxLayoutSecondLevel = new QHBoxLayout();
    functionConstantsBoxLayoutThirdLevel = new QHBoxLayout();

    functionConstantsBoxLayoutFirstLevel->setAlignment(Qt::AlignLeft);
    functionConstantsBoxLayoutSecondLevel->setAlignment(Qt::AlignLeft);
    functionConstantsBoxLayoutThirdLevel->setAlignment(Qt::AlignLeft);
    
    functionConstantsBoxLayoutLower->setAlignment(Qt::AlignRight);

    functionConstantsBoxLayoutFirstLevel->addWidget(currTermLabel);
    functionConstantsBoxLayoutFirstLevel->addWidget(currTermEdit);

    functionConstantsBoxLayoutSecondLevel->addWidget(freqpairLabel);
    functionConstantsBoxLayoutSecondLevel->addItem(espacer1);
    functionConstantsBoxLayoutSecondLevel->addWidget(nLabel);
    functionConstantsBoxLayoutSecondLevel->addWidget(nEdit);
    // functionConstantsBoxLayoutSecondLevel->addWidget(nValueLabel);
    functionConstantsBoxLayoutSecondLevel->addItem(espacer2);
    functionConstantsBoxLayoutSecondLevel->addWidget(mLabel);
    functionConstantsBoxLayoutSecondLevel->addWidget(mEdit);
    // functionConstantsBoxLayoutSecondLevel->addWidget(mValueLabel);

    functionConstantsBoxLayoutThirdLevel->addWidget(coeffLabel);
    functionConstantsBoxLayoutThirdLevel->addItem(espacer4);
    functionConstantsBoxLayoutThirdLevel->addWidget(rLabel);
    functionConstantsBoxLayoutThirdLevel->addWidget(rEdit);
    functionConstantsBoxLayoutThirdLevel->addWidget(rValueLabel);
    functionConstantsBoxLayoutThirdLevel->addItem(espacer3);
    functionConstantsBoxLayoutThirdLevel->addWidget(aLabel);
    functionConstantsBoxLayoutThirdLevel->addWidget(aEdit);
    functionConstantsBoxLayoutThirdLevel->addWidget(aValueLabel);
    functionConstantsBoxLayoutThirdLevel->addWidget(coeffPlaneEdit);

    //fill layouts
    functionConstantsBoxLayoutLower->addWidget(loadButton);
    functionConstantsBoxLayoutLower->addItem(espacer7);
    functionConstantsBoxLayoutLower->addWidget(saveButton);
    functionConstantsBoxLayoutLower->addItem(espacer5);
    functionConstantsBoxLayoutLower->addWidget(scaleRLabel);
    functionConstantsBoxLayoutLower->addWidget(scaleREdit);
    functionConstantsBoxLayoutLower->addItem(espacer6);
    functionConstantsBoxLayoutLower->addWidget(scaleALabel);
    functionConstantsBoxLayoutLower->addWidget(scaleAEdit);
    functionConstantsBoxLayoutLower->addWidget(scalePlaneEdit);
    
    functionConstantsBoxLayoutStack->addLayout(functionConstantsBoxLayoutFirstLevel);
    functionConstantsBoxLayoutStack->addLayout(functionConstantsBoxLayoutSecondLevel);
    functionConstantsBoxLayoutStack->addLayout(functionConstantsBoxLayoutThirdLevel);
    functionConstantsBoxLayoutStack->addLayout(functionConstantsBoxLayoutLower);

    functionConstantsBox->setLayout(functionConstantsBoxLayoutStack);

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
    
    patternTypeBoxLayout = new QVBoxLayout(patternTypeBox);     //set up layout
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
    //patternTypeBoxOverallLayout->addItem(gspacer2);
    
   // viewHistoryWidget SUBELEMENTS
    viewHistoryBox = new QGroupBox(tr("History"), viewHistoryWidget);
    viewHistoryBoxOverallLayout = new QVBoxLayout(viewHistoryWidget);
    
    clearHistoryButton = new QPushButton(tr("Clear All History"), viewHistoryBox);
    viewHistoryBoxLayout = new QVBoxLayout(viewHistoryBox);
    
    //viewHistoryBoxLayout->addItem(gspacer1);
    
    viewHistoryBoxOverallLayout->addWidget(viewHistoryBox);
    viewHistoryBoxLayout->addWidget(clearHistoryButton);
    viewHistoryBoxOverallLayout->addStretch(5);
   
    //viewHistoryBoxOverallLayout->addStretch();

    // imagePropsBox SUBELEMENTS
    outheightLabel = new QLabel(tr("Output Height"), imagePropsBox);          //initialize elements
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

//    outheightEdit->setFixedWidth(100);
//    outwidthEdit->setFixedWidth(100);
//    XCornerEdit->setFixedWidth(100);
//    YCornerEdit->setFixedWidth(100);
//    worldwidthEdit->setFixedWidth(100);
//    worldheightEdit->setFixedWidth(100);

//    outheightEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    outwidthEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    XCornerEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    YCornerEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    worldwidthEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    worldheightEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

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

    // DISP SUBELEMENTS
    disp = new Display(DEFAULT_PREVIEW_SIZE, displayWidget);
    updatePreview = new QPushButton(tr("Update Preview"), this);
    exportImage = new QPushButton(tr("Export Image..."), this);
    resetImage = new QPushButton(tr("Reset"), this);
    dispLayout = new QVBoxLayout(displayWidget);
    buttonLayout = new QHBoxLayout();

    // updatePreview->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    // exportImage->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    // resetImage->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // updatePreview->setFixedWidth(200);
    // exportImage->setFixedWidth(200);
    // resetImage->setFixedWidth(200);

    buttonLayout->addWidget(updatePreview);
    buttonLayout->addWidget(exportImage);
    buttonLayout->addWidget(resetImage);

    dispLayout->addWidget(disp);
    dispLayout->setAlignment(disp, Qt::AlignCenter);
    dispLayout->addLayout(buttonLayout);
    
    //SET UP SHORTCUTS
    updatePreviewShortcut = new QShortcut(QKeySequence("Ctrl+D"), this);

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
    connect(currTermEdit, SIGNAL(valueChanged(int)), this, SLOT(updateTerms(int)));

    connect(nEdit, SIGNAL(valueChanged(int)), this, SLOT(changeN(int)));
    connect(mEdit, SIGNAL(valueChanged(int)), this, SLOT(changeM(int)));
    connect(rEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeR(double)));
    connect(aEdit, SIGNAL(doubleValueChanged(double)), this, SLOT(changeA(double)));
    connect(coeffPlaneEdit, SIGNAL(clicked()), this, SLOT(showPlanePopUp()));
    connect(scaleREdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleR(QString)));
    connect(scaleAEdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleA(QString)));
    connect(scalePlaneEdit, SIGNAL(clicked()), this, SLOT(showPlanePopUp()));

    connect(outwidthEdit, SIGNAL(textChanged(QString)), this, SLOT(changeOWidth(QString)));
    connect(outheightEdit, SIGNAL(textChanged(QString)), this, SLOT(changeOHeight(QString)));
    connect(worldwidthEdit, SIGNAL(textChanged(QString)), this, SLOT(changeWorldWidth(QString)));
    connect(worldheightEdit, SIGNAL(textChanged(QString)), this, SLOT(changeWorldHeight(QString)));
    connect(XCornerEdit, SIGNAL(textChanged(QString)), this, SLOT(changeXCorner(QString)));
    connect(YCornerEdit, SIGNAL(textChanged(QString)), this, SLOT(changeYCorner(QString)));

    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromSettings()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveCurrSettings()));
    
    connect(clearHistoryButton, SIGNAL(clicked()), this, SLOT(clearAllHistory()));

    connect(viewMapper, SIGNAL(mapped(QString)), this, SLOT(loadSettings(QString)));
    connect(removeMapper, SIGNAL(mapped(QObject*)), this, SLOT(removePreview(QObject*)));
    
    connect(updatePreviewShortcut, SIGNAL(activated()), this, SLOT(updateSavePreview()));

    // SET DEFAULTS
    refreshTerms();
    scaleREdit->setText(QString::number(currFunction->scaleR()));
    scaleAEdit->setText(QString::number(currFunction->scaleA()));

    outwidthEdit->setText(QString::number(DEFAULT_OUTPUT_WIDTH));
    outheightEdit->setText(QString::number(DEFAULT_OUTPUT_HEIGHT));
    worldwidthEdit->setText(QString::number(DEFAULT_WIDTH));
    worldheightEdit->setText(QString::number(DEFAULT_HEIGHT));
    XCornerEdit->setText(QString::number(DEFAULT_XCORNER));
    YCornerEdit->setText(QString::number(DEFAULT_YCORNER));

    // functionSel->setCurrentIndex(1);    //set up current function
    // colorwheelSel->setCurrentIndex(9);

    //default set up (fixed)
    functionSel->setCurrentIndex(0);
    colorwheelSel->setCurrentIndex(0);

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
    nEdit->setValue(currFunction->getN(termIndex));
    mEdit->setValue(currFunction->getM(termIndex));
    rEdit->setValue(currFunction->getR(termIndex) * 100.0);
    aEdit->setValue(currFunction->getA(termIndex) * 100.0);

    // nValueLabel->setText(QString::number(currFunction->getN(termIndex)));
    // mValueLabel->setText(QString::number(currFunction->getM(termIndex)));
    rValueLabel->setText(QString::number(currFunction->getR(termIndex) * 1.0));
    aValueLabel->setText(QString::number(currFunction->getA(termIndex) * 1.0));
}

void interface::resetImageFunction()
{
    delete currFunction;

    currFunction = new hex3Function();
    functionSel->setCurrentIndex(0);
    colorwheelSel->setCurrentIndex(0);

    termIndex = 0;

    refreshTerms();
    scaleREdit->setText(QString::number(currFunction->scaleR()));
    scaleAEdit->setText(QString::number(currFunction->scaleA()));

    outwidthEdit->setText(QString::number(DEFAULT_OUTPUT_WIDTH));
    outheightEdit->setText(QString::number(DEFAULT_OUTPUT_HEIGHT));
    worldwidthEdit->setText(QString::number(DEFAULT_WIDTH));
    worldheightEdit->setText(QString::number(DEFAULT_HEIGHT));
    XCornerEdit->setText(QString::number(DEFAULT_XCORNER));
    YCornerEdit->setText(QString::number(DEFAULT_YCORNER));

    updatePreviewDisplay();
}

void interface::toggleViewMode() {
    if (advancedMode) {
        advancedMode = false;
        toggleViewButton->setText(tr("Switch to Advanced View"));
    } else {
        advancedMode = true;
        toggleViewButton->setText(tr("Switch to Default View"));
    }
}

void interface::updateTerms(int i)
{
    termIndex = i-1;

    refreshTerms();
    refreshLabels();
}

void interface::changeMaxTerms(int i)
{

    if (i >= currTermEdit->minimum()) {
        currTermEdit->setMaximum(i);
        currFunction->setNumTerms(i);
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
    out << settings::Width << settings::Height;
    out << settings::XCorner << settings::YCorner;
    out << settings::OWidth << settings::OHeight;
    out << functionSel->currentIndex();
    out << colorwheelSel->currentIndex();
    out << currFunction->scaleR() << currFunction->scaleA();
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
    
    in >> settings::Width >> settings::Height;
    in >> settings::XCorner >> settings::YCorner;
    in >> settings::OWidth >> settings::OHeight;
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
    outwidthEdit->setText(QString::number(settings::OWidth));
    outheightEdit->setText(QString::number(settings::OHeight));
    worldwidthEdit->setText(QString::number(settings::Width));
    worldheightEdit->setText(QString::number(settings::Height));
    XCornerEdit->setText(QString::number(settings::XCorner));
    YCornerEdit->setText(QString::number(settings::YCorner));
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
    Port *historyDisplay = new Port(currFunction, currColorWheel, item->preview->dim(), item->preview->dim());
    historyDisplay->paintHistoryIcon(item);
    
//    double worldY, worldX;
//    
//    for (int y = 0; y < item->preview->dim(); y++)
//    {
//        for (int x = 0; x <= ((item->preview->dim())-1); x++)
//        {
//            worldY= settings::Height-y*settings::Height/item->preview->dim()+settings::YCorner;
//            worldX= x*settings::Width/item->preview->dim()+settings::XCorner;
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
//    double worldY, worldX;
//
//    double worldYPreCalculations1 = settings::Height + settings::YCorner;
//    double worldYPreCalculations2 = settings::Height/disp->dim();
//    double worldXPreCalculations = settings::Width/disp->dim();
//
//    clock_t start, end, startLoop, endLoop;
//    double cpu_time_used = 0.0;
//    double cpu_time_used2 = 0.0;
//    double cpu_time_used3 = 0.0;

    //startLoop = clock();
    
    Port *previewDisplay = new Port(currFunction, currColorWheel, disp->dim(), disp->dim());
    previewDisplay->paintToDisplay(disp);

//    for (int y = 0; y < disp->dim(); y++) 
//    {
//        for (int x = 0; x <= ((disp->dim())-1); x++)
//        {
//            // worldY= settings::Height-y*settings::Height/disp->dim()+settings::YCorner;
//            // worldX= x*settings::Width/disp->dim()+settings::XCorner;
//
//            worldY = worldYPreCalculations1 - y * worldYPreCalculations2;
//            worldX = x * worldXPreCalculations + settings::XCorner;
//
//            //run the point through our mathematical function
//            //...then convert that complex output to a color according to our color wheel
//            start = clock();
//            std::complex<double> fout=(*currFunction)(worldX,worldY);
//            end = clock();
//            cpu_time_used += ((double) (end - start)) / CLOCKS_PER_SEC;
//
//            start = clock();
//            QRgb color = (*currColorWheel)(fout);
//            end = clock();
//            cpu_time_used2 += ((double) (end - start)) / CLOCKS_PER_SEC;                    
//
//            //finally push the determined color to the corresponding point on the display
//            start = clock();
//            disp->setPixel(x, y, color);  
//            end = clock();
//            cpu_time_used3 += ((double) (end - start)) / CLOCKS_PER_SEC;               
//        }
//    }
//    endLoop = clock();
//
//    qDebug() << "========================";
//    qDebug() << "function: " << cpu_time_used;
//    qDebug() << "color: " << cpu_time_used2;
//    qDebug() << "setPixel: " << cpu_time_used3;
//    qDebug() << "for-loop: " << ((double) (endLoop - startLoop)) / CLOCKS_PER_SEC;

    //disp->repaint();

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
    settings::OHeight = val.toInt();
}

void interface::changeOWidth(const QString &val)
{
    settings::OWidth = val.toInt();
}

void interface::changeWorldHeight(const QString &val)
{
    settings::Height = val.toDouble();
}

void interface::changeWorldWidth(const QString &val)
{
    settings::Width = val.toDouble();
}

void interface::changeXCorner(const QString &val)
{
    settings::XCorner = val.toDouble();
}

void interface::changeYCorner(const QString &val)
{
    settings::YCorner = val.toDouble();
}

void interface::changeN(int val)
{
    // int passedval = val.toInt();
    currFunction->setN(termIndex, val);
    // nValueLabel->setText(QString::number(val));
    updatePreviewDisplay();
}

void interface::changeM(int val)
{
    // int passedval = val.toInt();
    currFunction->setM(termIndex, val);
    // mValueLabel->setText(QString::number(val));
    updatePreviewDisplay();
}

void interface::changeR(double val)
{
    // double passedval = val.toDouble();
    currFunction->setR(termIndex, val);
    rValueLabel->setText(QString::number(val));
    updatePreviewDisplay();
}

void interface::changeA(double val)
{
    // double passedval = val.toDouble();
    currFunction->setA(termIndex, val);
    aValueLabel->setText(QString::number(val));
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
    
    Port *imageExport = new Port(currFunction, currColorWheel, settings::OWidth, settings::OHeight);
    imageExport->exportImage();
    
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
//                                                    saveloadPath,
//                               tr("PPM (*.ppm);;JPEG (*.jpg *.jpeg)"));
//
//    QFile inFile(fileName);
//    if (!inFile.open(QIODevice::WriteOnly))
//             return;

//    double worldY, worldX ;
//    QImage outputImage(settings::OWidth, settings::OHeight, QImage::Format_RGB32);
//
//    double worldYPreCalculations1 = settings::Height + settings::YCorner;
//    double worldYPreCalculations2 = settings::Height/settings::OHeight;
//    double worldXPreCalculations = settings::Width/settings::OWidth;
//
//    for (int y = 0; y < settings::OHeight; y++)
//    {
//        for (int x = 0; x <= ((settings::OWidth)-1); x++)
//        {   
//            // worldY = settings::Height - y*settings::Height/settings::OHeight + settings::YCorner;
//            // worldX = x*settings::Width/settings::OWidth + settings::XCorner;
//
//            worldY = worldYPreCalculations1 - y * worldYPreCalculations2;
//            worldX = x * worldXPreCalculations + settings::XCorner;
//
//            //run the point through our mathematical function
//            //...then turn that complex output into a color per our color wheel
//            clock_t start, end;
//            double cpu_time_used;
//
//            start = clock();
//            std::complex<double> fout=(*currFunction)(worldX,worldY);
//            end = clock();
//            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//            qDebug() << "function: " << cpu_time_used;
//
//            start = clock();
//            QRgb color = (*currColorWheel)(fout);
//            end = clock();
//            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//            qDebug() << "function: " << cpu_time_used;  
//
//            outputImage.setPixel(x, y, color);
//        }
//    }
//
//    outputImage.save(fileName);
//
//    QDir stickypath(fileName);
//    stickypath.cdUp();
//    saveloadPath = stickypath.path();
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

void interface::showPlanePopUp()
{
    // qDebug() << "hi";
    coeffPlanePopUp->show();
    // hide();
}
