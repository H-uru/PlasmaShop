SET(VaultShop_MOC_Headers
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
	QVaultImageNode.h)
SET(VaultShop_Sources
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
	../3rdParty/qticonloader.cpp)


include(${QT_USE_FILE})
# generate rules for building source files from the resources
QT4_ADD_RESOURCES(VaultShop_RCC images.qrc)
# generate rules for building source files that moc generates
QT4_WRAP_CPP(VaultShop_MOC ${VaultShop_MOC_Headers})

include_directories ("${PROJECT_SOURCE_DIR}/src/3rdParty")
include_directories("${HSPlasma_INCLUDE_DIRS}")

ADD_EXECUTABLE(VaultShop ${VaultShop_Sources} ${VaultShop_MOC} ${VaultShop_RCC})
TARGET_LINK_LIBRARIES(VaultShop ${QT_LIBRARIES} HSPlasma)

install(TARGETS VaultShop
        DESTINATION bin
)
