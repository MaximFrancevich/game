QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    characteristics/skills/skill.cpp \
    characteristics/baseattribute.cpp \
    characteristics/characteristic.cpp \
    interface/attackbutton.cpp \
    interface/attributeswindow.cpp \
    interface/buttonscreater.cpp \
    interface/enemylayout.cpp \
    interface/fightfriendlypersonlayout.cpp \
    interface/fightpersonlayout.cpp \
    interface/hex.cpp \
    interface/mainwindow.cpp \
    interface/personlayout.cpp \
    interface/playerattribute.cpp \
    interface/questlayout.cpp \
    interface/skilllayout.cpp \
    interface/skillswindow.cpp \
    allattributes.cpp \
    bonus.cpp \
    enemysgenerator.cpp \
    game.cpp \
    main.cpp \
    mfile.cpp \
    person.cpp \
    quest.cpp \
    record.cpp \
    staticattribute.cpp \
    uniquefilenamegenerator.cpp

HEADERS += \
    characteristics/skills/skill.h \
    characteristics/baseattribute.h \
    characteristics/characteristic.h \
    interface/attackbutton.h \
    interface/attributeswindow.h \
    interface/buttonscreater.h \
    interface/enemylayout.h \
    interface/fightfriendlypersonlayout.h \
    interface/fightpersonlayout.h \
    interface/hex.h \
    interface/mainwindow.h \
    interface/personlayout.h \
    interface/playerattribute.h \
    interface/questlayout.h \
    interface/skilllayout.h \
    interface/skillswindow.h \
    allattributes.h \
    bonus.h \
    constants.h \
    enemysgenerator.h \
    fightpersonlayout.h \
    game.h \
    mfile.h \
    person.h \
    quest.h \
    record.h \
    staticattribute.h \
    uniquefilenamegenerator.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
