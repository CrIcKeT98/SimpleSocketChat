QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basehost.cpp \
    chatwindow.cpp \
    client.cpp \
    connectionmanager.cpp \
    main.cpp \
    connectwindow.cpp \
    server.cpp

HEADERS += \
    basehost.h \
    chatwindow.h \
    client.h \
    connectionmanager.h \
    connectwindow.h \
    server.h

FORMS += \
    chatwindow.ui \
    connectwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
