QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminwindow.cpp \
    amaddcontrol.cpp \
    amadduser.cpp \
    ameditcontrol.cpp \
    amedituser.cpp \
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
    pdaddgroup.cpp \
    pdaddpass.cpp \
    pdeditgroup.cpp \
    pdeditpass.cpp \
    pdwindow.cpp \
    printwindow.cpp \
    settingswindow.cpp

HEADERS += \
    adminwindow.h \
    amaddcontrol.h \
    amadduser.h \
    ameditcontrol.h \
    amedituser.h \
    cogroupconsists.h \
    controllerwindow.h \
    dbconfig.h \
    hradddepo.h \
    hraddstaff.h \
    hreditdepo.h \
    hreditstaff.h \
    hrwindow.h \
    loginwindow.h \
    pdaddgroup.h \
    pdaddpass.h \
    pdeditgroup.h \
    pdeditpass.h \
    pdwindow.h \
    printwindow.h \
    settingswindow.h

FORMS += \
    adminwindow.ui \
    amaddcontrol.ui \
    amadduser.ui \
    ameditcontrol.ui \
    amedituser.ui \
    cogroupconsists.ui \
    controllerwindow.ui \
    hradddepo.ui \
    hraddstaff.ui \
    hreditdepo.ui \
    hreditstaff.ui \
    hrwindow.ui \
    loginwindow.ui \
    pdaddgroup.ui \
    pdaddpass.ui \
    pdeditgroup.ui \
    pdeditpass.ui \
    pdwindow.ui \
    printwindow.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    lw.qrc
