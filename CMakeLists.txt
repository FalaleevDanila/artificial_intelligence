cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(Kursach)

find_package(
        Qt5
        HINTS "$ENV{QTDIR}"
        REQUIRED COMPONENTS
        Gui
        Core
        Widgets
)


add_executable(
        ${PROJECT_NAME}
        sources/bullet.cpp
        sources/gun.cpp
        sources/main.cpp
        sources/mainwindow.cpp
        sources/man.cpp
        sources/mousecontrol.cpp
        sources/npc.cpp
        sources/player.cpp
        sources/widget.cpp

        include/bullet.h
        include/widget.h
        include/way.h
        include/npc.h
        include/mainwindow.h
        include/gun.h
        include/npc_way.h
        include/player.h
        include/mousecontrol.h

        sources/mainwindow.ui
        sources/widget.ui
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        "D:/qt/5.12.3/mingw73_64/include/"
        "D:/qt/5.12.3/mingw73_64/include/QtCore"
        "D:/qt/5.12.3/mingw73_64/include/QtWidgets"
        "D:/qt/5.12.3/mingw73_64/include/QtGui"
)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOUIC TRUE)

target_link_libraries(
        ${PROJECT_NAME}
        D:/qt/5.12.3/mingw73_64/lib/libQt5Core.a
        D:/qt/5.12.3/mingw73_64/lib/libQt5Gui.a
        D:/qt/5.12.3/mingw73_64/lib/libQt5Widgets.a
)
