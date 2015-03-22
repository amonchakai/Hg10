APP_NAME = Hg10

CONFIG += qt warn_on cascades10
QT += xml network
INCLUDEPATH += src/base

include(config.pri)

LIBS += -lsocket -lbbplatform -lbbsystem -lbbpim -lbb -lbbdata -lbbcascadespickers -lbbdevice
