set(PlasmaShop_MOC_Headers
    Main.h
    OptionsDialog.h
    GameBrowser.h
    GameScanner.h
    NewFile.h
    QPlasmaDocument.h
    QPlasmaTextDoc.h
    QPlasmaSumFile.h
    QPlasmaPakFile.h
)

set(PlasmaShop_Sources
    Main.cpp
    OptionsDialog.cpp
    GameBrowser.cpp
    GameScanner.cpp
    NewFile.cpp
    QPlasmaDocument.cpp
    QPlasmaTextDoc.cpp
    QPlasmaSumFile.cpp
    QPlasmaPakFile.cpp
)

if(NOT WIN32 AND NOT APPLE)
    set(PlasmaShop_Sources "${PlasmaShop_Sources}"
        ../3rdParty/qticonloader.cpp
    )
endif()

if(WIN32)
    set(PlasmaShop_Sources ${PlasmaShop_Sources} res/PlasmaShop.rc)
endif()

find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui QtMain)
include(${QT_USE_FILE})
# generate rules for building source files from the resources
qt4_add_resources(PlasmaShop_RCC images.qrc)
# generate rules for building source files that moc generates
qt4_wrap_cpp(PlasmaShop_MOC ${PlasmaShop_MOC_Headers})

include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty")
include_directories("${PROJECT_SOURCE_DIR}/QScintilla/Qt4")
include_directories("${HSPlasma_INCLUDE_DIRS}")

add_executable(PlasmaShop WIN32 MACOSX_BUNDLE
               ${PlasmaShop_Sources} ${PlasmaShop_MOC} ${PlasmaShop_RCC})
target_link_libraries(PlasmaShop qscintilla2-ps3 ${QT_LIBRARIES} HSPlasma)

if(WIN32)
    target_link_libraries(PlasmaShop shell32)
endif()

if(APPLE)
    set(MACOSX_BUNDLE true)
    set(MACOSX_BUNDLE_NAME PlasmaShop)
    set(MACOSX_BUNDLE_INFO_STRING "PlasmaShop 3.0 BETA")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "2.90")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "2.90")
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright (C) 2011 Michael Hansen")
    #set(MACOSX_BUNDLE_ICON_FILE "PlamsaShop.icns")

    install(TARGETS PlasmaShop
            BUNDLE DESTINATION .
    )
elseif(WIN32)
    install(TARGETS PlasmaShop
            DESTINATION .
    )
else()
    install(TARGETS PlasmaShop
            DESTINATION bin
    )
endif()
