

#ifndef historydisplay_h
#define historydisplay_h


#include "port.h"

#include <QGroupBox>
#include <QSignalMapper>
#include <QWidget>


const int MAX_HISTORY_ITEMS = 5;
const int HISTORY_ITEM_SIZE = 60;


class HistoryDisplay : public QObject
{
    Q_OBJECT

public:
    
    HistoryDisplay(QObject *parent = 0);
    
    // LAYOUT ELEMENTS
    QWidget *viewHistoryWidget;
    QGroupBox *viewHistoryBox;
    QVBoxLayout *viewHistoryBoxLayout;
    QVBoxLayout *viewHistoryBoxOverallLayout;
    QPushButton *clearHistoryButton;
    QSignalMapper *viewMapper;
    QSignalMapper *removeMapper;
    
    // used to call private addToHistory() function
    void triggerAddToHistory(const QDateTime &savedTime, const QString &filePathName, AbstractFunction *function, ColorWheel *colorwheel, Settings *settings);

    private slots:
        void removePreview(QObject *item);
        void clearAllHistory();
    
    
private:
    // maps an instance's timestamp to its corresponding HistoryItem or HistoryItem Port object
    QMap<QDateTime, HistoryItem*> historyItemsMap;
    QMap<QDateTime, Port*> historyPortsMap;
    
    // add a creation to the history list
    void addToHistory(const QDateTime &savedTime, const QString &filePathName, AbstractFunction *function, ColorWheel *colorwheel, Settings *settings);
    
};



#endif /* historydisplay_h */
