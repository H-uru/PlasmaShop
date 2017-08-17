include_directories("${PROJECT_SOURCE_DIR}/src")

include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty")
include_directories("${HSPlasma_INCLUDE_DIRS}")

set(PSCommon_Headers
    QPlasma.h
    QColorEdit.h
    QHexWidget.h
    QLinkLabel.h
    QNumerics.h
)

set(PSCommon_Sources
    QColorEdit.cpp
    QHexWidget.cpp
    QLinkLabel.cpp
    QNumerics.cpp
)

add_library(PSCommon STATIC ${PSCommon_Headers} ${PSCommon_Sources})
target_link_libraries(PSCommon HSPlasma Qt5::Widgets)

if(NOT WIN32 AND NOT APPLE)
    set(PSIconLoader_Headers
        3rdParty/qticonloader.h
    )

    set(PSIconLoader_Sources
        3rdParty/qticonloader.cpp
    )

    add_library(PSIconLoader STATIC ${PSIconLoader_Headers} ${PSIconLoader_Sources})
    target_link_libraries(PSIconLoader Qt5::Gui)
endif()

add_subdirectory(PlasmaShop)
add_subdirectory(PrpShop)
add_subdirectory(VaultShop)
