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

#ifndef _QPLASMASUMFILE_H
#define _QPLASMASUMFILE_H

#include "QPlasmaDocument.h"
#include <Stream/hsStream.h>
#include <Util/hsSumFile.h>
#include <QTreeWidget>
#include <QAction>
#include <vector>

class QPlasmaSumFile : public QPlasmaDocument
{
    Q_OBJECT

public:
    QPlasmaSumFile(QWidget* parent);

    virtual bool loadFile(QString filename);
    virtual bool saveTo(QString filename);

private:
    QTreeWidget* fFileList;
    hsSumFile fSumData;

    enum
    {
        kAUpdate, kAAdd, kADel, kANumActions
    };
    QAction* fActions[kANumActions];

    bool loadSumData(hsStream* S);
    bool saveSumData(hsStream* S);
    void addToSumFile(const QString& filename);

private slots:
    void onContextMenu(QPoint pos);
    void onItemChanged(QTreeWidgetItem* item, int column);
    void onUpdate();
    void onAdd();
    void onDel();
};

#endif
