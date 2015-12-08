#include "WatchDog.h"
#include "SharedData.h"



void WatchDog::run() Q_DECL_OVERRIDE
{
    /*while(1)
    {
        if(SharedData::next)
        {
            QFile file(SharedData::outputFileName);
            file.open(QFile::Append | QIODevice::Text);
            QTextStream out(&file);

            out<< "WILL BE HERE"<<endl;

            SharedData::next = false;
            emit reset();
        }
    }*/

}

