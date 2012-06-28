project(PlasmaShop)
cmake_minimum_required(VERSION 2.6)

find_package(HSPlasma REQUIRED)

if(MSVC)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()
add_subdirectory(QScintilla/Qt4)

if(WIN32)
    add_definitions(-DPLAT_FONT="Courier New")
else(WIN32)
    add_definitions(-DPLAT_FONT="Monospace")
    add_definitions(-DDATA_PATH="/usr/local/share/PlasmaShop")
endif(WIN32)

# do this after QScintilla - it generates tons of warnings
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(WARNING_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS "-std=c++0x ${WARNING_FLAGS} ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${WARNING_FLAGS} ${CMAKE_C_FLAGS}")
endif()

add_subdirectory(src/PlasmaShop)
add_subdirectory(src/PrpShop)
add_subdirectory(src/VaultShop)

if(WIN32)
    add_subdirectory(icons/win32)
endif()
