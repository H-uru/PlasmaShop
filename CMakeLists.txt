cmake_minimum_required(VERSION 3.1)
project(PlasmaShop)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC TRUE)

if(POLICY CMP0071)
    # Don't run generated source files (bytes/python_*.cpp) through automoc
    cmake_policy(SET CMP0071 OLD)
endif()

find_package(HSPlasma REQUIRED)
find_package(string_theory 2.0 REQUIRED)
find_package(Qt5 5.8 REQUIRED COMPONENTS Core Gui Widgets)
find_package(KF5SyntaxHighlighting REQUIRED)
find_package(PythonInterp REQUIRED)

if(KF5SyntaxHighlighting_VERSION VERSION_LESS "5.39.0")
    message(WARNING "KF5SyntaxHighlighting version 5.39.0 or later is required \
in order to use custom highlighting rules.  KF5SyntaxHighlighting version \
${KF5SyntaxHighlighting_VERSION} was detected, so these rules will be unavailable.")
    add_definitions("-DKSYN_NO_CUSTOM_RULES_SUPPORT")
endif()

if(MSVC)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS")
    # Fix <windows.h> doing stupid stuff to QDateTime's header
    add_definitions("/DNOMINMAX")
endif()

if(WIN32)
    # Requires Vista, Office 2007, or Visual Studio 2010 (or manual install).
    # If you're still on XP, who knows what you'll get (Probably Courier)
    add_definitions(-DPLAT_FONT="Consolas")
else()
    add_definitions(-DPLAT_FONT="Monospace")
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR
        CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(WARNING_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${WARNING_FLAGS} ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${WARNING_FLAGS} ${CMAKE_C_FLAGS}")
endif()

# Get the version number from git
set(PlasmaShop_VERSION "3.0-untracked")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    find_program(GIT_EXECUTABLE NAMES git git.cmd)
    mark_as_advanced(GIT_EXECUTABLE)
    if(GIT_EXECUTABLE)
        execute_process(COMMAND ${GIT_EXECUTABLE} describe --dirty
                        OUTPUT_VARIABLE PlasmaShop_VERSION
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    endif()
endif()
add_definitions(-DPLASMASHOP_VERSION="${PlasmaShop_VERSION}")

set(QTEXTPAD_WIDGET_ONLY ON)
add_subdirectory(qtextpad)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/qtextpad/lib")

add_subdirectory(src)

if(WIN32)
    add_subdirectory(icons/win32)
endif()
