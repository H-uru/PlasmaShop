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

#ifndef _QVAULTIMAGENODE_H
#define _QVAULTIMAGENODE_H

#include <QComboBox>
#include <QLineEdit>
#include <QPixmap>
#include "QVaultNodeEdit.h"

class QStaticImage : public QWidget {
    Q_OBJECT

public:
    QStaticImage(QWidget* parent);
    virtual ~QStaticImage();

    void set(QPixmap* img);
    QPixmap* get() const;
    void refresh();

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent*);

private:
    QPixmap* fImg;
    QSize fScale;
};


class QVaultImageNode : public QVaultNodeEdit {
    Q_OBJECT

protected:
    QLineEdit* fImgTitle;
    QComboBox* fImgType;
    QStaticImage* fImage; 

public:
    QVaultImageNode(QWidget* parent);
    virtual QString getEditorTitle() const;
    virtual plVaultNode saveNode();

protected slots:
    void IImportImage();
    void IExportImage();

protected:
    virtual void IRefreshNode();
};

#endif
