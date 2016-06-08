#ifndef INTERFACE_H
#define INTERFACE_H

#include <complex>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
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
    QLabel *nL;
    QLabel *mL;
    QLabel *aL;
    QLabel *rL;
    QLabel *scaleaL;
    QLabel *scalerL;
    // QLineEdit *nE;
    // QLineEdit *mE;
    // QLineEdit *aE;
    // QLineEdit *rE;
    QSpinBox *nE;
    QSpinBox *mE;
    QDoubleSpinBox *aE;
    QDoubleSpinBox *rE;

    QLineEdit *scaleaE;
    QLineEdit *scalerE;
    QSpinBox *currtermE;
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
    QLabel *colorwheelL;
    QLabel *functionL;
    QLabel *numtermsL;
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
    QLabel *outheightL;
    QLabel *outwidthL;
    QLabel *XCornerL;
    QLabel *YCornerL;
    QLabel *worldwidthL;
    QLabel *worldheightL;
    QLineEdit *outheightE;
    QLineEdit *outwidthE;
    QLineEdit *XCornerE;
    QLineEdit *YCornerE;
    QLineEdit *worldwidthE;
    QLineEdit *worldheightE;
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
    unsigned int termindex;              //internal index:  starts at 0
    QString genLabel(const char * in);
    void refreshLabels();
    QString saveloadPath;
    void refreshTerms();
    AbstractFunction * f;
    ColorWheel * c;
};

#endif // INTERFACE_H
