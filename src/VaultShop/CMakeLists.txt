set(VaultShop_MOC_Headers
    Main.h
    ../QColorEdit.h
    ../QLinkLabel.h
    QGameServerState.h
    QVaultNode.h
    QVaultNodeEdit.h
    QVaultFolderNode.h
    QVaultTextNode.h
    QVaultSDLNode.h
    QVaultChronicleNode.h
    QVaultLinkNode.h
    QVaultPlayerNode.h
    QVaultPlayerInfoNode.h
    QVaultAgeNode.h
    QVaultAgeInfoNode.h
    QVaultImageNode.h
)

set(VaultShop_Sources
    Main.cpp
    ../QColorEdit.cpp
    ../QLinkLabel.cpp
    QGameServerState.cpp
    QVaultNode.cpp
    QVaultNodeEdit.cpp
    QVaultFolderNode.cpp
    QVaultTextNode.cpp
    QVaultSDLNode.cpp
    QVaultChronicleNode.cpp
    QVaultLinkNode.cpp
    QVaultPlayerNode.cpp
    QVaultPlayerInfoNode.cpp
    QVaultAgeNode.cpp
    QVaultAgeInfoNode.cpp
    QVaultImageNode.cpp
)

if(NOT WIN32 AND NOT APPLE)
    set(VaultShop_Sources "${VaultShop_Sources}"
        ../3rdParty/qticonloader.cpp
    )
endif()

#if(WIN32)
#    set(VaultShop_Sources ${VaultShop_Sources} res/VaultShop.rc)
#endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

# generate rules for building source files from the resources
qt5_add_resources(VaultShop_RCC images.qrc)
# generate rules for building source files that moc generates
qt5_wrap_cpp(VaultShop_MOC ${VaultShop_MOC_Headers})

include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty")
include_directories("${HSPlasma_INCLUDE_DIRS}")

add_executable(VaultShop WIN32 MACOSX_BUNDLE
               ${VaultShop_Sources} ${VaultShop_MOC} ${VaultShop_RCC})
qt5_use_modules(VaultShop Core Widgets)
target_link_libraries(VaultShop ${Qt5Core_QTMAIN_LIBRARIES} HSPlasma)

if(APPLE)
    set(MACOSX_BUNDLE true)
    set(MACOSX_BUNDLE_NAME VaultShop)
    set(MACOSX_BUNDLE_INFO_STRING "VaultShop 1.0")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright (C) 2011 Michael Hansen")
    #set(MACOSX_BUNDLE_ICON_FILE "VaultShop.icns")

    install(TARGETS VaultShop
            BUNDLE DESTINATION .
    )
elseif(WIN32)
    install(TARGETS VaultShop
            DESTINATION .
    )
else()
    install(TARGETS VaultShop
            DESTINATION bin
    )
endif()
