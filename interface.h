#ifndef INTERFACE_H
#define INTERFACE_H

#include <complex>

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
#include <QDebug>
#include <QPushButton>
#include <QImage>
#include <QDir>
#include <QInputDialog>
#include <QFileDialog>
#include <QSignalMapper>
#include <QDateTime>

#include "functions.h"
#include "colorwheel.h"
#include "display.h"

#define MAX_HISTORY_ITEMS 5

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
        double doubleValue = value / 4.0;
        emit doubleValueChanged(doubleValue);
    }

};

class HistoryItem : public QObject
{
    Q_OBJECT
  public:
    HistoryItem(QObject *parent = 0) : QObject(parent) { }
    Display *preview;
    QStringList settings;
    QHBoxLayout *layoutItem;
    QVBoxLayout *buttonLayoutItem;
    QPushButton *viewButton;
    QPushButton *removeButton;
    QString filePathName;

    //is this data structure useful? 
    QDateTime savedTime;
};

class interface : public QWidget
{
    Q_OBJECT
public:
    explicit interface(QWidget *parent = 0);

    // ORGANIZATIONAL ELEMENTS
    QGroupBox *functionConstantsBox;
    QVBoxLayout *interfaceLayout;
    QWidget *displayWidget;
    QHBoxLayout *topbarLayout;
    QVBoxLayout *leftbarLayout;
    QGroupBox *imagePropsBox;
    QWidget *patternTypeWidget;
    QWidget *viewHistoryWidget;

    // INPUT VALIDATORS
    QDoubleValidator *doubleValidate;
    QDoubleValidator *posdoubleValidate;
    QIntValidator *intValidate;
    QIntValidator *posintValidate;
    QIntValidator *numtermsValidate;
    QIntValidator *dimValidate;

    // functionConstants SUBELEMENTS
    QLabel *currTermLabel;
    QLabel *nLabel;
    QLabel *mLabel;
    QLabel *aLabel;
    QLabel *rLabel;

    QLabel *nValueLabel;
    QLabel *mValueLabel;
    QLabel *aValueLabel;
    QLabel *rValueLabel;

    QLabel *scaleALabel;
    QLabel *scaleRLabel;
    // QLineEdit *nEdit;
    // QLineEdit *mEdit;
    // QLineEdit *aEdit;
    // QLineEdit *rEdit;

    // QSpinBox *nEdit;
    // QSpinBox *mEdit;
    // QDoubleSpinBox *aEdit;
    // QDoubleSpinBox *rEdit;

    QSlider *nEdit;
    QSlider *mEdit;
    QSlider *aEdit;
    QSlider *rEdit;

    QLineEdit *scaleAEdit;
    QLineEdit *scaleREdit;
    CustomSpinBox *currTermEdit;

    QHBoxLayout *functionConstantsBoxLayoutLower;
    QVBoxLayout *functionConstantsBoxLayoutStack;

    QHBoxLayout *functionConstantsBoxLayoutFirstLevel;
    QHBoxLayout *functionConstantsBoxLayoutSecondLevel;
    QHBoxLayout *functionConstantsBoxLayoutThirdLevel;

    QSpacerItem *espacer1;
    QSpacerItem *espacer2;
    QSpacerItem *espacer3;
    QSpacerItem *espacer4;
    QSpacerItem *espacer5;
    QSpacerItem *espacer6;
    QSpacerItem *espacer7;
    QPushButton *loadButton;
    QPushButton *saveButton;

    // patternTypeBox SUBELEMENTS
    QGroupBox *patternTypeBox;
    QVBoxLayout *patternTypeBoxOverallLayout;
    QVBoxLayout *patternTypeBoxLayout;
    QVBoxLayout *functionLayout;
    QVBoxLayout *colorwheelLayout;
    QHBoxLayout *numtermsLayout;
    QLabel *colorwheelLabel;
    QLabel *functionLabel;
    QLabel *numtermsLabel;
    QLabel *freqpairLabel;
    QLabel *coeffLabel;
    CustomSpinBox *numtermsEdit;
    QSpacerItem *gspacer1;
    QSpacerItem *gspacer2;
    QSpacerItem *gspacer3;
    QSpacerItem *gspacer4;
    QComboBox *colorwheelSel;
    QComboBox *functionSel;
    QPushButton *setLoadedImage;
    
    // viewHistoryBox SUBELEMENTS
    QGroupBox *viewHistoryBox;
    QVBoxLayout *viewHistoryBoxLayout;
    QVBoxLayout *viewHistoryBoxOverallLayout;
    QPushButton *clearHistoryButton;

    // std::vector<HistoryItem *> historyItems;

    QMap<QDateTime, HistoryItem*> historyItems;

    QSignalMapper *viewMapper;
    QSignalMapper *removeMapper;
    
    QHBoxLayout *historyItemsLayout;
    QVBoxLayout *historyItemButtonsLayout;

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
    QLineEdit *outheightEdit;
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

    // DISP SUBELEMENTS
    QPushButton *updatePreview;
    QPushButton *exportImage;
    QPushButton *resetImage;
    Display *disp;
    QVBoxLayout *dispLayout;
    QHBoxLayout *buttonLayout;


private slots:
    void updateTerms(int i);
    void changeMaxTerms(int i);
    void colorWheelChanged(int index);
    void setImagePushed();
    void changeFunction(int index);
    void changeWorldWidth(const QString &val);
    void changeWorldHeight(const QString &val);
    void changeXCorner(const QString &val);
    void changeYCorner(const QString &val);
    void changeOWidth(const QString &val);
    void changeOHeight(const QString &val);
    // void changeN(const QString &val);
    // void changeM(const QString &val);
    // void changeR(const QString &val);
    // void changeA(const QString &val);

    void changeN(int val);
    void changeM(int val);
    void changeR(double val);
    void changeA(double val);

    void changeScaleR(const QString &val);
    void changeScaleA(const QString &val);
    void saveImageStart();
    void resetImageFunction();

    void loadFromSettings();
    void saveCurrSettings();
    void updateSavePreview();
    void removeItem(QObject* item);
    void clearAllHistory();
    
    QString loadSettings(const QString &fileName);
    
private:
    unsigned int termIndex;              //internal index:  starts at 0
    int numHistoryItems = 0;
    QString genLabel(const char * in);
    void refreshLabels();
    void updatePreviewDisplay();
    void removePreview(HistoryItem *historyItemsLayout);
    QString saveloadPath;
    QString getCurrSettings(const HistoryItem &item);
    QString saveSettings(const QString &fileName);
    void addToHistory();    
    
    void refreshTerms();
    AbstractFunction * currFunction;
    ColorWheel * currColorWheel;
    
};

#endif // INTERFACE_H
