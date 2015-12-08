#include "NmeaParser.h"
#include "Nmea.h"
#include "config.h"
#include "SharedData.h"

#include <QRegExp>
#include <qtextstream.h>




void NmeaParser::run()
{

}

GPSWorkMode NmeaParser::getWorkMode(QString nmeaScentence)
{
    GPSWorkMode mode;
    if(nmeaScentence.startsWith("$GP"))
    {
        mode = GPS;
    }else if(nmeaScentence.startsWith("$BD"))
    {
        mode = BD;
    }else if(nmeaScentence.startsWith("$GN"))
    {
        mode = GPSBD;
    }else
    {
        mode = UnCertain;
    }
    return mode;
}

/**
 * @brief NmeaParser::getNmeaScentenceType 获取nmea语句类型
 * @param nmeaScentence nmea语句
 * @return nmea语句类型
 */
NmeaScentenceType NmeaParser::getNmeaScentenceType(QString nmeaScentence)
{
    NmeaScentenceType type;

    if(nmeaScentence.indexOf("GGA") == 3)
    {
        type = NMEA0183GGA;         
    }
    else if(nmeaScentence.indexOf("GLL") == 3)
    {
        type = NMEA0183GLL;                
    }
    else if(nmeaScentence.indexOf("GSA") == 3)
    {
        type = NMEA0183GSA;                
    }
    else if(nmeaScentence.indexOf("GSV") == 3)
    {
        type = NMEA0183GSV;                
    }
    else if(nmeaScentence.indexOf("RMC") == 3)
    {
        type = NMEA0183RMC;                
    }
    else if(nmeaScentence.indexOf("VTG") == 3)
    {
        type = NMEA0183VTG;                
    }
    else if(nmeaScentence.indexOf("ZDA") == 3)
    {
        type = NMEA0183ZDA;                
    }
    else if(nmeaScentence.indexOf("TXT") == 3)
    {
        type = NMEA0183TXT;                
    }
    else
    {
        type = NMEA0183PMTK;
    }    return type;
}

void NmeaParser::parserNmeaSentence(QByteArray ba)
{
    QString nmeaScentence =  QString(ba);

    GPSWorkMode gpsWorkMode = getWorkMode(nmeaScentence);
    NmeaScentenceType nmeaScentenceType = getNmeaScentenceType(nmeaScentence);

    Nmea *nmea = new Nmea();

    //将接收到的NMEA协议信息输出到文件NMEA.TXT中
    QFile file(SharedData::outputFileName);
    file.open(QFile::Append | QIODevice::Text);
    QTextStream out(&file);
    out<<nmeaScentence;
    file.close();

    connect(nmea, SIGNAL(go()),
            this, SLOT(goToWatchDog()));
    
    switch(nmeaScentenceType)
    {
       case NMEA0183GGA :
       {
           nmeaGPGGA nmeaGGA;
           nmeaGGA.gpsWorkMode = gpsWorkMode;
           nmea->nmeaParseGpgga(&nmeaScentence, &nmeaGGA);
           break;
       }

        default:
            break;
    }
}

void NmeaParser::goToWatchDog()
{
    emit goToReset();
}

