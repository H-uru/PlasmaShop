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

#include "QSceneNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QTabWidget>

QSceneNode::QSceneNode(plCreatable* pCre, QWidget* parent)
          : QCreatable(pCre, kSceneNode, parent)
{
    plSceneNode* node = plSceneNode::Convert(fCreatable);

    QTabWidget* tab = new QTabWidget(this);
    fSceneKeys = new QKeyList(node->getKey(), tab);
    fPoolKeys = new QKeyList(node->getKey(), tab);
    for (size_t i=0; i<node->getSceneObjects().size(); i++)
        fSceneKeys->addKey(node->getSceneObjects()[i]);
    for (size_t i=0; i<node->getPoolObjects().size(); i++)
        fPoolKeys->addKey(node->getPoolObjects()[i]);
    tab->addTab(fSceneKeys, tr("Scene Objects"));
    tab->addTab(fPoolKeys, tr("Pool Objects"));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(tab, 0, 0);
    fSceneKeys->adjustColumns();
    fPoolKeys->adjustColumns();
}

void QSceneNode::saveDamage()
{
    plSceneNode* node = plSceneNode::Convert(fCreatable);

    node->getSceneObjects().clear();
    node->getPoolObjects().clear();
    QList<plKey> sokeys = fSceneKeys->keys();
    while (!sokeys.isEmpty())
        node->getSceneObjects().push_back(sokeys.takeFirst());
    QList<plKey> pokeys = fPoolKeys->keys();
    while (!pokeys.isEmpty())
        node->getPoolObjects().push_back(pokeys.takeFirst());
}
