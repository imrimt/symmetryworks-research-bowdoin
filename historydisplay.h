#ifndef historydisplay_h
#define historydisplay_h

#include "port.h"

#include <QGroupBox>
#include <QSignalMapper>
#include <QLabel>

#include <QMessageBox>

const int MAX_HISTORY_ITEMS = 8;
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
    QLabel *noItemLabel;
    
    // used to call private addToHistory() function
    void hide();
    void show();
    void triggerAddToHistory(const QDateTime &savedTime, const QString &filePathName, AbstractFunction *function, ColorWheel *colorwheel, Settings *settings);
    void setPosition(QPoint point);
    
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
