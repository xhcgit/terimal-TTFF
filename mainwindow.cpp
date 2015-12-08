/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "mdichild.h"
#include "frmConsole.h"
#include "mytool.h"
#include "config.h"
#include "dataprocess.h"
#include "SharedData.h"
#include "NmeaParser.h"
#include "WatchDog.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QSemaphore>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //添加主窗口mdi区域
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    //添加binary console窗口，暂时添加到此，后面需要在界面上根据用户自己需要进行打开
    initConsoleWindows();

    //新建初始化串口
    serial = new QSerialPort(this);
    serialRelay = new QSerialPort(this);

    //定时器初始化
    timerOn = new QTimer(this);
    timerOff = new QTimer(this);

    //新建串口设置对话框
    settings = new SettingsDialog;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    endTest = 0;
    testIndex = 0;
    loadTimerConfig();

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));   //串口有数据，马上触发读取

    connect(settings, SIGNAL(sendPortNameRelay(QString)),
            this, SLOT(openSerialPortRelay(QString)));


    //分离、校验线程
    DataProcess *processRawData = new DataProcess;  //初始化消费者线程
    NmeaParser *n = new NmeaParser;
    connect(processRawData, SIGNAL(revAPacketNmeaData(QByteArray, bool)), n, SLOT(parserNmeaSentence(QByteArray)));    //收到Nmea语句后的显示程序
    connect(processRawData, SIGNAL(revAPacketNmeaData(QByteArray, bool)), textConsole, SLOT(appendAsString(QByteArray, bool)));    //收到Nmea语句后的显示程序

    connect(n, SIGNAL(goToReset()),
            this, SLOT(startTestNextROund()));

    timerOff->setSingleShot(1); //只执行一次，否则为循环定时器
    timerOn->setSingleShot(1);
    connect(timerOff, SIGNAL(timeout()), this, SLOT(powerOff()));
    connect(timerOn, SIGNAL(timeout()), this, SLOT(powerOn()));

    processRawData->start();    //启动线程
}

/**
 * @brief MainWindow::~MainWindow 析构函数
 */
MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

/**
 * @brief MainWindow::openSerialPort 打开串口
 */
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if(!serial->isOpen())
    {
        if (serial->open(QIODevice::ReadWrite))
        {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), serial->errorString());

            ui->statusBar->showMessage(tr("Open error"));
        }
    }

    if(!serialRelay->isOpen())
    {
        serialRelay->open(QIODevice::ReadWrite);
    }

    powerOn();
    startTestNextROund();
}

/**
 * @brief MainWindow::closeSerialPort 关闭串口
 */
void MainWindow::closeSerialPort()
{
    //关闭芯片串口
    if(serial->isOpen())
    {
        serial->close();
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->actionConfigure->setEnabled(true);
        ui->statusBar->showMessage(tr("Disconnected"));
    }

    //关闭继电器控制串口
    if(serialRelay->isOpen())
    {
        powerOff();
        serialRelay->close();
    }
}

/**
 * @brief MainWindow::about 关于对话框
 */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

/**
 * @brief MainWindow::readData 串口读
 */
void MainWindow::readData()
{
    QByteArray data = serial->readAll();    //读取所有数据
    //填充rawdata线程
    static int indexRawData = 0;
    for(int i =0; i<data.length(); i++)
    {
        SharedData::freeRawData.acquire();
        SharedData::bufferRawData[indexRawData%SERIAL_BUFFER_SIZE] = data.at(i);
        indexRawData++;
        SharedData::usedRawData.release();
    }
}

/**
 * @brief MainWindow::handleError 错误处理
 * @param error 串口错误
 */
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}


void MainWindow::powerOn()
{
    qDebug()<<"in powerOn begin " << endl;
    char messageRely[6];
    *messageRely = 0b10101010;
    *(messageRely+1) = 0b01011010;
    *(messageRely+2) = 0b00000000;
    *(messageRely+3) = 0b00000001;
    *(messageRely+4) = 0b00000000;
    *(messageRely+5) = 0b11111111;
    QByteArray message;
    message.append(messageRely, 6);

    serialRelay->write(message);

    QFile file(SharedData::outputFileName);
    QTime now = QTime::currentTime();
    file.open(QFile::Append | QIODevice::Text);
    QTextStream out(&file);
    if(SharedData::flagTTFF)
        out<< "TTFF begin at :"<<now.toString()<<endl;
    file.close();
    qDebug()<< "in powerOn end " << endl;
}

void MainWindow::powerOff()
{
    qDebug()<<"in powerOff begin " << endl;
    char messageRely[6];
    *messageRely = 0b10101010;
    *(messageRely+1) = 0b01011010;
    *(messageRely+2) = 0b00000000;
    *(messageRely+3) = 0b00000000;
    *(messageRely+4) = 0b00000000;
    *(messageRely+5) = 0b11111111;
    QByteArray message;
    message.append(messageRely, 6);

    serialRelay->write(message);

    QTime time;
    time.currentTime();

    SharedData::flagTTFF = true;

    QFile file(SharedData::outputFileName);
    file.open(QFile::Append | QIODevice::Text);
    QTextStream out(&file);
    out<<RESULT_SPLITTER;
    out<<"power off at "<<time.currentTime().toString()<<endl;
    out<<RESULT_SPLITTER;
    file.close();
    qDebug()<<"in powerOff end " << endl;
}

void MainWindow::coldStart()
{
    qDebug()<<"in coldStart begin " << endl;
    QByteArray msg;
    QFile file(SharedData::outputFileName);
    file.open(QFile::Append | QIODevice::Text);
    QTextStream out(&file);
    QDateTime now = QDateTime::currentDateTime();

    msg.append("$PMTK103*30\r\n");    //初始化向机器发送的消除指令

    serial->write(msg);
    //打印开始信息
    out<<RESULT_SPLITTER;
    out<<"Begin test at "<<now.toString("yyyy-MM-dd HH:mm:ss")<<endl;
    out<<RESULT_SPLITTER;
    out<<"Duration of Power On:"
        << offTime.at(testIndex)
        <<endl
        <<"Duration of Power Off:"
        << onTime.at(testIndex)
        <<endl;
    file.close();
    qDebug()<<"in coldStart end " << endl;
}

void MainWindow::startTestNextROund()
{
    qDebug()<<"in startTestNextROund begin " << endl;
    if(testIndex < offTime.size())
    {
        int durationOfOn = offTime.at(testIndex);
        int durationOfOff = onTime.at(testIndex);

        //更新输出文件文件名
        QString a;
        QString b;
        a.setNum(durationOfOn);
        b.setNum(durationOfOff);
        SharedData::outputFileName = a + "-" +b +".txt";

        coldStart();

        timerOff->start(durationOfOn * MILLISECOND_OF_A_SECOND);   //启动关机定时器
        timerOn->start((durationOfOff + durationOfOn) * MILLISECOND_OF_A_SECOND);  //启动开机定时器

        testIndex ++;
    }
    else
    {
        powerOff();
    }
    qDebug()<<"in startTestNextROund end " << endl;
}

/**
 * @brief MainWindow::openSerialPortRelay 设置继电器串口
 * @param serialPortRelay
 */
void MainWindow::openSerialPortRelay(QString serialPortRelay)
{
    serialRelay->setPortName(serialPortRelay);
}


/**
 * @brief MainWindow::initActionsConnections 初始化信号连接
 */
void MainWindow::initActionsConnections()
{
    //connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    //connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

/**
 * @brief MainWindow::addBinaryConsole 添加console mdi窗口
 */
void MainWindow::initConsoleWindows()
{
    binaryConsole = new FrmConsole;
    binaryConsole->setEnabled(true);
    binaryConsole->setWindowTitle("Binary Console");
    mdiArea->addSubWindow(binaryConsole);


    textConsole = new FrmConsole;
    textConsole->setEnabled(true);
    textConsole->setWindowTitle("NMEA1083 Console");
    mdiArea->addSubWindow(textConsole);
}

/**
 * @brief MainWindow::loadTimerConfig 从文件中加载测试所用定时器数据
 */
void MainWindow::loadTimerConfig()    //读取测试数据
{
    QFile inTestMessage("Test.txt");
    QStringList list;
    QString message;
    QRegExp rx("[,]");
    QString strOffTime;
    QString strOnTime;
    bool ok;
    int i;
    inTestMessage.open(QFile::ReadOnly);
    while(!(inTestMessage.atEnd()))
    {
        message = inTestMessage.readLine();
        message = message.simplified();
        if(message.startsWith("//"))
            continue;
        if(message.endsWith("\r\n"))
            message.remove(message.length()-2, 2);
        if(!message.isEmpty())
            list.append(message.split(rx));
    }
    for(i = 0;i < list.length(); i++)
    {
        strOffTime = list.at(i);
        if(strOffTime.endsWith('h') || strOffTime.endsWith('H'))
        {
            strOffTime.remove(strOffTime.length()-1, 1);
            offTime.append(strOffTime.toInt(&ok) * 3600);    //一小时对应3600秒
        }
        else if(strOffTime.endsWith('m') || strOffTime.endsWith('M'))
        {
            strOffTime.remove(strOffTime.length()-1, 1);
            offTime.append(strOffTime.toInt(&ok) * 60);    //一分钟对应60秒
        }
        else
            offTime.append(strOffTime.toInt(&ok));
        i++;

        strOnTime = list.at(i);

        if(strOnTime.endsWith('h') || strOnTime.endsWith('H'))
        {
            strOnTime.remove(strOnTime.length()-1, 1);
            onTime.append(strOnTime.toInt(&ok) * 3600);    //一小时对应3600秒
        }
        else if(strOnTime.endsWith('m') || strOnTime.endsWith('M'))
        {
            strOnTime.remove(strOnTime.length()-1, 1);
            onTime.append(strOnTime.toInt(&ok) * 60);    //一分钟对应60秒
        }
        else
            onTime.append(strOnTime.toInt(&ok));
    }
    inTestMessage.close();
}
