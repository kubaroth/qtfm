QT+= core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(../qtfm.pri)

TARGET = $${QTFM_TARGET}
TARGET_NAME = $${QTFM_TARGET_NAME}
VERSION = $${QTFM_MAJOR}.$${QTFM_MINOR}.$${QTFM_PATCH}
TEMPLATE = app

DEPENDPATH += . src
INCLUDEPATH += . src ../libfm

#LIBS += -L../libfm -lQtFM
LIBS += -L../libfm ../libfm/libQtFM.a

HEADERS += src/mainwindow.h \
    src/mymodel.h \
    src/bookmarkmodel.h \
    src/icondlg.h \
    src/propertiesdlg.h \
    src/tabbar.h \
    src/mymodelitem.h \
    src/settingsdialog.h \
    src/customactionsmanager.h \
    src/processdialog.h \
    src/applicationdialog.h \
    src/tabsets.h
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/mymodel.cpp \
    src/bookmarks.cpp \
    src/icondlg.cpp \
    src/propertiesdlg.cpp \
    src/tabbar.cpp \
    src/mymodelitem.cpp \
    src/settingsdialog.cpp \
    src/customactionsmanager.cpp \
    src/processdialog.cpp \
    src/applicationdialog.cpp \
    src/actiondefs.cpp \
    src/actiontriggers.cpp \
    src/tabsets.cpp

OTHER_FILES += $${TARGET}.desktop
RESOURCES += ../$${TARGET}.qrc
macx {
    DEFINES += NO_DBUS NO_UDISKS
    RESOURCES += bundle/adwaita.qrc
    ICON = images/QtFM.icns
    QMAKE_INFO_PLIST = Info.plist
}

DEFINES += APP=\"\\\"$${TARGET}\\\"\"
DEFINES += APP_NAME=\"\\\"$${TARGET_NAME}\\\"\"
DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\"

unix:!macx {
    target.path = $${PREFIX}/bin
    desktop.files += $${TARGET}.desktop
    desktop.path += $${PREFIX}/share/applications
    docs.path += $${DOCDIR}/$${TARGET}-$${VERSION}
    docs.files += ../LICENSE ../README.md ../AUTHORS ../ChangeLog
    man.files += qtfm.1
    man.path += $${MANDIR}/man1
    INSTALLS += target desktop docs man

    hicolor.files = hicolor
    hicolor.path = $${PREFIX}/share/icons
    INSTALLS += hicolor

    !CONFIG(no_udisks) {
        exists(../libdisks/libdisks.pro) {
            INCLUDEPATH += ../libdisks
            LIBS += -L../libdisks -lDisks
        } else {
            CONFIG += link_pkgconfig
            PKGCONFIG += Disks
        }
    }
    CONFIG(no_udisks): DEFINES += NO_UDISKS
    CONFIG(no_dbus) {
        DEFINES += NO_DBUS
        DEFINES += NO_UDISKS
    }
    !CONFIG(no_dbus) : QT += dbus
}

lessThan(QT_MAJOR_VERSION, 5): LIBS += -lmagic
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(deploy) : DEFINES += DEPLOY

CONFIG(with_magick) {
    DEFINES += WITH_MAGICK
    CONFIG(magick7): DEFINES += MAGICK7
    MAGICK_CONFIG = Magick++
    !isEmpty(MAGICK_PC): MAGICK_CONFIG = $${MAGICK}
    PKGCONFIG += $${MAGICK_CONFIG}
    CONFIG(deploy): LIBS += `pkg-config --libs --static $${MAGICK_CONFIG}`
}
