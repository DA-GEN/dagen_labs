QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Архітектурний поділ (To-Be): чиста C++ бібліотека ігрової логіки (без Qt)
# лежить у core/include, а Qt-специфічний шар - у ui/.
INCLUDEPATH += \
    core/include \
    ui

SOURCES += \
    ui/main.cpp \
    ui/mainwindow.cpp \
    ui/GameController.cpp

HEADERS += \
    core/include/Archer.hpp \
    core/include/Armor.hpp \
    core/include/Character.hpp \
    core/include/Enemy.hpp \
    core/include/EnemyFactory.hpp \
    core/include/GameEngine.hpp \
    core/include/GameEvents.hpp \
    core/include/GameMap.hpp \
    core/include/Goblin.hpp \
    core/include/Graph.hpp \
    core/include/Item.hpp \
    core/include/ItemFactory.hpp \
    core/include/Mage.hpp \
    core/include/MapNode.hpp \
    core/include/Orc.hpp \
    core/include/Player.hpp \
    core/include/Potion.hpp \
    core/include/RandomProvider.hpp \
    core/include/Warrior.hpp \
    core/include/Weapon.hpp \
    core/include/Wraith.hpp \
    ui/GameController.h \
    ui/mainwindow.h

FORMS += \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
