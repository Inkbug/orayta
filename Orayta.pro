# -------------------------------------------------
# Project created by QtCreator 2009-03-15T13:50:06
# Author: Moshe Wagner. <moshe.wagner@gmail.com>
# -------------------------------------------------


# Big todo list:

# Fix show/hide nikud button
# Desktop app translation dosn't work
# Desktop - Next on one page books gives empty page

# - Fix desktop version (with download manager too!)
# - Update help page

QT += core gui network widgets
#QT += declarative
#QT += qml quick
# Additional import path used to resolve QML modules in Creator's code model
#QML_IMPORT_PATH =
#qtcAddDeployment()
#lessThan(QT_MAJOR_VERSION, 5): error(This project requires Qt 5 or later)

TARGET = orayta
TEMPLATE = app

INCLUDEPATH += $$PWD

LIBS += -lz
CONFIG += qt mobility

#DEFINES += TIMEDBG

#MOBILITY =
    #for test only
    DEFINES += MOBILE
    CONFIG += MOBILE
    DEFINES+= mobile
    CONFIG+= mobile


#Small hack for Quazip on windows
win32{
    DEFINES += QUAZIP_STATIC
}

android {
    INSTALL_PATH = "/assets/Orayta/"
    INSTALL_BOOKS_PATH = $${INSTALL_PATH}

    DEFINES += QTSCROLLER_NO_WEBKIT

    #this is a mobile app
    DEFINES += MOBILE
    DEFINES += android
    CONFIG += MOBILE
    CONFIG += android
    CONFIG += mobile
    CONFIG += Kookita

}

else: win32{
    INSTALL_PATH = quote(!:\\progarm files\\orayta\\) #TODO: set the root dynamicly or ask yoch to fix this.
    INSTALL_BOOKS_PATH = quote(!:\\progarm files\\orayta\\books)
    QT += webkitwidgets printsupport webkit
    
    # install fonts
    #fonts.path = $${INSTALL_PATH}"fonts/"
    fonts.path = $${INSTALL_PATH}
    fonts.files = fonts/.

}
else:unix{

    CONFIG += linux
    #CONFIG +=  poppler


    INSTALL_PATH = /usr/share/Orayta/
    INSTALL_BOOKS_PATH = $${INSTALL_PATH}"Books/"

    #Zlib
    LIBS += -lz

    QT += webkitwidgets # printsupport webkit

   QMAKE_CFLAGS_RELEASE+=$(shell dpkg-buildflags --get CFLAGS) $(shell dpkg-buildflags --get CPPFLAGS)
   QMAKE_CFLAGS_DEBUG+=$(shell dpkg-buildflags --get CFLAGS) $(shell dpkg-buildflags --get CPPFLAGS)
   QMAKE_CXXFLAGS_RELEASE+=$(shell dpkg-buildflags --get CFLAGS) $(shell dpkg-buildflags --get CPPFLAGS)
   QMAKE_CXXFLAGS_DEBUG+=$(shell dpkg-buildflags --get CFLAGS)
   QMAKE_CXXFLAGS_DEBUG+=$(shell dpkg-buildflags --get CPPFLAGS)
}


poppler {
    DEFINES += POPPLER

    # The following are the correct include and library paths for poppler on my system (Ubuntu 10.10).
    # Modify these to refer to the directories on your system
    # that contain the poppler-qt4.h header file and [lib]poppler-qt4 library.
    INCLUDEPATH += /usr/include/poppler/qt4
    LIBS += -lpoppler-qt4

    #Fribidi
    LIBS += -lfribidi

    SOURCES +=  Desktop/pdfwidget.cpp
    HEADERS +=  Desktop/pdfwidget.h
}

SOURCES +=  \
    main.cpp \
    OraytaBase/booklist.cpp \
    OraytaBase/bookfind.cpp \
    OraytaBase/htmlgen.cpp \
    OraytaBase/functions.cpp \
    OraytaBase/filedownloader.cpp \
    OraytaBase/book.cpp \
    OraytaBase/bookiter.cpp \
    OraytaBase/search.cpp \
    OraytaBase/guematria.cpp \
    OraytaBase/quazip/quazip.cpp \
    OraytaBase/quazip/zip.c \
    OraytaBase/quazip/unzip.c \
    OraytaBase/quazip/quazipnewinfo.cpp \
    OraytaBase/quazip/quazipfile.cpp \
    OraytaBase/quazip/JlCompress.cpp \
    OraytaBase/quazip/quacrc32.cpp \
    OraytaBase/quazip/quaadler32.cpp \
    OraytaBase/quazip/qioapi.cpp \
    OraytaBase/minibmark.cpp \
    OraytaBase/bmarklist.cpp \


HEADERS += \
    OraytaBase/htmlgen.h \
    OraytaBase/functions.h \
    OraytaBase/filedownloader.h \
    OraytaBase/book.h \
    OraytaBase/bookiter.h \
    OraytaBase/booklist.h \
    OraytaBase/bookfind.h \
    OraytaBase/guematria.h \
    OraytaBase/search.h \
    OraytaBase/quazip/quazip.h \
    OraytaBase/quazip/zip.h \
    OraytaBase/quazip/unzip.h \
    OraytaBase/quazip/quazipnewinfo.h \
    OraytaBase/quazip/quazipfileinfo.h \
    OraytaBase/quazip/quazipfile.h \
    OraytaBase/quazip/quazip_global.h \
    OraytaBase/quazip/JlCompress.h \
    OraytaBase/quazip/ioapi.h \
    OraytaBase/quazip/crypt.h \
    OraytaBase/quazip/quacrc32.h \
    OraytaBase/quazip/quachecksum32.h \
    OraytaBase/quazip/quaadler32.h \
    OraytaBase/minibmark.h \
    OraytaBase/bmarklist.h

FORMS += \
    OraytaBase/bookfind.ui

!mobile{
    message("Compiling for desktop")
    HEADERS += \
        Desktop/desktopapp.h \
        Desktop/mywebview.h \
        Desktop/mytreetab.h \
        Desktop/bookdisplayer.h \
        Desktop/addcomment.h \
        Desktop/about.h \
        Desktop/errorreport.h \
        Desktop/settings.h \
        Desktop/importbook.h

    SOURCES += \
        Desktop/desktopapp.cpp \
        Desktop/mywebview.cpp \
        Desktop/mytreetab.cpp \
        Desktop/bookdisplayer.cpp \
        Desktop/addcomment.cpp \
        Desktop/about.cpp \
        Desktop/errorreport.cpp \
        Desktop/settings.cpp \
        Desktop/importbook.cpp \
        Desktop/desktopapp_bookmark.cpp

    FORMS += \
        Desktop/addcomment.ui \
        Desktop/about.ui \
        Desktop/errorreport.ui \
        Desktop/settings.ui \
        Desktop/importbook.ui \
        Desktop/desktopapp.ui
}

#Android stuff:
mobile {
    message("Compiling for mobile")
    DEFINES += QTSCROLLER_NO_WEBKIT
    DEFINES += MOBILE

RESOURCES += Mobile/mobile.qrc

    SOURCES += \
        Mobile/mobileapp.cpp \
        Mobile/textdisplayer.cpp \
        Mobile/mobileapp_download.cpp\
        Mobile/swipegesturerecognizer.cpp \
        Mobile/mobileapp_bookmark.cpp

    HEADERS += \
        Mobile/mobileapp.h \
        Mobile/textdisplayer.h \
        Mobile/swipegesturerecognizer.h \
        Mobile/jnifunc.h

    FORMS += \
        Mobile/mobileapp.ui \

  OTHER_FILES += \
      android-orayta/src/org/qtproject/qt5/android/bindings/QtActivity.java \
      android-orayta/src/org/qtproject/qt5/android/bindings/QtApplication.java \
      android-orayta/src/org/qtproject/qt5/android/bindings/Crypter.java \
      android-orayta/version.xml \
      android-orayta/AndroidManifest.xml
}

Kookita{
    DEFINES += KOOKITA
    SOURCES += Mobile/jnifunc.cpp
    HEADERS += Mobile/jnifunc.h
}



RESOURCES += Orayta.qrc
win32:RC_FILE = orayta.rc


TRANSLATIONS = Hebrew.ts \
    French.ts

# Install binary
target.path = /usr/bin

# Install books
books.path = $${INSTALL_BOOKS_PATH}
books.files = Books/.
#android: books.files += android/assets/.

# Install icon
icon.path = $${INSTALL_PATH}

icon.files = Icons/Orayta.png
#Install wait image
icon.files += Images/Wait.gif

#Install user css sample
css.path = $${INSTALL_PATH}
css.files = css/.

linux {
    # Desktop shortcut
    desktop.path = $${XDG_APPS_INSTALL_DIR}
    desktop.files = Orayta.desktop

    # Install shortcut
    menu.path = /usr/share/applications
    menu.files = Orayta.desktop
}

# Install translation
trans.path = $${INSTALL_PATH}
trans.files = Hebrew.qm

# copy licence info
licence.path = $${INSTALL_PATH}
licence.files = licence/.

INSTALLS += target
INSTALLS += books
message("Books target set to:" $${INSTALL_BOOKS_PATH})
INSTALLS += icon
INSTALLS += trans

#fonts are now in the qrc file, no need for them in assets.
INSTALLS += fonts
INSTALLS += licence

INSTALLS += css

linux: INSTALLS += menu desktop

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-orayta

