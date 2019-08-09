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

#ifndef _QGUILISTBOXMOD_H
#define _QGUILISTBOXMOD_H

#include "PRP/QCreatable.h"

#include <PRP/GUI/pfGUIListBoxMod.h>
#include <QCheckBox>
#include "PRP/QObjLink.h"

class QGUIListBoxMod : public QCreatable
{
    Q_OBJECT

protected:
    enum
    {
        kModFlagStart = pfGUIListBoxMod::kSingleSelect,
        kModFlagEnd = pfGUIListBoxMod::kForbidNoSelection,
        kModFlagCount = (kModFlagEnd - kModFlagStart) + 1
    };

    QCreatableLink* fControlModLink;
    QCheckBox* fModFlags[kModFlagCount];
    QCreatableLink* fScrollCtrl;

public:
    QGUIListBoxMod(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;

private slots:
    void setScrollCtrl();
    void unsetScrollCtrl();
};

#endif
