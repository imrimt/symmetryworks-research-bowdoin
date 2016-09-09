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

#ifndef Interface_H
#define Interface_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QKeyEvent>
#include <QCloseEvent>
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
#include <QStandardPaths>

#include "historydisplay.h"
#include "polarplane.h"
#include "port.h"

#define MAX_NUM_TERMS 99

const unsigned int INVALID_IMAGE_FILE_ERROR = 0;
const unsigned int INVALID_OUTPUT_IMAGE_DIM = 1;
const unsigned int INVALID_ASPECT_RATIO = 2;

const int MIN_IMAGE_DIM = 10;
const int MAX_IMAGE_DIM = 20000;
const double ASPECT_RATIO_SCALE = 0.025;
const double MIN_ASPECT_RATIO = 0.1;
const double MAX_ASPECT_RATIO = 12;
const int MAX_FREQ_VALUE = 10;
const int MIN_FREQ_VALUE = -10;
const int FREQ_SPINBOX_STEP = 1;
const double MAX_RADIUS = 10.0;
const double MIN_RADIUS = -10.0;
const double RADIUS_SPINBOX_STEP = 0.1;
const double ANGLE_SPINBOX_STEP = 0.25;
const double DISPLAY_RESIZE_SCALE_STEP = 0.1;
const int SNAPSHOT_ACTION = 0;
const int WORKSPACE_ACTION = 1;

// QLineEdit subclass that undo changes (if not entered) when loses focus
class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomLineEdit(QWidget *parent = 0) : QLineEdit(parent) { }
    
    void focusOutEvent(QFocusEvent * /*unused*/) {
        if (isModified()) this->undo();
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
    
protected:
    void mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            newVal = (double)this->value() / 100.0;
            //qDebug() << "release";
            emit newSliderAction(this, oldVal, newVal);
        }
    }
    
    void mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            oldVal = (double)this->value() / 100.0;
            QSlider::mousePressEvent(event);
        }
    }
    
signals:
    void doubleValueChanged(double value);
    void newSliderAction(QObject *item, double oldVal, double newVal);
    
    public slots:
    void notifyValueChanged(int value) {
        double doubleValue = value / 100.0;
        emit doubleValueChanged(doubleValue);
    }
    
private:
    double oldVal;
    double newVal;
    
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
        if(oldVal == newVal) return;
        this->item = item;
        
        this->oldVal = oldVal;
        //qDebug() << "old value" << oldVal;
        this->newVal = newVal;
        //qDebug() << "new value" << newVal;
        
        
        canUndo = true;
        canRedo = false;
    }
    
    ~ChangeCommand() {}
    
    void undo() Q_DECL_OVERRIDE {
        // qDebug() << "size of undo stack" << undoStack->count();
        if(!canUndo){ qDebug() << "can't undo"; return; }
        qDebug() << "UNDO to" << oldVal;
        
        if (QSpinBox *boxItem = dynamic_cast<QSpinBox*>(item) ) {
            boxItem->setValue(oldVal);
            // emit boxItem->returnPressed();
        }
        else if (QDoubleSpinBox *boxItem = dynamic_cast<QDoubleSpinBox*>(item)) {
            boxItem->setValue(oldVal);
            // emit boxItem->returnPressed();
        }
        if (CustomLineEdit *lineEditItem = dynamic_cast<CustomLineEdit*>(item)) {
            
            lineEditItem->setText(QString::number(oldVal));
            emit lineEditItem->returnPressed();
        }
        else if (QLineEdit *lineEditItem = dynamic_cast<QLineEdit*>(item)) {
            lineEditItem->setText(QString::number(oldVal));
            emit lineEditItem->returnPressed();
        }
        else if (QDoubleSlider *sliderItem = dynamic_cast<QDoubleSlider*>(item)) {
            
            sliderItem->setValue(oldVal * 100.0);
        }
        
        canRedo = true;
        //canUndo = true;
    }
    void redo() Q_DECL_OVERRIDE {
        if(!canRedo) return;
        qDebug() << "REDO to" << newVal;
        qDebug() << "is Item null?" << item;
        
        if (QSpinBox *boxItem = dynamic_cast<QSpinBox*>(item) ) {
            boxItem->setValue(newVal);
            // emit boxItem->returnPressed();
        }
        else if (QDoubleSpinBox *boxItem = dynamic_cast<QDoubleSpinBox*>(item)) {
            
            boxItem->setValue(newVal);
            // emit boxItem->returnPressed();
        }
        if (CustomLineEdit *lineEditItem = dynamic_cast<CustomLineEdit*>(item)) {
            lineEditItem->setText(QString::number(newVal));
            emit lineEditItem->returnPressed();
        }
        else if (QLineEdit *lineEditItem = dynamic_cast<QLineEdit*>(item)) {
            lineEditItem->setText(QString::number(newVal));
            emit lineEditItem->returnPressed();
        }
        else if (QDoubleSlider *sliderItem = dynamic_cast<QDoubleSlider*>(item)) {
            sliderItem->setValue(newVal * 100.0);
        }
    }
    
    
private:
    QObject *item;
    
    double oldVal, newVal;
    bool canUndo, canRedo;
    
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
    
    QMessageBox *infoPopUp;
    
    
    void setSnapShotWindow(HistoryDisplay* window);
    void closing();
    
signals:
    void imageActionStatus(bool status);
    //    void undoEnabled();
    //    void undoDisabled();
    //    void redoEnabled();
    //    void redoDisabled();
    
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
    void changeOverflowColor(const QColor &color) { currColorWheel->changeOverflowColor(color); updatePreviewDisplay(); }
    
    void exportImageFunction() { imageDimensionsPopUp->show(); }
    void cancelImageExport() { imageDimensionsPopUp->hide(); }
    void startImageExport();
    
    void resetFunction();
    void loadFromSettings();
    void saveCurrWorkspace();
    void saveCurrWorkspaceAs();
   
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
    void finishShifting(const QPoint &point);
    
    void changeAspectRatio();
    
    void handleUndo();
    void handleRedo();
    void createUndoAction(QObject *item, double oldVal, double newVal);
    
    void showInfo() { infoPopUp->show(); }

    
protected:
    void mousePressEvent(QMouseEvent *event);
    //void keyPressEvent(QKeyEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);
    
private:
    QString genLabel(const char * in);
    QString getCurrSettings(const HistoryItem &item);
    QString saveSettings(const QString &fileName, const int &actionFlag);
    
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
    void initShortcuts();
    void connectAllSignals();
    void removeTerm(int row);
    void removeTableTerm(int row);
    void refreshLabels();
    void updatePreviewDisplay();
    void errorHandler(const int &flag);
    void refreshTableTerms();
    void refreshMainWindowTerms();
    void updateAspectRatio();
    bool eventFilter(QObject* object,QEvent* event);
    
    
    //main data structures
    QVector<AbstractFunction *> functionVector;
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    Settings *settings;
    Port *previewDisplayPort, *imageExportPort, *aspectRatioPreviewDisplayPort;
    
    //operational variables
    int previewWidth, previewHeight, previewSize;       //preview display size
    double aspectRatio;
    int numTerms;
    int oldM, oldN;
    int oldXShift, oldYShift;
    QVector<int> oldMTable;
    QVector<int> oldNTable;
    QVector<double> oldATable;
    QVector<double> oldRTable;
    unsigned int termIndex;     //currently editing term
    int coeffFlag;      //mapping variable for polar plane
    bool newUpdate;     //guard variable for preview update
    bool newAction;     //new action performed, not because of undo/redo 
    bool heightChanged;
    bool widthChanged;
    bool errPrint;
    
    //I/O-related variables    
    QString saveloadPath;
    QString currFileName;
    QString imageSetPath;
    QString openImageName;
    QString snapshotFolderPath;
    
    //mouse-related variables
    bool mouseMoving;
    QPoint prevMousePos;
};

#endif // Interface_H
