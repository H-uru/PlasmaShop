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

#ifndef _QIMAGELIBMOD_H
#define _QIMAGELIBMOD_H

#include "PRP/QCreatable.h"

#include <PRP/GUI/plImageLibMod.h>
#include "PRP/QObjLink.h"
#include "PRP/QKeyList.h"

class QImageLibMod : public QCreatable
{
    Q_OBJECT

protected:
    QCreatableLink* fSynchObjLink;
    QKeyList* fImages;

public:
    QImageLibMod(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;
};

#endif
