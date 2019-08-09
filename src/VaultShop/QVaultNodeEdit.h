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

#ifndef _QVAULTNODEEDIT_H
#define _QVAULTNODEEDIT_H

#include <QWidget>
#include <Vault/plVaultNode.h>
#include <SDL/plSDLMgr.h>
#include <ResManager/plResManager.h>
#include "QPlasma.h"

class QVaultNodeEdit : public QWidget
{
    Q_OBJECT

protected:
    plVaultNode fNode;

public:
    QVaultNodeEdit(QWidget* parent) : QWidget(parent) { }
    static QVaultNodeEdit* MakeEditor(QWidget* parent, const plVaultNode& node,
                                      plResManager* mgr, plSDLMgr* sdl);

    virtual QString getEditorTitle() const = 0;
    virtual plVaultNode saveNode() = 0;
    virtual void setMgrs(plResManager*, plSDLMgr*) { }

protected:
    virtual void IRefreshNode() = 0;

signals:
    void subscribe(unsigned int nodeId);

public slots:
    void setNode(const plVaultNode& node);
};

#endif
