#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QtWidgets>
#include "Interface.h"

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
    QMenu *viewMenu;
    // QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    // QAction *newAct;
    QAction *loadAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exportImageAct;
    QAction *snapshotAct;
    QAction *resetImageAct;
    //QAction *clearSnapshotsAct;
    QAction *setOverflowColorAct;
    QAction *showImageDataGraphAct;
    // QAction *viewTermsTableAct;
    
    QAction *undoAct;
    QAction *redoAct;
    
    QShortcut *undoShortcut;
    
    // QAction *printAct;
    // QAction *exitAct;
    // QAction *undoAct;
    // QAction *redoAct;
    // QAction *cutAct;
    // QAction *copyAct;
    // QAction *pasteAct;
    // QAction *boldAct;
    // QAction *italicAct;
    // QAction *leftAlignAct;
    // QAction *rightAlignAct;
    // QAction *justifyAct;
    // QAction *centerAct;
    // QAction *setLineSpacingAct;
    // QAction *setParagraphSpacingAct;
    // QAction *aboutAct;
    // QAction *aboutQtAct;
    // QLabel *infoLabel;
    Interface *currInterface;
    HistoryDisplay *snapShotWindow;
    // QWidget *functionIconsWindow;
    
    private slots:
    void updateImageActionStatus(bool status) {
        setOverflowColorAct->setEnabled(status);
        showImageDataGraphAct->setEnabled(status);
    }
    //    void enableUndo() { undoAct->setEnabled(true); qDebug() << "success!"; }
    //    void enableRedo() { redoAct->setEnabled(true); }
    //     void disableUndo() { undoAct->setEnabled(false); }
    //     void disableRedo() { undoAct->setEnabled(false); }
};

#endif
