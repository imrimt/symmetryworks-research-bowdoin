#ifndef INTERFACE_H
#define INTERFACE_H

#include <complex>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
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

#include "functions.h"
#include "colorwheel.h"
#include "display.h"

class CustomSpinBox : public QSpinBox
{
  public:
    CustomSpinBox(QWidget *parent = 0) : QSpinBox(parent) { }

  protected:
    void keyPressEvent(QKeyEvent *event) {event->ignore();}

};

class interface : public QWidget
{
    Q_OBJECT
public:
    explicit interface(QWidget *parent = 0);

    // ORGANIZATIONAL ELEMENTS
    QGroupBox *editconst;
    QVBoxLayout *interfaceLayout;
    QWidget *displayWidget;
    QHBoxLayout *topbarLayout;
    QGroupBox *propsBox;
    QWidget *genBoxWidget;

    // INPUT VALIDATORS
    QDoubleValidator *doubleValidate;
    QDoubleValidator *posdoubleValidate;
    QIntValidator *intValidate;
    QIntValidator *posintValidate;
    QIntValidator *numtermsValidate;
    QIntValidator *dimValidate;

    // EDITCONST SUBELEMENTS
    QLabel *currtermL;
    QLabel *nLabel;
    QLabel *mLabel;
    QLabel *aLabel;
    QLabel *rLabel;
    QLabel *scaleaLabel;
    QLabel *scalerLabel;
    // QLineEdit *nE;
    // QLineEdit *mE;
    // QLineEdit *aE;
    // QLineEdit *rE;
    QSpinBox *nEdit;
    QSpinBox *mEdit;
    QDoubleSpinBox *aEdit;
    QDoubleSpinBox *rEdit;

    QLineEdit *scaleaE;
    QLineEdit *scalerE;
    CustomSpinBox *currtermE;
    QHBoxLayout *editconstLayout;
    QHBoxLayout *editconstLayoutLower;
    QVBoxLayout *editconstLayoutStack;
    QSpacerItem *espacer1;
    QSpacerItem *espacer2;
    QSpacerItem *espacer3;
    QSpacerItem *espacer4;
    QSpacerItem *espacer5;
    QSpacerItem *espacer6;
    QSpacerItem *espacer7;
    QSpacerItem *espacer8;
    QPushButton *loadButton;
    QPushButton *saveButton;

    // GENBOX SUBELEMENTS
    QGroupBox *genBox;
    QVBoxLayout *genBoxOverallLayout;
    QVBoxLayout *genBoxLayout;
    QVBoxLayout *functionLayout;
    QVBoxLayout *colorwheelLayout;
    QHBoxLayout *numtermsLayout;
    QLabel *colorwheelLabel;
    QLabel *functionLabel;
    QLabel *numtermsLabel;
    QLineEdit *numtermsE;
    QSpacerItem *gspacer1;
    QSpacerItem *gspacer2;
    QSpacerItem *gspacer3;
    QSpacerItem *gspacer4;
    QComboBox *colorwheelSel;
    QComboBox *functionSel;
    QPushButton *setLoadedImage;

    // PROPSBOX SUBELEMENTS
    QVBoxLayout *propslabelStack;
    QVBoxLayout *propseditStack;
    QHBoxLayout *propsBoxLayout;
    QHBoxLayout *savePushLayout;
    QVBoxLayout *propsBoxOverallLayout;
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
    QPushButton *saveImagePush;
    QSpacerItem *pspacer1;
    QSpacerItem *pspacer2;
    QSpacerItem *pspacer3;
    QSpacerItem *pspacer4;
    QSpacerItem *pspacer5;

    // DISP SUBELEMENTS
    QPushButton *updatePreview;
    Display *disp;
    QVBoxLayout *dispLayout;


private slots:
    void updateTerms(int i);
    void updatePreviewDisplay();
    void changeMaxTerms(const QString &val);
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
    void loadFunction();
    void saveFunction();

private:
    unsigned int termIndex;              //internal index:  starts at 0
    QString genLabel(const char * in);
    void refreshLabels();
    QString saveloadPath;
    void refreshTerms();
    AbstractFunction * currFunction;
    ColorWheel * currColorWheel;
};

#endif // INTERFACE_H
