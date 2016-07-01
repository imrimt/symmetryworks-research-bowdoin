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
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QSplineSeries>
#include <QLineSeries>
#include <QScatterSeries>
#include <QMouseEvent>
#include <QProgressBar>
#include <QTableWidget>
#include <QHeaderView>

#include "historydisplay.h"
#include "port.h"

using namespace QtCharts;

//const int MAX_HISTORY_ITEMS = 5;
const int LOCAL_FLAG = 0;
const int GLOBAL_FLAG = 1;
//const int HISTORY_ITEM_SIZE = 60;

const unsigned int INVALID_FILE_ERROR = 0;

class CustomSpinBox : public QSpinBox
{
  public:
    CustomSpinBox(QWidget *parent = 0) : QSpinBox(parent) { }

  protected:
    void keyPressEvent(QKeyEvent *event) {event->ignore();}

};


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
        label->setVisible(visible);
        
        layout = new QHBoxLayout();
        layout->addWidget(label);
        layout->addWidget(progBar);
        layout->addWidget(percentLabel);
        
        this->port = port;
        this->visible = visible;
        
    }
    
    QProgressBar *progBar;
    QLabel *label;
    QLabel *percentLabel;
    QHBoxLayout *layout;
    
private:
    bool visible;
    Port *port;
    
protected:
    void setValue(int val)
    {
        progBar->setValue(val);
        percentLabel->setText(progBar->text());
    }
        
public slots:
    void update(const int &numThreadsFinished)
    {
        if (!visible) { progBar->setVisible(true); label->setVisible(true); }
        double percentComplete = ((double)(numThreadsFinished)/(double)(port->getControllerObject()->getNumThreadsActive()) * 100.0);
        this->setValue(percentComplete);
        
    }
    
};

class CoeffPlaneView : public QChartView {
    Q_OBJECT
  public:
    CoeffPlaneView(QChart *chart, QScatterSeries *coordinateSeries) : QChartView(chart) { this->chart = chart; this->coordinateSeries = coordinateSeries; setMouseTracking(false);}

  protected:
    void mouseReleaseEvent(QMouseEvent *event) 
    {
        if(event->button() == Qt::LeftButton)
        {
            // qDebug() << "Clicked on: " << event->pos();
            coordinateSeries->replace(0, chart->mapToValue(event->pos(), coordinateSeries));
            mouseMoving = false;   
        }     
    };

    void mousePressEvent(QMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton)
        {
            // qDebug() << "Clicked on: " << event->pos();
            coordinateSeries->replace(0, chart->mapToValue(event->pos(), coordinateSeries));
            mouseMoving = true;   
        } 
    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        // qDebug() << "in move event " << event->pos();;
        if(mouseMoving)
        {
            // qDebug() << "Clicked on: " << event->pos();
            coordinateSeries->replace(0, chart->mapToValue(event->pos(), coordinateSeries));   
        } 
    };
    
  private:
    QScatterSeries* coordinateSeries;
    QChart *chart;
    bool mouseMoving;
};

class interface : public QWidget
{
    Q_OBJECT
public:
    explicit interface(QWidget *parent = 0);
    
    // ORGANIZATIONAL ELEMENTS
    QWidget *functionConstantsWidget;
    QVBoxLayout *interfaceLayout;
    QWidget *displayWidget;
    QHBoxLayout *topbarLayout;
    QVBoxLayout *leftbarLayout;
    QGroupBox *imagePropsBox;
    QWidget *toggleViewWidget;
    QWidget *patternTypeWidget;
    //QWidget *viewHistoryWidget;
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
    QPushButton *toggleViewButton;
    QVBoxLayout *toggleViewLayout;
    
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
    // QLabel *nValueLabel;
    // QLabel *mValueLabel;
    QLabel *aValueLabel;
    QLabel *rValueLabel;
    QLabel *scaleALabel;
    QLabel *scaleRLabel;

    QSpinBox *nEdit;
    QSpinBox *mEdit;

    QDoubleSlider *aEdit;
    QDoubleSlider *rEdit;
    QPushButton *coeffPlaneEdit;
    
    QLabel *globalConsantsLabel;

    QLineEdit *scaleAEdit;
    QLineEdit *scaleREdit;
    QPushButton *scalePlaneEdit;


    QGroupBox *functionConstantsBox;
    
    QVBoxLayout *functionConstantsBoxLayout;
    QVBoxLayout *functionConstantsWidgetLayout;
    QHBoxLayout *functionConstantsScalingTerms;
    QHBoxLayout *functionConstantsTerm1;
    QVBoxLayout *functionConstantsPairs;
    QHBoxLayout *functionConstantsFreqs;
    QHBoxLayout *functionConstantsCoeffs;
    
    
//    QHBoxLayout *functionConstantsBoxLayoutLower;
//    QVBoxLayout *functionConstantsBoxLayoutStack;
//    QHBoxLayout *functionConstantsBoxLayoutFirstLevel;
//    QHBoxLayout *functionConstantsBoxLayoutSecondLevel;
//    QHBoxLayout *functionConstantsBoxLayoutThirdLevel;
//    QSpacerItem *espacer1;
//    QSpacerItem *espacer2;
//    QSpacerItem *espacer3;
//    QSpacerItem *espacer4;
//    QSpacerItem *espacer5;
//    QSpacerItem *espacer6;
//    QSpacerItem *espacer7;
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
    QLabel *colorwheelLabel;
    QLabel *functionLabel;
    QLabel *numTermsLabel;
    QLabel *freqpairLabel;
    QLabel *coeffLabel;
    CustomSpinBox *numTermsEdit;
    QSpacerItem *gspacer1;
    QSpacerItem *gspacer2;
    QSpacerItem *gspacer3;
    QSpacerItem *gspacer4;
    QComboBox *colorwheelSel;
    QComboBox *functionSel;
    QPushButton *setLoadedImage;
//    
//    // viewHistoryBox SUBELEMENTS
//    QGroupBox *viewHistoryBox;
//    QVBoxLayout *viewHistoryBoxLayout;
//    QVBoxLayout *viewHistoryBoxOverallLayout;
//    QPushButton *clearHistoryButton;
//    QSignalMapper *viewMapper;
//    QSignalMapper *removeMapper;
    HistoryDisplay *historyDisplay;
    
    // imagePropsBox SUBELEMENTS
    QVBoxLayout *imagePropsBoxStack;
    QVBoxLayout *imagePropsEditStack;
    QHBoxLayout *imagePropsBoxLayout;
    QHBoxLayout *savePushLayout;
    QVBoxLayout *imagePropsBoxOverallLayout;
    QLabel *outheightLabel;
    QLabel *outwidthLabel;
    QLabel *XCornerLabel;
    QLabel *YCornerLabel;
    QLabel *worldwidthLabel;
    QLabel *worldheightLabel;
    QLineEdit
     *outheightEdit;
    QLineEdit *outwidthEdit;
    QLineEdit *XCornerEdit;
    QLineEdit *YCornerEdit;
    QLineEdit *worldwidthEdit;
    QLineEdit *worldheightEdit;
    QSpacerItem *pspacer1;
    QSpacerItem *pspacer2;
    QSpacerItem *pspacer3;
    QSpacerItem *pspacer4;
    QSpacerItem *pspacer5;

    // PolarPlane SUBELEMENTS
    QWidget *coeffPlanePopUp;
    QGroupBox *polarCoordinatesBox;
    QHBoxLayout *coeffPlanePopUpLayout;
    QVBoxLayout *polarCoordinatesLayout;
    QHBoxLayout *actionButtonLayout;
    QHBoxLayout *zoomButtonLayout;
    QVBoxLayout *polarPlaneWithZoomLayout;
    CoeffPlaneView *graphDisplay;
    QChart *graph;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QLabel *radiusLabel;
    QLabel *angleLabel;
    QLineEdit *radiusEdit;
    QLineEdit *angleEdit;
    QPushButton *confirmButton;
    QPushButton *resetButton;
    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;
    QSpacerItem *planeSpacer1;
    QSpacerItem *planeSpacer2;
    QLineSeries *series;
    QLineSeries *series2;
    QScatterSeries *coordinateSeries;
    QLineSeries *xSeries;
    QLineSeries *ySeries;
    QSignalMapper *coeffMapper;

    // DISP SUBELEMENTS
    QPushButton *updatePreview;
    QPushButton *exportImage;
    QPushButton *resetImage;
    Display *disp;
    QVBoxLayout *dispLayout;
    QHBoxLayout *buttonLayout;

    // SHORTCUTS
    QShortcut *updatePreviewShortcut;
    
//    QProgressBar *progressBarDisp;
//    QLabel *progressBarDispLabel;
//    QHBoxLayout *progressBarDispLayout;
//    
//    QProgressBar *progressBarDisp;
//    QLabel *progressBarDispLabel;
//    QHBoxLayout *progressBarDispLayout;
    ProgressBar *displayProgressBar;
    ProgressBar *exportProgressBar;
    
    

    // void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void toggleViewMode();
    void updateCurrTerm(int i);
    //void changeMaxTerms(int i);
    void colorWheelChanged(int index);
    void setImagePushed();
    void changeFunction(int index);
    void changeWorldWidth(const QString &val);
    void changeWorldHeight(const QString &val);
    void changeXCorner(const QString &val);
    void changeYCorner(const QString &val);
    void changeOWidth(const QString &val);
    void changeOHeight(const QString &val);
    void changeN(int val);
    void changeM(int val);
    void changeR(double val);
    void changeA(double val);
    void changeScaleR(const QString &val);
    void changeScaleA(const QString &val);
    void saveImageStart();
    void resetImageFunction();
//    void removePreview(QObject *item);
    void loadFromSettings();
    void saveCurrSettings();
    void previewDisplayEnlarge() {disp->enlarge();}
    void previewDisplayShrink() {disp->shrink();}
    void previewDisplayResetSize() {disp->resetSize();}
    void updateSavePreview();
//    void clearAllHistory();
    
    void termViewPopUp();
    void addTerm();
    void updateTermTable(QObject *cell);
    void termViewCellClicked(int row, int col);
    
    void showPlanePopUp(int flag);
    void updatePolarCoordinatesWithIndex(const int &index);
    void updatePolarCoordinates();
    void polarPlaneZoomIn();
    void polarPlaneZoomOut();

    void setPolarCoordinates();
    void resetPolarCoordinates();
    
    QString loadSettings(const QString &fileName);
  //  void updateProgressBar(const int &numThreadsFinished);
    void popUpImageExportFinished(const QString &filePath);
    
private:    
    QString genLabel(const char * in);    
    QString getCurrSettings(const HistoryItem &item);
    QString saveSettings(const QString &fileName);

    // interface display and formatting functions
    void initInterfaceLayout();
    void initPreviewDisplay();
    void initFunctionConstants();
    void initPatternType();
  //  void initViewHistory();
    void initImageProps();
    void initCoeffPlane();
    void connectAllSignals();

    void removeTerm(int row);
    void refreshLabels();
    void updatePreviewDisplay();
//    void addToHistory();
    void errorHandler(const int &flag);
    void refreshTerms();
    void refreshMainWindowTerms();

    void updatePolarCoordinates(QPointF point);

    int numTerms;
    unsigned int termIndex; 
    unsigned int highestIndex;
    QString saveloadPath;
    AbstractFunction * currFunction;
    ColorWheel * currColorWheel;
    Port *previewDisplayPort, *imageExportPort;
    bool advancedMode;
    int coeffFlag;
    
    Settings *settings;

//    QMap<QDateTime, HistoryItem*> historyItemsMap;
//    QMap<QDateTime, Port*> historyPortsMap;
    
};

#endif // INTERFACE_H
