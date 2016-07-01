//
//  historydisplay.cpp
//  
//
//  Created by Bridget E. Went on 6/30/16.
//
//

#include "historydisplay.h"


HistoryDisplay::HistoryDisplay(AbstractFunction *currFunction, ColorWheel *currColorWheel, Settings *settings, QObject *parent) : QObject(parent)
{
  
    viewHistoryWidget = new QWidget(static_cast<QWidget *>(parent));
    viewHistoryBox = new QGroupBox(tr("History"), viewHistoryWidget);
    viewHistoryBoxOverallLayout = new QVBoxLayout(viewHistoryWidget);
    
    clearHistoryButton = new QPushButton(tr("Clear All History"), viewHistoryBox);
    viewHistoryBoxLayout = new QVBoxLayout(viewHistoryBox);
    
    viewHistoryBoxOverallLayout->addWidget(viewHistoryBox);
    viewHistoryBoxLayout->addWidget(clearHistoryButton);
    viewHistoryBoxOverallLayout->addStretch();
    
    viewMapper = new QSignalMapper(this);
    removeMapper = new QSignalMapper(this);
    
    this->currFunction = currFunction;
    this->currColorWheel = currColorWheel;
    this->settings = settings;
    
    connect(clearHistoryButton, SIGNAL(clicked()), this, SLOT(clearAllHistory()));
    
//    connect(clearHistoryButton, SIGNAL(clicked()), this, SLOT(clearAllHistory()));
//    connect(viewMapper, SIGNAL(mapped(QString)), this, SLOT(loadSettings(QString)));
    connect(removeMapper, SIGNAL(mapped(QObject*)), this, SLOT(removePreview(QObject*)));
    
}

void HistoryDisplay::addToHistory(const QDateTime &savedTime, const QString &filePathName)
{
    HistoryItem *item = new HistoryItem();
    
    QVBoxLayout *historyItemsWithLabelLayout = new QVBoxLayout();
    QHBoxLayout *historyItemsLayout = new QHBoxLayout();
    QVBoxLayout *historyItemsButtonsLayout = new QVBoxLayout();
    Display *d = new Display(HISTORY_ITEM_SIZE, HISTORY_ITEM_SIZE, viewHistoryBox);
    QPushButton *viewButton = new QPushButton(tr("View"), viewHistoryBox);
    QPushButton *removeButton = new QPushButton(tr("Remove"), viewHistoryBox);
    QLabel *timeStampLabel = new QLabel(viewHistoryBox);
    
//    QDateTime savedTime = QDateTime::currentDateTimeUtc();
    QString newFile = savedTime.toString("MM.dd.yyyy.hh.mm.ss.zzz.t").append(".wpr");
    QString savedTimeLabel = "Saved: " + savedTime.toString("MM/dd/yyyy") + " at " + savedTime.toString("hh:mm:ss");
    timeStampLabel->setText(savedTimeLabel);
    
    historyItemsLayout->addWidget(d);
    historyItemsButtonsLayout->addWidget(viewButton);
    historyItemsButtonsLayout->addWidget(removeButton);
    historyItemsLayout->addLayout(historyItemsButtonsLayout);
    historyItemsWithLabelLayout->addLayout(historyItemsLayout);
    historyItemsWithLabelLayout->addWidget(timeStampLabel);
    viewHistoryBoxLayout->insertLayout(1, historyItemsWithLabelLayout);
    
    //saving all values to history item
    item->preview = d;
    item->savedTime = savedTime;
    item->layoutWithLabelItem = historyItemsWithLabelLayout;
    item->layoutItem = historyItemsLayout;
    item->buttonLayoutItem = historyItemsButtonsLayout;
    item->viewButton = viewButton;
    item->removeButton = removeButton;
    item->labelItem = timeStampLabel;
    item->filePathName = filePathName;
    
    Port *historyDisplayPort = new Port(currFunction, currColorWheel, item->preview->width(), item->preview->height(), settings);
    historyDisplayPort->paintHistoryIcon(item);
    
    connect(viewButton, SIGNAL(clicked()), viewMapper, SLOT(map()));
    connect(removeButton, SIGNAL(clicked()), removeMapper, SLOT(map()));
    
    viewMapper->setMapping(viewButton, newFile);
    removeMapper->setMapping(removeButton, item);
    
    historyItemsMap.insert(savedTime, item);
    historyPortsMap.insert(savedTime, historyDisplayPort);
    
}

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
    
    delete *(historyPortsMap.find(historyItemToRemove->savedTime));
    historyPortsMap.erase(historyPortsMap.find(historyItemToRemove->savedTime));
    
    historyItemsMap.erase(historyItemsMap.find(historyItemToRemove->savedTime));
    
    QFile::remove(historyItemToRemove->filePathName);
}



void HistoryDisplay::clearAllHistory()
{
    while (!historyItemsMap.empty()) {
        qDebug() << "removing";
        removePreview(historyItemsMap.begin().value());
    }
}


// TODO rewrite and call from udpateSavePreview in interface class
void HistoryDisplay::triggerAddToHistory(const QDateTime &savedTime, const QString &filePathName)
{
    
    // qDebug() << "Updating Preview";
    
    if (historyItemsMap.size() < MAX_HISTORY_ITEMS) {
        addToHistory(savedTime, filePathName);
    } else {
        removePreview(*(historyItemsMap.begin()));
        addToHistory(savedTime, filePathName);
    }
    
    //updatePreviewDisplay();
    
}




