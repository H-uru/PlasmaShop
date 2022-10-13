set(PrpShop_Headers
    Main.h
    QBitmaskCheckBox.h
    QKeyDialog.h
    QPrcEditor.h
    QHexViewer.h
    QTargetList.h
    PRP/QCreatable.h
    PRP/QKeyList.h
    PRP/QMatrix44.h
    PRP/QObjLink.h
    PRP/QSceneNode.h
    PRP/QVector3.h
    PRP/Animation/QAnimTimeConvert.h
    PRP/Audio/QAudible.h
    PRP/Audio/QSoundBuffer.h
    PRP/Audio/QWinSound.h
    PRP/Avatar/QMultistageBehMod.h
    PRP/Avatar/QAvLadderMod.h
    PRP/Avatar/QSeekPointMod.h
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
    PRP/Modifier/QMaintainersMarkerModifier.h
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
    PRP/Render/QCEMap_Preview.h
    PRP/Render/QPlasmaCubicRender.h
    PRP/Render/QPlasmaRender.h
    PRP/Render/QSceneNode_Preview.h
    PRP/Render/QSceneObj_Preview.h
)

set(PrpShop_Sources
    Main.cpp
    QBitmaskCheckBox.cpp
    QKeyDialog.cpp
    QPlasmaUtils.cpp
    QPlasmaTreeItem.cpp
    QPrcEditor.cpp
    QHexViewer.cpp
    QTargetList.cpp
    PRP/QCreatable.cpp
    PRP/QKeyList.cpp
    PRP/QMatrix44.cpp
    PRP/QObjLink.cpp
    PRP/QSceneNode.cpp
    PRP/QVector3.cpp
    PRP/Animation/QAnimTimeConvert.cpp
    PRP/Audio/QAudible.cpp
    PRP/Audio/QSoundBuffer.cpp
    PRP/Audio/QWinSound.cpp
    PRP/Avatar/QMultistageBehMod.cpp
    PRP/Avatar/QAvLadderMod.cpp
    PRP/Avatar/QSeekPointMod.cpp
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
    PRP/Modifier/QMaintainersMarkerModifier.cpp
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
    PRP/Render/QCEMap_Preview.cpp
    PRP/Render/QPlasmaCubicRender.cpp
    PRP/Render/QPlasmaRender.cpp
    PRP/Render/QSceneNode_Preview.cpp
    PRP/Render/QSceneObj_Preview.cpp
    PRP/Render/QTrackball.cpp
)

if(WIN32)
    set(PrpShop_Sources ${PrpShop_Sources} res/PrpShop.rc)
endif()

if(QT_VERSION_MAJOR VERSION_GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)
endif()

# generate rules for building source files from the resources
set(PrpShop_QRC images.qrc)

include_directories("${PROJECT_SOURCE_DIR}/src/PrpShop")

# QtOpenGL dependencies (maybe the Qt OpenGL cmake stuff is broken, but I had
# to copy this here from Qt4ConfigDependentSettings.cmake)
find_package(OpenGL)

add_executable(PrpShop WIN32 MACOSX_BUNDLE
               ${PrpShop_Sources} ${PrpShop_Headers} ${PrpShop_QRC})
target_link_libraries(PrpShop PSCommon HSPlasma OpenGL::GL Qt::Core Qt::Widgets Qt::OpenGL)
if(QT_VERSION_MAJOR VERSION_GREATER_EQUAL 6)
    target_link_libraries(PrpShop Qt::OpenGLWidgets)
endif()

if(APPLE)
    set(MACOSX_BUNDLE true)
    set(MACOSX_BUNDLE_NAME PrpShop)
    set(MACOSX_BUNDLE_INFO_STRING "PrpShop 1.0 BETA")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "0.90")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "0.90")
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright (C) 2011 Michael Hansen")
    #set(MACOSX_BUNDLE_ICON_FILE "PlamsaShop.icns")

    install(TARGETS PrpShop
            BUNDLE DESTINATION .
    )
else()
    install(TARGETS PrpShop
            DESTINATION bin
    )
endif()
