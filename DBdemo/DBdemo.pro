#-------------------------------------------------
#
# Project created by QtCreator 2018-08-07T21:26:34
#
#-------------------------------------------------

QT       += core gui widgets network
QT       += sql
CONFIG += console

TARGET = DBdemo
TEMPLATE = app

INCLUDEPATH += ../src/Messenger ../src ../src/transactions
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += c++14


INCLUDEPATH += $$PWD/openssl_linux/include/
LIBS += -L$$PWD/openssl_linux/lib -lssl -lcrypto

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        ../src/HttpClient.cpp \
        ../src/Log.cpp \
        ../src/Paths.cpp \
        ../src/utils.cpp \
        ../src/btctx/Base58.cpp \
        ../src/dbstorage.cpp \
        ../src/BigNumber.cpp \
        ../src/Messenger/MessengerDBStorage.cpp \
        ../src/transactions/TransactionsDBStorage.cpp


HEADERS += \
        mainwindow.h \
        ../src/HttpClient.h \
        ../src/Log.h \
        ../src/Paths.h \
        ../src/utils.h \
        ../src/dbstorage.h \
        ../src/BigNumber.h \
        ../src/Messenger/MessengerDBStorage.h \
        ../src/transactions/TransactionsDBStorage.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
