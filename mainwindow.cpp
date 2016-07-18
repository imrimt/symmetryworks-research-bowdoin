/****************************************************************************

****************************************************************************/

#include "mainwindow.h"

MainWindow::MainWindow()
{
    
    currInterface = new Interface(this);
  
    // qDebug() << "setting " << this->currInterface << " to " << currInterface;

    QHBoxLayout *centerLayout = new QHBoxLayout();
    QWidget *centerWidget = new QWidget();
    centerLayout->addWidget(this->currInterface);
    centerWidget->setLayout(centerLayout);

    setCentralWidget(centerWidget);
    
    
    // QWidget *topFiller = new QWidget;
    // topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
    //                           "invoke a context menu</i>"));
    // infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    // infoLabel->setAlignment(Qt::AlignCenter);

    // QWidget *bottomFiller = new QWidget;
    // bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    createActions();
    createMenus();
    createDockWindows();

    // QString message = tr("A context menu is available by right-clicking");
    // statusBar()->showMessage(message);
    
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) * 0.3;
    int y = (screenGeometry.height()-this->height()) * 0.15;
    this->move(x, y);
                               
    setWindowTitle(tr("Wallpaper Generation"));
    resize(this->currInterface->width() * 1.05, this->currInterface->height() * 1.05);
}

MainWindow::~MainWindow()
{
    delete currInterface;
}

// #ifndef QT_NO_CONTEXTMENU
// void MainWindow::contextMenuEvent(QContextMenuEvent *event)
// {
//     QMenu menu(this);
//     menu.addAction(cutAct);
//     menu.addAction(copyAct);
//     menu.addAction(pasteAct);
//     menu.exec(event->globalPos());
// }

// #endif // QT_NO_CONTEXTMENU

void MainWindow::createActions()
{
    // newAct = new QAction(tr("&New"), this);
    // newAct->setShortcuts(QKeySequence::New);
    // newAct->setStatusTip(tr("Create a new file"));
    // connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    loadAct = new QAction(tr("Load..."), this);
    loadAct->setShortcuts(QKeySequence::Open);
    loadAct->setStatusTip(tr("Load an existing file"));
    connect(loadAct, SIGNAL(triggered()), currInterface, SLOT(loadFromSettings()));

    saveAct = new QAction(tr("Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the setting to disk"));
    connect(saveAct, SIGNAL(triggered()), currInterface, SLOT(saveCurrSettings()));

    // printAct = new QAction(tr("&Print..."), this);
    // printAct->setShortcuts(QKeySequence::Print);
    // printAct->setStatusTip(tr("Print the document"));
    // connect(printAct, &QAction::triggered, this, &MainWindow::print);

    exportImageAct = new QAction(tr("Export Image"), this);
    // exportImageAct->setShortCuts()
    exportImageAct->setStatusTip(tr("Export to an image file"));
    connect(exportImageAct, SIGNAL(triggered()), currInterface, SLOT(exportImageFunction()));

    // exitAct = new QAction(tr("Exit"), this);
    // exitAct->setShortcuts(QKeySequence::Quit);
    // exitAct->setStatusTip(tr("Exit the application"));
    // connect(exitAct, &QAction::triggered, this, &QWidget::close);

    enlargePreviewAct = new QAction(tr("Enlarge Preview"), this);
    enlargePreviewAct->setStatusTip(tr("Enlarge Preview Display by 25 %%"));
    connect(enlargePreviewAct, SIGNAL(triggered()), currInterface, SLOT(previewDisplayEnlarge()));

    shrinkPreviewAct = new QAction(tr("Shrink Preview"), this);
    shrinkPreviewAct->setStatusTip(tr("Shrink Preview Display by 25 %%"));
    connect(shrinkPreviewAct, SIGNAL(triggered()), currInterface, SLOT(previewDisplayShrink()));

    resetPreviewAct = new QAction(tr("Reset Preview"), this);
    resetPreviewAct->setStatusTip(tr("Reset Preview Display Size"));
    connect(resetPreviewAct, SIGNAL(triggered()), currInterface, SLOT(previewDisplayResetSize()));

    // undoAct = new QAction(tr("&Undo"), this);
    // undoAct->setShortcuts(QKeySequence::Undo);
    // undoAct->setStatusTip(tr("Undo the last operation"));
    // connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    // redoAct = new QAction(tr("&Redo"), this);
    // redoAct->setShortcuts(QKeySequence::Redo);
    // redoAct->setStatusTip(tr("Redo the last operation"));
    // connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    // cutAct = new QAction(tr("Cu&t"), this);
    // cutAct->setShortcuts(QKeySequence::Cut);
    // cutAct->setStatusTip(tr("Cut the current selection's contents to the "
    //                         "clipboard"));
    // connect(cutAct, &QAction::triggered, this, &MainWindow::cut);

    // copyAct = new QAction(tr("&Copy"), this);
    // copyAct->setShortcuts(QKeySequence::Copy);
    // copyAct->setStatusTip(tr("Copy the current selection's contents to the "
    //                          "clipboard"));
    // connect(copyAct, &QAction::triggered, this, &MainWindow::copy);

    // pasteAct = new QAction(tr("&Paste"), this);
    // pasteAct->setShortcuts(QKeySequence::Paste);
    // pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
    //                           "selection"));
    // connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);

    // boldAct = new QAction(tr("&Bold"), this);
    // boldAct->setCheckable(true);
    // boldAct->setShortcut(QKeySequence::Bold);
    // boldAct->setStatusTip(tr("Make the text bold"));
    // connect(boldAct, &QAction::triggered, this, &MainWindow::bold);

    // QFont boldFont = boldAct->font();
    // boldFont.setBold(true);
    // boldAct->setFont(boldFont);

    // italicAct = new QAction(tr("&Italic"), this);
    // italicAct->setCheckable(true);
    // italicAct->setShortcut(QKeySequence::Italic);
    // italicAct->setStatusTip(tr("Make the text italic"));
    // connect(italicAct, &QAction::triggered, this, &MainWindow::italic);

    // QFont italicFont = italicAct->font();
    // italicFont.setItalic(true);
    // italicAct->setFont(italicFont);

    // setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    // setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
    //                                    "paragraph"));
    // connect(setLineSpacingAct, &QAction::triggered, this, &MainWindow::setLineSpacing);

    // setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    // setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    // connect(setParagraphSpacingAct, &QAction::triggered,
    //         this, &MainWindow::setParagraphSpacing);

    // aboutAct = new QAction(tr("&About"), this);
    // aboutAct->setStatusTip(tr("Show the application's About box"));
    // connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    // aboutQtAct = new QAction(tr("About &Qt"), this);
    // aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    // connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    // connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);

    // leftAlignAct = new QAction(tr("&Left Align"), this);
    // leftAlignAct->setCheckable(true);
    // leftAlignAct->setShortcut(tr("Ctrl+L"));
    // leftAlignAct->setStatusTip(tr("Left align the selected text"));
    // connect(leftAlignAct, &QAction::triggered, this, &MainWindow::leftAlign);

    // rightAlignAct = new QAction(tr("&Right Align"), this);
    // rightAlignAct->setCheckable(true);
    // rightAlignAct->setShortcut(tr("Ctrl+R"));
    // rightAlignAct->setStatusTip(tr("Right align the selected text"));
    // connect(rightAlignAct, &QAction::triggered, this, &MainWindow::rightAlign);

    // justifyAct = new QAction(tr("&Justify"), this);
    // justifyAct->setCheckable(true);
    // justifyAct->setShortcut(tr("Ctrl+J"));
    // justifyAct->setStatusTip(tr("Justify the selected text"));
    // connect(justifyAct, &QAction::triggered, this, &MainWindow::justify);

    // centerAct = new QAction(tr("&Center"), this);
    // centerAct->setCheckable(true);
    // centerAct->setShortcut(tr("Ctrl+E"));
    // centerAct->setStatusTip(tr("Center the selected text"));
    // connect(centerAct, &QAction::triggered, this, &MainWindow::center);

    // alignmentGroup = new QActionGroup(this);
    // alignmentGroup->addAction(leftAlignAct);
    // alignmentGroup->addAction(rightAlignAct);
    // alignmentGroup->addAction(justifyAct);
    // alignmentGroup->addAction(centerAct);
    // leftAlignAct->setChecked(true);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    // fileMenu->addAction(newAct);
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);
    // fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    // fileMenu->addAction(exitAct);

    // editMenu = menuBar()->addMenu(tr("&Edit"));
    // editMenu->addAction(undoAct);
    // editMenu->addAction(redoAct);
    // editMenu->addSeparator();
    // editMenu->addAction(cutAct);
    // editMenu->addAction(copyAct);
    // editMenu->addAction(pasteAct);
    // editMenu->addSeparator();

    // helpMenu = menuBar()->addMenu(tr("&Help"));
    // helpMenu->addAction(aboutAct);
    // helpMenu->addAction(aboutQtAct);

    viewMenu = menuBar()->addMenu(tr("View"));
    viewMenu->addAction(enlargePreviewAct);
    viewMenu->addAction(shrinkPreviewAct);
    viewMenu->addAction(resetPreviewAct);
}

void MainWindow::createDockWindows() 
{
    rightDock = new QDockWidget(tr("Snapshots"), this);
    rightDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    // rightDock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // leftDock = new QDockWidget(tr("Pattern Previews"), this);
    // leftDock->setFeatures(QDockWidget::DockWidgetClosable);

    snapShotWindow = new HistoryDisplay(this);
    // functionIconsWindow = new QWidget(this, Qt::Window);

    currInterface->setSnapShotWindow(snapShotWindow);
    // currInterface->setFunctionIconsWindow(functionIconsWindow);

    rightDock->setWidget(snapShotWindow->viewHistoryWidget);
    // leftDock->setWidget(functionIconsWindow);

    // leftDock->setFixedSize(500, 600);

    // leftDock->hide();

    addDockWidget(Qt::RightDockWidgetArea, rightDock);
    // addDockWidget(Qt::LeftDockWidgetArea, leftDock);
}


//void MainWindow::moveEvent(QMoveEvent* event)
//{
//    const QPoint global = this->mapToGlobal(rect().center());
//    
//    QList<QWidget *> children = this->findChildren<QWidget *>();
//    
//    QList<QWidget *>::iterator i = children.begin();
//    
//    while (i != children.end()) {
//        if((*i)->isWindow()) {
//            (*i)->move(global.x() - (*i)->width() / 2, global.y() - (*i)->height() / 2);
//        }
//        ++i;
//       
//        
//    }
//    
//}
