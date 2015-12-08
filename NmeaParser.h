#ifndef NMEAPARSER_H
#define NMEAPARSER_H

#include "Nmea.h"
#include <QObject>
#include <QThread>
#include <QFile>

class NmeaParser : public QThread
{
    Q_OBJECT

public:
    NmeaParser(){}
    ~NmeaParser(){}

    void run();

    GPSWorkMode getWorkMode(QString str);
    NmeaScentenceType getNmeaScentenceType(QString str);

signals:
    void goToReset();
public slots:
    void goToWatchDog();

private slots:
    void parserNmeaSentence(QByteArray ba);

};

#endif // NMEAPARSER_H
