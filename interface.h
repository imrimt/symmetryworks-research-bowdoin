#ifndef INTERFACE_H
#define INTERFACE_H

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


// interface class
class interface : public QWidget
{
    Q_OBJECT
public:
    explicit interface(QWidget *parent = 0);
    
    // ORGANIZATIONAL ELEMENTS
    QWidget *functionConstantsWidget;
    QHBoxLayout *overallLayout;
    QVBoxLayout *interfaceLayout;
    QWidget *displayWidget;
    QHBoxLayout *topbarLayout;
    QVBoxLayout *leftbarLayout;
    QGroupBox *imagePropsBox;
    //QWidget *toggleViewWidget;
    QWidget *patternTypeWidget;
    QMessageBox *errorMessageBox;

    // INPUT VALIDATORS
    QDoubleValidator *doubleValidate;
    QDoubleValidator *posdoubleValidate;
    QDoubleValidator *angleValidate;
    QIntValidator *intValidate;
    QIntValidator *posintValidate;
    QIntValidator *numtermsValidate;
    QIntValidator *dimValidate;

    //modeToggle SUBELEMENTS
//    QPushButton *toggleViewButton;
//    QVBoxLayout *toggleViewLayout;
    
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
    QLabel *globalConstantsLabel;
    QLineEdit *scaleAEdit;
    QLineEdit *scaleREdit;
    QPushButton *scalePlaneEdit;
    
    QSignalMapper *polarPlaneMapper;

    QGroupBox *functionConstantsBox;
    
    QGridLayout *functionTermsGrid;

    QVBoxLayout *functionConstantsBoxLayout;
    QVBoxLayout *functionConstantsWidgetLayout;
    QHBoxLayout *functionConstantsScalingTerms;
    QHBoxLayout *numTermsLayout;
    QVBoxLayout *termsLayout;
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
    QVBoxLayout *functionLayout;
    QVBoxLayout *colorwheelLayout;
    QHBoxLayout *numtermsLayout;
    QHBoxLayout *colorButtonLayout;
    QVBoxLayout *globalConstantsLayout;
    QGridLayout *globalConstantsGrid;
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

    HistoryDisplay *historyDisplay;
    PolarPlane *polarPlane;
    
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
    QDoubleSlider *XShiftEdit;
    QDoubleSlider *YShiftEdit;
    QLineEdit *XShiftValueLabel;
    QLineEdit *YShiftValueLabel;
    
//    QLabel *outHeightLabel;
//    QLabel *outWidthLabel;
    
    QLabel *worldWidthLabel;
    QLabel *worldHeightLabel;
    
//    QLineEdit *worldWidthEdit;
//    QLineEdit *worldHeightEdit;
    QDoubleSlider *worldWidthEdit;
    QDoubleSlider *worldHeightEdit;
    QLineEdit *worldWidthValueLabel;
    QLineEdit *worldHeightValueLabel;
    
    QSpacerItem *pspacer1;
//    QSpacerItem *pspacer2;
//    QSpacerItem *pspacer3;
//    QSpacerItem *pspacer4;
//    QSpacerItem *pspacer5;

    // DISP SUBELEMENTS
    QPushButton *updatePreview;
    QPushButton *exportImage;
    QPushButton *resetImage;
    Display *disp;
    QVBoxLayout *dispLayout;
    QHBoxLayout *buttonLayout;

    // SHORTCUTS
    QShortcut *updatePreviewShortcut;
    
    
    // PROGRESS BARS
    ProgressBar *displayProgressBar;
    ProgressBar *exportProgressBar;
    
    // OUTPUT IMAGE DIM POP UP
    QWidget *settingsPopUp;
    QVBoxLayout *settingsPopUpLayout;
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
    void changeScaleR(const QString &val);
    void changeScaleA(const QString &val);

    void exportImageFunction() { settingsPopUp->show(); }
    void cancelImageExport() { settingsPopUp->hide(); }
    void startImageExport();
    
    void resetImageFunction();
    void loadFromSettings();
    void saveCurrSettings();
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
    //void hideFunctionicon(int index) { functionIconsWindow->hide(); }
    
private:    
    QString genLabel(const char * in);    
    QString getCurrSettings(const HistoryItem &item);
    QString saveSettings(const QString &fileName);

    // interface display and formatting functions
    void initInterfaceLayout();
    void initPreviewDisplay();
    void initFunctionConstants();
    void initPatternType();
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

    int numTerms;
    unsigned int termIndex; 
    
    QString saveloadPath;
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

#endif // INTERFACE_H
