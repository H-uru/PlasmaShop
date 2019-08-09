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

#ifndef _QMIPMAP_H
#define _QMIPMAP_H

#include "PRP/QCreatable.h"
#include "PRP/QObjLink.h"
#include <QImage>
#include <QCheckBox>
#include <QSpinBox>
#include <PRP/Surface/plMipmap.h>

class QTextureBox : public QWidget
{
    Q_OBJECT

protected:
    QImage* fImage;
    unsigned char* fImageData;

public:
    QTextureBox(QWidget* parent = NULL)
        : QWidget(parent), fImage(), fImageData() { }

    ~QTextureBox();
    void setTexture(plMipmap* tex, int level = 0);

public slots:
    void saveAs();

protected:
    void paintEvent(QPaintEvent*) override;

signals:
    void textureChanged(bool success);
};

class QMipmap_Preview : public QCreatable
{
    Q_OBJECT

protected:
    QTextureBox* fTexture;
    int fLevel;

public:
    QMipmap_Preview(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override { }

    int level() const { return fLevel; }

public slots:
    void setLevel(int level);
};

class QMipmap : public QCreatable
{
    Q_OBJECT

protected:
    enum
    {
        kAlphaChannelFlag, kAlphaBitFlag, kBumpEnvMap, kForce32Bit,
        kDontThrowAwayImage, kForceOneMipLevel, kNoMaxSize, kIntensityMap,
        kHalfSize, kUserOwnsBitmap, kForceRewrite, kForceNonCompressed,
        kIsTexture, kIsOffscreen, kIsProjected, kIsOrtho, kNumBitmapFlags
    };
    QCheckBox* fFlags[kNumBitmapFlags];
    QCreatableLink* fPreviewLink;

public:
    QMipmap(plCreatable* pCre, QWidget* parent = NULL);
    void saveDamage() override;

private slots:
    void onExportDDS();
    void onExportJPEG();
    void onImportDDS();
    void onImportJPEG();
};

QString getCompressionText(plBitmap* tex);

#endif
