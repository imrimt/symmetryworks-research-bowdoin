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
#include <QImage>
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
#include <QUndoStack>
#include <QUndoCommand>
#include <QProgressBar>
#include <QTableWidget>
#include <QHeaderView>
#include <QToolTip>

#include "historydisplay.h"
#include "polarplane.h"
#include "port.h"

#define MAX_NUM_TERMS 99

const unsigned int INVALID_FILE_ERROR = 0;
const unsigned int INVALID_OUTPUT_IMAGE_DIM = 1;
const int MIN_IMAGE_DIM = 20;
const int MAX_IMAGE_DIM = 10000;
const double ASPECT_SCALE = 0.02;
const double MAX_ASPECT_RATIO_PREVIEW = MAX_IMAGE_DIM * ASPECT_SCALE;
const double MIN_ASPECT_RATIO_PREVIEW = MIN_IMAGE_DIM * ASPECT_SCALE;

// QLineEdit subclass that undo changes (if not entered) when loses focus
class CustomLineEdit : public QLineEdit 
{
    Q_OBJECT
public:
    CustomLineEdit(QWidget *parent = 0) : QLineEdit(parent) { }

    void focusOutEvent(QFocusEvent * /*unused*/) {
        if (isModified()) undo();
        setReadOnly(true);
        deselect();
    }

    void focusInEvent(QFocusEvent * /*unused*/) {
        setReadOnly(false);
    }
    
};



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
    QLabel *infoLabel;
    
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
    ProgressBar(const QString &title, Port *port) {
        
        label = new QLabel(title, this);
        percentLabel = new QLabel(this);
        progBar = new QProgressBar(this);
        progBar->setRange(0, 100);
        progBar->setAlignment(Qt::AlignCenter);
        progBar->setValue(0);
        // progBar->setVisible(false);
        progBar->setPalette(QColor(Qt::gray));
        this->setVisible(true);
        
        layout = new QHBoxLayout();
        layout->addWidget(label);
        layout->addWidget(progBar);
        layout->addWidget(percentLabel);

        this->port = port;
        
        
    }

    double value() { return this->text().toDouble(); }
    
    void remove() {
        delete label;
        delete percentLabel;
        delete progBar;
    }

    void resetBar(const QString &title, Port *port) {
        label = new QLabel(title);
        percentLabel = new QLabel();
        progBar = new QProgressBar();
        progBar->setRange(0, 100);
        progBar->setAlignment(Qt::AlignCenter);
        progBar->setValue(0);
        progBar->setPalette(QColor(Qt::gray));
        
        layout = new QHBoxLayout();
        layout->addWidget(label);
        layout->addWidget(progBar);
        layout->addWidget(percentLabel);

        this->port = port;
    
    }

    void setVisible(bool visible) { 
        progBar->setVisible(visible);
        label->setVisible(visible);
        percentLabel->setVisible(visible);
    }

    void setPermanentVisibility(bool visibility) {
        this->visibility = visibility;
    }
    
    QProgressBar *progBar;
    QLabel *label;
    QLabel *percentLabel;
    QHBoxLayout *layout;
    
private:
    Port *port;
    bool visibility;
    
protected:
    
    void setNewValue(int val) {
        progBar->setValue(val);
        percentLabel->setText(progBar->text());
    }
    

signals: 
    void renderFinished();

public slots:
    // called when a thread has finished rendering its portion of the work
    // called to update progress bar throughout the rendering process
    void partialUpdate(const double &progress)
    {
        if (!this->isVisible() && visibility) { this->setVisible(true); }

        this->setNewValue(progress);

        //emit signals to reset table buttons
        if (progress == 100) {
            emit renderFinished();
            this->setVisible(false);
        }
    }


};


class ChangeCommand : public QUndoCommand
{
public:
    
    // ChangeCommand(QSpinBox *item, double oldVal, double newVal, QUndoCommand *parent = 0) : QUndoCommand(parent)
    ChangeCommand(QObject *item, double oldVal, double newVal, QUndoCommand *parent = 0) : QUndoCommand(parent)
    {

        this->item = item;
        this->oldVal = oldVal;
        qDebug() << "old value" << oldVal;
        this->newVal = newVal;
        qDebug() << "new value" << newVal;
        canRedo = false;
    }
    
    ~ChangeCommand() {}
    
    void undo() Q_DECL_OVERRIDE {
        qDebug() << "UNDO to" << oldVal;
        if (QSpinBox* boxItem = dynamic_cast<QSpinBox*>(item)) {
            boxItem->setValue(oldVal);
        }   

        canRedo = true;
    }
    void redo() Q_DECL_OVERRIDE {
        if(!canRedo) return;
        qDebug() << "REDO to" << newVal;

        if (QSpinBox* boxItem = dynamic_cast<QSpinBox*>(item)) {
            boxItem->setValue(newVal);
        }         
        
    }



    
private:
    // QSpinBox *item;
    QObject *item;

    double oldVal, newVal;
    bool canRedo;
    
};

// Interface class
class Interface : public QWidget
{
    Q_OBJECT
public:
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
    QSignalMapper *polarPlaneMapper;    
    
    // global scaling factors SUBELEMENTS
    QGroupBox *globalScalingBox;
    QVBoxLayout *globalScalingBoxLayout;
    QHBoxLayout *scaleALayout;
    QHBoxLayout *scaleRLayout;
    QLabel *scaleALabel;
    QLabel *scaleRLabel;
    QDoubleSlider *scaleAEditSlider;
    QDoubleSlider *scaleREditSlider;
    CustomLineEdit *scaleAEdit;
    CustomLineEdit *scaleREdit;
    QPushButton *scalePlaneEdit;
    
    // functionConstants SUBELEMENTS
    QLabel *currTermLabel;
    QLabel *nLabel;
    QLabel *mLabel;
    QLabel *aLabel;
    QLabel *rLabel;
    QLabel *aValueLabel;
    QLabel *rValueLabel;
    CustomSpinBox *currTermEdit;
    QSpinBox *nEdit;
    QSpinBox *mEdit;
    QDoubleSlider *aEdit;
    QDoubleSlider *rEdit;
    QPushButton *coeffPlaneEdit;

    QGroupBox *functionConstantsBox;
    QGridLayout *functionTermsGrid;
    QVBoxLayout *functionConstantsBoxLayout;
    QVBoxLayout *functionConstantsWidgetLayout;
    QHBoxLayout *functionConstantsScalingTerms;
    QHBoxLayout *termEditLayout;
    QVBoxLayout *functionConstantsPairs;
    QHBoxLayout *functionConstantsFreqs;
    QHBoxLayout *functionConstantsCoeffs;

    // TERM VIEW TABLE
    QPushButton *termViewButton;
    QWidget *termViewWidget;
    QVBoxLayout *termViewLayout;
    QTableWidget *termViewTable;
    QHeaderView *termViewHeaderHorizontal;
    QHeaderView *termViewHeaderVertical;
    QStringList termViewHorizontalHeaders;
    QStringList termViewVerticalHeaders;
    QPushButton *addTermButton;
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
    QColorDialog *setOverflowColorPopUp;

    // IMAGE DATA POINTS
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
    QVBoxLayout *imagePropsBoxLayout;
    QHBoxLayout *imageShiftXLayout;
    QHBoxLayout *imageShiftYLayout;
    QHBoxLayout *imageStretchXLayout;
    QHBoxLayout *imageStretchYLayout;
    
    QLabel *XShiftLabel;
    QLabel *YShiftLabel;
    QDoubleSlider *XShiftEditSlider;
    QDoubleSlider *YShiftEditSlider;
    CustomLineEdit *XShiftEdit;
    CustomLineEdit *YShiftEdit;
    QLabel *worldWidthLabel;
    QLabel *worldHeightLabel;
    QDoubleSlider *worldWidthEditSlider;
    QDoubleSlider *worldHeightEditSlider;
    CustomLineEdit *worldWidthEdit;
    CustomLineEdit *worldHeightEdit;
    
    QSpacerItem *pspacer1;

    // DISP SUBELEMENTS
    QPushButton *snapshotButton;
    Display *disp;
    QVBoxLayout *dispLayout;
    QHBoxLayout *buttonLayout;    

    // SHORTCUTS
    QShortcut *updatePreviewShortcut;
    
    QUndoStack *undoStack;
    
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
    
    QWidget *aspectRatioWidget;
    Display *aspectRatioPreview;
    QLineEdit *aspectRatioEdit;
    QLabel *aspectRatioLabel;
    QHBoxLayout *aspectRatioEditLayout;
    QHBoxLayout *aspectRatioPreviewLayout;
    
    void setSnapShotWindow(HistoryDisplay* window);

signals:
    void imageActionStatus(bool status);

private slots:
    void updateCurrTerm(int i);
    void changeNumTerms(int i);
    void colorWheelChanged(int index);
    void setImagePushed();
    void selectColorWheel();
    void selectImage();
    
    //TODO for each change function, push current value onto the undostack...each action has its own command?
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

    void addNewImageDataPoint(const ComplexValue &data) { *imageDataSeries << QPointF(data.real(), data.imag()); }
    void showImageDataGraph() { updateImageDataGraph(); imageDataWindow->hide(); imageDataWindow->show(); }
    void updateImageDataGraph();

    void startShifting(const QPoint &point);
    void updateShifting(const QPoint &point);
    void finishShifting();
    
    void changeAspectRatio();

    void handleUndo();
    void handleRedo();

protected:
    void mousePressEvent(QMouseEvent *event);
    //void keyPressEvent(QKeyEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);
    
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
    void initToolTips();
    void connectAllSignals();
    void removeTerm(int row);
    void removeTableTerm(int row);
    void refreshLabels();
    void updatePreviewDisplay();
    void errorHandler(const int &flag);
    void refreshTableTerms();
    void refreshMainWindowTerms();
    void updateAspectRatio();

    //main data structures
    QVector<AbstractFunction *> functionVector;
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    Settings *settings;
    Port *previewDisplayPort, *imageExportPort, *aspectRatioPreviewDisplayPort;

    //operational variables
    int previewWidth, previewHeight, previewSize;       //preview display size
    int numTerms;               //total number of terms
    unsigned int termIndex;     //currently editing term
    int coeffFlag;      //mapping variable for polar plane
    bool newUpdate;     //guard variable for preview update
    double aspectRatio;
    bool newAction;     //new action performed, not because of undo/redo 
    
    //I/O-related variables    
    QString saveloadPath;
    QString currFileName;
    QString imageSetPath;
    QString openImageName;

    //mouse-related variables
    bool mouseMoving;
    QPoint prevMousePos;    
};

#endif // Interface_H
