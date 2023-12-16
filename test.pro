QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO




# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/../openwrt-2305/staging_dir/target-mipsel_24kc_glibc/usr/include
LIBS += -L $$PWD/../openwrt-2305/staging_dir/target-mipsel_24kc_glibc/usr/lib/

LIBS += -lpcre2-16

LIBS += -lpjsua-mipsel-openwrt-linux-gnu
LIBS += -lpjsip-ua-mipsel-openwrt-linux-gnu
LIBS += -lpjsip-simple-mipsel-openwrt-linux-gnu
LIBS += -lpjsip-mipsel-openwrt-linux-gnu
LIBS += -lpjmedia-codec-mipsel-openwrt-linux-gnu
LIBS += -lpjmedia-mipsel-openwrt-linux-gnu
LIBS += -lpjmedia-videodev-mipsel-openwrt-linux-gnu
LIBS += -lpjmedia-audiodev-mipsel-openwrt-linux-gnu
LIBS += -lpjmedia-mipsel-openwrt-linux-gnu
LIBS += -lpjnath-mipsel-openwrt-linux-gnu
LIBS += -lpjlib-util-mipsel-openwrt-linux-gnu
LIBS += -lpj-mipsel-openwrt-linux-gnu

# 这些是编译 pjproject 时自带的第三方库
LIBS += -lsrtp-mipsel-openwrt-linux-gnu
LIBS += -lresample-mipsel-openwrt-linux-gnu
LIBS += -lgsmcodec-mipsel-openwrt-linux-gnu
LIBS += -lspeex-mipsel-openwrt-linux-gnu
LIBS += -lilbccodec-mipsel-openwrt-linux-gnu
LIBS += -lg7221codec-mipsel-openwrt-linux-gnu
LIBS += -lyuv-mipsel-openwrt-linux-gnu
LIBS += -lwebrtc-mipsel-openwrt-linux-gnu

LIBS += $$PWD/../openwrt-2305/staging_dir/toolchain-mipsel_24kc_gcc-13.1.0_glibc/lib/librt.a
# 这两个库默认的musl系列交叉工具链编译出来的openwrt没有，必须glibc才有
LIBS += $$PWD/../openwrt-2305/staging_dir/toolchain-mipsel_24kc_gcc-13.1.0_glibc/lib/libm.so
LIBS += $$PWD/../openwrt-2305/staging_dir/toolchain-mipsel_24kc_gcc-13.1.0_glibc/lib/libc.so

LIBS += $$PWD/../openwrt-2305/staging_dir/target-mipsel_24kc_glibc/usr/lib/libssl.so
LIBS += $$PWD/../openwrt-2305/staging_dir/target-mipsel_24kc_glibc/usr/lib/libcrypto.so

# 必须在 openwrt make menuconfig 选上 alsa-lib 才会出现
LIBS += $$PWD/../openwrt-2305/staging_dir/target-mipsel_24kc_glibc/usr/lib/libasound.so


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
