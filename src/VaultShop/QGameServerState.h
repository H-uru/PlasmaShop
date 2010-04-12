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

#ifndef _QGAMESERVERSTATE_H
#define _QGAMESERVERSTATE_H

#include "QVaultSDLNode.h"
#include <QComboBox>
#include "SDL/plNetGameServerState.h"

class QGameServerState : public QWidget {
    Q_OBJECT

public:
    QGameServerState(QWidget* parent);
    void loadSav(plNetGameServerState* state);

private slots:
    void objectChanged(int idx);

private:
    QComboBox* fObjectSel;
    QSDLEditor* fSDLEditor;

    plNetGameServerState* fState;
};

#endif
