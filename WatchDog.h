#ifndef Check_H
#define Check_H

#include "config.h"
#include "mytool.h"

#include <QObject>
#include <QSemaphore>
#include <QThread>

class WatchDog : public QThread
{
    Q_OBJECT

public:
    WatchDog(){}

    ~WatchDog(){}
public slots:
    void run();


signals:
    void reset();
};

#endif // Check_H
