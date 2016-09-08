/****************************************************************************
 
 Deploy on Mac: /Users/sngo/Qt/5.7/clang_64/bin/macdeployqt
 
 ****************************************************************************/

#include "mainwindow.h"

MainWindow::MainWindow()
{
    
    currInterface = new Interface(this);
    
    QHBoxLayout *centerLayout = new QHBoxLayout();
    QWidget *centerWidget = new QWidget();
    centerLayout->addWidget(this->currInterface);
    centerWidget->setLayout(centerLayout);
    
    setCentralWidget(centerWidget);
    
    connect(this->currInterface, SIGNAL(imageActionStatus(bool)), this, SLOT(updateImageActionStatus(bool)));
    
    // QWidget *topFiller = new QWidget;
    // topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
    //                           "invoke a context menu</i>"));
    // infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    // infoLabel->setAlignment(Qt::AlignCenter);
    
    // QWidget *bottomFiller = new QWidget;
    // bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // undoStack = new QUndoStack(this);
    
    createActions();
    createMenus();
    createDockWindows();
    
    
    // QString message = tr("A context menu is available by right-clicking");
    // statusBar()->showMessage(message);
    
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) * SCREEN_WIDTH_MARGIN;
    int y = (screenGeometry.height()-this->height()) * SCREEN_HEIGHT_MARGIN;
    this->move(x, y);
    
    //resize(QDesktopWidget().availableGeometry(this).size() * 0.5);
    
    if (qFabs(screenGeometry.height() - this->height()) < SCREEN_INTERFACE_MARGIN){
        QFont font;
        font.setFamily(font.defaultFamily());
        font.setPointSize(font.pointSize() - 2.0);
        currInterface->setFont(font);
    }
    
    setWindowTitle(tr("Wallpaper Generation"));
    
    //setFixedSize(sizeHint());
    
    
}

MainWindow::~MainWindow()
{
    delete currInterface;
}

void MainWindow::createActions()
{
    
    
    loadAct = new QAction(tr("Load Workspace..."), this);
    loadAct->setShortcuts(QKeySequence::Open);
    loadAct->setStatusTip(tr("Load an existing .wpr file"));
    connect(loadAct, SIGNAL(triggered()), currInterface, SLOT(loadFromSettings()));
    
    saveAct = new QAction(tr("Save Current Workspace"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the setting to disk"));
    connect(saveAct, SIGNAL(triggered()), currInterface, SLOT(saveCurrWorkspace()));
    
    saveAsAct = new QAction(tr("Save Current Workspace As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAct, SIGNAL(triggered()), currInterface, SLOT(saveCurrWorkspaceAs()));
    
    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setStatusTip(tr("Undo the most current action"));
    undoAct->setShortcut(QKeySequence::Undo);
    connect(undoAct, SIGNAL(triggered()), currInterface, SLOT(handleUndo()));
    
    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setStatusTip(tr("Redo the most current action"));
    redoAct->setShortcut(QKeySequence::Redo);
    connect(redoAct, SIGNAL(triggered()), currInterface, SLOT(handleRedo()));
    
    // printAct = new QAction(tr("&Print..."), this);
    // printAct->setShortcuts(QKeySequence::Print);
    // printAct->setStatusTip(tr("Print the document"));
    // connect(printAct, &QAction::triggered, this, &MainWindow::print);
    
    exportImageAct = new QAction(tr("Export Image..."), this);
    exportImageAct->setShortcut(QKeySequence("Ctrl+E"));
    exportImageAct->setStatusTip(tr("Export to an image file"));
    connect(exportImageAct, SIGNAL(triggered()), currInterface, SLOT(exportImageFunction()));
    
    snapshotAct = new QAction(tr("Snapshot Image"), this);
    snapshotAct->setShortcut(QKeySequence("Ctrl+D"));
    snapshotAct->setStatusTip(tr("Save current instance to snapshots"));
    connect(snapshotAct, SIGNAL(triggered()), currInterface, SLOT(snapshotFunction()));
    
    resetImageAct = new QAction(tr("Reset All Settings"), this);
    resetImageAct->setStatusTip(tr("Reset the current image to its default settings"));
    connect(resetImageAct, SIGNAL(triggered()), currInterface, SLOT(resetFunction()));
    
    
    setOverflowColorAct = new QAction(tr("Set Overflow Color..."), this);
    setOverflowColorAct->setStatusTip(tr("Set default color for overflow pixel on image"));
    connect(setOverflowColorAct, SIGNAL(triggered()), currInterface, SLOT(showOverflowColorPopUp()));
    setOverflowColorAct->setEnabled(false);
    
    showImageDataGraphAct = new QAction(tr("Show Image Data Points"), this);
    showImageDataGraphAct->setStatusTip(tr("Display a graph that shows points of image that are called up"));
    connect(showImageDataGraphAct, SIGNAL(triggered()), currInterface, SLOT(showImageDataGraph()));
    showImageDataGraphAct->setEnabled(false);
    
    aboutAct = new QAction(tr("About wallgen"), this);
    connect(aboutAct, SIGNAL(triggered()), currInterface, SLOT(showInfo()));
    
    
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(snapshotAct);
    fileMenu->addAction(exportImageAct);
    
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(resetImageAct);
   
    advancedMenu = menuBar()->addMenu(tr("&Advanced"));
    advancedMenu->addSeparator();
    advancedMenu->addAction(setOverflowColorAct);
    advancedMenu->addAction(showImageDataGraphAct);
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);
    
}

void MainWindow::createDockWindows()
{
    rightDock = new QDockWidget(tr("Snapshots"), this);
    rightDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    
    snapShotWindow = new HistoryDisplay(this);
    
    currInterface->setSnapShotWindow(snapShotWindow);
    
    rightDock->setWidget(snapShotWindow->viewHistoryWidget);
    
    addDockWidget(Qt::RightDockWidgetArea, rightDock);

}

