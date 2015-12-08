#include "dataprocess.h"
#include "mytool.h"
#include "shareddata.h"

#include <QDebug>
#include <QSemaphore>

void DataProcess::run() Q_DECL_OVERRIDE
{
    initHead();

    nmeaEnd.append(NMEA_SYNC_END_1);
    nmeaEnd.append(NMEA_SYNC_END_2);

    while(1)    //死循环一直读
    {
        if(isNmeaHead())
        {
            QByteArray nmea;
            nmea.append(NMEA_SYNC_HEAD_1);
            nmea.append(NMEA_SYNC_HEAD_2);

            while(!nmea.endsWith(nmeaEnd))
            {
                nmea.append(readOneByte());
            }

            bool success = MyTool::CheckNmeaCode(nmea);    //nmea数据校验
            emit revAPacketNmeaData(nmea, success);    //发射读取到一帧nmea的信号
            initHead();     //清除header，进行下一帧读取
        }
        else if(isRawDataHead())
        {
            QByteArray raw;
            raw.append(UBX_SYNC_HEAD_1);
            raw.append(UBX_SYNC_HEAD_2);
            raw.append(readOneByte());
            raw.append(readOneByte());
            //读取长度
            raw.append(readOneByte());
            raw.append(readOneByte());
            raw.append(readOneByte());
            raw.append(readOneByte());

            int len = MyTool::ByteArrayToUInt(raw.mid(4, 4)); //长度从协议中第4位开始，共4个字节

            while(len--)    //根据长度读取一帧rawdata数据
            {
                raw.append(readOneByte());
            }

            bool success = MyTool::checkCrc(raw);    //nmea数据校验
            qDebug()<<raw;
            emit revAPacketRawData(raw, success); //发射读取到一帧rawdata的信号
            initHead();     //清除header，进行下一帧读取
        }
        else
        {
            updateHead(readOneByte());
        }
    }
}

/**
 * @brief DataProcess::readOneByte 从环形缓冲区读取one byte
 * @return byte
 */
char DataProcess::readOneByte()
{
    static int index = 0;
    char temp;

    //读取1个字节
    SharedData::usedRawData.acquire(1);
    temp = SharedData::bufferRawData[index%SERIAL_BUFFER_SIZE];
    index++;
    SharedData::freeRawData.release(1);

    return temp;
}

/**
 * @brief DataProcess::updateHead 更新同步头队列
 * @param ch 新读取到的字节
 */
void DataProcess::updateHead(char ch)
{
    header.remove(0, 1);
    header.append(ch);
}

/**
 * @brief DataProcess::isNmeaHead 判断是否是nmea 同步头
 * @return success
 */
bool DataProcess::isNmeaHead()
{
    return header.at(0) == NMEA_SYNC_HEAD_1
            &&header.at(1) == NMEA_SYNC_HEAD_2;
}

/**
 * @brief DataProcess::isRawDataHead 判断是否raw data 同步头
 * @return success
 */
bool DataProcess::isRawDataHead()
{
    return header.at(0) == UBX_SYNC_HEAD_1
            &&header.at(1) == UBX_SYNC_HEAD_2;
}

/**
 * @brief DataProcess::initHead 初始化数据校验头
 */
void DataProcess::initHead()
{
    header.clear();
    header.append('\0');
    header.append('\0');
}
