include_directories("${PROJECT_SOURCE_DIR}/src")

include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty")
include_directories("${HSPlasma_INCLUDE_DIRS}")

set(PSCommon_Headers
    QPlasma.h
    QColorEdit.h
    QHexWidget.h
    QLinkLabel.h
    QNumerics.h
    QPlasmaTextEdit.h
)

set(PSCommon_Sources
    QColorEdit.cpp
    QHexWidget.cpp
    QLinkLabel.cpp
    QNumerics.cpp
    QPlasmaTextEdit.cpp
)

add_library(PSCommon STATIC ${PSCommon_Headers} ${PSCommon_Sources})
target_link_libraries(PSCommon PUBLIC HSPlasma Qt5::Widgets qtextpad)

add_subdirectory(PlasmaShop)
add_subdirectory(PrpShop)
add_subdirectory(VaultShop)
