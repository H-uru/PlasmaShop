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
#include "../../QPlasmaUtils.h"

/* QDynamicTextMap */
QDynamicTextMap::QDynamicTextMap(plCreatable* pCre, QWidget* parent)
               : QCreatable(pCre, kDynamicTextMap, parent)
{
    plDynamicTextMap* tex = plDynamicTextMap::Convert(fCreatable);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setVerticalSpacing(0);
    layFlags->setHorizontalSpacing(8);
    fFlags[kAlphaChannelFlag] = new QCheckBox(tr("Alpha Channel"), grpFlags);
    fFlags[kAlphaBitFlag] = new QCheckBox(tr("Alpha Bit"), grpFlags);
    fFlags[kBumpEnvMap] = new QCheckBox(tr("Bump Env Map"), grpFlags);
    fFlags[kForce32Bit] = new QCheckBox(tr("Force 32-bit"), grpFlags);
    fFlags[kDontThrowAwayImage] = new QCheckBox(tr("Don't Throw Away"), grpFlags);
    fFlags[kForceOneMipLevel] = new QCheckBox(tr("Force One Level"), grpFlags);
    fFlags[kNoMaxSize] = new QCheckBox(tr("No Maximum Size"), grpFlags);
    fFlags[kIntensityMap] = new QCheckBox(tr("Intensity Map"), grpFlags);
    fFlags[kHalfSize] = new QCheckBox(tr("Half Size"), grpFlags);
    fFlags[kUserOwnsBitmap] = new QCheckBox(tr("User Owned"), grpFlags);
    fFlags[kForceRewrite] = new QCheckBox(tr("Force Rewrite"), grpFlags);
    fFlags[kForceNonCompressed] = new QCheckBox(tr("Force Non-compressed"), grpFlags);
    fFlags[kIsTexture] = new QCheckBox(tr("Is Texture"), grpFlags);
    fFlags[kIsOffscreen] = new QCheckBox(tr("Is Offscreen"), grpFlags);
    fFlags[kIsProjected] = new QCheckBox(tr("Is Projected"), grpFlags);
    fFlags[kIsOrtho] = new QCheckBox(tr("Is Orthogonal"), grpFlags);
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
    fFlags[kAlphaChannelFlag]->setChecked(tex->getFlags() & plBitmap::kAlphaChannelFlag);
    fFlags[kAlphaBitFlag]->setChecked(tex->getFlags() & plBitmap::kAlphaBitFlag);
    fFlags[kBumpEnvMap]->setChecked(tex->getFlags() & plBitmap::kBumpEnvMap);
    fFlags[kForce32Bit]->setChecked(tex->getFlags() & plBitmap::kForce32Bit);
    fFlags[kDontThrowAwayImage]->setChecked(tex->getFlags() & plBitmap::kDontThrowAwayImage);
    fFlags[kForceOneMipLevel]->setChecked(tex->getFlags() & plBitmap::kForceOneMipLevel);
    fFlags[kNoMaxSize]->setChecked(tex->getFlags() & plBitmap::kNoMaxSize);
    fFlags[kIntensityMap]->setChecked(tex->getFlags() & plBitmap::kIntensityMap);
    fFlags[kHalfSize]->setChecked(tex->getFlags() & plBitmap::kHalfSize);
    fFlags[kUserOwnsBitmap]->setChecked(tex->getFlags() & plBitmap::kUserOwnsBitmap);
    fFlags[kForceRewrite]->setChecked(tex->getFlags() & plBitmap::kForceRewrite);
    fFlags[kForceNonCompressed]->setChecked(tex->getFlags() & plBitmap::kForceNonCompressed);
    fFlags[kIsTexture]->setChecked(tex->getFlags() & plBitmap::kIsTexture);
    fFlags[kIsOffscreen]->setChecked(tex->getFlags() & plBitmap::kIsOffscreen);
    fFlags[kIsProjected]->setChecked(tex->getFlags() & plBitmap::kIsProjected);
    fFlags[kIsOrtho]->setChecked(tex->getFlags() & plBitmap::kIsOrtho);

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

    tex->setFlags((fFlags[kAlphaChannelFlag]->isChecked() ? plBitmap::kAlphaChannelFlag : 0)
                | (fFlags[kAlphaBitFlag]->isChecked() ? plBitmap::kAlphaBitFlag : 0)
                | (fFlags[kBumpEnvMap]->isChecked() ? plBitmap::kBumpEnvMap : 0)
                | (fFlags[kForce32Bit]->isChecked() ? plBitmap::kForce32Bit : 0)
                | (fFlags[kDontThrowAwayImage]->isChecked() ? plBitmap::kDontThrowAwayImage : 0)
                | (fFlags[kForceOneMipLevel]->isChecked() ? plBitmap::kForceOneMipLevel : 0)
                | (fFlags[kNoMaxSize]->isChecked() ? plBitmap::kNoMaxSize : 0)
                | (fFlags[kIntensityMap]->isChecked() ? plBitmap::kIntensityMap : 0)
                | (fFlags[kHalfSize]->isChecked() ? plBitmap::kHalfSize : 0)
                | (fFlags[kUserOwnsBitmap]->isChecked() ? plBitmap::kUserOwnsBitmap : 0)
                | (fFlags[kForceRewrite]->isChecked() ? plBitmap::kForceRewrite : 0)
                | (fFlags[kForceNonCompressed]->isChecked() ? plBitmap::kForceNonCompressed : 0)
                | (fFlags[kIsTexture]->isChecked() ? plBitmap::kIsTexture : 0)
                | (fFlags[kIsOffscreen]->isChecked() ? plBitmap::kIsOffscreen : 0)
                | (fFlags[kIsProjected]->isChecked() ? plBitmap::kIsProjected : 0)
                | (fFlags[kIsOrtho]->isChecked() ? plBitmap::kIsOrtho : 0));
    tex->setVisWidth(fWidth->value());
    tex->setVisHeight(fHeight->value());
    tex->setHasAlpha(fHasAlpha->isChecked());
}
