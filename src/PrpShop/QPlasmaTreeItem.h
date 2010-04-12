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

#ifndef _QPLASMATREEITEM_H
#define _QPLASMATREEITEM_H

#include <QTreeWidgetItem>
#include <ResManager/plResManager.h>
#include <PRP/KeyedObject/hsKeyedObject.h>

class QPlasmaTreeItem : public QTreeWidgetItem {
private:
    plKey fObjKey;
    plPageInfo* fPage;
    bool fHasBuiltIn, fHasTextures;
    QString fAge;

    QString fFilename;

public:
    enum ItemType {
        kTypeNone = QTreeWidgetItem::UserType, kTypeAge, kTypePage, kTypeKO,
        kMaxPlasmaTypes
    };

    QPlasmaTreeItem();
    QPlasmaTreeItem(plKey obj);
    QPlasmaTreeItem(const QString& age);
    QPlasmaTreeItem(plPageInfo* page);
    QPlasmaTreeItem(QTreeWidget* parent);
    QPlasmaTreeItem(QTreeWidget* parent, plKey obj);
    QPlasmaTreeItem(QTreeWidget* parent, const QString& age);
    QPlasmaTreeItem(QTreeWidget* parent, plPageInfo* page);
    QPlasmaTreeItem(QTreeWidgetItem* parent);
    QPlasmaTreeItem(QTreeWidgetItem* parent, plKey obj);
    QPlasmaTreeItem(QTreeWidgetItem* parent, const QString& age);
    QPlasmaTreeItem(QTreeWidgetItem* parent, plPageInfo* page);

    hsKeyedObject* obj() const;
    QString age() const;
    plPageInfo* page() const;

    bool hasBuiltIn() const;
    bool hasTextures() const;
    void setHasBuiltIn(bool has=true);
    void setHasTextures(bool has=true);

    QString filename() const;
    void setFilename(const QString& filename);
};

#endif
