TEMPLATE = lib
CONFIG += plugin c++17 qt
QT += core widgets

TARGET = overseer-plugin
DESTDIR = .

HEADERS += src/overseer-dock.h
SOURCES += src/overseer-plugin.cpp

INCLUDEPATH += /usr/include/obs

LIBS += -lobs-frontend-api -lobs

QMAKE_CXXFLAGS += -fPIC
QMAKE_LFLAGS += -shared
