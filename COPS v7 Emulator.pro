QT += core
QT += network sql
QT -= gui

TARGET = "COPS v7 Emulator"
TEMPLATE = app
CONFIG   += console

macx {
CONFIG -= app_bundle
#CONFIG += ppc ppc64 x86 x86_64
}

# Check Qt version
QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0)
QT_VER_MIN = $$member(QT_VERSION, 1)

lessThan(QT_VER_MAJ, 4) | lessThan(QT_VER_MIN, 7) {
   error(Faith Emulator requires Qt 4.7 or newer but Qt $$[QT_VERSION] was detected.)
}

OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp

SOURCES += \
    src/server.cpp \
    src/program.cpp \
    src/client.cpp \
    src/Network/Sockets/tcpserver.cpp \
    src/Network/Sockets/networkclient.cpp \
    src/Security/Cryptography/blowfish.cpp \
    src/Security/Cryptography/fnv.cpp \
    src/Security/Cryptography/tqcipher.cpp \
    src/Network/msg.cpp \
    src/Network/msgaccount.cpp \
    src/Network/msgconnect.cpp \
    src/Network/msgtalk.cpp \
    src/Network/stringpacker.cpp \
    src/Network/msgaction.cpp \
    src/Network/msguserinfo.cpp \
    src/Network/msguserattrib.cpp \
    src/Database/database.cpp \
    src/Network/msgiteminfo.cpp \
    src/Security/Cryptography/rc5.cpp \
    src/Entity/entity.cpp \
    src/Entity/player.cpp \
    src/Network/msgwalk.cpp \
    src/Map/mapmanager.cpp \
    src/Map/gamemap.cpp \
    src/IO/binaryreader.cpp \
    src/IO/finder.cpp \
    src/Network/msgplayer.cpp \
    src/Network/msgnpcinfo.cpp \
    src/Entity/npc.cpp \
    src/Entity/advancedentity.cpp \
    src/Entity/monster.cpp \
    src/world.cpp \
    src/Common/env.cpp \
    src/item.cpp \
    src/third_party/lua-5.2.2/src/lzio.c \
    src/third_party/lua-5.2.2/src/lvm.c \
    src/third_party/lua-5.2.2/src/lundump.c \
    src/third_party/lua-5.2.2/src/ltm.c \
    src/third_party/lua-5.2.2/src/ltablib.c \
    src/third_party/lua-5.2.2/src/ltable.c \
    src/third_party/lua-5.2.2/src/lstrlib.c \
    src/third_party/lua-5.2.2/src/lstring.c \
    src/third_party/lua-5.2.2/src/lstate.c \
    src/third_party/lua-5.2.2/src/lparser.c \
    src/third_party/lua-5.2.2/src/loslib.c \
    src/third_party/lua-5.2.2/src/lopcodes.c \
    src/third_party/lua-5.2.2/src/lobject.c \
    src/third_party/lua-5.2.2/src/loadlib.c \
    src/third_party/lua-5.2.2/src/lmem.c \
    src/third_party/lua-5.2.2/src/lmathlib.c \
    src/third_party/lua-5.2.2/src/llex.c \
    src/third_party/lua-5.2.2/src/liolib.c \
    src/third_party/lua-5.2.2/src/linit.c \
    src/third_party/lua-5.2.2/src/lgc.c \
    src/third_party/lua-5.2.2/src/lfunc.c \
    src/third_party/lua-5.2.2/src/ldump.c \
    src/third_party/lua-5.2.2/src/ldo.c \
    src/third_party/lua-5.2.2/src/ldebug.c \
    src/third_party/lua-5.2.2/src/ldblib.c \
    src/third_party/lua-5.2.2/src/lctype.c \
    src/third_party/lua-5.2.2/src/lcorolib.c \
    src/third_party/lua-5.2.2/src/lcode.c \
    src/third_party/lua-5.2.2/src/lbitlib.c \
    src/third_party/lua-5.2.2/src/lbaselib.c \
    src/third_party/lua-5.2.2/src/lauxlib.c \
    src/third_party/lua-5.2.2/src/lapi.c \
    src/Script/script.cpp \
    src/Script/npctask.cpp \
    src/Network/msgnpc.cpp \
    src/Network/msgdialog.cpp \
    src/Map/mapdata.cpp \
    src/Common/log.cpp \
    src/inifile.cpp \
    src/Network/msgregister.cpp \
    src/basefunc.cpp \
    src/Script/itemtask.cpp \
    src/Network/msgallot.cpp \
    src/Network/msgtick.cpp \
    src/third_party/minilzo-2.06/minilzo.c \
    src/Network/msgmapinfo.cpp \
    src/Security/Cryptography/diffiehellman.cpp \
    src/third_party/bigint/bigint_int.c \
    src/third_party/bigint/bigint_ext.c

HEADERS += \
    src/server.h \
    src/client.h \
    src/world.h \
    src/item.h \
    src/strres.h \
    src/Database/database.h \
    src/Entity/entity.h \
    src/Entity/player.h \
    src/Entity/npc.h \
    src/Entity/advancedentity.h \
    src/Entity/monster.h \
    src/IO/binaryreader.h \
    src/IO/finder.h \
    src/Map/mapmanager.h \
    src/Map/gamemap.h \
    src/Network/Sockets/tcpserver.h \
    src/Network/Sockets/networkclient.h \
    src/Network/msg.h \
    src/Network/msgaccount.h \
    src/Network/networkdef.h \
    src/Network/msgconnect.h \
    src/Network/allmsg.h \
    src/Network/msgtalk.h \
    src/Network/stringpacker.h \
    src/Network/msgaction.h \
    src/Network/msguserinfo.h \
    src/Network/msguserattrib.h \
    src/Network/msgiteminfo.h \
    src/Network/msgwalk.h \
    src/Network/msgplayer.h \
    src/Network/msgnpcinfo.h \
    src/Network/msgnpc.h \
    src/Network/msgdialog.h \
    src/Script/script.h \
    src/Script/npctask.h \
    src/Security/Cryptography/blowfish.h \
    src/Security/Cryptography/fnv.h \
    src/Security/Cryptography/tqcipher.h \
    src/Security/Cryptography/rc5.h \
    src/third_party/lua-5.2.2/src/lzio.h \
    src/third_party/lua-5.2.2/src/lvm.h \
    src/third_party/lua-5.2.2/src/lundump.h \
    src/third_party/lua-5.2.2/src/lualib.h \
    src/third_party/lua-5.2.2/src/luaconf.h \
    src/third_party/lua-5.2.2/src/lua.hpp \
    src/third_party/lua-5.2.2/src/lua.h \
    src/third_party/lua-5.2.2/src/ltm.h \
    src/third_party/lua-5.2.2/src/ltable.h \
    src/third_party/lua-5.2.2/src/lstring.h \
    src/third_party/lua-5.2.2/src/lstate.h \
    src/third_party/lua-5.2.2/src/lparser.h \
    src/third_party/lua-5.2.2/src/lopcodes.h \
    src/third_party/lua-5.2.2/src/lobject.h \
    src/third_party/lua-5.2.2/src/lmem.h \
    src/third_party/lua-5.2.2/src/llimits.h \
    src/third_party/lua-5.2.2/src/llex.h \
    src/third_party/lua-5.2.2/src/lgc.h \
    src/third_party/lua-5.2.2/src/lfunc.h \
    src/third_party/lua-5.2.2/src/ldo.h \
    src/third_party/lua-5.2.2/src/ldebug.h \
    src/third_party/lua-5.2.2/src/lctype.h \
    src/third_party/lua-5.2.2/src/lcode.h \
    src/third_party/lua-5.2.2/src/lauxlib.h \
    src/third_party/lua-5.2.2/src/lapi.h \
    src/Map/mapdata.h \
    src/Map/mapbase.h \
    src/Common/env.h \
    src/Common/log.h \
    src/Common/types.h \
    src/Common/err.h \
    src/Common/arch.h \
    src/Common/common.h \
    src/Common/endianness.h \
    src/inifile.h \
    src/Network/msgregister.h \
    src/basefunc.h \
    src/Script/itemtask.h \
    src/Network/msgallot.h \
    src/Network/msgtick.h \
    src/third_party/minilzo-2.06/minilzo.h \
    src/third_party/minilzo-2.06/lzodefs.h \
    src/third_party/minilzo-2.06/lzoconf.h \
    src/Network/msgmapinfo.h \
    src/Security/Cryptography/diffiehellman.h \
    src/third_party/bigint/bigint.h

INCLUDEPATH += \
    src \
    src/Common \
    src/Database \
    src/Entity \
    src/IO \
    src/Map \
    src/Network \
    src/Network/Sockets \
    src/Script \
    src/Security/Cryptography \
    src/third_party/bigint \
    src/third_party/lua-5.2.2/src \
    src/third_party/minilzo-2.06

OTHER_FILES += \
    data/settings.cfg

# WIN32 stuff
win32 {
#OTHER_FILES += \
#    res/win32.rc

#RC_FILE += \
#    res/win32.rc

QMAKE_CFLAGS += -D_CRT_SECURE_NO_WARNINGS
QMAKE_CXXFLAGS += -D_CRT_SECURE_NO_WARNINGS
}

# Mac OS X stuff
macx {

}

# UNIX stuff...
unix:!macx {

}

# UNIX-like stuff...
unix {
QMAKE_CFLAGS += -Wextra
QMAKE_CXXFLAGS += -Wextra
}


# copying data to build directory...
win32 {
    WIN_PWD = $${replace(PWD, /, \\)}
    WIN_OUT_PWD = $${replace(OUT_PWD, /, \\)}

    copyfiles.commands = "xcopy /e /y \"$${WIN_PWD}\\data\" \"$${WIN_OUT_PWD}\\\""
}

macx {
    copyfiles.commands = "cp -r \"$${PWD}/data/\" \"$${OUT_PWD}/\""
}

unix:!macx {
    copyfiles.commands = "cp -r \"$${PWD}/data/.\" \"$${OUT_PWD}\""
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
