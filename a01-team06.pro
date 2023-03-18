QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CV/histogram.cpp \
    CV/filters.cpp \
    CV/frequency.cpp \
    Plot/plotter.cpp \
    qcustomplot.cpp \
    clickable.cpp \
    main.cpp \
    pages/page3.cpp \
    pages/mainwindow.cpp \
    pages/page2.cpp \
    pages/page1.cpp
HEADERS += \
    CV/histogram.h \
    CV/filters.h \
    CV/frequency.h \
    Plot/plotter.h \
    qcustomplot.h \
    clickable.h \
    pages/page3.h \
    pages/mainwindow.h \
    pages/page2.h \
    pages/page1.h

FORMS += \
    pages/page3.ui \
    pages/mainwindow.ui \
    pages/page2.ui \
    pages/page1.ui


INCLUDEPATH += C:\opencv\release\install\include

LIBS += C:\opencv\release\bin\libopencv_core470.dll
LIBS += C:\opencv\release\bin\libopencv_highgui470.dll
LIBS += C:\opencv\release\bin\libopencv_imgcodecs470.dll
LIBS += C:\opencv\release\bin\libopencv_imgproc470.dll
LIBS += C:\opencv\release\bin\libopencv_calib3d470.dll
LIBS += C:\opencv\release\bin\libopencv_features2d470.dll


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

