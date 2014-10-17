QT += core
QT += network sql
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

TARGET = "COPS v7 Emulator"
TEMPLATE = app
CONFIG   += console

macx {
CONFIG -= app_bundle
}

lessThan(QT_MAJOR_VERSION, 4): \
    error(COPS v7 Emulator requires Qt 4.5 or newer but Qt $$[QT_VERSION] was detected.)

equals(QT_MAJOR_VERSION, 4): \
    lessThan(QT_MINOR_VERSION, 5): \
        error(COPS v7 Emulator requires Qt 4.5 or newer but Qt $$[QT_VERSION] was detected.)

QMAKE_CFLAGS_RELEASE += -DNDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp

SOURCES += \
    src/basefunc.cpp \
    src/client.cpp \
    src/generator.cpp \
    src/inifile.cpp \
    src/item.cpp \
    src/magic.cpp \
    src/program.cpp \
    src/server.cpp \
    src/weaponskill.cpp \
    src/world.cpp \
    src/Common/env.cpp \
    src/Common/log.cpp \
    src/Database/database.cpp \
    src/Entity/advancedentity.cpp \
    src/Entity/entity.cpp \
    src/Entity/monster.cpp \
    src/Entity/npc.cpp \
    src/Entity/player.cpp \
    src/IO/binaryreader.cpp \
    src/IO/finder.cpp \
    src/Map/gamemap.cpp \
    src/Map/mapdata.cpp \
    src/Map/mapmanager.cpp \
    src/Network/Sockets/networkclient.cpp \
    src/Network/Sockets/tcpserver.cpp \
    src/Network/msg.cpp \
    src/Network/msgaccount.cpp \
    src/Network/msgaction.cpp \
    src/Network/msgallot.cpp \
    src/Network/msgconnect.cpp \
    src/Network/msgconnectex.cpp \
    src/Network/msgdate.cpp \
    src/Network/msgdialog.cpp \
    src/Network/msgflushexp.cpp \
    src/Network/msgfriendinfo.cpp \
    src/Network/msgitem.cpp \
    src/Network/msgiteminfo.cpp \
    src/Network/msgloginchallenges.cpp \
    src/Network/msgloginproofa.cpp \
    src/Network/msgmagicinfo.cpp \
    src/Network/msgmapinfo.cpp \
    src/Network/msgnpc.cpp \
    src/Network/msgnpcinfo.cpp \
    src/Network/msgplayer.cpp \
    src/Network/msgregister.cpp \
    src/Network/msgtalk.cpp \
    src/Network/msgtick.cpp \
    src/Network/msguserattrib.cpp \
    src/Network/msguserinfo.cpp \
    src/Network/msgwalk.cpp \
    src/Network/msgweaponskill.cpp \
    src/Network/msgweather.cpp \
    src/Network/stringpacker.cpp \
    src/Script/itemtask.cpp \
    src/Script/npctask.cpp \
    src/Script/script.cpp \
    src/Security/Cryptography/blowfish.cpp \
    src/Security/Cryptography/cast5.cpp \
    src/Security/Cryptography/diffiehellman.cpp \
    src/Security/Cryptography/rc5.cpp \
    src/Security/Cryptography/tqcipher.cpp \
    src/third_party/bigint/bigint_int.c \
    src/third_party/bigint/bigint_ext.c \
    src/third_party/lua-5.2.3/src/lzio.c \
    src/third_party/lua-5.2.3/src/lvm.c \
    src/third_party/lua-5.2.3/src/lundump.c \
    src/third_party/lua-5.2.3/src/ltm.c \
    src/third_party/lua-5.2.3/src/ltablib.c \
    src/third_party/lua-5.2.3/src/ltable.c \
    src/third_party/lua-5.2.3/src/lstrlib.c \
    src/third_party/lua-5.2.3/src/lstring.c \
    src/third_party/lua-5.2.3/src/lstate.c \
    src/third_party/lua-5.2.3/src/lparser.c \
    src/third_party/lua-5.2.3/src/loslib.c \
    src/third_party/lua-5.2.3/src/lopcodes.c \
    src/third_party/lua-5.2.3/src/lobject.c \
    src/third_party/lua-5.2.3/src/loadlib.c \
    src/third_party/lua-5.2.3/src/lmem.c \
    src/third_party/lua-5.2.3/src/lmathlib.c \
    src/third_party/lua-5.2.3/src/llex.c \
    src/third_party/lua-5.2.3/src/liolib.c \
    src/third_party/lua-5.2.3/src/linit.c \
    src/third_party/lua-5.2.3/src/lgc.c \
    src/third_party/lua-5.2.3/src/lfunc.c \
    src/third_party/lua-5.2.3/src/ldump.c \
    src/third_party/lua-5.2.3/src/ldo.c \
    src/third_party/lua-5.2.3/src/ldebug.c \
    src/third_party/lua-5.2.3/src/ldblib.c \
    src/third_party/lua-5.2.3/src/lctype.c \
    src/third_party/lua-5.2.3/src/lcorolib.c \
    src/third_party/lua-5.2.3/src/lcode.c \
    src/third_party/lua-5.2.3/src/lbitlib.c \
    src/third_party/lua-5.2.3/src/lbaselib.c \
    src/third_party/lua-5.2.3/src/lauxlib.c \
    src/third_party/lua-5.2.3/src/lapi.c \
    src/third_party/lz4-r123/lz4.c

HEADERS += \
    src/basefunc.h \
    src/client.h \
    src/generator.h \
    src/inifile.h \
    src/item.h \
    src/magic.h \
    src/mstimer.h \
    src/server.h \
    src/strres.h \
    src/timer.h \
    src/weaponskill.h \
    src/world.h \
    src/Common/arch.h \
    src/Common/common.h \
    src/Common/endianness.h \
    src/Common/env.h \
    src/Common/err.h \
    src/Common/log.h \
    src/Common/types.h \
    src/Database/database.h \
    src/Entity/advancedentity.h \
    src/Entity/entity.h \
    src/Entity/monster.h \
    src/Entity/npc.h \
    src/Entity/player.h \
    src/IO/binaryreader.h \
    src/IO/finder.h \
    src/Map/gamemap.h \
    src/Map/mapbase.h \
    src/Map/mapdata.h \
    src/Map/mapmanager.h \
    src/Network/Sockets/networkclient.h \
    src/Network/Sockets/tcpserver.h \
    src/Network/allmsg.h \
    src/Network/msg.h \
    src/Network/msgaccount.h \
    src/Network/msgaction.h \
    src/Network/msgallot.h \
    src/Network/msgconnect.h \
    src/Network/msgconnectex.h \
    src/Network/msgdate.h \
    src/Network/msgdialog.h \
    src/Network/msgflushexp.h \
    src/Network/msgfriendinfo.h \
    src/Network/msgiteminfo.h \
    src/Network/msgitem.h \
    src/Network/msgloginchallenges.h \
    src/Network/msgloginproofa.h \
    src/Network/msgmagicinfo.h \
    src/Network/msgmapinfo.h \
    src/Network/msgnpc.h \
    src/Network/msgnpcinfo.h \
    src/Network/msgplayer.h \
    src/Network/msgregister.h \
    src/Network/msgtalk.h \
    src/Network/msgtick.h \
    src/Network/msguserattrib.h \
    src/Network/msguserinfo.h \
    src/Network/msgwalk.h \
    src/Network/msgweaponskill.h \
    src/Network/msgweather.h \
    src/Network/networkdef.h \
    src/Network/stringpacker.h \
    src/Script/itemtask.h \
    src/Script/npctask.h \
    src/Script/script.h \
    src/Security/Cryptography/blowfish.h \
    src/Security/Cryptography/cast5.h \
    src/Security/Cryptography/diffiehellman.h \
    src/Security/Cryptography/icipher.h \
    src/Security/Cryptography/rc5.h \
    src/Security/Cryptography/tqcipher.h \
    src/third_party/bigint/bigint.h \
    src/third_party/lua-5.2.3/src/lzio.h \
    src/third_party/lua-5.2.3/src/lvm.h \
    src/third_party/lua-5.2.3/src/lundump.h \
    src/third_party/lua-5.2.3/src/lualib.h \
    src/third_party/lua-5.2.3/src/luaconf.h \
    src/third_party/lua-5.2.3/src/lua.hpp \
    src/third_party/lua-5.2.3/src/lua.h \
    src/third_party/lua-5.2.3/src/ltm.h \
    src/third_party/lua-5.2.3/src/ltable.h \
    src/third_party/lua-5.2.3/src/lstring.h \
    src/third_party/lua-5.2.3/src/lstate.h \
    src/third_party/lua-5.2.3/src/lparser.h \
    src/third_party/lua-5.2.3/src/lopcodes.h \
    src/third_party/lua-5.2.3/src/lobject.h \
    src/third_party/lua-5.2.3/src/lmem.h \
    src/third_party/lua-5.2.3/src/llimits.h \
    src/third_party/lua-5.2.3/src/llex.h \
    src/third_party/lua-5.2.3/src/lgc.h \
    src/third_party/lua-5.2.3/src/lfunc.h \
    src/third_party/lua-5.2.3/src/ldo.h \
    src/third_party/lua-5.2.3/src/ldebug.h \
    src/third_party/lua-5.2.3/src/lctype.h \
    src/third_party/lua-5.2.3/src/lcode.h \
    src/third_party/lua-5.2.3/src/lauxlib.h \
    src/third_party/lua-5.2.3/src/lapi.h \
    src/third_party/lz4-r123/lz4.h

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
    src/third_party/lua-5.2.3/src \
    src/third_party/lz4-r123

OTHER_FILES += \
    data/settings.cfg \
    CHANGES \
    LICENSE \
    README

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
