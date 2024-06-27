QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    economie.cpp \
    main.cpp \
    mainqt.cpp \
    sqlite3.c

HEADERS += \
    economie.h \
    mainqt.h \
    sqlite3.h

FORMS += \
    mainqt.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

TRANSLATIONS += \
    economie_cpp_ja_JP.ts \
    economie_cpp_ko_KR.ts

DEFINES += ko \
    linux
