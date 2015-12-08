#ifndef CONFIG_H
#define CONFIG_H

#include <QTime>

//日志文件保存目录
#define LOG_FILE_SAVE_DIR ("log")

//自定义协议头
#define UBX_SYNC_HEAD_1 (0x6d)
#define UBX_SYNC_HEAD_2 (0x63)
#define UBX_SYNC_HEAD_3 (0x02)

//NMEA协议头
#define NMEA_SYNC_HEAD_1 (0x24) //$
#define NMEA_SYNC_HEAD_2 (0x47) //G

//NMEA协议尾
#define NMEA_SYNC_END_1 (0x0d)
#define NMEA_SYNC_END_2 (0x0a)

#define SERIAL_BUFFER_SIZE (4096)

//nmea语句缓存队列大小
#define NMEA_BUFFER_QUEUE_SIZE (5)

//
#define RESULT_SPLITTER ("**************************************************************************************************************************************\n")
#define MILLISECOND_OF_A_SECOND (1000)
#endif // CONFIG
