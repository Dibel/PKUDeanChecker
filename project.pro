TEMPLATE = app
CONFIG += qt debug warn_on
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
debug {
      DEFINES += _DEBUG_ON
}