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
    pages/SIFT_page.cpp \
    pages/contour_page.cpp \
    pages/filters_page.cpp \
    pages/frequency_page.cpp \
    pages/harris_page.cpp \
    pages/histogram_page.cpp \
    pages/hough_page.cpp \
    pages/matching_page.cpp \
    utilities/qcustomplot.cpp \
    pages/page11.cpp \
    pages/segmenation_page.cpp \
    pages/thresholding_page.cpp \
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
    pages/SIFT_page.h \
    pages/contour_page.h \
    pages/filters_page.h \
    pages/frequency_page.h \
    pages/harris_page.h \
    pages/histogram_page.h \
    pages/hough_page.h \
    pages/matching_page.h \
    utilities/qcustomplot.h \
    pages/page11.h \
    pages/segmenation_page.h \
    pages/thresholding_page.h \
    utilities/clickable.h \
    pages/mainwindow.h \
    utilities/scene.h

FORMS += \
    pages/SIFT_page.ui \
    pages/contour_page.ui \
    pages/filters_page.ui \
    pages/frequency_page.ui \
    pages/harris_page.ui \
    pages/histogram_page.ui \
    pages/hough_page.ui \
    pages/matching_page.ui \
    pages/mainwindow.ui \
    pages/page11.ui \
    pages/segmenation_page.ui \
    pages/thresholding_page.ui


INCLUDEPATH += C:\Users\youss\Documents\libraries\opencv\build\include

LIBS += C:\Users\youss\Documents\libraries\opencv\release\bin\libopencv_core470.dll
LIBS += C:\Users\youss\Documents\libraries\opencv\release\bin\libopencv_highgui470.dll
LIBS += C:\Users\youss\Documents\libraries\opencv\release\bin\libopencv_imgcodecs470.dll
LIBS += C:\Users\youss\Documents\libraries\opencv\release\bin\libopencv_imgproc470.dll
LIBS += C:\Users\youss\Documents\libraries\opencv\release\bin\libopencv_features2d470.dll
LIBS += C:\Users\youss\Documents\libraries\opencv\release\bin\libopencv_calib3d470.dll
LIBS += C:\Users\youss\Documents\libraries\opencv\release\bin\libopencv_objdetect470.dll


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    models/haarcascade_frontalface_default.xml

