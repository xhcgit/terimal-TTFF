#include "SharedData.h"

SharedData::SharedData()
{

}

SharedData::~SharedData()
{

}

//在类外对静态成员进行初始化
char SharedData::bufferRawData [SERIAL_BUFFER_SIZE];
QSemaphore SharedData::freeRawData(SERIAL_BUFFER_SIZE);
QSemaphore SharedData::usedRawData(0);

bool SharedData::flagTTFF = false;
bool SharedData::next = false;
QString SharedData::outputFileName = "default";
