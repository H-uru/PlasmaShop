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

#include "QLayerMovie.h"

#include <QLabel>
#include <QGridLayout>
#include <ResManager/plFactory.h>

QLayerMovie::QLayerMovie(plCreatable* pCre, QWidget* parent)
           : QCreatable(pCre, pCre->ClassIndex(), parent)
{
    plLayerMovie* lay = (plLayerMovie*)fCreatable;

    fLayerAnimLink = new QCreatableLink(this, false);
    fLayerAnimLink->setText(tr("Layer Animation Properties"));
    fLayerAnimLink->setCreatable(lay);
    fLayerAnimLink->setForceType(kLayerAnimation);

    fMovieName = new QLineEdit(this);
    fMovieName->setText(~lay->getMovieName());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fLayerAnimLink, 0, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Movie Name:"), this), 3, 0);
    layout->addWidget(fMovieName, 3, 1);
}

void QLayerMovie::saveDamage()
{
    plLayerMovie* lay = (plLayerMovie*)fCreatable;
    lay->setMovieName(~fMovieName->text());
}
