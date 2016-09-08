//
//  historydisplay.cpp
//
//
//  Created by Bridget E. Went on 6/30/16.
//
//

#include "historydisplay.h"


// CONSTRUCTOR
HistoryDisplay::HistoryDisplay(QObject *parent) : QObject(parent)
{
    
    // create layout elements
    
    viewHistoryWidget = new QWidget(static_cast<QWidget *>(parent), Qt::Drawer);
    viewHistoryWidget->setWindowTitle(tr("Saved Snapshots"));
    
    viewHistoryBox = new QGroupBox(viewHistoryWidget);
    viewHistoryBoxOverallLayout = new QVBoxLayout(viewHistoryWidget);
    
    clearHistoryButton = new QPushButton(tr("Remove All"), viewHistoryBox);
    viewHistoryBoxLayout = new QVBoxLayout(viewHistoryBox);
    
    noItemLabel = new QLabel(tr("<i> (no snapshots to show) </i>"), viewHistoryWidget);
    noItemLabel->setFixedWidth(200);
    noItemLabel->setAlignment(Qt::AlignCenter);
    
    //initial layout
    viewHistoryBoxOverallLayout->addWidget(clearHistoryButton);
    viewHistoryBoxOverallLayout->addWidget(noItemLabel);
    viewHistoryBoxOverallLayout->addWidget(viewHistoryBox);
    viewHistoryBoxOverallLayout->addStretch();
    
    //initial set up
    clearHistoryButton->setEnabled(false);
    viewHistoryBox->hide();
    
    // connect signals
    viewMapper = new QSignalMapper(this);
    removeMapper = new QSignalMapper(this);
    
    connect(clearHistoryButton, SIGNAL(clicked()), this, SLOT(clearAllHistory()));
    connect(removeMapper, SIGNAL(mapped(QObject*)), this, SLOT(removePreview(QObject*)));
    
}


// add an instance of the function to the running history
void HistoryDisplay::addToHistory(const QDateTime &savedTime, const QString &filePathName, AbstractFunction *function, ColorWheel *colorwheel, Settings *settings)
{
    
    if (viewHistoryWidget->isHidden()) viewHistoryWidget->show();
    
    HistoryItem *item = new HistoryItem();
    
    QVBoxLayout *historyItemsWithLabelLayout = new QVBoxLayout();
    QHBoxLayout *historyItemsLayout = new QHBoxLayout();
    QVBoxLayout *historyItemsButtonsLayout = new QVBoxLayout();
    Display *d = new Display(HISTORY_ITEM_SIZE, HISTORY_ITEM_SIZE, viewHistoryBox);
    QPushButton *viewButton = new QPushButton(tr("Restore"), viewHistoryBox);
    QPushButton *removeButton = new QPushButton(tr("Remove"), viewHistoryBox);
    QLabel *timeStampLabel = new QLabel(viewHistoryBox);
    
    QString newFile = savedTime.toString("MM.dd.yyyy.hh.mm.ss.zzz.t").append(".wpr");
    QString savedTimeLabel = "Saved: " + savedTime.toString("MM/dd/yyyy") + " at " + savedTime.toString("hh:mm:ss");
    timeStampLabel->setText(savedTimeLabel);
    
    historyItemsLayout->addWidget(d);
    historyItemsButtonsLayout->addWidget(viewButton);
    historyItemsButtonsLayout->addWidget(removeButton);
    historyItemsLayout->addLayout(historyItemsButtonsLayout);
    historyItemsWithLabelLayout->addLayout(historyItemsLayout);
    historyItemsWithLabelLayout->addWidget(timeStampLabel);
    
    viewHistoryBoxLayout->insertLayout(0, historyItemsWithLabelLayout);
    
    // saving all values to history item object
    item->preview = d;
    item->savedTime = savedTime;
    item->layoutWithLabelItem = historyItemsWithLabelLayout;
    item->layoutItem = historyItemsLayout;
    item->buttonLayoutItem = historyItemsButtonsLayout;
    item->viewButton = viewButton;
    item->removeButton = removeButton;
    item->labelItem = timeStampLabel;
    item->filePathName = filePathName;
    
    AbstractFunction *currFunction = function;
    ColorWheel *currColorWheel = colorwheel;
    Settings *clonedSettings = settings;
    
    Port *historyDisplayPort = new Port(currFunction, currColorWheel, item->preview->getWidth(), item->preview->getHeight(), clonedSettings);
    historyDisplayPort->paintHistoryIcon(item);
    
    // connect and map signals
    connect(viewButton, SIGNAL(clicked()), viewMapper, SLOT(map()));
    connect(removeButton, SIGNAL(clicked()), removeMapper, SLOT(map()));
    
    viewMapper->setMapping(viewButton, item->filePathName);
    removeMapper->setMapping(removeButton, item);
    
    // map the timestamp to the history item and port
    historyItemsMap.insert(savedTime, item);
    historyPortsMap.insert(savedTime, historyDisplayPort);
    
    
}

// remove all elements of the history item and its UI
void HistoryDisplay::removePreview(QObject *item)
{
    HistoryItem *historyItemToRemove = qobject_cast<HistoryItem*>(item);
    
    historyItemToRemove->buttonLayoutItem->removeWidget(historyItemToRemove->viewButton);
    delete historyItemToRemove->viewButton;
    
    historyItemToRemove->buttonLayoutItem->removeWidget(historyItemToRemove->removeButton);
    delete historyItemToRemove->removeButton;
    
    historyItemToRemove->layoutItem->removeItem(historyItemToRemove->buttonLayoutItem);
    delete historyItemToRemove->buttonLayoutItem;
    
    historyItemToRemove->layoutItem->removeWidget(historyItemToRemove->preview);
    delete historyItemToRemove->preview;
    
    historyItemToRemove->layoutWithLabelItem->removeItem(historyItemToRemove->layoutItem);
    delete historyItemToRemove->layoutItem;
    
    historyItemToRemove->layoutWithLabelItem->removeWidget(historyItemToRemove->labelItem);
    delete historyItemToRemove->labelItem;
    
    viewHistoryBoxLayout->removeItem(historyItemToRemove->layoutWithLabelItem);
    delete historyItemToRemove->layoutWithLabelItem;
    
    (*(historyPortsMap.find(historyItemToRemove->savedTime)))->deleteMembers();
    
    delete *(historyPortsMap.find(historyItemToRemove->savedTime));
    historyPortsMap.erase(historyPortsMap.find(historyItemToRemove->savedTime));
    
    historyItemsMap.erase(historyItemsMap.find(historyItemToRemove->savedTime));
    
    QFile::remove(historyItemToRemove->filePathName);
    
    //delete historyItemToRemove;
    
    if (historyItemsMap.empty()) {
        // this->hide();
        clearHistoryButton->setEnabled(false);
        noItemLabel->show();
        viewHistoryBox->hide();
    }
    
}


// remove all elements from the history
void HistoryDisplay::clearAllHistory()
{
    while (!historyItemsMap.empty()) {
        removePreview(historyItemsMap.begin().value());
    }
    
    
}


// called from Interface to add to history if we haven't reached the maximum number of history items
void HistoryDisplay::triggerAddToHistory(const QDateTime &savedTime, const QString &filePathName, AbstractFunction *function, ColorWheel *colorwheel, Settings *settings)
{
    
    if (noItemLabel->isVisible()) {
        clearHistoryButton->setEnabled(true);
        viewHistoryBox->show();
        noItemLabel->hide();
    }
    
    if (historyItemsMap.size() < MAX_HISTORY_ITEMS) {
        addToHistory(savedTime, filePathName, function, colorwheel, settings);
    } else {
        removePreview(*(historyItemsMap.begin()));
        addToHistory(savedTime, filePathName, function, colorwheel, settings);
    }
    
}

void HistoryDisplay::show()
{
    viewHistoryWidget->show();
    viewHistoryBox->show();
}


void HistoryDisplay::hide()
{
    
    viewHistoryWidget->hide();
    viewHistoryBox->hide();
    
}




