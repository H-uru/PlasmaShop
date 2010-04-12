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
#include <Util/plMD5.h>
#include <QTreeWidget>
#include <QAction>
#include <vector>

struct SumData {
    struct Entry {
        plString fPath;
        plMD5Hash fHash;
        time_t fTimestamp;
        unsigned int fUnknown;

        Entry();
    };

    unsigned int fUnknown;
    std::vector<Entry> fEntries;

    SumData();
    void read(hsStream* S);
    void write(hsStream* S);
};

class QPlasmaSumFile : public QPlasmaDocument {
    Q_OBJECT

public:
    QPlasmaSumFile(QWidget* parent);

    virtual bool loadFile(QString filename);
    virtual bool saveTo(QString filename);

private:
    QTreeWidget* fFileList;
    SumData fSumData;

    enum {
        kAUpdate, kAAdd, kADel, kANumActions
    };
    QAction* fActions[kANumActions];

    bool loadSumData(hsStream* S);
    bool saveSumData(hsStream* S);

private slots:
    void onContextMenu(QPoint pos);
    void onItemChanged(QTreeWidgetItem* item, int column);
    void onUpdate();
    void onAdd();
    void onDel();
};

#endif
