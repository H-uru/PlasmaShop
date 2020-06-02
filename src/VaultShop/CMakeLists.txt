set(VaultShop_Headers
    Main.h
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

#if(WIN32)
#    set(VaultShop_Sources ${VaultShop_Sources} res/VaultShop.rc)
#endif()

# generate rules for building source files from the resources
qt5_add_resources(VaultShop_RCC images.qrc)

add_executable(VaultShop WIN32 MACOSX_BUNDLE
               ${VaultShop_Sources} ${VaultShop_Headers} ${VaultShop_RCC})
target_link_libraries(VaultShop PSCommon Qt5::Widgets)
target_link_libraries(VaultShop HSPlasma)

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
else()
    install(TARGETS VaultShop
            DESTINATION bin
    )
endif()
