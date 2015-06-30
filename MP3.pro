TEMPLATE = app

QT += qml quick widgets core multimedia


SOURCES += src/about.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/option.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += inc/about.h \
    inc/mainwindow.h \
    inc/option.h \
    inc/stylesheet.h
