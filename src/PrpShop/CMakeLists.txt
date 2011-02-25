SET(PrpShop_MOC_Headers
	Main.h
	../QColorEdit.h
	../QLinkLabel.h
	../QNumerics.h
	QKeyDialog.h
	QPrcEditor.h
	PRP/QCreatable.h
	PRP/QKeyList.h
	PRP/QMatrix44.h
	PRP/QObjLink.h
	PRP/QSceneNode.h
	PRP/Animation/QAnimTimeConvert.h
	PRP/Audio/QAudible.h
	PRP/Audio/QSoundBuffer.h
	PRP/Audio/QWinSound.h
	PRP/Avatar/QMultistageBehMod.h
	PRP/GUI/QGUIButtonMod.h
	PRP/GUI/QGUICheckBoxCtrl.h
	PRP/GUI/QGUIClickMapCtrl.h
	PRP/GUI/QGUIControlMod.h
	PRP/GUI/QGUIDialogMod.h
	PRP/GUI/QGUIDraggableMod.h
	PRP/GUI/QGUIDynDisplayCtrl.h
	PRP/GUI/QGUIKnobCtrl.h
	PRP/GUI/QGUIListBoxMod.h
	PRP/GUI/QGUIMenuItem.h
	PRP/GUI/QGUIMultiLineEditCtrl.h
	PRP/GUI/QGUIPopUpMenu.h
	PRP/GUI/QGUIProgressCtrl.h
	PRP/GUI/QGUIRadioGroupCtrl.h
	PRP/GUI/QGUISkin.h
	PRP/GUI/QGUITextBoxMod.h
	PRP/GUI/QGUIUpDownPairMod.h
	PRP/GUI/QImageLibMod.h
	PRP/Light/QShadowMaster.h
	PRP/Message/QMsgForwarder.h
	PRP/Modifier/QInterfaceInfoModifier.h
	PRP/Modifier/QOneShotMod.h
	PRP/Modifier/QPythonFileMod.h
	PRP/Modifier/QSpawnModifier.h
	PRP/Object/QAudioInterface.h
	PRP/Object/QCoordinateInterface.h
	PRP/Object/QDrawInterface.h
	PRP/Object/QSceneObject.h
	PRP/Object/QSimulationInterface.h
	PRP/Object/QSynchedObject.h
	PRP/Physics/QCollisionDetector.h
	PRP/Physics/QDetectorModifier.h
	PRP/Surface/QCubicEnvironmap.h
	PRP/Surface/QDynamicTextMap.h
	PRP/Surface/QFadeOpacityMod.h
	PRP/Surface/QLayer.h
	PRP/Surface/QLayerAnimation.h
	PRP/Surface/QLayerLinkAnimation.h
	PRP/Surface/QLayerMovie.h
	PRP/Surface/QLayerSDLAnimation.h
	PRP/Surface/QMaterial.h
	PRP/Surface/QMipmap.h
	PRP/Render/QSceneObj_Preview.h)
SET(PrpShop_Sources
	Main.cpp
	../QColorEdit.cpp
	../QLinkLabel.cpp
	../QNumerics.cpp
	QKeyDialog.cpp
	QPlasmaUtils.cpp
	QPlasmaTreeItem.cpp
	QPrcEditor.cpp
	PRP/QCreatable.cpp
	PRP/QKeyList.cpp
	PRP/QMatrix44.cpp
	PRP/QObjLink.cpp
	PRP/QSceneNode.cpp
	PRP/Animation/QAnimTimeConvert.cpp
	PRP/Audio/QAudible.cpp
	PRP/Audio/QSoundBuffer.cpp
	PRP/Audio/QWinSound.cpp
	PRP/Avatar/QMultistageBehMod.cpp
	PRP/GUI/QGUIButtonMod.cpp
	PRP/GUI/QGUICheckBoxCtrl.cpp
	PRP/GUI/QGUIClickMapCtrl.cpp
	PRP/GUI/QGUIControlMod.cpp
	PRP/GUI/QGUIDialogMod.cpp
	PRP/GUI/QGUIDraggableMod.cpp
	PRP/GUI/QGUIDynDisplayCtrl.cpp
	PRP/GUI/QGUIKnobCtrl.cpp
	PRP/GUI/QGUIListBoxMod.cpp
	PRP/GUI/QGUIMenuItem.cpp
	PRP/GUI/QGUIMultiLineEditCtrl.cpp
	PRP/GUI/QGUIPopUpMenu.cpp
	PRP/GUI/QGUIProgressCtrl.cpp
	PRP/GUI/QGUIRadioGroupCtrl.cpp
	PRP/GUI/QGUISkin.cpp
	PRP/GUI/QGUITextBoxMod.cpp
	PRP/GUI/QGUIUpDownPairMod.cpp
	PRP/GUI/QImageLibMod.cpp
	PRP/Light/QShadowMaster.cpp
	PRP/Message/QMsgForwarder.cpp
	PRP/Modifier/QInterfaceInfoModifier.cpp
	PRP/Modifier/QOneShotMod.cpp
	PRP/Modifier/QPythonFileMod.cpp
	PRP/Modifier/QSpawnModifier.cpp
	PRP/Object/QAudioInterface.cpp
	PRP/Object/QCoordinateInterface.cpp
	PRP/Object/QDrawInterface.cpp
	PRP/Object/QSceneObject.cpp
	PRP/Object/QSimulationInterface.cpp
	PRP/Object/QSynchedObject.cpp
	PRP/Physics/QCollisionDetector.cpp
	PRP/Physics/QDetectorModifier.cpp
	PRP/Surface/QCubicEnvironmap.cpp
	PRP/Surface/QDynamicTextMap.cpp
	PRP/Surface/QFadeOpacityMod.cpp
	PRP/Surface/QLayer.cpp
	PRP/Surface/QLayerAnimation.cpp
	PRP/Surface/QLayerLinkAnimation.cpp
	PRP/Surface/QLayerMovie.cpp
	PRP/Surface/QLayerSDLAnimation.cpp
	PRP/Surface/QMaterial.cpp
	PRP/Surface/QMipmap.cpp
	PRP/Render/QPlasmaRender.cpp
	PRP/Render/QSceneObj_Preview.cpp
	../3rdParty/qticonloader.cpp)

set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTSVG TRUE)
include(${QT_USE_FILE})


# generate rules for building source files from the resources
QT4_ADD_RESOURCES(PrpShop_RCC images.qrc)
# generate rules for building source files that moc generates
QT4_WRAP_CPP(PrpShop_MOC ${PrpShop_MOC_Headers})

include_directories("${PROJECT_SOURCE_DIR}/src/3rdParty")
include_directories("${PROJECT_SOURCE_DIR}/QScintilla/Qt4")
include_directories("${PROJECT_SOURCE_DIR}/src/PrpShop")
include_directories("${HSPlasma_INCLUDE_DIRS}")

# QtOpenGL dependencies (maybe the Qt OpenGL cmake stuff is broken, but I had to copy this here from Qt4ConfigDependentSettings.cmake)
find_package(OpenGL)
SET (QT_QTOPENGL_LIB_DEPENDENCIES ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

ADD_EXECUTABLE(PrpShop ${PrpShop_Sources} ${PrpShop_MOC} ${PrpShop_RCC})
TARGET_LINK_LIBRARIES(PrpShop qscintilla2-ps3 ${QT_LIBRARIES} ${QT_QTOPENGL_LIB_DEPENDENCIES} HSPlasma)

install(TARGETS PrpShop
        DESTINATION bin
)
