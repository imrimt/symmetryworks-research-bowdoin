#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QtWidgets>
#include <QCloseEvent>
#include "interface.h"

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

const int SCREEN_INTERFACE_MARGIN = 50;
const double SCREEN_WIDTH_MARGIN = 0.3;
const double SCREEN_HEIGHT_MARGIN = 0.15;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow();
    ~MainWindow();

    void close() {currInterface->closing();}
    
protected:
    // #ifndef QT_NO_CONTEXTMENU
    //     void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
    // #endif // QT_NO_CONTEXTMENU
    //void keyPressEvent(QKeyEvent *event);
    
private:
    void createActions();
    void createMenus();
    void createDockWindows();
    
    QDockWidget *rightDock;
    // QDockWidget *leftDock;
    
    QMenu *fileMenu;
    QMenu *editMenu;
    //QMenu *viewMenu;
    QMenu *advancedMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    // QAction *newAct;
    QAction *loadAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exportImageAct;
    QAction *snapshotAct;
    QAction *resetImageAct;
    QAction *aboutAct;
    QAction *setOverflowColorAct;
    QAction *showImageDataGraphAct;
    QAction *undoAct;
    QAction *redoAct;
    QShortcut *undoShortcut;
    
    
    Interface *currInterface;
    HistoryDisplay *snapShotWindow;
    
    private slots:
    void updateImageActionStatus(bool status) {
        setOverflowColorAct->setEnabled(status);
        showImageDataGraphAct->setEnabled(status);
    }
    
};

#endif
