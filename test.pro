QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO




# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /usr/local/include
LIBS += -L /usr/local/lib/


LIBS += -lpjsua-x86_64-unknown-linux-gnu
LIBS += -lpjsip-ua-x86_64-unknown-linux-gnu
LIBS += -lpjsip-simple-x86_64-unknown-linux-gnu
LIBS += -lpjsip-x86_64-unknown-linux-gnu
LIBS += -lpjmedia-codec-x86_64-unknown-linux-gnu
LIBS += -lpjmedia-x86_64-unknown-linux-gnu
LIBS += -lpjmedia-videodev-x86_64-unknown-linux-gnu
LIBS += -lpjmedia-audiodev-x86_64-unknown-linux-gnu
LIBS += -lpjmedia-x86_64-unknown-linux-gnu
LIBS += -lpjnath-x86_64-unknown-linux-gnu
LIBS += -lpjlib-util-x86_64-unknown-linux-gnu
LIBS += -lsrtp-x86_64-unknown-linux-gnu
LIBS += -lresample-x86_64-unknown-linux-gnu
LIBS += -lgsmcodec-x86_64-unknown-linux-gnu
LIBS += -lspeex-x86_64-unknown-linux-gnu
LIBS += -lilbccodec-x86_64-unknown-linux-gnu
LIBS += -lg7221codec-x86_64-unknown-linux-gnu
LIBS += -lyuv-x86_64-unknown-linux-gnu
LIBS += -lwebrtc-x86_64-unknown-linux-gnu
LIBS += -lpj-x86_64-unknown-linux-gnu



LIBS += /usr/lib/x86_64-linux-gnu/libssl.so
LIBS += /usr/lib/x86_64-linux-gnu/libcrypto.so
LIBS += /usr/lib/x86_64-linux-gnu/libm.so
LIBS += /usr/lib/x86_64-linux-gnu/librt.a
LIBS += /usr/lib/x86_64-linux-gnu/libasound.so
LIBS += /usr/lib/x86_64-linux-gnu/libc.so
SOURCES += \
    formactivecalls.cpp \
    main.cpp \
    mycpp.cpp \
    mywork.cpp \
    phdialog.cpp \
    pjsua_app.c \
    pjsua_app_cli.c \
    pjsua_app_common.c \
    pjsua_app_config.c \
    qt_main.c \
    qt_pjsip_interface.c

HEADERS += \
    formactivecalls.h \
    mycpp.h \
    mywork.h \
    phdialog.h \
    pjsua_app.h \
    pjsua_app_common.h \
    pjsua_app_config.h \
    qt_pjsip_interface.h

FORMS += \
    formactivecalls.ui \
    phdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    myicons.qrc
