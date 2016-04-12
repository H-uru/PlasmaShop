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

#ifndef _QOBJLINK_H
#define _QOBJLINK_H

#include <PRP/KeyedObject/hsKeyedObject.h>
#include "../../QLinkLabel.h"

class QCreatableLink : public QWidget {
    Q_OBJECT

protected:
    plCreatable* fCreatable;
    QLinkLabel* fObjLabel;
    QLinkLabel* fEditLabel;
    short fForceType;

public:
    QCreatableLink(QWidget* parent = NULL, bool canEdit = true);
    void setText(const QString& str) { fObjLabel->setText(str); }
    void setCreatable(plCreatable* pCre, const QString &text = QString());
    void setKey(plKey key, bool updateText = true);
    void setForceType(short forceType) { fForceType = forceType; }

protected slots:
    void objectActivated();
    virtual void menuRequested();

signals:
    void createObject(short type);
    void addObject();
    void delObject();
};

class QControllerLink : public QCreatableLink {
    Q_OBJECT

public:
    QControllerLink(const QString& text, QWidget* parent = NULL,
                    bool canEdit = true);

protected slots:
    virtual void menuRequested();
};

#endif
