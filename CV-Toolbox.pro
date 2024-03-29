QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CV/face_recognition.cpp \
    CV/faces_detection.cpp \
    CV/harris.cpp \
    CV/histogram.cpp \
    CV/filters.cpp \
    CV/frequency.cpp \
    CV/logisticregression.cpp \
    CV/matching.cpp \
    CV/segmentation.cpp \
    CV/sift.cpp \
    CV/thresholding.cpp \
    Plot/plotter.cpp \
    CV/hough.cpp \
    CV/snake.cpp \
    pages/page1.cpp \
    pages/page10.cpp \
    pages/page2.cpp \
    pages/page3.cpp \
    pages/page4.cpp \
    pages/page5.cpp \
    pages/page6.cpp \
    pages/page7.cpp \
    pages/page8.cpp \
    pages/page9.cpp \
    utilities/qcustomplot.cpp \
    pages/page11.cpp \
    utilities/clickable.cpp \
    utilities/scene.cpp \
    main.cpp \
    pages/mainwindow.cpp

HEADERS += \
    CV/face_recognition.h \
    CV/faces_detection.h \
    CV/harris.h \
    CV/histogram.h \
    CV/filters.h \
    CV/frequency.h \
    CV/logisticregression.h \
    CV/matching.h \
    CV/segmentation.h \
    CV/sift.h \
    CV/thresholding.h \
    Plot/plotter.h \
    CV/hough.h \
    CV/snake.h \
    pages/page1.h \
    pages/page10.h \
    pages/page2.h \
    pages/page3.h \
    pages/page4.h \
    pages/page5.h \
    pages/page6.h \
    pages/page7.h \
    pages/page8.h \
    pages/page9.h \
    utilities/qcustomplot.h \
    pages/page11.h \
    utilities/clickable.h \
    pages/mainwindow.h \
    utilities/scene.h

FORMS += \
    pages/mainwindow.ui \
    pages/page1.ui \
    pages/page10.ui \
    pages/page11.ui \
    pages/page2.ui \
    pages/page3.ui \
    pages/page4.ui \
    pages/page5.ui \
    pages/page6.ui \
    pages/page7.ui \
    pages/page8.ui \
    pages/page9.ui


INCLUDEPATH += C:\opencv\build\include

LIBS += C:\opencv\release\bin\libopencv_core470.dll
LIBS += C:\opencv\release\bin\libopencv_highgui470.dll
LIBS += C:\opencv\release\bin\libopencv_imgcodecs470.dll
LIBS += C:\opencv\release\bin\libopencv_imgproc470.dll
LIBS += C:\opencv\release\bin\libopencv_features2d470.dll
LIBS += C:\opencv\release\bin\libopencv_calib3d470.dll
LIBS += C:\opencv\release\bin\libopencv_objdetect470.dll


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    models/haarcascade_frontalface_default.xml

