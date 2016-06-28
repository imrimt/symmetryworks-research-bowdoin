#include "interface.h"

namespace settings      //the settings namespace stores
{                       //a few variables used to compute the
    double Width=5.0;   //image via the mathematical function
    double Height=4.0;  //and the color wheel, as well as
    double XCorner=0.0; //the Output Width and Height.
    double YCorner=0.0;
    int OWidth=6000;
    int OHeight=4800;
}

interface::interface(QWidget *parent) :
    QWidget(parent)
{

    // FUNCTIONAL VARIABLES
    termindex = 0;
    saveloadPath = QDir::homePath();

    // FUNCTIONAL OBJECTS
    f = new holoFunction();
    c = new ColorWheel();


    // ORGANIZATIONAL ELEMENTS
    editconst = new QGroupBox(tr("Function Constants"), this);          //create elements
    interfaceLayout = new QVBoxLayout(this);
    topbarLayout = new QHBoxLayout();
    propsBox = new QGroupBox(tr("Image Properties"), this);
    displayWidget = new QWidget(this);
    genBoxWidget = new QWidget(this);

    topbarLayout->addWidget(genBoxWidget);
    topbarLayout->addWidget(displayWidget);
    topbarLayout->addWidget(propsBox);
    interfaceLayout->addLayout(topbarLayout);                       //lay out vertically
    interfaceLayout->addWidget(editconst);
    setLayout(interfaceLayout);

    // INPUT VALIDATORS (NUMERICAL)
    doubleValidate = new QDoubleValidator(-9999999.0, 9999999.0, 5, this);
    intValidate = new QIntValidator(-9999999, 9999999, this);
    posdoubleValidate = new QDoubleValidator(1.0, 9999999.0, 5, this);
    posintValidate = new QIntValidator(1, 9999999, this);
    numtermsValidate = new QIntValidator(1, 99, this);
    dimValidate = new QIntValidator(1, 2000, this);

    // EDITCONST SUBELEMENTS
    currtermL = new QLabel(editconst);                              //create labels
    currtermL->setText(tr("Term"));
    nL = new QLabel(editconst);
    mL = new QLabel(editconst);
    aL = new QLabel(editconst);
    rL = new QLabel(editconst);
    scaleaL = new QLabel(tr("sa"), editconst);
    scalerL = new QLabel(tr("sr"), editconst);
    refreshLabels();
    nE = new QLineEdit(editconst);                              //create input boxes
    mE = new QLineEdit(editconst);
    aE = new QLineEdit(editconst);
    rE = new QLineEdit(editconst);
    scaleaE = new QLineEdit(editconst);
    scalerE = new QLineEdit(editconst);
    currtermE = new QSpinBox(editconst);
    espacer1 = new QSpacerItem(10, 15);
    espacer2 = new QSpacerItem(10, 15);
    espacer3 = new QSpacerItem(10, 15);
    espacer4 = new QSpacerItem(10, 15);
    espacer5 = new QSpacerItem(130, 15);
    espacer6 = new QSpacerItem(2, 0);
    espacer7 = new QSpacerItem(15, 15);
    espacer8 = new QSpacerItem(8, 15);
    loadButton = new QPushButton(tr("Load..."), editconst);
    saveButton = new QPushButton(tr("Save"), editconst);
    currtermE->setMaximum(4);
    currtermE->setMinimum(1);
    nE->setFixedWidth(75);
    mE->setFixedWidth(75);
    rE->setFixedWidth(75);
    aE->setFixedWidth(75);
    scaleaE->setFixedWidth(75);
    scalerE->setFixedWidth(75);
    nL->setFixedWidth(18);
    mL->setFixedWidth(20);
    rL->setFixedWidth(18);
    aL->setFixedWidth(18);
    scaleaL->setFixedWidth(18);
    scalerL->setFixedWidth(18);
    nE->setValidator(intValidate);
    mE->setValidator(intValidate);
    rE->setValidator(doubleValidate);
    aE->setValidator(doubleValidate);
    scaleaE->setValidator(doubleValidate);
    scalerE->setValidator(doubleValidate);


    editconstLayoutStack = new QVBoxLayout(editconst);  //initialize layout
    editconstLayout = new QHBoxLayout();
    editconstLayoutLower = new QHBoxLayout();
    editconstLayoutLower->setAlignment(Qt::AlignRight);
    editconstLayout->setAlignment(Qt::AlignRight);

    editconstLayout->addWidget(currtermL);              //fill layouts
    editconstLayout->addWidget(currtermE);
    editconstLayout->addItem(espacer1);
    editconstLayout->addWidget(nL);
    editconstLayout->addWidget(nE);
    editconstLayout->addItem(espacer2);
    editconstLayout->addWidget(mL);
    editconstLayout->addWidget(mE);
    editconstLayout->addItem(espacer3);
    editconstLayout->addWidget(rL);
    editconstLayout->addWidget(rE);
    editconstLayout->addItem(espacer4);
    editconstLayout->addWidget(aL);
    editconstLayout->addWidget(aE);
    editconstLayoutLower->addItem(espacer8);
    editconstLayoutLower->addWidget(loadButton);
    editconstLayoutLower->addItem(espacer7);
    editconstLayoutLower->addWidget(saveButton);
    editconstLayoutLower->addItem(espacer5);
    editconstLayoutLower->addWidget(scalerL);
    editconstLayoutLower->addWidget(scalerE);
    editconstLayoutLower->addItem(espacer6);
    editconstLayoutLower->addWidget(scaleaL);
    editconstLayoutLower->addWidget(scaleaE);
    editconstLayoutStack->addLayout(editconstLayout);
    editconstLayoutStack->addLayout(editconstLayoutLower);
    editconst->setLayout(editconstLayoutStack);

    // GENBOX SUBELEMENTS
    genBox = new QGroupBox(tr("Image Generation"), genBoxWidget);
    functionSel = new QComboBox(genBox);        //initialize elements
    colorwheelSel = new QComboBox(genBox);
    functionL = new QLabel(genBox);
    colorwheelL = new QLabel(genBox);
    numtermsL = new QLabel(tr("<i>No. Terms</i>"), genBox);
    numtermsE = new QLineEdit("6", genBox);
    gspacer1 = new QSpacerItem(0,20);
    gspacer2 = new QSpacerItem(0,10);
    gspacer3 = new QSpacerItem(0,10);
    gspacer4 = new QSpacerItem(0,50);
    setLoadedImage = new QPushButton(tr("Set Image..."), genBox);

    genBoxOverallLayout = new QVBoxLayout(genBoxWidget);
    functionLayout = new QVBoxLayout();
    colorwheelLayout = new QVBoxLayout();
    numtermsLayout = new QHBoxLayout();

    functionSel->addItem("Hex3");
    functionSel->addItem("Hex6");
    functionSel->addItem("Square");
    functionSel->addItem("Rhombic");
    functionSel->addItem("RhombicPaired");
    functionSel->addItem("Rectangular");
    functionSel->addItem("RectangularPaired");
    functionSel->addItem("General");
    functionSel->addItem("GeneralPaired");
    functionSel->addItem("HoloSum");
    functionSel->addItem("SumConjugates");
    functionSel->addItem("ZZbarFcn");
    functionSel->addItem("SquareR4Neg");
    functionSel->addItem("SquareColTurn");
    functionSel->addItem("HexColorTurn");
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
    functionL->setText(tr("Function"));
    colorwheelL->setText(tr("Color Wheel"));
    numtermsE->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    numtermsL->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    colorwheelSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    colorwheelL->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionSel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    functionL->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);



    numtermsE->setFixedWidth(30);
    numtermsE->setValidator(numtermsValidate);

    genBoxLayout = new QVBoxLayout(genBox);     //set up layout
    functionLayout->addWidget(functionL);
    functionLayout->addWidget(functionSel);
    numtermsLayout->addWidget(numtermsL);
    numtermsLayout->addWidget(numtermsE);
    functionLayout->addLayout(numtermsLayout);
    genBoxLayout->addLayout(functionLayout);
    genBoxLayout->addItem(gspacer1);
    colorwheelLayout->addItem(gspacer3);
    colorwheelLayout->addWidget(colorwheelL);
    colorwheelLayout->addWidget(colorwheelSel);
    colorwheelLayout->addWidget(setLoadedImage);
    setLoadedImage->hide();
    genBoxLayout->addLayout(colorwheelLayout);
    genBoxLayout->addItem(gspacer2);

    genBoxOverallLayout->addWidget(genBox);
    genBoxOverallLayout->addStretch();
    //genBoxOverallLayout->addItem(gspacer4);

    // PROPSBOX SUBELEMENTS
    outheightL = new QLabel(tr("Output Height"), propsBox);          //initialize elements
    outwidthL = new QLabel(tr("Output Width"), propsBox);
    XCornerL = new QLabel(tr("XCorner"), propsBox);
    YCornerL = new QLabel(tr("YCorner"), propsBox);
    worldwidthL = new QLabel(tr("World Width"), propsBox);
    worldheightL = new QLabel(tr("World Height"), propsBox);
    outheightE = new QLineEdit(propsBox);
    outwidthE = new QLineEdit(propsBox);
    XCornerE = new QLineEdit(propsBox);
    YCornerE = new QLineEdit(propsBox);
    worldwidthE = new QLineEdit(propsBox);
    worldheightE = new QLineEdit(propsBox);
    saveImagePush = new QPushButton(tr("Save Image"), propsBox);
    pspacer1 = new QSpacerItem(0, 12);
    pspacer2 = new QSpacerItem(0, 14);
    pspacer3 = new QSpacerItem(15, 30);
    pspacer4 = new QSpacerItem(20, 60);
    pspacer5 = new QSpacerItem(20, 15);

//    outheightE->setFixedWidth(100);
//    outwidthE->setFixedWidth(100);
//    XCornerE->setFixedWidth(100);
//    YCornerE->setFixedWidth(100);
//    worldwidthE->setFixedWidth(100);
//    worldheightE->setFixedWidth(100);

//    outheightE->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    outwidthE->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    XCornerE->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    YCornerE->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    worldwidthE->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    worldheightE->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    saveImagePush->setFixedHeight(25);
    propsBoxOverallLayout = new QVBoxLayout(propsBox);
    propsBoxLayout = new QHBoxLayout();
    propseditStack = new QVBoxLayout();
    propslabelStack = new QVBoxLayout();
    savePushLayout = new QHBoxLayout();

    outheightE->setValidator(dimValidate);
    outwidthE->setValidator(dimValidate);
    XCornerE->setValidator(doubleValidate);
    YCornerE->setValidator(doubleValidate);
    worldwidthE->setValidator(doubleValidate);
    worldheightE->setValidator(doubleValidate);

    outheightE->setFixedWidth(100);
    outwidthE->setFixedWidth(100);
    XCornerE->setFixedWidth(100);
    YCornerE->setFixedWidth(100);
    worldwidthE->setFixedWidth(100);
    worldheightE->setFixedWidth(100);

    saveImagePush->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    saveImagePush->setFixedWidth(150);

    propslabelStack->addWidget(XCornerL);
    propslabelStack->addWidget(YCornerL);
    propslabelStack->addWidget(worldwidthL);
    propslabelStack->addWidget(worldheightL);
    propslabelStack->addItem(pspacer1);
    propslabelStack->addWidget(outwidthL);
    propslabelStack->addWidget(outheightL);

    propslabelStack->setAlignment(XCornerL, Qt::AlignLeft);
    propslabelStack->setAlignment(YCornerL, Qt::AlignLeft);
    propslabelStack->setAlignment(worldwidthL, Qt::AlignLeft);
    propslabelStack->setAlignment(worldheightL, Qt::AlignLeft);
    propslabelStack->setAlignment(outwidthL, Qt::AlignLeft);
    propslabelStack->setAlignment(outheightL, Qt::AlignLeft);

    propseditStack->addWidget(XCornerE);
    propseditStack->addWidget(YCornerE);
    propseditStack->addWidget(worldwidthE);
    propseditStack->addWidget(worldheightE);
    propseditStack->addItem(pspacer2);
    propseditStack->addWidget(outwidthE);
    propseditStack->addWidget(outheightE);

    //propsBoxLayout->addItem(pspacer3);
    propsBoxLayout->addLayout(propslabelStack);
    propsBoxLayout->addLayout(propseditStack);
    propsBoxLayout->addItem(pspacer4);

    //savePushLayout->addWidget(saveImagePush);
    //savePushLayout->addItem(pspacer5);

    propsBoxOverallLayout->addLayout(propsBoxLayout);
    //propsBoxOverallLayout->addItem(pspacer3);
    propsBoxOverallLayout->addWidget(saveImagePush);
    propsBoxOverallLayout->setAlignment(saveImagePush, Qt::AlignRight);

    // DISP SUBELEMENTS
    disp = new Display(200, displayWidget);
    updatePreview = new QPushButton(tr("Update Preview"), this);
    dispLayout = new QVBoxLayout(displayWidget);
    dispLayout->addWidget(disp);
    dispLayout->addWidget(updatePreview);

    // CONNECT SIGNALS & SLOTS
    connect(updatePreview, SIGNAL(clicked()), this, SLOT(updatePreviewDisplay()));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), c, SLOT(setCurrent(int)));
    connect(colorwheelSel, SIGNAL(currentIndexChanged(int)), this, SLOT(colorWheelChanged(int)));
    connect(setLoadedImage, SIGNAL(clicked()), this, SLOT(setImagePushed()));
    connect(functionSel, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFunction(int)));
    connect(numtermsE, SIGNAL(textChanged(QString)), this, SLOT(changeMaxTerms(QString)));
    connect(currtermE, SIGNAL(valueChanged(int)), this, SLOT(updateTerms(int)));

    connect(outwidthE, SIGNAL(textChanged(QString)), this, SLOT(changeOWidth(QString)));
    connect(outheightE, SIGNAL(textChanged(QString)), this, SLOT(changeOHeight(QString)));
    connect(worldwidthE, SIGNAL(textChanged(QString)), this, SLOT(changeWorldWidth(QString)));
    connect(worldheightE, SIGNAL(textChanged(QString)), this, SLOT(changeWorldHeight(QString)));
    connect(XCornerE, SIGNAL(textChanged(QString)), this, SLOT(changeXCorner(QString)));
    connect(YCornerE, SIGNAL(textChanged(QString)), this, SLOT(changeYCorner(QString)));

    connect(nE, SIGNAL(textChanged(QString)), this, SLOT(changeN(QString)));
    connect(mE, SIGNAL(textChanged(QString)), this, SLOT(changeM(QString)));
    connect(rE, SIGNAL(textChanged(QString)), this, SLOT(changeR(QString)));
    connect(aE, SIGNAL(textChanged(QString)), this, SLOT(changeA(QString)));
    connect(scalerE, SIGNAL(textChanged(QString)), this, SLOT(changeScaleR(QString)));
    connect(scaleaE, SIGNAL(textChanged(QString)), this, SLOT(changeScaleA(QString)));

    connect(saveImagePush, SIGNAL(clicked()), this, SLOT(saveImageStart()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFunction()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveFunction()));

    // SET DEFAULTS
    refreshTerms();
    scalerE->setText(QString::number(f->scaleR()));
    scaleaE->setText(QString::number(f->scaleA()));

    outwidthE->setText(QString::number(settings->OWidth));
    outheightE->setText(QString::number(settings->OHeight));
    worldwidthE->setText(QString::number(settings->Width));
    worldheightE->setText(QString::number(settings->Height));
    XCornerE->setText(QString::number(settings->XCorner));
    YCornerE->setText(QString::number(settings->YCorner));

    functionSel->setCurrentIndex(1);    //set up current function
    colorwheelSel->setCurrentIndex(7);


    // FINALIZE WINDOW
    setFixedSize(sizeHint());
    setWindowTitle(tr("Wallpaper Generation"));
    setTabOrder(nE, mE);
    setTabOrder(mE, rE);
    setTabOrder(rE, aE);
    setTabOrder(aE, scalerE);
    setTabOrder(scalerE, scaleaE);
    setTabOrder(scaleaE, XCornerE);
    setTabOrder(XCornerE, YCornerE);
    setTabOrder(YCornerE, worldwidthE);
    setTabOrder(worldwidthE, worldheightE);
    setTabOrder(worldheightE, outwidthE);
    setTabOrder(outwidthE, outheightE);
}

QString interface::genLabel(const char *in)     //concatenate the constant name
{                                               //with the current index number
    QString out;
    out.setNum(termindex+1);
    out.prepend(in);

    return out;
}

void interface::refreshLabels()                 //for updating our label names
{                                               //with the current term #
    nL->setText(genLabel("n"));
    mL->setText(genLabel("m"));
    rL->setText(genLabel("r"));
    aL->setText(genLabel("a"));
}

void interface::refreshTerms()
{
    nE->setText(QString::number(f->N(termindex)));
    mE->setText(QString::number(f->M(termindex)));
    rE->setText(QString::number(f->R(termindex)));
    aE->setText(QString::number(f->A(termindex)));
}

void interface::updateTerms(int i)
{
    termindex = i-1;

    refreshTerms();
    refreshLabels();
}

void interface::changeMaxTerms(const QString &val)
{
    int passedval = val.toInt();
    currtermE->setMaximum(passedval);
    f->setNumTerms(passedval);
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

    c->loadImage(fileName);

    QDir stickypath(fileName);
    stickypath.cdUp();
    saveloadPath = stickypath.path();
}

void interface::changeFunction(int index)
{
    delete f;

    switch(index)
    {
    case 0:
        f = new hex3Function();
        colorwheelSel->setCurrentIndex(7);
        break;
    case 1:
        f = new hex6Function();
        colorwheelSel->setCurrentIndex(7);
        break;
    case 2:
        f = new squareFunction();
        colorwheelSel->setCurrentIndex(6);
        break;
    case 3:
        f = new rhombicFunction();
        colorwheelSel->setCurrentIndex(3);
        break;
    case 4:
        f = new rhombicpairedFunction();
        colorwheelSel->setCurrentIndex(3);
        break;
    case 5:
        f = new rectangularFunction();
        colorwheelSel->setCurrentIndex(7);
        break;
    case 6:
        f = new rectangularpairedFunction();
        colorwheelSel->setCurrentIndex(7);
        break;
    case 7:
        f = new generalFunction();
        colorwheelSel->setCurrentIndex(1);
        break;
    case 8:
        f = new generalpairedFunction();
        colorwheelSel->setCurrentIndex(1);
        break;
    case 9:
        f = new holoFunction();
        colorwheelSel->setCurrentIndex(6);
        break;
    case 10:
        f = new contFunction();
        colorwheelSel->setCurrentIndex(6);
        break;
    case 11:
        f = new zzbarFunction();
        colorwheelSel->setCurrentIndex(6);
        break;
    case 12:
        f = new squareMFunction();
        colorwheelSel->setCurrentIndex(9);
        break;
    case 13:
        f = new squareTFunction();
        colorwheelSel->setCurrentIndex(9);
        break;
    case 14:
        f = new hex3CTFunction();
        colorwheelSel->setCurrentIndex(9);
        break;
    }

    refreshTerms();

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

    out << settings->Width << settings->Height;
    out << settings->XCorner << settings->YCorner;
    out << settings->OWidth << settings->OHeight;
    out << functionSel->currentIndex();
    out << colorwheelSel->currentIndex();
    out << f->scaleR() << f->scaleA();
    out << f->numterms();
    for(unsigned int i=0; i<f->numterms(); i++)
        out << f->N(i) << f->M(i) << f->R(i) << f->A(i);

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

    in >> settings->Width >> settings->Height;
    in >> settings->XCorner >> settings->YCorner;
    in >> settings->OWidth >> settings->OHeight;
    in >> tempint;
    functionSel->setCurrentIndex(tempint);
    in >> tempint;
    colorwheelSel->setCurrentIndex(tempint);

    in >> tempdouble;
    f->setScaleR(tempdouble);

    in >> tempdouble;
    f->setScaleA(tempdouble);

    in >> count;
    for(unsigned int i=0; i<count; i++)
    {
        in >> tempint; f->setN(i, tempint);
        in >> tempint; f->setM(i, tempint);
        in >> tempdouble; f->setR(i, tempdouble);
        in >> tempdouble; f->setA(i, tempdouble);
    }

    inFile.close();

    refreshTerms();
    outwidthE->setText(QString::number(settings->OWidth));
    outheightE->setText(QString::number(settings->OHeight));
    worldwidthE->setText(QString::number(settings->Width));
    worldheightE->setText(QString::number(settings->Height));
    XCornerE->setText(QString::number(settings->XCorner));
    YCornerE->setText(QString::number(settings->YCorner));
    updatePreviewDisplay();

    QDir stickypath(fileName);
    stickypath.cdUp();
    saveloadPath = stickypath.path();


}

void interface::updatePreviewDisplay()
{
    double worldY, worldX, pixX;

      for (int y = 0; y < disp->dim(); y++)
          for (int x = 0; x <= ((disp->dim()* 3)-1); x++)
          {   pixX=x/3;
              worldY= settings->Height-y*settings->Height/disp->dim()+settings->YCorner;
              worldX= pixX*settings->Width/disp->dim()+settings->XCorner;

              std::complex<double> fout = (*f)(worldX,worldY);  //run the point through our mathematical function
              QRgb color = (*c)(fout);                          //...then convert that complex output to a color according to our color wheel

              disp->setPixel(x/3, y, color);                    //finally push the determined color to the corresponding point on the display

          }

      //qDebug() << "hit" << thecount++;
      disp->repaint();

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

void interface::changeN(const QString &val)
{
    int passedval = val.toInt();
    f->setN(termindex, passedval);
}

void interface::changeM(const QString &val)
{
    int passedval = val.toInt();
    f->setM(termindex, passedval);
}

void interface::changeR(const QString &val)
{
    double passedval = val.toDouble();
    f->setR(termindex, passedval);
}

void interface::changeA(const QString &val)
{
    double passedval = val.toDouble();
    f->setA(termindex, passedval);
}

void interface::changeScaleA(const QString &val)
{
    double passedval = val.toDouble();
    f->setScaleA(passedval);
}

void interface::changeScaleR(const QString &val)
{
    double passedval = val.toDouble();
    f->setScaleR(passedval);
}

void interface::saveImageStart()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    saveloadPath,
                               tr("PPM (*.ppm);;JPEG (*.jpg *.jpeg)"));

    QFile inFile(fileName);
    if (!inFile.open(QIODevice::WriteOnly))
             return;

      double worldY, worldX, pixX;
      QImage outimg(settings->OWidth, settings->OHeight, QImage::Format_RGB32);

      for (int y = 0; y < settings->OHeight; y++)
          for (int x = 0; x <= ((settings->OWidth* 3)-1); x++)

          {   pixX=x/3;
              worldY= settings->Height-y*settings->Height/settings->OHeight+settings->YCorner ;
              worldX= pixX*settings->Width/settings->OWidth+settings->XCorner;

              std::complex<double> fout=(*f)(worldX,worldY);        //run the point through our mathematical function
              QRgb color = (*c)(fout);                              //...then turn that complex output into a color per our color wheel

              outimg.setPixel(x/3, y, color);

    }

          outimg.save(fileName);

          QDir stickypath(fileName);
          stickypath.cdUp();
          saveloadPath = stickypath.path();

}
