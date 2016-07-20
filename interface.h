#ifndef Interface_H
#define Interface_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QComboBox>
#include <QString>
#include <QPushButton>
#include <QImage>
#include <QDir>
#include <QInputDialog>
#include <QFileDialog>
#include <QSignalMapper>
#include <QDateTime>
#include <QShortCut>
#include <QAction>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QTimer>
#include <QRadioButton>
#include <QColorDialog>
#include <QObject>
#include <QApplication>
#include <QDesktopWidget>
//#include <QUndoStack>

#include <QProgressBar>
#include <QTableWidget>
#include <QHeaderView>

#include "historydisplay.h"
#include "polarplane.h"
#include "port.h"

#define MAX_NUM_TERMS 99

const unsigned int INVALID_FILE_ERROR = 0;
const unsigned int INVALID_OUTPUT_IMAGE_DIM = 1;

const int MIN_IMAGE_DIM = 20;
const int MAX_IMAGE_DIM = 10000;

enum wallpaperFunctionSet { };

// QSpinBox subclass that disallows user input
class CustomSpinBox : public QSpinBox
{
    Q_OBJECT
  public:
    CustomSpinBox(QWidget *parent = 0) : QSpinBox(parent) { }

  protected:
    void keyPressEvent(QKeyEvent *event) {event->ignore();}

};

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QWidget *parent = 0) : QLabel(parent) { }
    explicit ClickableLabel(int index, QWidget *parent = 0) : QLabel(parent) { this->index = index; }
    
signals:
    void doubleClickFunctionIcon(int index) ;
    
protected:
    void mouseDoubleClickEvent(QMouseEvent * /* unused */) {
        emit doubleClickFunctionIcon(index);
    }
    
private:
    int index;
};



// QSlider subclass that takes doubles
class QDoubleSlider : public QSlider
{
    Q_OBJECT
public:
    QDoubleSlider(QWidget *parent = 0) : QSlider(parent) { 
        connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(notifyValueChanged(int)));
    }
    

signals:
    void doubleValueChanged(double value);

public slots:
    void notifyValueChanged(int value) {
        double doubleValue = value / 100.0;
        emit doubleValueChanged(doubleValue);
    }

};

// QProgressBar subclass for tracking wallgen rendering
class ProgressBar : public QProgressBar
{
    Q_OBJECT
    
public:
    ProgressBar(const QString &title, bool visible, Port *port) {
        
        label = new QLabel(title);
        percentLabel = new QLabel();
        progBar = new QProgressBar();
        progBar->setRange(0, 100);
        progBar->setAlignment(Qt::AlignCenter);
        progBar->setValue(0);
        progBar->setVisible(visible);
        progBar->setPalette(QColor(Qt::gray));
        label->setVisible(visible);
        
        layout = new QHBoxLayout();
        layout->addWidget(label);
        layout->addWidget(progBar);
        layout->addWidget(percentLabel);

        this->port = port;
        this->visible = visible;
        
    }

    double value() { return this->text().toDouble(); }
    
    void remove() {
        delete label;
        delete percentLabel;
        delete progBar;
    }

    void resetBar(const QString &title, bool visible, Port *port) {
        label = new QLabel(title);
        percentLabel = new QLabel();
        progBar = new QProgressBar();
        progBar->setRange(0, 100);
        progBar->setAlignment(Qt::AlignCenter);
        progBar->setValue(0);
        progBar->setVisible(visible);
        progBar->setPalette(QColor(Qt::gray));
        label->setVisible(visible);
        
        layout = new QHBoxLayout();
        layout->addWidget(label);
        layout->addWidget(progBar);
        layout->addWidget(percentLabel);

        this->port = port;
        this->visible = visible;
    }

    void setVisible(bool visible) { 
        this->visible = visible;
        progBar->setVisible(visible);
        label->setVisible(visible);
    }
    
    QProgressBar *progBar;
    QLabel *label;
    QLabel *percentLabel;
    QHBoxLayout *layout;
    
private:
    bool visible;
    Port *port;
    
protected:
    
    void setNewValue(int val) {
        // qDebug() << "progBar exists?:" << (bool)(progBar != NULL);
        progBar->setValue(val);
        // qDebug() << "why am I crashing here?";
        percentLabel->setText(progBar->text());
    }
    

signals: 
    void renderFinished();

public slots:
    // called when a thread has finished rendering its portion of the work
    // called to update progress bar throughout the rendering process
    void partialUpdate(const double &progress)
    {
        if (!visible) { progBar->setVisible(true); label->setVisible(true); }
        //qDebug() << "progress" << progress;
        this->setNewValue(progress);

        //emit signals to reset table buttons
        if (progress == 100) {
            emit renderFinished();
            QTimer::singleShot(2000, this, SLOT(updateColor()));
        }
    }

    void updateColor() {
        // qDebug() << "updating color";
        // QString value1 = "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #F10350,stop: 0.4999 #FF3320,stop: 0.5 #FF0019,stop: 1 #FF0000 );}";
        // this->setStyleSheet(value1);
    }

};


// Interface class
class Interface : public QWidget
{
    Q_OBJECT
public:
    // Interface(QWidget *parent = 0) : QWidget(parent) { }
    // explicit Interface(QWidget *parent = 0);
    Interface(QWidget *parent = 0); 
    ~Interface() {;} 

    // ORGANIZATIONAL ELEMENTS
    QWidget *functionConstantsWidget;
    QHBoxLayout *overallLayout;
    QVBoxLayout *InterfaceLayout;
    QWidget *displayWidget;
    QHBoxLayout *topbarLayout;
    QVBoxLayout *leftbarLayout;
    QGroupBox *imagePropsBox;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem2;
    //QWidget *toggleViewWidget;
    //QWidget *patternTypeWidget;
    QMessageBox *errorMessageBox;

    // INPUT VALIDATORS
    QDoubleValidator *doubleValidate;
    QDoubleValidator *posdoubleValidate;
    QDoubleValidator *angleValidate;
    QIntValidator *intValidate;
    QIntValidator *posintValidate;
    QIntValidator *numtermsValidate;
    QIntValidator *dimValidate;

    // SUPPLEMENTAL WINDOWS
    HistoryDisplay *historyDisplay;
    PolarPlane *polarPlane;

    //modeToggle SUBELEMENTS
//    QPushButton *toggleViewButton;
//    QVBoxLayout *toggleViewLayout;
    
    
    // global scaling factors SUBELEMENTS
    QGroupBox *globalScalingBox;
    //QLabel *globalConstantsLabel;
    QVBoxLayout *globalScalingBoxLayout;
    QHBoxLayout *scaleALayout;
    QHBoxLayout *scaleRLayout;
    //QGridLayout *globalScalingBoxGrid;
    QDoubleSlider *scaleAEditSlider;
    QDoubleSlider *scaleREditSlider;
    QLineEdit *scaleAEdit;
    QLineEdit *scaleREdit;
    QPushButton *scalePlaneEdit;
    
    // functionConstants SUBELEMENTS
    QPushButton *termViewButton;
    QWidget *termViewWidget;
    QVBoxLayout *termViewLayout;
    QTableWidget *termViewTable;
    QHeaderView *termViewHeaderHorizontal;
    QHeaderView *termViewHeaderVertical;
    QStringList termViewHorizontalHeaders;
    QStringList termViewVerticalHeaders;
    QPushButton *addTermButton;
    
    QLabel *currTermLabel;
    CustomSpinBox *currTermEdit;
    QLabel *nLabel;
    QLabel *mLabel;
    QLabel *aLabel;
    QLabel *rLabel;
    QLabel *aValueLabel;
    QLabel *rValueLabel;
    QLabel *scaleALabel;
    QLabel *scaleRLabel;
    QSpinBox *nEdit;
    QSpinBox *mEdit;
    QDoubleSlider *aEdit;
    QDoubleSlider *rEdit;
    QPushButton *coeffPlaneEdit;
    
    QSignalMapper *polarPlaneMapper;

    QGroupBox *functionConstantsBox;
    
    QGridLayout *functionTermsGrid;

    QVBoxLayout *functionConstantsBoxLayout;
    QVBoxLayout *functionConstantsWidgetLayout;
    QHBoxLayout *functionConstantsScalingTerms;
    QHBoxLayout *termEditLayout;
    QVBoxLayout *functionConstantsPairs;
    QHBoxLayout *functionConstantsFreqs;
    QHBoxLayout *functionConstantsCoeffs;
    
    QPushButton *loadButton;
    QPushButton *saveButton;
    
    QSignalMapper *termViewTableMapper;
    QSignalMapper *removeTermMapper;

    // patternTypeBox SUBELEMENTS
    QGroupBox *patternTypeBox;
    QVBoxLayout *patternTypeBoxOverallLayout;
    QVBoxLayout *patternTypeBoxLayout;
    QHBoxLayout *functionLayout;
    QHBoxLayout *colorwheelLayout;
    QHBoxLayout *fromImageLayout;
    
    QLabel *colorwheelLabel;
    QLabel *functionLabel;
    QLabel *imagePathLabel;
    QLabel *numTermsLabel;
    QLabel *freqpairLabel;
    QLabel *coeffLabel;
    QSpinBox *numTermsEdit;
    QSpacerItem *gspacer1;
    QSpacerItem *gspacer2;
    QSpacerItem *gspacer3;
    QSpacerItem *gspacer4;
    QSpacerItem *gspacer5;
    QComboBox *colorwheelSel;
    QComboBox *functionSel;
    QPushButton *setLoadedImage;
    QRadioButton *fromImageButton;
    QRadioButton *fromColorWheelButton;
    // QPushButton *setOverflowColorButton;
    QColorDialog *setOverflowColorPopUp;

    // IMAGE DATA POINTS
    // QPushButton *showImageDataGraphButton;
    QPushButton *updateImageDataGraphButton;
    QScatterSeries *imageDataSeries;
    QScatterSeries *prevDataSeries;
    QChart *imageDataGraph;
    QValueAxis *imageDataGraphAxisX;
    QValueAxis *imageDataGraphAxisY;
    QChartView *imageDataGraphView;
    QLabel *imageLabel;
    QPixmap imagePixmap;
    QWidget *imageDataWindow;
    QVBoxLayout *imageDataWindowGraphLayout;
    QHBoxLayout *imageDataWindowLayout;

    // imagePropsBox SUBELEMENTS
    //QVBoxLayout *imagePropsBoxStack;
    //QVBoxLayout *imagePropsEditStack;
    QVBoxLayout *imagePropsBoxLayout;
    //QHBoxLayout *savePushLayout;
    //QVBoxLayout *imagePropsBoxOverallLayout;
    
    QHBoxLayout *imageShiftXLayout;
    QHBoxLayout *imageShiftYLayout;
    QHBoxLayout *imageStretchXLayout;
    QHBoxLayout *imageStretchYLayout;
    
    QLabel *XShiftLabel;
    QLabel *YShiftLabel;
    QDoubleSlider *XShiftEditSlider;
    QDoubleSlider *YShiftEditSlider;
    QLineEdit *XShiftEdit;
    QLineEdit *YShiftEdit;
    
//    QLabel *outHeightLabel;
//    QLabel *outWidthLabel;
    
    QLabel *worldWidthLabel;
    QLabel *worldHeightLabel;
    
//    QLineEdit *worldWidthEditSlider;
//    QLineEdit *worldHeightEditSlider;
    QDoubleSlider *worldWidthEditSlider;
    QDoubleSlider *worldHeightEditSlider;
    QLineEdit *worldWidthEdit;
    QLineEdit *worldHeightEdit;
    
    QSpacerItem *pspacer1;
//    QSpacerItem *pspacer2;
//    QSpacerItem *pspacer3;
//    QSpacerItem *pspacer4;
//    QSpacerItem *pspacer5;

    // DISP SUBELEMENTS
    QPushButton *snapshotButton;
    //QPushButton *exportImage;
    //QPushButton *resetButton;
    Display *disp;
    QVBoxLayout *dispLayout;
    QHBoxLayout *buttonLayout;
    
    

    // SHORTCUTS
    //QShortcut *updatePreviewShortcut;
    
   
    // PROGRESS BARS
    ProgressBar *displayProgressBar;
    ProgressBar *exportProgressBar;
    
    // OUTPUT IMAGE DIM POP UP
    QWidget *imageDimensionsPopUp;
    QVBoxLayout *imageDimensionsPopUpLayout;
    QDialogButtonBox *buttonBox;
    QHBoxLayout *outWidthLayout;
    QHBoxLayout *outHeightLayout;
    
    QLabel *outWidthLabel;
    QLabel *outHeightLabel;
    QLineEdit*outHeightEdit;
    QLineEdit *outWidthEdit;
    
    QWidget *functionIconsWindow;
    QGridLayout *functionIconsWindowLayout;
    QPushButton *viewFunctionIconsButton;
    
    void setSnapShotWindow(HistoryDisplay* window);
    // void setFunctionIconsWindow(QWidget* window);   

signals:
    void imageActionStatus(bool status);

private slots:
    //void toggleViewMode();
    void updateCurrTerm(int i);
    void changeNumTerms(int i);
    void colorWheelChanged(int index);
    void setImagePushed();
    void selectColorWheel();
    void selectImage();
    void changeFunction(int index);
    void changeWorldWidth(double val);
    void changeWorldWidth();
    void changeWorldHeight(double val);
    void changeWorldHeight();
    void changeXCorner(double val);
    void changeXCorner();
    void changeYCorner(double val);
    void changeYCorner();
    void changeOWidth();
    void changeOHeight();
    void changeN(int val);
    void changeM(int val);
    void changeR(double val);
    void changeA(double val);
    void changeScaleA();
    void changeScaleA(double val);
    void changeScaleR();
    void changeScaleR(double val);

    void exportImageFunction() { imageDimensionsPopUp->show(); }
    void cancelImageExport() { imageDimensionsPopUp->hide(); }
    void startImageExport();
    
//    void undo();
//    void redo();
    
    void resetFunction();
    void loadFromSettings();
    void saveCurrWorkspace();
    void saveCurrWorkspaceAs();
    void previewDisplayEnlarge() {disp->enlarge();}
    void previewDisplayShrink() {disp->shrink();}
    void previewDisplayResetSize() {disp->resetSize();}
    void snapshotFunction();
    void termViewPopUp();
    void addTerm();
    void updateTermTable(QObject *cell);
    void addTermTable();
    void termViewCellClicked(int row, int col);
    void resetTableButton();
    void setPolarCoordinates(int coeffFlag, const QString &radius, const QString &angle);
    QString loadSettings(const QString &fileName);
    void popUpImageExportFinished(const QString &filePath);
    void resetMainWindowButton(const bool &status);
    
    void showFunctionIcons() { functionIconsWindow->hide(), functionIconsWindow->show(); }
    void showOverflowColorPopUp() { setOverflowColorPopUp->show(); }
    //void hideFunctionicon(int index) { functionIconsWindow->hide(); }

    void addNewImageDataPoint(const ComplexValue &data) { *imageDataSeries << QPointF(data.real(), data.imag()); }
    void showImageDataGraph() { updateImageDataGraph(); imageDataWindow->hide(); imageDataWindow->show(); }
    void updateImageDataGraph();
    
private:    
    QString genLabel(const char * in);    
    QString getCurrSettings(const HistoryItem &item);
    QString saveSettings(const QString &fileName);

    // Interface display and formatting functions
    void initInterfaceLayout();
    void initPreviewDisplay();
    void initFunctionConstants();
    void initPatternType();
    void initGlobalScaling();
    void initImageProps();
    void initImageExportPopUp();
    void initCoeffPlane();
    void connectAllSignals();
    void removeTerm(int row);
    void refreshLabels();
    void updatePreviewDisplay();
    void errorHandler(const int &flag);
    void refreshTableTerms();
    void refreshMainWindowTerms();

    int previewWidth, previewHeight, previewSize;
    int numTerms;
    unsigned int termIndex; 
    
    QString saveloadPath;
    QString currFileName;
    QString imageSetPath;
    QString openImageName;
    
    AbstractFunction * currFunction;
    ColorWheel * currColorWheel;
    QSharedPointer<AbstractFunction> currFunctionPtr;
    QSharedPointer<ColorWheel> currColorWheelPtr;
    
    Port *previewDisplayPort, *imageExportPort;

    //bool advancedMode;
    int coeffFlag;
    
    QVector<AbstractFunction *> functionVector;
    
    Settings *settings;
    
};

#endif // Interface_H
