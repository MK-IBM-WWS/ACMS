QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminwindow.cpp \
    cogroupconsists.cpp \
    controllerwindow.cpp \
    hrwindow.cpp \
    main.cpp \
    loginwindow.cpp \
    pdwindow.cpp \
    settingswindow.cpp

HEADERS += \
    adminwindow.h \
    cogroupconsists.h \
    controllerwindow.h \
    hrwindow.h \
    loginwindow.h \
    pdwindow.h \
    settingswindow.h

FORMS += \
    adminwindow.ui \
    cogroupconsists.ui \
    controllerwindow.ui \
    hrwindow.ui \
    loginwindow.ui \
    pdwindow.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    lw.qrc
