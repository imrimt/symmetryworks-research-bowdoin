

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
    
    HistoryDisplay(QObject *parent = 0) : QObject(parent) { }
    
    explicit HistoryDisplay(AbstractFunction *currFunction, ColorWheel *currColorWheel, Settings *settings, QObject *parent = 0);
    ~HistoryDisplay() {}
    
    QWidget *viewHistoryWidget;
    QGroupBox *viewHistoryBox;
    QVBoxLayout *viewHistoryBoxLayout;
    QVBoxLayout *viewHistoryBoxOverallLayout;
    QPushButton *clearHistoryButton;
    QSignalMapper *viewMapper;
    QSignalMapper *removeMapper;
    
    void triggerAddToHistory(const QDateTime &savedTime, const QString &filePathName);

    
    private slots:
        void removePreview(QObject *item);
        void clearAllHistory();
    
    
private:
    
    QMap<QDateTime, HistoryItem*> historyItemsMap;
    QMap<QDateTime, Port*> historyPortsMap;
    void addToHistory(const QDateTime &savedTime, const QString &filePathName);
    
    AbstractFunction *currFunction;
    ColorWheel *currColorWheel;
    Settings *settings;
    
    
};








#endif /* historydisplay_h */
