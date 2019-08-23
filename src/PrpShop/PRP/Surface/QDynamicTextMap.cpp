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

#include "QDynamicTextMap.h"

#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include "QPlasmaUtils.h"

/* QDynamicTextMap */
QDynamicTextMap::QDynamicTextMap(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kDynamicTextMap, parent)
{
    plDynamicTextMap* tex = plDynamicTextMap::Convert(fCreatable);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setVerticalSpacing(0);
    layFlags->setHorizontalSpacing(8);
    fFlags[kAlphaChannelFlag] = new QBitmaskCheckBox(plBitmap::kAlphaChannelFlag,
                                                     tr("Alpha Channel"), grpFlags);
    fFlags[kAlphaBitFlag] = new QBitmaskCheckBox(plBitmap::kAlphaBitFlag,
                                                 tr("Alpha Bit"), grpFlags);
    fFlags[kBumpEnvMap] = new QBitmaskCheckBox(plBitmap::kBumpEnvMap,
                                               tr("Bump Env Map"), grpFlags);
    fFlags[kForce32Bit] = new QBitmaskCheckBox(plBitmap::kForce32Bit,
                                               tr("Force 32-bit"), grpFlags);
    fFlags[kDontThrowAwayImage] = new QBitmaskCheckBox(plBitmap::kDontThrowAwayImage,
                                                       tr("Don't Throw Away"), grpFlags);
    fFlags[kForceOneMipLevel] = new QBitmaskCheckBox(plBitmap::kForceOneMipLevel,
                                                     tr("Force One Level"), grpFlags);
    fFlags[kNoMaxSize] = new QBitmaskCheckBox(plBitmap::kNoMaxSize,
                                              tr("No Maximum Size"), grpFlags);
    fFlags[kIntensityMap] = new QBitmaskCheckBox(plBitmap::kIntensityMap,
                                                 tr("Intensity Map"), grpFlags);
    fFlags[kHalfSize] = new QBitmaskCheckBox(plBitmap::kHalfSize,
                                             tr("Half Size"), grpFlags);
    fFlags[kUserOwnsBitmap] = new QBitmaskCheckBox(plBitmap::kUserOwnsBitmap,
                                                   tr("User Owned"), grpFlags);
    fFlags[kForceRewrite] = new QBitmaskCheckBox(plBitmap::kForceRewrite,
                                                 tr("Force Rewrite"), grpFlags);
    fFlags[kForceNonCompressed] = new QBitmaskCheckBox(plBitmap::kForceNonCompressed,
                                                       tr("Force Non-compressed"), grpFlags);
    fFlags[kIsTexture] = new QBitmaskCheckBox(plBitmap::kIsTexture,
                                              tr("Is Texture"), grpFlags);
    fFlags[kIsOffscreen] = new QBitmaskCheckBox(plBitmap::kIsOffscreen,
                                                tr("Is Offscreen"), grpFlags);
    fFlags[kIsProjected] = new QBitmaskCheckBox(plBitmap::kIsProjected,
                                                tr("Is Projected"), grpFlags);
    fFlags[kIsOrtho] = new QBitmaskCheckBox(plBitmap::kIsOrtho,
                                            tr("Is Orthogonal"), grpFlags);
    layFlags->addWidget(fFlags[kAlphaChannelFlag], 0, 0);
    layFlags->addWidget(fFlags[kAlphaBitFlag], 1, 0);
    layFlags->addWidget(fFlags[kBumpEnvMap], 2, 0);
    layFlags->addWidget(fFlags[kForce32Bit], 3, 0);
    layFlags->addWidget(fFlags[kDontThrowAwayImage], 0, 1);
    layFlags->addWidget(fFlags[kForceOneMipLevel], 1, 1);
    layFlags->addWidget(fFlags[kNoMaxSize], 2, 1);
    layFlags->addWidget(fFlags[kForceNonCompressed], 3, 1);
    layFlags->addWidget(fFlags[kHalfSize], 0, 2);
    layFlags->addWidget(fFlags[kUserOwnsBitmap], 1, 2);
    layFlags->addWidget(fFlags[kForceRewrite], 2, 2);
    layFlags->addWidget(fFlags[kIntensityMap], 3, 2);
    layFlags->addWidget(fFlags[kIsTexture], 0, 3);
    layFlags->addWidget(fFlags[kIsOffscreen], 1, 3);
    layFlags->addWidget(fFlags[kIsProjected], 2, 3);
    layFlags->addWidget(fFlags[kIsOrtho], 3, 3);

    for (auto cb : fFlags) {
        cb->setFrom(tex->getFlags());
        connect(cb, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
            plDynamicTextMap* tex = plDynamicTextMap::Convert(fCreatable);
            tex->setFlags(tex->getFlags() | mask);
        });
        connect(cb, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
            plDynamicTextMap* tex = plDynamicTextMap::Convert(fCreatable);
            tex->setFlags(tex->getFlags() & ~mask);
        });
    }

    fWidth = new QIntEdit(this);
    fWidth->setRange(0, 0x7FFFFFFF);
    fWidth->setValue(tex->getVisWidth());
    fHeight = new QIntEdit(this);
    fHeight->setRange(0, 0x7FFFFFFF);
    fHeight->setValue(tex->getVisHeight());

    fHasAlpha = new QCheckBox(tr("Has Alpha"), this);
    fHasAlpha->setChecked(tex->hasAlpha());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpFlags, 0, 0, 1, 4);
    layout->addWidget(new QLabel(tr("Width:"), this), 1, 0);
    layout->addWidget(fWidth, 1, 1);
    layout->addItem(new QSpacerItem(16, 0), 1, 2);
    layout->addWidget(new QLabel(tr("Height:"), this), 2, 0);
    layout->addWidget(fHeight, 2, 1);
    layout->addWidget(fHasAlpha, 1, 3);
}

void QDynamicTextMap::saveDamage()
{
    plDynamicTextMap* tex = plDynamicTextMap::Convert(fCreatable);

    tex->setVisWidth(fWidth->value());
    tex->setVisHeight(fHeight->value());
    tex->setHasAlpha(fHasAlpha->isChecked());
}
