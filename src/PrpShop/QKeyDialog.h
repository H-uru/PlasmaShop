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

#ifndef _QKEYDIALOG_H
#define _QKEYDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <ResManager/plResManager.h>
#include <vector>

class QNewKeyDialog : public QDialog {
    Q_OBJECT

protected:
    QComboBox* fLocationBox;
    QComboBox* fTypeBox;
    QLineEdit* fName;
    std::vector<plLocation> fLocations;
    std::vector<short> fTypes;

public:
    QNewKeyDialog(QWidget* parent = NULL);
    void init(plResManager* mgr, plLocation loc = plLocation(), short type = -1);

    plLocation location() const;
    short type() const;
    QString name() const;
};

class QFindKeyDialog : public QDialog {
    Q_OBJECT

protected:
    QComboBox* fLocationBox;
    QComboBox* fTypeBox;
    QComboBox* fKeyBox;
    plResManager* fResMgr;
    std::vector<plLocation> fLocations;
    std::vector<short> fTypes;
    std::vector<plKey> fKeys;

public:
    QFindKeyDialog(QWidget* parent = NULL);
    void init(plResManager* mgr, plLocation loc = plLocation(), short type = -1);
    void init(plResManager* mgr, plKey current);

    plKey selection() const;

protected slots:
    void locationSelected(int idx);
    void typeSelected(int idx);
};

#endif
