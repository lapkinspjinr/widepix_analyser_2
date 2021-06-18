#-------------------------------------------------
#
# Project created by QtCreator 2021-03-01T12:15:47
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = widepix_analyser_2
TEMPLATE = app thread

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 -moc

SOURCES += \
        main.cpp \
        widepix_analyser_2.cpp \
    uc_plot.cpp \
    qcustomplot.cpp \
    uc_wai.cpp \
    uc_data_container.cpp \
    uc_pixels_info.cpp \
    common_functions.cpp \
    uc_identification.cpp

HEADERS += \
        widepix_analyser_2.h \
    uc_plot.h \
    qcustomplot.h \
    uc_wai.h \
    uc_data_container.h \
    uc_pixels_info.h \
    common_functions.h \
    uc_identification.h

FORMS += \
        widepix_analyser_2.ui

LIBS += -L/home/aleksandr/root/root_6.22/install/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -lm -ldl -rdynamic
INCLUDEPATH += /home/aleksandr/root/root_6.22/install/include/

#LIBS += -L${ROOTSYS}/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -lm -ldl -rdynamic
#INCLUDEPATH += ${ROOTSYS}/include/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
