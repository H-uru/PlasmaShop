project(PlasmaShop)
cmake_minimum_required(VERSION 2.8.11.2)

find_package(HSPlasma REQUIRED)
find_package(string_theory REQUIRED)

option(PlasmaShop_FORCE_QT4 "Force Qt4 build even if Qt5 can be found" OFF)
if(NOT PlasmaShop_FORCE_QT4)
    find_package(Qt5Core)
endif()

if(Qt5Core_FOUND)
    set(PS_USE_QT5 TRUE)
    macro(QTX_WRAP_CPP dest files)
        qt5_wrap_cpp(${dest} ${files})
    endmacro()
    macro(QTX_ADD_RESOURCES dest files)
        qt5_add_resources(${dest} ${files})
    endmacro()
else()
    ### UGLY:  Qt4's CMake module can only be run once.  Therefore, we must
    ###        find all required modules now, as subsequent finds won't work.
    ###        Now I understand better why Qt5 split them up more sanely
    find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL)
    if(QT4_FOUND)
        set(PS_USE_QT4 TRUE)
        include(${QT_USE_FILE})
        macro(QTX_WRAP_CPP dest files)
            qt4_wrap_cpp(${dest} ${files})
        endmacro()
        macro(QTX_ADD_RESOURCES dest files)
            qt4_add_resources(${dest} ${files})
        endmacro()
    else()
        message(FATAL_ERROR "Either Qt4 or Qt5 is required to compile PlasmaShop")
    endif()
endif()

if(MSVC)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS")
    # Fix <windows.h> doing stupid stuff to QDateTime's header
    add_definitions("/DNOMINMAX")
endif()
add_subdirectory(QScintilla/Qt4Qt5)

if(WIN32)
    add_definitions(-DPLAT_FONT="Courier New")
else()
    add_definitions(-DPLAT_FONT="Monospace")
endif()

# do this after QScintilla - it generates tons of warnings
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR
        CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(WARNING_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS "-std=c++0x ${WARNING_FLAGS} ${CMAKE_CXX_FLAGS}")
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

include_directories(${STRING_THEORY_INCLUDE_DIRS})

add_subdirectory(src/PlasmaShop)
add_subdirectory(src/PrpShop)
add_subdirectory(src/VaultShop)

if(WIN32)
    add_subdirectory(icons/win32)
endif()
