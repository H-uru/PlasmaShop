SET(PlasmaShop_MOC_Headers
	Main.h
	OptionsDialog.h
	GameBrowser.h
	GameScanner.h
	QPlasmaDocument.h
	QPlasmaTextDoc.h
	QPlasmaSumFile.h)
SET(PlasmaShop_Sources
	Main.cpp
	OptionsDialog.cpp
	GameBrowser.cpp
	GameScanner.cpp
	QPlasmaDocument.cpp
	QPlasmaTextDoc.cpp
	QPlasmaSumFile.cpp
	../3rdParty/qticonloader.cpp)


include(${QT_USE_FILE})
# generate rules for building source files from the resources
QT4_ADD_RESOURCES(PlasmaShop_RCC images.qrc)
# generate rules for building source files that moc generates
QT4_WRAP_CPP(PlasmaShop_MOC ${PlasmaShop_MOC_Headers})

include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty")
include_directories("${PROJECT_SOURCE_DIR}/QScintilla/Qt4")
include_directories("${HSPlasma_INCLUDE_DIRS}")

ADD_EXECUTABLE(PlasmaShop ${PlasmaShop_Sources} ${PlasmaShop_MOC} ${PlasmaShop_RCC})
TARGET_LINK_LIBRARIES(PlasmaShop qscintilla2-ps3 ${QT_LIBRARIES} HSPlasma)

install(TARGETS PlasmaShop
        DESTINATION bin
)
