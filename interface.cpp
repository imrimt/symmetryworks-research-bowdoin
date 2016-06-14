#include "interface.h"

namespace settings      //the settings namespace stores
{                       //a few variables used to compute the
    double Width=2.5;   //image via the mathematical function
    double Height=2.0;  //and the color wheel, as well as
    double XCorner=-0.5; //the Output Width and Height.
    double YCorner=-0.5;
    int OWidth=6000;//6000 width 4800 height standard for art prints
    int OHeight=4800;
}

interface::interface(QWidget *parent) :
    QWidget(parent)
{

    // FUNCTIONAL VARIABLES
    termIndex = 0;
    saveloadPath = QDir::homePath();

    // FUNCTIONAL OBJECTS
    currFunction = new hex3Function();
    currColorWheel = new ColorWheel();

    // ORGANIZATIONAL ELEMENTS
    
    //create elements
    functionConstantsBox = new QGroupBox(tr("Function Constants"), this);
    interfaceLayout = new QVBoxLayout(this);
    topbarLayout = new QHBoxLayout();
    imagePropsBox = new QGroupBox(tr("Image Properties"), this);
    displayWidget = new QWidget(this);
    patternTypeWidget = new QWidget(this);
    
    topbarLayout->addWidget(patternTypeWidget);
    topbarLayout->addWidget(displayWidget);
    topbarLayout->addWidget(imagePropsBox);
    interfaceLayout->addLayout(topbarLayout);                       //lay out vertically
    interfaceLayout->addWidget(functionConstantsBox);
    setLayout(interfaceLayout);

    // INPUT VALIDATORS (NUMERICAL)
    doubleValidate = new QDoubleValidator(-9999999.0, 9999999.0, 5, this);
    intValidate = new QIntValidator(-9999999, 9999999, this);
    posdoubleValidate = new QDoubleValidator(1.0, 9999999.0, 5, this);
    posintValidate = new QIntValidator(1, 9999999, this);
    numtermsValidate = new QIntValidator(1, 99, this);
    dimValidate = new QIntValidator(1, 10000, this);

    // functionConstantsBox SUBELEMENTS
    currTermLabel = new QLabel(functionConstantsBox);                              //create labels
    currTermLabel->setText(tr("Term"));
    nLabel = new QLabel(functionConstantsBox);
    mLabel = new QLabel(functionConstantsBox);
    aLabel = new QLabel(functionConstantsBox);
    rLabel = new QLabel(functionConstantsBox);
    scaleALabel = new QLabel(tr("sa"), functionConstantsBox);
    scaleRLabel = new QLabel(tr("sr"), functionConstantsBox);
    refreshLabels();
    // nEdit = new QLineEdit(functionConstantsBox);                              //create input boxes
    // mEdit = new QLineEdit(functionConstantsBox);
    // aEdit = new QLineEdit(functionConstantsBox);
    // rEdit = new QLineEdit(functionConstantsBox);
    nEdit = new QSpinBox(functionConstantsBox);
    mEdit = new QSpinBox(functionConstantsBox);
    aEdit = new QDoubleSpinBox(functionConstantsBox);
    rEdit = new QDoubleSpinBox(functionConstantsBox);
    scaleAEdit = new QLineEdit(functionConstantsBox);
    scaleREdit = new QLineEdit(functionConstantsBox);
    currTermEdit = new CustomSpinBox(functionConstantsBox);
    espacer1 = new QSpacerItem(100, 15);
    espacer2 = new QSpacerItem(10, 15);
    espacer3 = new QSpacerItem(10, 15);
    espacer4 = new QSpacerItem(10, 15);
    espacer5 = new QSpacerItem(130, 15);
    espacer6 = new QSpacerItem(2, 0);
    espacer7 = new QSpacerItem(15, 15);
    espacer8 = new QSpacerItem(8, 15);

    loadButton = new QPushButton(tr("Load..."), functionConstantsBox);
    saveButton = new QPushButton(tr("Save"), functionConstantsBox);
    // currTermEdit->setMaximum(4);
    // currTermEdit->setMinimum(1);
    currTermEdit->setRange(1,1);
    nEdit->setFixedWidth(75);
    mEdit->setFixedWidth(75);
    rEdit->setFixedWidth(75);
    aEdit->setFixedWidth(75);
    scaleAEdit->setFixedWidth(75);
    scaleREdit->setFixedWidth(75);
    nLabel->setFixedWidth(18);
    mLabel->setFixedWidth(20);
    rLabel->setFixedWidth(18);
    aLabel->setFixedWidth(18);
    scaleALabel->setFixedWidth(18);
    scaleRLabel->setFixedWidth(18);

    // nEdit->setValidator(intValidate);
    // mEdit->setValidator(intValidate);
    // rEdit->setValidator(doubleValidate);
    // aEdit->setValidator(doubleValidate);

    nEdit->setRange(-9999999,9999999);
    nEdit->setSingleStep(1);
    mEdit->setRange(-9999999,9999999);
    mEdit->setSingleStep(1);
    rEdit->setRange(-9999999.0,9999999.0);
    rEdit->setSingleStep(0.25);
    aEdit->setRange(-9999999.0,9999999.0);
    aEdit->setSingleStep(0.25);
    // nEdit->setMaximum(9999999);
    // nEdit->setMinimum(-9999999);
    // mEdit->setMaximum(9999999);
    // mEdit->setMinimum(-9999999);
    // rEdit->setMaximum(9999999.0);
    // rEdit->setMinimum(-9999999.0);
    // aEdit->setMaximum(9999999.0);
    // aEdit->setMinimum(-9999999.0);

    scaleAEdit->setValidator(doubleValidate);
    scaleREdit->setValidator(doubleValidate);


    functionConstantsBoxLayoutStack = new QVBoxLayout(functionConstantsBox);  //initialize layout
    functionConstantsBoxLayout = new QHBoxLayout();
    functionConstantsBoxLayoutLower = new QHBoxLayout();
    functionConstantsBoxLayoutLower->setAlignment(Qt::AlignRight);
    functionConstantsBoxLayout->setAlignment(Qt::AlignRight);

    functionConstantsBoxLayout->addWidget(currTermLabel);              //fill layouts
    functionConstantsBoxLayout->addWidget(currTermEdit);
    functionConstantsBoxLayout->addItem(espacer1);
    functionConstantsBoxLayout->addWidget(nLabel);
    functionConstantsBoxLayout->addWidget(nEdit);
    functionConstantsBoxLayout->addItem(espacer2);
    functionConstantsBoxLayout->addWidget(mLabel);
    functionConstantsBoxLayout->addWidget(mEdit);
    functionConstantsBoxLayout->addItem(espacer3);
    functionConstantsBoxLayout->addWidget(rLabel);
    functionConstantsBoxLayout->addWidget(rEdit);
    functionConstantsBoxLayout->addItem(espacer4);
    functionConstantsBoxLayout->addWidget(aLabel);
    functionConstantsBoxLayout->addWidget(aEdit);
    functionConstantsBoxLayoutLower->addItem(espacer8);
    functionConstantsBoxLayoutLower->addWidget(loadButton);
    functionConstantsBoxLayoutLower->addItem(espacer7);
    functionConstantsBoxLayoutLower->addWidget(saveButton);
    functionConstantsBoxLayoutLower->addItem(espacer5);
    functionConstantsBoxLayoutLower->addWidget(scaleRLabel);
    functionConstantsBoxLayoutLower->addWidget(scaleREdit);
    functionConstantsBoxLayoutLower->addItem(espacer6);
    functionConstantsBoxLayoutLower->addWidget(scaleALabel);
    functionConstantsBoxLayoutLower->addWidget(scaleAEdit);
    functionConstantsBoxLayoutStack->addLayout(functionConstantsBoxLayout);
    functionConstantsBoxLayoutStack->addLayout(functionConstantsBoxLayoutLower);
    functionConstantsBox->setLayout(functionConstantsBoxLayoutStack);

    // patternType SUBELEMENTS
    patternTypeBox = new QGroupBox(tr("Pattern Type"), patternTypeWidget);
    functionSel = new QComboBox(patternTypeBox);        //initialize elements
    colorwheelSel = new QComboBox(patternTypeBox);
    functionLabel = new QLabel(patternTypeBox);
    colorwheelLabel = new QLabel(patternTypeBox);
    numtermsLabel = new QLabel(tr("<i>No. Terms</i>"), patternTypeBox);
    numtermsEdit = new QLineEdit("1", patternTypeBox);
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
    numtermsEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    numtermsLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    colorwheelSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    colorwheelLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    numtermsEdit->setFixedWidth(30);
    numtermsEdit->setValidator(numtermsValidate);
    
    patternTypeBoxLayout = new QVBoxLayout(patternTypeBox);     //set up layout
    functionLayout->addWidget(functionLabel);
    functionLayout->addWidget(functionSel);
    numtermsLayout->addWidget(numtermsLabel);
    numtermsLayout->addWidget(numtermsEdit);
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
    //patternTypeWidgetOverallLayout->addItem(gspacer4);

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
    saveImagePush = new QPushButton(tr("Save Image"), imagePropsBox);
    pspacer1 = new QSpacerItem(0, 12);
    pspacer2 = new QSpacerItem(0, 14);
    pspacer3 = new QSpacerItem(15, 30);
    pspacer4 = new QSpacerItem(20, 60);
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

    saveImagePush->setFixedHeight(25);
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

    saveImagePush->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    saveImagePush->setFixedWidth(150);

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

    //imagePropsBoxLayout->addItem(pspacer3);
    imagePropsBoxLayout->addLayout(imagePropsBoxStack);
    imagePropsBoxLayout->addLayout(imagePropsEditStack);
    imagePropsBoxLayout->addItem(pspacer4);

    //savePushLayout->addWidget(saveImagePush);
    //savePushLayout->addItem(pspacer5);

    imagePropsBoxOverallLayout->addLayout(imagePropsBoxLayout);
    //imagePropsBoxOverallLayout->addItem(pspacer3);
    imagePropsBoxOverallLayout->addWidget(saveImagePush);
    imagePropsBoxOverallLayout->setAlignment(saveImagePush, Qt::AlignRight);

    // DISP SUBELEMENTS
    disp = new Display(500, displayWidget);
    updatePreview = new QPushButton(tr("Update Preview"), this);
    dispLayout = new QVBoxLayout(displayWidget);
    dispLayout->addWidget(disp);
    dispLayout->addWidget(updatePreview);

    // CONNECT SIGNALS & SLOTS
    connect(updatePreview, SIGNAL(clicked()), this, SLOT(updatePreviewDisplay()));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), currColorWheel, SLOT(setCurrent(int)));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), this, SLOT(colorWheelChanged(int)));
    connect(setLoadedImage, SIGNAL(clicked()), this, SLOT(setImagePushed()));
    connect(functionSel, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFunction(int)));
    connect(numtermsEdit, SIGNAL(textChanged(QString)), this, SLOT(changeMaxTerms(QString)));
    connect(currTermEdit, SIGNAL(valueChanged(int)), this, SLOT(updateTerms(int)));

    connect(nEdit, SIGNAL(valueChanged(int)), this, SLOT(changeN(int)));
    connect(mEdit, SIGNAL(valueChanged(int)), this, SLOT(changeM(int)));
    connect(rEdit, SIGNAL(valueChanged(double)), this, SLOT(changeR(double)));
    connect(aEdit, SIGNAL(valueChanged(double)), this, SLOT(changeA(double)));

    connect(outwidthEdit, SIGNAL(textChanged(QString)), this, SLOT(changeOWidth(QString)));
    connect(outheightEdit, SIGNAL(textChanged(QString)), this, SLOT(changeOHeight(QString)));
    connect(worldwidthEdit, SIGNAL(textChanged(QString)), this, SLOT(changeWorldWidth(QString)));
    connect(worldheightEdit, SIGNAL(textChanged(QString)), this, SLOT(changeWorldHeight(QString)));
    connect(XCornerEdit, SIGNAL(textChanged(QString)), this, SLOT(changeXCorner(QString)));
    connect(YCornerEdit, SIGNAL(textChanged(QString)), this, SLOT(changeYCorner(QString)));

    // connect(nEdit, SIGNAL(textChanged(QString)), this, SLOT(changeN(QString)));
    // connect(mEdit, SIGNAL(textChanged(QString)), this, SLOT(changeM(QString)));
    // connect(rEdit, SIGNAL(textChanged(QString)), this, SLOT(changeR(QString)));
    // connect(aEdit, SIGNAL(textChanged(QString)), this, SLOT(changeA(QString)));

    connect(scaleREdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleR(QString)));
    connect(scaleAEdit, SIGNAL(textChanged(QString)), this, SLOT(changeScaleA(QString)));

    connect(saveImagePush, SIGNAL(clicked()), this, SLOT(saveImageStart()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFunction()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveFunction()));

    // SET DEFAULTS
    refreshTerms();
    scaleREdit->setText(QString::number(currFunction->scaleR()));
    scaleAEdit->setText(QString::number(currFunction->scaleA()));

    outwidthEdit->setText(QString::number(settings::OWidth));
    outheightEdit->setText(QString::number(settings::OHeight));
    worldwidthEdit->setText(QString::number(settings::Width));
    worldheightEdit->setText(QString::number(settings::Height));
    XCornerEdit->setText(QString::number(settings::XCorner));
    YCornerEdit->setText(QString::number(settings::YCorner));

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
    // nEdit->setText(QString::number(f->N(termIndex)));
    // mEdit->setText(QString::number(f->M(termIndex)));
    // rEdit->setText(QString::number(f->R(termIndex)));
    // aEdit->setText(QString::number(f->A(termIndex)));

    nEdit->setValue(currFunction->getN(termIndex));
    mEdit->setValue(currFunction->getM(termIndex));
    rEdit->setValue(currFunction->getR(termIndex));
    aEdit->setValue(currFunction->getA(termIndex));
}

void interface::updateTerms(int i)
{
    termIndex = i-1;

    refreshTerms();
    refreshLabels();
}

void interface::changeMaxTerms(const QString &val)
{   
    int passedval = val.toInt();

    if (passedval >= currTermEdit->minimum()) {
        currTermEdit->setMaximum(passedval);
        currFunction->setNumTerms(passedval);
    }
    // else {
    //     qDebug() << "pass in : " << passedval;
    //     qDebug() << "currTermEdit max: " << currTermEdit->maximum();
    //     qDebug() << "currTermEdit min: " << currTermEdit->minimum();
    // }
    
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

    // refreshTerms();
}

void interface::saveFunction()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    saveloadPath,
                               tr("Wallpapers (*.wpr)"));

    QFile outFile(fileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
             return;

    QDataStream out(&outFile);
    qDebug() << "Width" << settings::Width;
    out << settings::Width << settings::Height;
    out << settings::XCorner << settings::YCorner;
    out << settings::OWidth << settings::OHeight;
    out << functionSel->currentIndex();
    out << colorwheelSel->currentIndex();
    out << currFunction->scaleR() << currFunction->scaleA();
    out << currFunction->numterms();
    for(unsigned int i=0; i<currFunction->numterms(); i++)
        out << currFunction->getN(i) << currFunction->getM(i) << currFunction->getR(i) << currFunction->getA(i);

    outFile.close();

    QDir stickypath(fileName);
    stickypath.cdUp();
    saveloadPath = stickypath.path();
}

void interface::loadFunction()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    saveloadPath,
                               tr("Wallpapers (*.wpr)"));

    QFile inFile(fileName);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
             return;

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
    numtermsEdit->setText(QString::number(count));
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
    saveloadPath = stickypath.path();


}

void interface::updatePreviewDisplay()
{
    double worldY, worldX;

      for (int y = 0; y < disp->dim(); y++)
          for (int x = 0; x <= ((disp->dim())-1); x++)
          {
              worldY= settings::Height-y*settings::Height/disp->dim()+settings::YCorner;
              worldX= x*settings::Width/disp->dim()+settings::XCorner;

              std::complex<double> fout = (*currFunction)(worldX,worldY);  //run the point through our mathematical function
              QRgb color = (*currColorWheel)(fout);                          //...then convert that complex output to a color according to our color wheel

              disp->setPixel(x, y, color);                    //finally push the determined color to the corresponding point on the display

          }

      //qDebug() << "hit" << thecount++;
      disp->repaint();

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

// void interface::changeN(const QString &val)
// {
//     int passedval = val.toInt();
//     currFunction->setN(termIndex, passedval);
// }

// void interface::changeM(const QString &val)
// {
//     int passedval = val.toInt();
//     currFunction->setM(termIndex, passedval);
// }

// void interface::changeR(const QString &val)
// {
//     double passedval = val.toDouble();
//     currFunction->setR(termIndex, passedval);
// }

// void interface::changeA(const QString &val)
// {
//     double passedval = val.toDouble();
//     currFunction->setA(termIndex, passedval);
// }

void interface::changeN(int val)
{
    // int passedval = val.toInt();
    currFunction->setN(termIndex, val);
    updatePreviewDisplay();
}

void interface::changeM(int val)
{
    // int passedval = val.toInt();
    currFunction->setM(termIndex, val);
    updatePreviewDisplay();
}

void interface::changeR(double val)
{
    // double passedval = val.toDouble();
    currFunction->setR(termIndex, val);
    updatePreviewDisplay();
}

void interface::changeA(double val)
{
    // double passedval = val.toDouble();
    currFunction->setA(termIndex, val);
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    saveloadPath,
                               tr("PPM (*.ppm);;JPEG (*.jpg *.jpeg)"));

    QFile inFile(fileName);
    if (!inFile.open(QIODevice::WriteOnly))
             return;

      double worldY, worldX ;
      QImage outimg(settings::OWidth, settings::OHeight, QImage::Format_RGB32);

      for (int y = 0; y < settings::OHeight; y++)
          for (int x = 0; x <= ((settings::OWidth)-1); x++)

          {   worldY= settings::Height-y*settings::Height/settings::OHeight+settings::YCorner ;
              worldX= x*settings::Width/settings::OWidth+settings::XCorner;

              std::complex<double> fout=(*currFunction)(worldX,worldY);        //run the point through our mathematical function
              QRgb color = (*currColorWheel)(fout);                              //...then turn that complex output into a color per our color wheel

              outimg.setPixel(x, y, color);

    }

          outimg.save(fileName);

          QDir stickypath(fileName);
          stickypath.cdUp();
          saveloadPath = stickypath.path();
}
