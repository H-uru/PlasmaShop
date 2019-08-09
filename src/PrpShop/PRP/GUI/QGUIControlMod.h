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

#ifndef _QGUICONTROLMOD_H
#define _QGUICONTROLMOD_H

#include "PRP/QCreatable.h"

#include <PRP/GUI/pfGUIControlMod.h>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include "PRP/QObjLink.h"
#include "PRP/QKeyList.h"
#include "QColorEdit.h"

class QGUIColorScheme : public QWidget
{
    Q_OBJECT

private:
    QColorEdit* fForeColor;
    QColorEdit* fBackColor;
    QColorEdit* fSelForeColor;
    QColorEdit* fSelBackColor;
    QCheckBox* fTransparent;
    QIntEdit* fFontSize;
    QLineEdit* fFontFace;

    enum { kCBBold, kCBItalic, kCBShadowed, kNumFontFlags };
    QCheckBox* fFontFlags[kNumFontFlags];

public:
    QGUIColorScheme(QWidget* parent);
    void setColorScheme(pfGUIColorScheme* scheme);
    void saveColorScheme(pfGUIColorScheme* scheme);
};

class QGUIControlMod : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QCheckBox* fModFlags[pfGUIControlMod::kBetterHitTesting + 1];
    QIntEdit* fTagID;
    QCheckBox* fVisible;
    QCreatableLink* fDynTextMap;
    QCreatableLink* fDynTextLayer;
    QIntListWidget* fSoundIndices;
    QCreatableLink* fProxy;
    QCreatableLink* fSkin;
    QComboBox* fProcType;
    QLineEdit* fProcCommand;
    QLabel* fProcCommandLabel;
    QGroupBox* fColorSchemeGrp;
    QGUIColorScheme* fColorScheme;

public:
    QGUIControlMod(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;

protected slots:
    void setDynTextMap();
    void setDynTextLayer();
    void setProxy();
    void setSkin();
    void unsetDynTextMap();
    void unsetDynTextLayer();
    void unsetProxy();
    void unsetSkin();

    void setProcType(int typeIdx);
};

#endif
