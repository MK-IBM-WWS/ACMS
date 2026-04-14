QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminwindow.cpp \
    cogroupconsists.cpp \
    controllerwindow.cpp \
    dbconfig.cpp \
    hradddepo.cpp \
    hraddstaff.cpp \
    hreditdepo.cpp \
    hreditstaff.cpp \
    hrwindow.cpp \
    main.cpp \
    loginwindow.cpp \
    pdwindow.cpp \
    printwindow.cpp \
    settingswindow.cpp

HEADERS += \
    adminwindow.h \
    cogroupconsists.h \
    controllerwindow.h \
    dbconfig.h \
    hradddepo.h \
    hraddstaff.h \
    hreditdepo.h \
    hreditstaff.h \
    hrwindow.h \
    loginwindow.h \
    pdwindow.h \
    printwindow.h \
    settingswindow.h

FORMS += \
    adminwindow.ui \
    cogroupconsists.ui \
    controllerwindow.ui \
    hradddepo.ui \
    hraddstaff.ui \
    hreditdepo.ui \
    hreditstaff.ui \
    hrwindow.ui \
    loginwindow.ui \
    pdwindow.ui \
    printwindow.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    lw.qrc
