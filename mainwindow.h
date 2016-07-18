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

private:
    void createActions();
    void createMenus();
    void createDockWindows();

    QDockWidget *rightDock;
    // QDockWidget *leftDock;

    QMenu *fileMenu;
    // QMenu *editMenu;
    QMenu *viewMenu;
    // QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    // QAction *newAct;
    QAction *loadAct;
    QAction *saveAct;
    QAction *exportImageAct;
    QAction *enlargePreviewAct;
    QAction *shrinkPreviewAct;
    QAction *resetPreviewAct;
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

// private slots:
//     void showFunctionIcons() { leftDock->show(); } 
};

#endif
