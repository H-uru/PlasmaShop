cmake_minimum_required(VERSION 2.6)
# make Release mode the default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
project(PLASMASHOP)
find_package(HSPlasma REQUIRED)
find_package(Qt4 REQUIRED)

if(WIN32)
	add_definitions(-DPLAT_FONT="Courier New")
else(WIN32)
	add_definitions(-DPLAT_FONT="Droid Sans Mono")
	add_definitions(-DDATA_PATH="/usr/local/share/PlasmaShop")
endif(WIN32)

add_subdirectory(QScintilla/Qt4)

# do this after QScintilla - it'd generate tons of warnings
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "-Wall -Wextra ${CMAKE_C_FLAGS}")
endif()


add_subdirectory (src/PlasmaShop)
add_subdirectory (src/VaultShop)
add_subdirectory (src/PrpShop)

