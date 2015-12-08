#ifndef MYHELPER_H
#define MYHELPER_H

#include <QDesktopWidget>
#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include <QAbstractButton>
#include <QCoreApplication>
#include <QFileDialog>
#include <QTime>
#include <QProcess>
#include <QDir>
#include <QApplication>
#include <QStyleFactory>
#include <QInputDialog>
#include <QStyle>
#include <QDebug>

/**
 * @brief The MyTool class 工具类，包含公共对话框、文件处理、公共static方法
 */
class MyTool:public QObject
{
public:
    enum CheckResuleCode{
        Ok = 1,
        Error = 0
    };
    Q_DECLARE_FLAGS(CheckResuleCodes, CheckResuleCode)

    //设置全局为plastique样式
    static void SetStyle()
    {
        QApplication::setStyle(QStyleFactory::create("Plastique"));
        //QApplication::setPalette(QApplication::style()->standardPalette());//替换应用程序的颜色组合
    }

    //设置编码为GB2312
    static void SetGB2312Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("GB2312");
        QTextCodec::setCodecForLocale(codec);
    }

    //设置编码为UTF8
    static void SetUTF8Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
    }

    //显示信息框，仅确定按钮
    static void ShowMessageBoxInfo(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet("font:12pt '宋体'");
        msg.setWindowTitle("提示");
        msg.setText(info);
        msg.setIcon(QMessageBox::Information);
        msg.addButton("确定",QMessageBox::ActionRole);
        msg.exec();
    }

    static void ShowMessageBoxInfoX(QString info)
    {
        QMessageBox::information(0,"提示",info,QMessageBox::Ok);
    }

    //显示错误框，仅确定按钮
    static void ShowMessageBoxError(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet("font:12pt '宋体'");
        msg.setWindowTitle("错误");
        msg.setText(info);
        msg.setIcon(QMessageBox::Critical);
        msg.addButton("确定",QMessageBox::ActionRole);
        msg.exec();
    }

    static void ShowMessageBoxErrorX(QString info)
    {
        QMessageBox::critical(0,"错误",info,QMessageBox::Ok);
    }

    //显示询问框，确定和取消按钮
    static int ShowMessageBoxQuesion(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet("font:12pt '宋体'");
        msg.setWindowTitle("询问");
        msg.setText(info);
        msg.setIcon(QMessageBox::Question);
        msg.addButton("确定",QMessageBox::ActionRole);
        msg.addButton("取消",QMessageBox::RejectRole);

        return msg.exec();
    }

    static int ShowMessageBoxQuesionX(QString info)
    {
        return QMessageBox::question(0,"询问",info,QMessageBox::Yes|QMessageBox::No);
    }

    //显示标准输入框
    static QString ShowInputBox(QWidget *frm,QString info)
    {
        bool ok;
        return QInputDialog::getText(frm,"提示",info,QLineEdit::Password,"",&ok);
    }

    //16进制字符串转字节数组
    static QByteArray HexStrToByteArray(QString str)
    {
        QByteArray senddata;
        //为了消除警告
        str = str;
//        int hexdata,lowhexdata;
//        int hexdatalen = 0;
//        int len = str.length();
//        senddata.resize(len/2);
//        char lstr,hstr;
//        for(int i=0; i<len; )
//        {
//            hstr=str[i].toAscii();
//            if(hstr == ' ')
//            {
//                i++;
//                continue;
//            }
//            i++;
//            if(i >= len)
//                break;
//            lstr = str[i].toAscii();
//            hexdata = ConvertHexChar(hstr);
//            lowhexdata = ConvertHexChar(lstr);
//            if((hexdata == 16) || (lowhexdata == 16))
//                break;
//            else
//                hexdata = hexdata*16+lowhexdata;
//            i++;
//            senddata[hexdatalen] = (char)hexdata;
//            hexdatalen++;
//        }
//        senddata.resize(hexdatalen);
        return senddata;
    }

    static char ConvertHexChar(char ch)
    {
        if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
    }

    static unsigned int ByteArrayToUInt(QByteArray data)
    {
        return *(unsigned int*)(data.data());
    }

    //字节数组转16进制字符串
    static QString ByteArrayToHexStr(QByteArray data)
    {
        QString temp="";
        QString hex=data.toHex();
        for (int i=0;i<hex.length();i=i+2)
        {
            temp+=hex.mid(i,2)+" ";
        }
        return temp.trimmed().toUpper();
    }

    //16进制字符串转10进制
    static int StrHexToDecimal(QString strHex)
    {
        bool ok;
        return strHex.toInt(&ok,16);
    }

    //10进制字符串转10进制
    static int StrDecimalToDecimal(QString strDecimal)
    {
        bool ok;
        return strDecimal.toInt(&ok,10);
    }

    //2进制字符串转10进制
    static int StrBinToDecimal(QString strBin)
    {
        bool ok;
        return strBin.toInt(&ok,2);
    }

    //16进制字符串转2进制字符串
    static QString StrHexToStrBin(QString strHex)
    {
        uchar decimal=StrHexToDecimal(strHex);
        QString bin=QString::number(decimal,2);

        uchar len=bin.length();
        if (len<8)
        {
            for (int i=0;i<8-len;i++)
            {
                bin="0"+bin;
            }
        }

        return bin;
    }

    //10进制转2进制字符串一个字节
    static QString DecimalToStrBin1(int decimal)
    {
        QString bin=QString::number(decimal,2);

        uchar len=bin.length();
        if (len<=8)
        {
            for (int i=0;i<8-len;i++)
            {
                bin="0"+bin;
            }
        }

        return bin;
    }

    //10进制转2进制字符串两个字节
    static QString DecimalToStrBin2(int decimal)
    {
        QString bin=QString::number(decimal,2);

        uchar len=bin.length();
        if (len<=16)
        {
            for (int i=0;i<16-len;i++)
            {
                bin="0"+bin;
            }
        }

        return bin;
    }

    //计算校验码
    static uchar GetCheckCode(uchar data[],uchar len)
    {
        uchar temp=0;

        for (uchar i=0;i<len;i++)
        {
            temp+=data[i];
        }

        return temp%256;
    }

    //计算校验码
    static int GetCRCCode(QByteArray data)
    {
        const unsigned char chCRCHTalbe[] =                                // CRC 高位字节值表
        {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40
        };

        const unsigned char chCRCLTalbe[] =                           // CRC 低位字节值表
        {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
        0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
        0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
        0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
        0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
        0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
        0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
        0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
        0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
        0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
        0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
        0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
        0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
        0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
        0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
        0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
        0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
        0x41, 0x81, 0x80, 0x40
        };

        char* pchMsg = data.data();
        int wDataLen = data.length();
        char chCRCHi = 0xFF; // 高CRC字节初始化
        char chCRCLo = 0xFF; // 低CRC字节初始化
        int wIndex;            // CRC循环中的索引

        while (wDataLen--)
        {
            // 计算CRC
            wIndex = chCRCLo ^ *pchMsg++ ;
            chCRCLo = chCRCHi ^ chCRCHTalbe[wIndex];
            chCRCHi = chCRCLTalbe[wIndex] ;
        }

        return ((chCRCHi << 8) | chCRCLo) ;
    }

    //检查crc校验码
    static bool CheckCRCCode(QByteArray data)
    {
        const unsigned char chCRCHTalbe[] =                                // CRC 高位字节值表
        {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40
        };

        const unsigned char chCRCLTalbe[] =                           // CRC 低位字节值表
        {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
        0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
        0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
        0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
        0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
        0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
        0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
        0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
        0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
        0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
        0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
        0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
        0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
        0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
        0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
        0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
        0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
        0x41, 0x81, 0x80, 0x40
        };

        char* pchMsg = data.data();
        int len = data.length();
        int count = len;
        char chCRCHi = 0xFF; // 高CRC字节初始化
        char chCRCLo = 0xFF; // 低CRC字节初始化
        int wIndex;            // CRC循环中的索引

        while (count--)
        {
            // 计算CRC
            wIndex = chCRCLo ^ *pchMsg++ ;
            chCRCLo = chCRCHi ^ chCRCHTalbe[wIndex];
            chCRCHi = chCRCLTalbe[wIndex] ;
        }

        int CrcCalculate = (chCRCHi << 8) | chCRCLo;
        int CrcInData = (data.at(len -1) << 8) | data.at(len - 2);

        return CrcCalculate == CrcInData;
    }

    /**
     * @brief checkCrc 校验自定义crc
     * @param ba 待校验数据
     * @return success
     */
    static bool checkCrc(QByteArray ba)
    {
        unsigned int datalen = ba.length();

        unsigned short i, crc = 0;
        for (i = 0; i < datalen; i++)
        {
            crc = crc_ccitt_update(crc, ba.at(i) /*data[i]*/);
        }\
        if(crc)
        {
            return false;
        }

        return true;
    }

    static bool checkCrc16(QByteArray ba)
    {
        unsigned char* Pdata = (unsigned char*)ba.data();
        unsigned int len = ba.length();

        unsigned short taps;
        unsigned int j, end;
        end = len;
        taps = 0x0000;

        if(end >= 2)
        {
            end = end - 2;

            for (j = 0; j<end; j++)
            {
                taps = crc_ccitt_update(taps, Pdata[j]);
            }

            return Pdata[j] == (unsigned char)((taps) & 0x00ff)
                    && Pdata[j+1] == (unsigned char)(((taps) >> 8) & 0x00ff);
        } else
        {
            return false;
        }
    }

    static unsigned short crc_ccitt_update(unsigned short crc, unsigned char data)
    {
        data ^= crc &0xff;
        data ^= data<<4;

        return (((unsigned short)data << 8) | ((crc && 0xff00) >>8))
                ^ (unsigned char)(data >>4)
                ^ ((unsigned short)data << 3);
    }

    /**
     * @brief CheckXorCode 校验nmea校验和
     * @param data nmea byte数组
     * @return  success
     */
    static bool CheckNmeaCode(QByteArray data)
    {
        int len = data.length();
        bool success = true;

        //计算校验
        int sum = data.at(1);   //异或初始值
        for (int i = 2; i < len - 5; ++i)   //2和5均为magic，请参照nmea协议，即可明白
        {
            sum ^= data.at(i);
        }
        QString strSum = QString::number(sum, 16);  //10进制转为16进制字符串
        if(sum <=16)    //小于f时，高位补零
        {
            strSum.insert(0, '0');
        }

        //数据中自带校验
        QString str = QString::fromLatin1(data.mid(len-4), 2);

        //校验和比较
        int code = QString::compare(strSum, str, Qt::CaseInsensitive);

        if(code != 0)
        {
            success = false;
        }

        return success;
    }

//    //将溢出的char转为正确的uchar
//    static uchar GetUChar(char data)
//    {
//        uchar temp;
//        if (data>127)
//        {
//            temp=data+256;
//        }
//        else
//        {
//            temp=data;
//        }
//        return temp;
//    }

    //延时
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    //获取当前路径
    static QString GetCurrentPath()
    {
        return QString(QCoreApplication::applicationDirPath()+"/");
    }

//    //播放声音
//    static void PlaySound(QString soundName)
//    {
////        QSound::play(soundName);
//    }

    //设置系统日期时间
    /*static void SetSystemDateTime(int year,int month,int day,int hour,int min,int sec)
    {
//        QProcess p(0);

//        p.start("cmd");
//        p.waitForStarted();
//        p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toAscii());
//        p.closeWriteChannel();
//        p.waitForFinished(1000);
//        p.close();

//        p.start("cmd");
//        p.waitForStarted();
//        p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toAscii());
//        p.closeWriteChannel();
//        p.waitForFinished(1000);
//        p.close();
    }*/

    //窗体居中，并且只有关闭按钮，不能调整大小
    static void FormOnlyCloseInCenter(QWidget *frm)
    {
        //设置窗体居中
        QDesktopWidget desktop;
        int screenX=desktop.availableGeometry().width();
        int screenY=desktop.availableGeometry().height()-40;
        int frmX=frm->width();
        int frmY=frm->height();
        QPoint movePoint(screenX/2-frmX/2,screenY/2-frmY/2);
        frm->move(movePoint);

        //设置窗体不能调整大小
        frm->setFixedSize(frmX,frmY);

        //设置窗体只有最小化按钮
        frm->setWindowFlags(Qt::WindowCloseButtonHint);
    }

    //窗体居中显示
    static void FormInCenter(QWidget *frm)
    {
        int screenX=qApp->desktop()->width();
        int screenY=qApp->desktop()->height()-60;
        int wndX=frm->width();
        int wndY=frm->height();
        QPoint movePoint((screenX-wndX)/2,(screenY-wndY)/2);
        frm->move(movePoint);
    }

    //窗体没有最大化按钮
    static void FormNoMaxButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowMinimizeButtonHint);
    }

    //窗体只有关闭按钮
    static void FormOnlyCloseButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowCloseButtonHint);
    }

    //窗体不能改变大小
    static void FormNotResize(QWidget *frm)
    {
        frm->setFixedSize(frm->width(),frm->height());
    }

    //获取桌面大小
    static QSize GetDesktopSize()
    {
        QDesktopWidget desktop;
        return QSize(desktop.availableGeometry().width(),desktop.availableGeometry().height());
    }

    //获取选择的文件
    static QString GetFileName(QString filter)
    {
        return QFileDialog::getOpenFileName(NULL,"选择文件",QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择的文件集合
    static QStringList GetFileNames(QString filter)
    {
        return QFileDialog::getOpenFileNames(NULL,"选择文件",QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择的目录
    static QString GetFolderName()
    {
        return QFileDialog::getExistingDirectory();;
    }

    //获取文件名，含拓展名
    static QString GetFileNameWithExtension(QString strFilePath)
    {
        QFileInfo fileInfo(strFilePath);
        return fileInfo.fileName();
    }

    //获取选择文件夹中的文件
    static QStringList GetFolderFileNames(QStringList filter)
    {
        QStringList fileList;
        QString strFolder = QFileDialog::getExistingDirectory();
        if (!strFolder.length()==0)
        {
            QDir myFolder(strFolder);

            if (myFolder.exists())
            {
                fileList= myFolder.entryList(filter);
            }
        }
        return fileList;
    }

    //文件夹是否存在
    static bool FolderIsExist(QString strFolder)
    {
        QDir tempFolder(strFolder);
        return tempFolder.exists();
    }

    //文件是否存在
    static bool FileIsExist(QString strFile)
    {
        QFile tempFile(strFile);
        return tempFile.exists();
    }

    //复制文件
    static bool CopyFile(QString sourceFile, QString targetFile)
    {
        if (FileIsExist(targetFile))
        {
            int ret=QMessageBox::information(NULL,"提示","文件已经存在，是否替换？",QMessageBox::Yes | QMessageBox::No);
            if (ret!=QMessageBox::Yes)
            {
                return false;
            }
        }
        return QFile::copy(sourceFile,targetFile);
    }
};

#endif // MYHELPER_H
