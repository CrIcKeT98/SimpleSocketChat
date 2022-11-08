QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basesocket.cpp \
    chatwindow.cpp \
    clientsocket.cpp \
    connectionhandler.cpp \
    main.cpp \
    connectwindow.cpp \
    serversocket.cpp

HEADERS += \
    basesocket.h \
    chatwindow.h \
    clientsocket.h \
    connectionhandler.h \
    connectwindow.h \
    serversocket.h

FORMS += \
    chatwindow.ui \
    connectwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
