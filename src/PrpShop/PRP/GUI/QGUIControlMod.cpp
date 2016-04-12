/* This file is part of PlasmaShop.
 *
 * PlasmaShop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlasmaShop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlasmaShop.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QGUIControlMod.h"

#include <QTabWidget>
#include <QGridLayout>
#include "../../Main.h"
#include "../../QKeyDialog.h"

/* QGUIColorScheme */
QGUIColorScheme::QGUIColorScheme(QWidget* parent)
               : QWidget(parent)
{
    fForeColor = new QColorEdit(true, this);
    fBackColor = new QColorEdit(true, this);
    fSelForeColor = new QColorEdit(true, this);
    fSelBackColor = new QColorEdit(true, this);

    fTransparent = new QCheckBox(tr("Transparent"), this);
    fFontFace = new QLineEdit(this);
    fFontSize = new QIntEdit(this);
    fFontSize->setRange(0, 255);

    QWidget* grpFlags = new QWidget(this);
    fFontFlags[kCBBold] = new QCheckBox(tr("Bold"), grpFlags);
    fFontFlags[kCBItalic] = new QCheckBox(tr("Italic"), grpFlags);
    fFontFlags[kCBShadowed] = new QCheckBox(tr("Shadowed"), grpFlags);
    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setContentsMargins(0, 0, 0, 0);
    layFlags->addWidget(fFontFlags[kCBBold], 0, 0);
    layFlags->addWidget(fFontFlags[kCBItalic], 0, 1);
    layFlags->addWidget(fFontFlags[kCBShadowed], 0, 2);
    layFlags->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 0, 3);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("Foreground:"), this), 0, 0);
    layout->addWidget(fForeColor, 0, 1);
    layout->addWidget(new QLabel(tr("Background:"), this), 1, 0);
    layout->addWidget(fBackColor, 1, 1);
    layout->addItem(new QSpacerItem(8, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 0, 2);
    layout->addWidget(new QLabel(tr("Sel Fore:"), this), 0, 3);
    layout->addWidget(fSelForeColor, 0, 4);
    layout->addWidget(new QLabel(tr("Sel Back:"), this), 1, 3);
    layout->addWidget(fSelBackColor, 1, 4);
    layout->addItem(new QSpacerItem(0, 8, QSizePolicy::Minimum, QSizePolicy::Minimum), 2, 0, 1, 5);
    layout->addWidget(new QLabel(tr("Font:"), this), 3, 0);
    layout->addWidget(fFontFace, 3, 1, 1, 4);
    layout->addWidget(new QLabel(tr("Size:"), this), 4, 0);
    layout->addWidget(fFontSize, 4, 1);
    layout->addWidget(fTransparent, 4, 3, 1, 2);
    layout->addWidget(new QLabel(tr("Flags:"), this), 5, 0);
    layout->addWidget(grpFlags, 5, 1, 1, 4);
}

void QGUIColorScheme::setColorScheme(pfGUIColorScheme* scheme)
{
    if (scheme == NULL) {
        fForeColor->setColor(hsColorRGBA(0.0f, 0.0f, 0.0f, 1.0f));
        fBackColor->setColor(hsColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
        fSelForeColor->setColor(hsColorRGBA(0.0f, 0.0f, 0.0f, 1.0f));
        fSelBackColor->setColor(hsColorRGBA(0.0f, 0.0f, 1.0f, 1.0f));
        fTransparent->setChecked(false);
        fFontFace->setText("Times New Roman");
        fFontSize->setValue(10);
        fFontFlags[kCBBold]->setChecked(false);
        fFontFlags[kCBItalic]->setChecked(false);
        fFontFlags[kCBShadowed]->setChecked(false);
    } else {
        fForeColor->setColor(scheme->getForeColor());
        fBackColor->setColor(scheme->getBackColor());
        fSelForeColor->setColor(scheme->getSelForeColor());
        fSelBackColor->setColor(scheme->getSelBackColor());
        fTransparent->setChecked(scheme->getTransparent() != 0);
        fFontFace->setText(st2qstr(scheme->getFontFace()));
        fFontSize->setValue(scheme->getFontSize());
        fFontFlags[kCBBold]->setChecked((scheme->getFontFlags() & pfGUIColorScheme::kFontBold) != 0);
        fFontFlags[kCBItalic]->setChecked((scheme->getFontFlags() & pfGUIColorScheme::kFontItalic) != 0);
        fFontFlags[kCBShadowed]->setChecked((scheme->getFontFlags() & pfGUIColorScheme::kFontShadowed) != 0);
    }
}

void QGUIColorScheme::saveColorScheme(pfGUIColorScheme* scheme)
{
    scheme->setForeColor(fForeColor->color());
    scheme->setBackColor(fBackColor->color());
    scheme->setSelForeColor(fSelForeColor->color());
    scheme->setSelBackColor(fSelBackColor->color());
    scheme->setTransparent(fTransparent->isChecked() ? 1 : 0);
    scheme->setFontFace(qstr2st(fFontFace->text()));
    scheme->setFontSize(fFontSize->value());
    scheme->setFontFlags((fFontFlags[kCBBold]->isChecked() ? pfGUIColorScheme::kFontBold : 0)
                       | (fFontFlags[kCBItalic]->isChecked() ? pfGUIColorScheme::kFontItalic : 0)
                       | (fFontFlags[kCBShadowed]->isChecked() ? pfGUIColorScheme::kFontShadowed : 0));
}


/* QGUIControlMod */
QGUIControlMod::QGUIControlMod(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);

    fSynchObjLink = new QCreatableLink(this, false);
    fSynchObjLink->setCreatable(ctrl, tr("Synch Flags"));
    fSynchObjLink->setForceType(kSynchedObject);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUIControlMod::kWantsInterest] = new QCheckBox(tr("Wants Interest"), grpFlags);
    fModFlags[pfGUIControlMod::kInheritProcFromDlg] = new QCheckBox(tr("Inherit Proc from Dialog"), grpFlags);
    fModFlags[pfGUIControlMod::kIntangible] = new QCheckBox(tr("Intangible"), grpFlags);
    fModFlags[pfGUIControlMod::kXparentBgnd] = new QCheckBox(tr("Transparent Background"), grpFlags);
    fModFlags[pfGUIControlMod::kScaleTextWithResolution] = new QCheckBox(tr("Scale Text"), grpFlags);
    fModFlags[pfGUIControlMod::kTakesSpecialKeys] = new QCheckBox(tr("Takes Special Keys"), grpFlags);
    fModFlags[pfGUIControlMod::kHasProxy] = new QCheckBox(tr("Has Proxy"), grpFlags);
    fModFlags[pfGUIControlMod::kBetterHitTesting] = new QCheckBox(tr("Better Hit Testing"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUIControlMod::kWantsInterest], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUIControlMod::kInheritProcFromDlg], 0, 1);
    flagLayout->addWidget(fModFlags[pfGUIControlMod::kIntangible], 1, 0);
    flagLayout->addWidget(fModFlags[pfGUIControlMod::kXparentBgnd], 1, 1);
    flagLayout->addWidget(fModFlags[pfGUIControlMod::kScaleTextWithResolution], 2, 0);
    flagLayout->addWidget(fModFlags[pfGUIControlMod::kTakesSpecialKeys], 2, 1);
    flagLayout->addWidget(fModFlags[pfGUIControlMod::kHasProxy], 3, 0);
    flagLayout->addWidget(fModFlags[pfGUIControlMod::kBetterHitTesting], 3, 1);
    for (size_t i=0; i<=pfGUIControlMod::kBetterHitTesting; i++)
        fModFlags[i]->setChecked(ctrl->getFlag(i));

    fColorSchemeGrp = new QGroupBox(tr("Color Scheme"), this);
    fColorScheme = new QGUIColorScheme(fColorSchemeGrp);
    fColorScheme->setColorScheme(ctrl->getColorScheme());
    QGridLayout* skinLayout = new QGridLayout(fColorSchemeGrp);
    skinLayout->setContentsMargins(0, 0, 0, 0);
    skinLayout->addWidget(fColorScheme, 0, 0);
    fColorSchemeGrp->setCheckable(true);
    fColorSchemeGrp->setChecked(ctrl->getColorScheme() != NULL);

    fDynTextMap = new QCreatableLink(this);
    fDynTextMap->setKey(ctrl->getDynTextMap());

    fDynTextLayer = new QCreatableLink(this);
    fDynTextLayer->setKey(ctrl->getDynTextLayer());

    fProxy = new QCreatableLink(this);
    fProxy->setKey(ctrl->getProxy());

    fSkin = new QCreatableLink(this);
    fSkin->setKey(ctrl->getSkin());

    QTabWidget* xTabs = new QTabWidget(this);
    QWidget* guiProcTab = new QWidget(xTabs);
    fTagID = new QIntEdit(guiProcTab);
    fTagID->setRange(0, 0x7FFFFFFF);
    fTagID->setValue(ctrl->getTagID());
    fVisible = new QCheckBox(tr("Visible"), guiProcTab);
    fVisible->setChecked(ctrl->isVisible());

    fProcType = new QComboBox(guiProcTab);
    fProcType->addItems(QStringList() << "(NULL)" << tr("Console Command")
                                      << tr("Python Script") << tr("Close Dialog"));
    fProcType->setCurrentIndex(0);
    fProcCommand = new QLineEdit(guiProcTab);
    fProcCommandLabel = new QLabel(tr("Command:"), guiProcTab);
    if (ctrl->getHandler() != NULL) {
        fProcType->setCurrentIndex(ctrl->getHandler()->getType());
        if (ctrl->getHandler()->getType() == pfGUICtrlProcWriteableObject::kConsoleCmd)
            fProcCommand->setText(st2qstr(((pfGUIConsoleCmdProc*)ctrl->getHandler())->getCommand()));
    }
    setProcType(fProcType->currentIndex());
    QGridLayout* guiProcLayout = new QGridLayout(guiProcTab);
    guiProcLayout->setContentsMargins(8, 8, 8, 8);
    guiProcLayout->addWidget(new QLabel(tr("Tag ID:"), this), 0, 0);
    guiProcLayout->addWidget(fTagID, 0, 1);
    guiProcLayout->addWidget(fVisible, 0, 2);
    guiProcLayout->addWidget(new QLabel(tr("Proc Type:"), guiProcTab), 1, 0);
    guiProcLayout->addWidget(fProcType, 1, 1, 1, 2);
    guiProcLayout->addWidget(fProcCommandLabel, 2, 0);
    guiProcLayout->addWidget(fProcCommand, 2, 1, 1, 2);
    fSoundIndices = new QIntListWidget(-0x80000000, 0x7FFFFFFF, xTabs);
    for (size_t i=0; i<ctrl->getSoundIndices().size(); i++)
        fSoundIndices->addValue(ctrl->getSoundIndices()[i]);
    xTabs->addTab(guiProcTab, tr("Proc Handler"));
    xTabs->addTab(fSoundIndices, tr("Sound Indices"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fSynchObjLink, 0, 0, 1, 2);
    layout->addWidget(grpFlags, 1, 0, 1, 2);
    layout->addWidget(fColorSchemeGrp, 2, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Text Map:"), this), 3, 0);
    layout->addWidget(fDynTextMap, 3, 1);
    layout->addWidget(new QLabel(tr("Text Layer:"), this), 4, 0);
    layout->addWidget(fDynTextLayer, 4, 1);
    layout->addWidget(new QLabel(tr("Proxy:"), this), 5, 0);
    layout->addWidget(fProxy, 5, 1);
    layout->addWidget(new QLabel(tr("Skin:"), this), 6, 0);
    layout->addWidget(fSkin, 6, 1);
    layout->addWidget(xTabs, 7, 0, 1, 2);

    connect(fDynTextMap, SIGNAL(addObject()), this, SLOT(setDynTextMap()));
    connect(fDynTextMap, SIGNAL(delObject()), this, SLOT(unsetDynTextMap()));
    connect(fDynTextLayer, SIGNAL(addObject()), this, SLOT(setDynTextLayer()));
    connect(fDynTextLayer, SIGNAL(delObject()), this, SLOT(unsetDynTextLayer()));
    connect(fProxy, SIGNAL(addObject()), this, SLOT(setProxy()));
    connect(fProxy, SIGNAL(delObject()), this, SLOT(unsetProxy()));
    connect(fSkin, SIGNAL(addObject()), this, SLOT(setSkin()));
    connect(fSkin, SIGNAL(delObject()), this, SLOT(unsetSkin()));
    connect(fProcType, SIGNAL(currentIndexChanged(int)), this, SLOT(setProcType(int)));
}

void QGUIControlMod::saveDamage()
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);

    for (size_t i=0; i<=pfGUIControlMod::kBetterHitTesting; i++)
        ctrl->setFlag(i, fModFlags[i]->isChecked());

    if (fColorSchemeGrp->isChecked()) {
        if (ctrl->getColorScheme() == NULL)
            ctrl->setColorScheme(new pfGUIColorScheme());
        fColorScheme->saveColorScheme(ctrl->getColorScheme());
    } else {
        ctrl->setColorScheme(NULL);
    }

    ctrl->setTagID(fTagID->value());
    ctrl->setVisible(fVisible->isChecked());

    if (fProcType->currentIndex() == pfGUICtrlProcWriteableObject::kNull) {
        ctrl->setHandler(NULL);
    } else if (fProcType->currentIndex() == pfGUICtrlProcWriteableObject::kConsoleCmd) {
        pfGUIConsoleCmdProc* proc = new pfGUIConsoleCmdProc();
        proc->setCommand(qstr2st(fProcCommand->text()));
        ctrl->setHandler(proc);
    } else if (fProcType->currentIndex() == pfGUICtrlProcWriteableObject::kPythonScript) {
        ctrl->setHandler(new pfGUIPythonScriptProc());
    } else if (fProcType->currentIndex() == pfGUICtrlProcWriteableObject::kCloseDlg) {
        ctrl->setHandler(new pfGUICloseDlgProc());
    }

    std::vector<int> indices;
    indices.reserve(fSoundIndices->values().size());
    foreach (int idx, fSoundIndices->values())
        indices.push_back(idx);
    ctrl->setSoundIndices(indices);
}

void QGUIControlMod::setDynTextMap()
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getDynTextMap().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getDynTextMap());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kDynamicTextMap);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setDynTextMap(dlg.selection());
        fDynTextMap->setKey(ctrl->getDynTextMap());
    }
}

void QGUIControlMod::setDynTextLayer()
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getDynTextLayer().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getDynTextLayer());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kLayer);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setDynTextLayer(dlg.selection());
        fDynTextLayer->setKey(ctrl->getDynTextLayer());
    }
}

void QGUIControlMod::setProxy()
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getProxy().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getProxy());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation());
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setProxy(dlg.selection());
        fProxy->setKey(ctrl->getProxy());
    }
}

void QGUIControlMod::setSkin()
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);
    QFindKeyDialog dlg(this);
    if (ctrl->getSkin().Exists())
        dlg.init(PrpShopMain::ResManager(), ctrl->getSkin());
    else
        dlg.init(PrpShopMain::ResManager(), ctrl->getKey()->getLocation(), kGUISkin);
    if (dlg.exec() == QDialog::Accepted) {
        ctrl->setSkin(dlg.selection());
        fSkin->setKey(ctrl->getSkin());
    }
}

void QGUIControlMod::unsetDynTextMap()
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);
    ctrl->setDynTextMap(plKey());
    fDynTextMap->setCreatable(NULL, "(None)");
}

void QGUIControlMod::unsetDynTextLayer()
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);
    ctrl->setDynTextLayer(plKey());
    fDynTextLayer->setCreatable(NULL, "(None)");
}

void QGUIControlMod::unsetProxy()
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);
    ctrl->setProxy(plKey());
    fProxy->setCreatable(NULL, "(None)");
}

void QGUIControlMod::unsetSkin()
{
    pfGUIControlMod* ctrl = pfGUIControlMod::Convert(fCreatable);
    ctrl->setSkin(plKey());
    fSkin->setCreatable(NULL, "(None)");
}

void QGUIControlMod::setProcType(int typeIdx)
{
    if (typeIdx == pfGUICtrlProcWriteableObject::kConsoleCmd)
        fProcCommand->setEnabled(true);
    else
        fProcCommand->setEnabled(false);
    fProcCommandLabel->setEnabled(fProcCommand->isEnabled());
}
