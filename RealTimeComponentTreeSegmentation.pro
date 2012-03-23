# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xEACB23CA

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=


#QT += core gui
QT += opengl
#QT += webkit

#DEFINES += QT_OPENGL_ES_2

#CONFIG += opengl

QMAKE_CXXFLAGS_RELEASE+= -O2
QMAKE_CXXFLAGS += -fexceptions

#QMAKE_INCDIR_OPENGL     = /usr/X11R6/include
#QMAKE_LIBDIR_OPENGL     = /usr/X11R6/lib

#QMAKE_LIBS_OPENGL       = -lGLU -lGL -lXmu
#QMAKE_LIBS_OPENGL_QT    = -lGL -lXmu

#LIBS = -lGL -lGLU -lXmu

SOURCES += main.cpp mainwindow.cpp \
    camerasupport.cpp \
    qglcanvas.cpp \
    qopenglpaintengine.cpp
HEADERS += mainwindow.h\
    include/ImageIterators.h \
    include/ImageIO.hxx \
    include/ImageIO_InrGz.hxx \
    include/Image.hxx \
    include/Image.h \
    include/FlatSE.hxx \
    include/FlatSE.h \
    include/ComponentTree.hxx \
    include/ComponentTree.h \
    include/Morphology.hxx \
    include/Morphology.h \
    include/OrderedQueue.h \
    camerasupport.h \
    qglcanvas.h \
    qopenglpaintengine.h
FORMS += mainwindow.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/JavaManager.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/pl/ekk/mkk/cameratest/ImageManager.java \
    android/version.xml















