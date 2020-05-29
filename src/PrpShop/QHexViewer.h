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

#ifndef _QHEXVIEWER_H
#define _QHEXVIEWER_H

#include "PRP/QCreatable.h"

class QHexWidget;
class QLabel;
class QCheckBox;
class QStatusBar;

class QHexViewer : public QCreatable
{
    Q_OBJECT

protected:
    QHexWidget* fViewer;
    QLabel* fI32Val;
    QLabel* fI16Val;
    QLabel* fI8Val;
    QLabel* fFloatVal;
    QLabel* fDoubleVal;
    QLabel* fStringVal;
    QCheckBox* fSigned;
    QStatusBar* fStatusBar;

public:
    QHexViewer(plCreatable* pCre, QWidget* parent = Q_NULLPTR);

    void loadObject(const QString& filename, uint32_t offset, uint32_t size);

private slots:
    void cursorChanged(int address);
    void signedChanged(bool);
};

#endif
