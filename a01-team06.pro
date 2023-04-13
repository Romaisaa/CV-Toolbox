QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CV/harris.cpp \
    CV/histogram.cpp \
    CV/filters.cpp \
    CV/frequency.cpp \
    CV/matching.cpp \
    CV/sift.cpp \
#    Plot/plotter.cpp \
    CV/hough.cpp \
    CV/snake.cpp \
    pages/page4.cpp \
#    utilities/qcustomplot.cpp \
    pages/page8.cpp \
    utilities/clickable.cpp \
    utilities/scene.cpp \
    main.cpp \
    pages/page3.cpp \
    pages/mainwindow.cpp \
#    pages/page2.cpp \
    pages/page1.cpp \
    pages/page5.cpp \
    pages/page6.cpp\
    pages/page7.cpp

HEADERS += \
    CV/harris.h \
    CV/histogram.h \
    CV/filters.h \
    CV/frequency.h \
    CV/matching.h \
    CV/sift.h \
#    Plot/plotter.h \
    CV/hough.h \
    CV/snake.h \
    pages/page4.h \
#    utilities/qcustomplot.h \
    pages/page8.h \
    utilities/clickable.h \
    pages/page3.h \
    pages/mainwindow.h \
#    pages/page2.h \
    pages/page1.h \
    pages/page5.h \
    pages/page6.h  \
    pages/page7.h\
    utilities/scene.h

FORMS += \
    pages/page4.ui \
    pages/page3.ui \
    pages/mainwindow.ui \
#    pages/page2.ui \
    pages/page1.ui \
    pages/page5.ui \
    pages/page6.ui\
    pages/page7.ui \
    pages/page8.ui

INCLUDEPATH += C:\opencv\build\include

LIBS += C:\opencv\release\bin\libopencv_core470.dll
LIBS += C:\opencv\release\bin\libopencv_highgui470.dll
LIBS += C:\opencv\release\bin\libopencv_imgcodecs470.dll
LIBS += C:\opencv\release\bin\libopencv_imgproc470.dll
LIBS += C:\opencv\release\bin\libopencv_features2d470.dll
LIBS += C:\opencv\release\bin\libopencv_calib3d470.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

