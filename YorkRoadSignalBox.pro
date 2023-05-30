QT       += core gui svgwidgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audio.cxx \
    blocksection.cxx \
    framelever.cxx \
    graphics.cxx \
    interlocking.cxx \
    leverframe.cxx \
    main.cpp \
    signal_2.cxx \
    signal_34.cxx \
    simulationpanel.cxx \
    trackcircuit.cxx \
    yrmainwindow.cxx

HEADERS += \
    audio.hxx \
    blocksection.hxx \
    framelever.hxx \
    global_params.hxx \
    graphics.hxx \
    interlocking.hxx \
    leverframe.hxx \
    points.hxx \
    scaling.hxx \
    signal.hxx \
    signal_2.hxx \
    signal_34.hxx \
    simulationpanel.hxx \
    trackcircuit.hxx \
    yrmainwindow.hxx

FORMS += \
    yrmainwindow.ui

TRANSLATIONS += \
    YorkRoadSignalBox_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    yrresources.qrc
