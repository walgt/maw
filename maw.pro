QT += widgets
QT += gui widgets
QMAKE_CXXFLAGS += -std=c++14 -O3
TEMPLATE = app
QT -= core
TARGET = main
SOURCES += amenu.cpp  main.cpp \
    aamenu.cpp \
    menu.cpp \
    pinho.cpp
HEADERS +=  amenu.h \
    aamenu.h \
    menu.h \
    pinho.h



