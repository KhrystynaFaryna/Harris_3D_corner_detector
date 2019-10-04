#-------------------------------------------------
#
# Project created by QtCreator 2013-09-25T09:11:42
#
#-------------------------------------------------
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = k-Ring_Neighborhood
TEMPLATE = app


SOURCES += main.cpp\
           window.cpp \
           myglwidget.cpp \
           harrisresponse.cpp \
           mywidgets.cpp \

HEADERS  += window.h \
            myglwidget.h \
            harrisresponse.h \
            mywidgets.h \

FORMS    += window.ui \
            mywidgets.ui \
            window.ui
LIBS += -lopengl32
