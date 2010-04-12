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

#include "QGUISkin.h"

#include <QGroupBox>
#include <QGridLayout>
#include "../../Main.h"
#include "../../QKeyDialog.h"

/* QGUISkin */
QGUISkin::QGUISkin(plCreatable* pCre, QWidget* parent)
        : QCreatable(pCre, kGUISkin, parent), fCurElement(-1)
{
    pfGUISkin* skin = (pfGUISkin*)fCreatable;

    fItemMargin = new QIntEdit(this);
    fItemMargin->setRange(0, 0xFFFF);
    fItemMargin->setValue(skin->getItemMargin());

    fBorderMargin = new QIntEdit(this);
    fBorderMargin->setRange(0, 0xFFFF);
    fBorderMargin->setValue(skin->getBorderMargin());

    fTexture = new QCreatableLink(this);
    fTexture->setKey(skin->getTexture());
    fTexture->setText(skin->getTexture().Exists()
                     ? ~skin->getTexture()->getName()
                     : "(None)");

    fElements = new QComboBox(this);
    fElements->addItems(QStringList() << tr("Upper-Left Corner") << tr("Top Span")
                                      << tr("Upper-Right Corner") << tr("Right Span")
                                      << tr("Lower-Right Corner") << tr("Bottom Span")
                                      << tr("Lower-Left Corner") << tr("Left Span")
                                      << tr("Middle Fill") << tr("Selection Fill")
                                      << tr("Submenu Arrow") << tr("Selected Submenu Arrow")
                                      << tr("Tree Button Closed") << tr("Tree Button Open"));

    QGroupBox* grpElement = new QGroupBox(tr("Element Texture Source"), this);
    fX = new QIntEdit(grpElement);
    fX->setRange(0, 0xFFFF);
    fY = new QIntEdit(grpElement);
    fY->setRange(0, 0xFFFF);
    fWidth = new QIntEdit(grpElement);
    fWidth->setRange(0, 0xFFFF);
    fHeight = new QIntEdit(grpElement);
    fHeight->setRange(0, 0xFFFF);
    setElement(0);
    QGridLayout* layElement = new QGridLayout(grpElement);
    layElement->setContentsMargins(8, 8, 8, 8);
    layElement->setHorizontalSpacing(4);
    layElement->setVerticalSpacing(4);
    layElement->addWidget(new QLabel("X", grpElement), 0, 1, 1, 1, Qt::AlignCenter);
    layElement->addWidget(new QLabel("Y", grpElement), 0, 2, 1, 1, Qt::AlignCenter);
    layElement->addWidget(new QLabel("Position:", grpElement), 1, 0);
    layElement->addWidget(new QLabel("Size:", grpElement), 2, 0);
    layElement->addWidget(fX, 1, 1);
    layElement->addWidget(fY, 1, 2);
    layElement->addWidget(fWidth, 2, 1);
    layElement->addWidget(fHeight, 2, 2);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(new QLabel(tr("Item Margin:"), this), 0, 0);
    layout->addWidget(fItemMargin, 0, 1);
    layout->addWidget(new QLabel(tr("Border:"), this), 1, 0);
    layout->addWidget(fBorderMargin, 1, 1);
    layout->addWidget(new QLabel(tr("Texture:"), this), 2, 0);
    layout->addWidget(fTexture, 2, 1);
    layout->addItem(new QSpacerItem(0, 16, QSizePolicy::Minimum, QSizePolicy::Minimum), 3, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Element:"), this), 4, 0);
    layout->addWidget(fElements, 4, 1);
    layout->addWidget(grpElement, 5, 0, 1, 2);

    connect(fTexture, SIGNAL(addObject()), this, SLOT(setTexture()));
    connect(fTexture, SIGNAL(delObject()), this, SLOT(unsetTexture()));
    connect(fElements, SIGNAL(currentIndexChanged(int)), this, SLOT(setElement(int)));
}

void QGUISkin::saveDamage()
{
    pfGUISkin* skin = (pfGUISkin*)fCreatable;

    skin->setItemMargin(fItemMargin->value());
    skin->setBorderMargin(fBorderMargin->value());
    saveElement();
}

void QGUISkin::saveElement()
{
    if (fCurElement < 0)
        return;
    pfGUISkin* skin = (pfGUISkin*)fCreatable;
    skin->getElement(fCurElement).fX = fX->value();
    skin->getElement(fCurElement).fY = fY->value();
    skin->getElement(fCurElement).fWidth = fWidth->value();
    skin->getElement(fCurElement).fHeight = fHeight->value();
}

void QGUISkin::setTexture()
{
    pfGUISkin* skin = (pfGUISkin*)fCreatable;
    QFindKeyDialog dlg(this);
    if (skin->getTexture().Exists())
        dlg.init(PrpShopMain::ResManager(), skin->getTexture());
    else
        dlg.init(PrpShopMain::ResManager(), skin->getKey()->getLocation(), kMipmap);
    if (dlg.exec() == QDialog::Accepted) {
        skin->setTexture(dlg.selection());
        fTexture->setKey(skin->getTexture());
        fTexture->setText(~skin->getTexture()->getName());
    }
}

void QGUISkin::unsetTexture()
{
    pfGUISkin* skin = (pfGUISkin*)fCreatable;
    skin->setTexture(plKey());
    fTexture->setCreatable(NULL);
    fTexture->setText("(None)");
}

void QGUISkin::setElement(int which)
{
    pfGUISkin* skin = (pfGUISkin*)fCreatable;
    saveElement();

    fCurElement = which;
    fX->setValue(skin->getElement(fCurElement).fX);
    fY->setValue(skin->getElement(fCurElement).fY);
    fWidth->setValue(skin->getElement(fCurElement).fWidth);
    fHeight->setValue(skin->getElement(fCurElement).fHeight);
}
