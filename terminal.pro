greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    mdichild.cpp \
    FrmConsole.cpp \
    dataprocess.cpp \
    SharedData.cpp \
    Nmea.cpp \
    NmeaParser.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    mdichild.h \
    mytool.h \
    config.h \
    FrmConsole.h \
    dataprocess.h \
    SharedData.h \
    Nmea.h \
    NmeaParser.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    FrmConsole.ui

RESOURCES += \
    terminal.qrc

QT += multimedia
