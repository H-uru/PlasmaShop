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

#include "QCubicEnvironmap.h"

#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include "../../QPlasmaUtils.h"

/* QCubicEnvironmap */
QCubicEnvironmap::QCubicEnvironmap(plCreatable* pCre, QWidget* parent)
                : QCreatable(pCre, kCubicEnvironmap, parent)
{
    plCubicEnvironmap* tex = plCubicEnvironmap::Convert(fCreatable);

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

    QGroupBox* grpFaces = new QGroupBox(tr("Faces"), this);
    fFaces[plCubicEnvironmap::kLeftFace] = new QCreatableLink(grpFaces, false);
    fFaces[plCubicEnvironmap::kLeftFace]->setText(tr("Left Face"));
    fFaces[plCubicEnvironmap::kRightFace] = new QCreatableLink(grpFaces, false);
    fFaces[plCubicEnvironmap::kRightFace]->setText(tr("Right Face"));
    fFaces[plCubicEnvironmap::kFrontFace] = new QCreatableLink(grpFaces, false);
    fFaces[plCubicEnvironmap::kFrontFace]->setText(tr("Front Face"));
    fFaces[plCubicEnvironmap::kBackFace] = new QCreatableLink(grpFaces, false);
    fFaces[plCubicEnvironmap::kBackFace]->setText(tr("Back Face"));
    fFaces[plCubicEnvironmap::kTopFace] = new QCreatableLink(grpFaces, false);
    fFaces[plCubicEnvironmap::kTopFace]->setText(tr("Top Face"));
    fFaces[plCubicEnvironmap::kBottomFace] = new QCreatableLink(grpFaces, false);
    fFaces[plCubicEnvironmap::kBottomFace]->setText(tr("Bottom Face"));
    QGridLayout* layFaces = new QGridLayout(grpFaces);
    layFaces->setVerticalSpacing(0);
    layFaces->setHorizontalSpacing(8);
    for (size_t i=0; i<plCubicEnvironmap::kNumFaces; i++) {
        fFaces[i]->setCreatable(tex->getFace(i));
        layFaces->addWidget(fFaces[i], i%2, i/2);
    }

    fPreviewLink = new QCreatableLink(this, false);
    fPreviewLink->setCreatable(tex, tr("Preview"));
    fPreviewLink->setForceType(kPreviewCubicEnvironmap);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpFlags, 0, 0, 1, 3);
    layout->addWidget(grpFaces, 1, 0, 1, 3);
    layout->addWidget(fPreviewLink, 3, 0, 1, 3);
}

void QCubicEnvironmap::saveDamage()
{
    plCubicEnvironmap* tex = plCubicEnvironmap::Convert(fCreatable);

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
}
