#ifndef ONLINEHISTORY_HPP
#define ONLINEHISTORY_HPP

#include <QString>

class OnlineHistory : public QObject {
    Q_OBJECT;

public:
    OnlineHistory(QObject *parent = 0) : QObject(parent) {};
    virtual ~OnlineHistory() {}


    virtual void getMessages            (const QString &from, int nbMessages = 1) = 0;
    virtual void getRemainingMessages   (QString lastMessageId) = 0;

};

#endif

