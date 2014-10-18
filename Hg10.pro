APP_NAME = Hg10

CONFIG += qt warn_on cascades10
QT += xml network
INCLUDEPATH += src/base
LIBS += -lsocket -lbbplatform

include(config.pri)
