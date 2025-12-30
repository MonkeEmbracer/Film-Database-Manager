cmake_minimum_required(VERSION 3.28)
project(a10_MonkeEmbracer_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(Domain)
include_directories(Exceptions)
include_directories(Repo)
include_directories(Service)
include_directories(Tests)
include_directories(GUI)
include_directories(Validator)
include_directories(Watchlist)

add_executable(a10_MonkeEmbracer_1
    Domain/Film.h
    Exceptions/Exceptions.h
    Repo/Repo.cpp
    Repo/Repo.h
    Service/Service.cpp
    Service/Service.h
    Tests/Tests.cpp
    Tests/Tests.h
        GUI/GUI.cpp
        GUI/GUI.h
        UI/UI.cpp
        UI/UI.h
    Validator/Validator.cpp
    Validator/Validator.h
    Watchlist/Watchlist.cpp
    Watchlist/Watchlist.h
    Start.cpp
        GUI/adminGUI.cpp
        GUI/adminGUI.h
        GUI/userGUI.cpp
        GUI/userGUI.h
        GUI/WatchlistModel.cpp
        GUI/WatchlistModel.h
        Service/Undo.cpp
        Service/Undo.h)

# QT config
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "D:/Tools/Qt/6.9.0/mingw_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

target_link_libraries(a10_MonkeEmbracer_1
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif()
    endif()
    if(EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif()
    foreach(QT_LIB Core UI Widgets )
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach(QT_LIB)
endif()
