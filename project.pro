TEMPLATE = app
CONFIG += qt warn_on release #debug
QT += network
TARGET = PKUDeanChecker
SOURCES += main.cpp \
           checker.cpp
HEADERS += checker.h
RESOURCES += resources.qrc
win32 {
      CONFIG -= debug debug_and_release
      CONFIG += release
      QMAKE_LFLAGS += -static-libgcc -static-libstdc++
      LIBS += /usr/i486-mingw32/qt-static/bin/qt-plugins/plugins/codecs/libqcncodecs.a
      RC_FILE = win.rc
}
debug_messages {
      DEFINES += _DEBUG_ON
}