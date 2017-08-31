set(PlasmaShop_Headers
    Main.h
    OptionsDialog.h
    GameBrowser.h
    GameScanner.h
    NewFile.h
    QPlasmaDevModeDat.h
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
    QPlasmaDevModeDat.cpp
    QPlasmaDocument.cpp
    QPlasmaTextDoc.cpp
    QPlasmaSumFile.cpp
    QPlasmaPakFile.cpp
)

# include pycdc sources
set(pycdc_SOURCE_DIR ${PROJECT_SOURCE_DIR}/pycdc)
set(pycdc_BINARY_DIR ${PROJECT_BINARY_DIR}/pycdc)
set(pycdc_Headers
    ${pycdc_SOURCE_DIR}/ASTNode.h
    ${pycdc_SOURCE_DIR}/ASTree.h
    ${pycdc_SOURCE_DIR}/bytecode.h
    ${pycdc_SOURCE_DIR}/data.h
    ${pycdc_SOURCE_DIR}/pyc_code.h
    ${pycdc_SOURCE_DIR}/pyc_module.h
    ${pycdc_SOURCE_DIR}/pyc_numeric.h
    ${pycdc_SOURCE_DIR}/pyc_object.h
    ${pycdc_SOURCE_DIR}/pyc_sequence.h
    ${pycdc_SOURCE_DIR}/pyc_string.h
)
set(pycdc_Sources
    ${pycdc_SOURCE_DIR}/ASTNode.cpp
    ${pycdc_SOURCE_DIR}/ASTree.cpp
    ${pycdc_SOURCE_DIR}/bytecode.cpp
    ${pycdc_SOURCE_DIR}/data.cpp
    ${pycdc_SOURCE_DIR}/pyc_code.cpp
    ${pycdc_SOURCE_DIR}/pyc_module.cpp
    ${pycdc_SOURCE_DIR}/pyc_numeric.cpp
    ${pycdc_SOURCE_DIR}/pyc_object.cpp
    ${pycdc_SOURCE_DIR}/pyc_sequence.cpp
    ${pycdc_SOURCE_DIR}/pyc_string.cpp
)
set(pycdc_Maps
    ${pycdc_SOURCE_DIR}/bytes/python_10.map
    ${pycdc_SOURCE_DIR}/bytes/python_11.map
    ${pycdc_SOURCE_DIR}/bytes/python_13.map
    ${pycdc_SOURCE_DIR}/bytes/python_14.map
    ${pycdc_SOURCE_DIR}/bytes/python_15.map
    ${pycdc_SOURCE_DIR}/bytes/python_16.map
    ${pycdc_SOURCE_DIR}/bytes/python_20.map
    ${pycdc_SOURCE_DIR}/bytes/python_21.map
    ${pycdc_SOURCE_DIR}/bytes/python_22.map
    ${pycdc_SOURCE_DIR}/bytes/python_23.map
    ${pycdc_SOURCE_DIR}/bytes/python_24.map
    ${pycdc_SOURCE_DIR}/bytes/python_25.map
    ${pycdc_SOURCE_DIR}/bytes/python_26.map
    ${pycdc_SOURCE_DIR}/bytes/python_27.map
    ${pycdc_SOURCE_DIR}/bytes/python_30.map
    ${pycdc_SOURCE_DIR}/bytes/python_31.map
    ${pycdc_SOURCE_DIR}/bytes/python_32.map
    ${pycdc_SOURCE_DIR}/bytes/python_33.map
    ${pycdc_SOURCE_DIR}/bytes/python_34.map
    ${pycdc_SOURCE_DIR}/bytes/python_35.map
    ${pycdc_SOURCE_DIR}/bytes/python_36.map
)
set(pycdc_GeneratedSources
    ${pycdc_BINARY_DIR}/bytes/python_10.cpp
    ${pycdc_BINARY_DIR}/bytes/python_11.cpp
    ${pycdc_BINARY_DIR}/bytes/python_13.cpp
    ${pycdc_BINARY_DIR}/bytes/python_14.cpp
    ${pycdc_BINARY_DIR}/bytes/python_15.cpp
    ${pycdc_BINARY_DIR}/bytes/python_16.cpp
    ${pycdc_BINARY_DIR}/bytes/python_20.cpp
    ${pycdc_BINARY_DIR}/bytes/python_21.cpp
    ${pycdc_BINARY_DIR}/bytes/python_22.cpp
    ${pycdc_BINARY_DIR}/bytes/python_23.cpp
    ${pycdc_BINARY_DIR}/bytes/python_24.cpp
    ${pycdc_BINARY_DIR}/bytes/python_25.cpp
    ${pycdc_BINARY_DIR}/bytes/python_26.cpp
    ${pycdc_BINARY_DIR}/bytes/python_27.cpp
    ${pycdc_BINARY_DIR}/bytes/python_30.cpp
    ${pycdc_BINARY_DIR}/bytes/python_31.cpp
    ${pycdc_BINARY_DIR}/bytes/python_32.cpp
    ${pycdc_BINARY_DIR}/bytes/python_33.cpp
    ${pycdc_BINARY_DIR}/bytes/python_34.cpp
    ${pycdc_BINARY_DIR}/bytes/python_35.cpp
    ${pycdc_BINARY_DIR}/bytes/python_36.cpp
)
# run "comp_map.py" in the pycdc folder to generate the source code
add_custom_target(create-bytes-source-dir ALL
                  COMMAND ${CMAKE_COMMAND} -E make_directory ${pycdc_BINARY_DIR}/bytes)
add_custom_command(OUTPUT ${pycdc_GeneratedSources}
                   COMMAND ${pycdc_SOURCE_DIR}/bytes/comp_map.py
                           ${pycdc_SOURCE_DIR}/bytes ${pycdc_BINARY_DIR}/bytes
                   DEPENDS ${pycdc_Maps} ${pycdc_SOURCE_DIR}/bytes/comp_map.py
                           create-bytes-source-dir
                   WORKING_DIRECTORY ${pycdc_SOURCE_DIR}/bytes)

if(WIN32)
    set(PlasmaShop_Sources ${PlasmaShop_Sources} res/PlasmaShop.rc)
endif()

# generate rules for building source files from the resources
qt5_add_resources(PlasmaShop_RCC images.qrc)

include_directories("${PROJECT_SOURCE_DIR}/QScintilla/Qt4Qt5")
include_directories("${pycdc_SOURCE_DIR}")

add_executable(PlasmaShop WIN32 MACOSX_BUNDLE
               ${PlasmaShop_Headers} ${PlasmaShop_Sources}
               ${pycdc_Headers} ${pycdc_Sources} ${pycdc_GeneratedSources}
               ${PlasmaShop_RCC})
target_link_libraries(PlasmaShop Qt5::Widgets)
target_link_libraries(PlasmaShop qscintilla2-ps3 HSPlasma)
if(NOT WIN32 AND NOT APPLE)
    target_link_libraries(PlasmaShop PSIconLoader)
endif()

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
