set(PlasmaShop_Headers
    Main.h
    OptionsDialog.h
    GameBrowser.h
    GameScanner.h
    NewFile.h
    QPlasmaDevModeDat.h
    QPlasmaDocument.h
    QPlasmaFont.h
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
    QPlasmaFont.cpp
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

    ${pycdc_SOURCE_DIR}/bytes/python_1_0.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_1_1.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_1_3.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_1_4.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_1_5.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_1_6.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_2_0.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_2_1.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_2_2.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_2_3.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_2_4.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_2_5.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_2_6.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_2_7.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_0.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_1.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_2.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_3.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_4.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_5.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_6.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_7.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_8.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_9.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_10.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_11.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_12.cpp
    ${pycdc_SOURCE_DIR}/bytes/python_3_13.cpp
)

if(WIN32)
    set(PlasmaShop_Sources ${PlasmaShop_Sources} res/PlasmaShop.rc)
endif()

# generate rules for building source files from the resources
set(PlasmaShop_QRC images.qrc PlasmaSyntax.qrc)

include_directories("${pycdc_SOURCE_DIR}")

add_executable(PlasmaShop WIN32 MACOSX_BUNDLE
               ${PlasmaShop_Headers} ${PlasmaShop_Sources}
               ${pycdc_Headers} ${pycdc_Sources} ${PlasmaShop_QRC})
target_link_libraries(PlasmaShop PSCommon Qt::Core Qt::Widgets)
target_link_libraries(PlasmaShop HSPlasma)

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
else()
    install(TARGETS PlasmaShop
            DESTINATION bin
    )
endif()
