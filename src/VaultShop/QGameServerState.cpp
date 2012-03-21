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

#include "QGameServerState.h"
#include <QLabel>

QGameServerState::QGameServerState(QWidget* parent)
                : QWidget(parent)
{
    fObjectSel = new QComboBox(this);
    fSDLEditor = new QSDLEditor(this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Object:"), this), 0, 0);
    layout->addWidget(fObjectSel, 0, 1);
    layout->addWidget(fSDLEditor, 1, 1);

    connect(fObjectSel, SIGNAL(currentIndexChanged(int)),
            this, SLOT(objectChanged(int)));
}

void QGameServerState::loadSav(plNetGameServerState* state)
{
    fObjectSel->clear();
    fState = state;

    int selId = 0;
    for (size_t i=0; i<fState->numRecords(); i++) {
        fObjectSel->addItem(QIcon(":img/sceneobj.png"),
                            ~fState->getObject(i).getName());

        // If this is the AgeSDLHook, select it by default
        if (fState->getObject(i).getName() == "AgeSDLHook")
            selId = (int)i;
    }

    if (fObjectSel->count() > 0) {
        fObjectSel->setEnabled(true);
        fSDLEditor->setEnabled(true);
        fSDLEditor->setMgrs(NULL, &fState->getSDLMgr());
        fObjectSel->setCurrentIndex(selId);
    } else {
        // No SDL records, so disable everything
        fObjectSel->setEnabled(false);
        fSDLEditor->setEnabled(false);
    }
}

void QGameServerState::objectChanged(int idx)
{
    fSDLEditor->setRecord(fState->getRecord(idx));
}
