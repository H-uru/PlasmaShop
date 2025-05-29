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

#include "QMipmap.h"

#include <QScrollArea>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QPainter>
#include <QSettings>
#include <QTabBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <Util/plDDSurface.h>
#include "QLinkLabel.h"
#include "QPlasmaUtils.h"
#include <Util/plJPEG.h>
#include <Util/plPNG.h>

/* Helpers */
static QString getExportDir()
{
    QSettings settings("PlasmaShop", "PrpShop");
    QString exportDir = settings.value("ExportDir").toString();
    if (exportDir.isEmpty())
        exportDir = settings.value("DialogDir").toString();
    return exportDir;
}

static void setExportDir(const QString& filename)
{
    QDir dir = QDir(filename);
    dir.cdUp();
    QSettings settings("PlasmaShop", "PrpShop");
    settings.setValue("ExportDir", dir.absolutePath());
}

static std::unique_ptr<unsigned char[]> getTextureData(plMipmap *tex, size_t level=0)
{
    Q_ASSERT(tex->getNumLevels() > 0);
    if (level >= tex->getNumLevels())
        level = tex->getNumLevels() - 1;

    size_t size = tex->GetUncompressedSize(level);
    auto imageData = std::make_unique<unsigned char[]>(size);
    tex->DecompressImage(level, imageData.get(), size);

    if (tex->getCompressionType() == plMipmap::kDirectXCompression) {
        // Manipulate the data from RGBA to BGRA
        unsigned int* dp = (unsigned int*)imageData.get();
        for (size_t i = 0; i < size; i += 4) {
            //unsigned int alpha = doAlpha ? (*dp & 0xFF000000) : 0xFF000000;
            *dp = (*dp & 0xFF00FF00)
                | (*dp & 0x00FF0000) >> 16
                | (*dp & 0x000000FF) << 16;
            dp++;
        }
    }

    return imageData;
}

/* QTextureBox */
QTextureBox::~QTextureBox()
{
    delete fImage;
}

void QTextureBox::setTexture(plMipmap* tex, int level)
{
    delete fImage;

    if (tex == NULL) {
        fImage = NULL;
        fImageData = NULL;
        update();
        emit textureChanged(false);
        return;
    }

    if (level < 0) {
        fImage = NULL;
        fImageData = NULL;
        update();
        emit textureChanged(false);
        return;
    }

    fImageData = getTextureData(tex, level);
    fImage = new QImage(fImageData.get(), tex->getLevelWidth(level),
                        tex->getLevelHeight(level), QImage::Format_ARGB32);
    resize(tex->getLevelWidth(level), tex->getLevelHeight(level));
    update();
    emit textureChanged(true);
}

void QTextureBox::saveAs()
{
    if (!fImage)
        return;
    QString filename = QFileDialog::getSaveFileName(this, tr("Save As..."),
        getExportDir(), "Images (*.bmp *.jpg *.png *.tiff)");
    if (filename.isEmpty())
        return;
    fImage->save(filename);
    setExportDir(filename);
}

void QTextureBox::paintEvent(QPaintEvent* evt)
{
    if (fImage == NULL) {
        QWidget::paintEvent(evt);
    } else {
        QPainter painter(this);
        painter.drawImage(QPoint(0, 0), *fImage);
        painter.end();
    }
}


/* QMipmap_Preview */
QMipmap_Preview::QMipmap_Preview(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kPreviewMipmap, parent), fLevel()
{
    plMipmap* tex = plMipmap::Convert(fCreatable);

    QScrollArea* scroll = new QScrollArea(this);
    fTexture = new QTextureBox(scroll);
    scroll->setWidget(fTexture);

    QTabBar* levelSel = new QTabBar(this);
    for (size_t level = 0; level < tex->getNumLevels(); level++) {
        levelSel->insertTab(level, QString("%1x%2 (%3)")
            .arg(tex->getLevelWidth(level))
            .arg(tex->getLevelHeight(level))
            .arg(level));
    }
    connect(levelSel, &QTabBar::currentChanged,
            this, &QMipmap_Preview::setLevel);

    QWidget* saveAsWidget = new QWidget(this);
    QGridLayout* saveAsLayout = new QGridLayout(saveAsWidget);
    saveAsLayout->setContentsMargins(4, 4, 4, 4);
    saveAsLayout->setHorizontalSpacing(8);
    QLinkLabel* saveAsLink = new QLinkLabel(tr("Save As..."), saveAsWidget);
    saveAsLayout->addWidget(saveAsLink, 0, 0);
    connect(fTexture, &QTextureBox::textureChanged, saveAsLink, &QWidget::setEnabled);
    connect(saveAsLink, &QLinkLabel::activated, fTexture, &QTextureBox::saveAs);

    fTexture->setTexture(tex);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setVerticalSpacing(0);
    layout->addWidget(levelSel, 0, 0);
    layout->addWidget(saveAsWidget, 1, 0);
    layout->addWidget(scroll, 2, 0);
}

void QMipmap_Preview::setLevel(int level)
{
    fLevel = level;
    fTexture->setTexture(plMipmap::Convert(fCreatable), fLevel);
}


/* QMipmap */
QString getCompressionText(plBitmap* tex)
{
    if (tex->getCompressionType() == plBitmap::kDirectXCompression) {
        switch (tex->getDXCompression()) {
        case plBitmap::kDXT1:
            return "DXT1";
        case plBitmap::kDXT3:
            return "DXT3";
        case plBitmap::kDXT5:
            return "DXT5";
        }
    } else if (tex->getCompressionType() == plBitmap::kJPEGCompression) {
        switch (tex->getARGBType()) {
        case plBitmap::kRGB8888:
            return "JPEG (ARGB8888)";
        case plBitmap::kRGB4444:
            return "JPEG (ARGB4444)";
        case plBitmap::kRGB1555:
            return "JPEG (ARGB1555)";
        case plBitmap::kInten8:
            return "JPEG (Greyscale)";
        case plBitmap::kAInten88:
            return "JPEG (Alpha+Greyscale)";
        }
    } else if (tex->getCompressionType() == plBitmap::kPNGCompression) {
        switch (tex->getARGBType()) {
        case plBitmap::kRGB8888:
            return "PNG (ARGB8888)";
        case plBitmap::kRGB4444:
            return "PNG (ARGB4444)";
        case plBitmap::kRGB1555:
            return "PNG (ARGB1555)";
        case plBitmap::kInten8:
            return "PNG (Greyscale)";
        case plBitmap::kAInten88:
            return "PNG (Alpha+Greyscale)";
        }
    } else {
        switch (tex->getARGBType()) {
        case plBitmap::kRGB8888:
            return "Uncompressed (ARGB8888)";
        case plBitmap::kRGB4444:
            return "Uncompressed (ARGB4444)";
        case plBitmap::kRGB1555:
            return "Uncompressed (ARGB1555)";
        case plBitmap::kInten8:
            return "Uncompressed (Greyscale)";
        case plBitmap::kAInten88:
            return "Uncompressed (Alpha+Greyscale)";
        }
    }
    return "(INVALID)";
}

QMipmap::QMipmap(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kMipmap, parent)
{
    plMipmap* tex = plMipmap::Convert(fCreatable);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setVerticalSpacing(0);
    layFlags->setHorizontalSpacing(8);
    fFlags[kAlphaChannelFlag] = new QBitmaskCheckBox(plBitmap::kAlphaChannelFlag,
            tr("Alpha Channel"), grpFlags);
    fFlags[kAlphaBitFlag] = new QBitmaskCheckBox(plBitmap::kAlphaBitFlag,
            tr("Alpha Bit"), grpFlags);
    fFlags[kBumpEnvMap] = new QBitmaskCheckBox(plBitmap::kBumpEnvMap,
            tr("Bump Env Map"), grpFlags);
    fFlags[kForce32Bit] = new QBitmaskCheckBox(plBitmap::kForce32Bit,
            tr("Force 32-bit"), grpFlags);
    fFlags[kDontThrowAwayImage] = new QBitmaskCheckBox(plBitmap::kDontThrowAwayImage,
            tr("Don't Throw Away"), grpFlags);
    fFlags[kForceOneMipLevel] = new QBitmaskCheckBox(plBitmap::kForceOneMipLevel,
            tr("Force One Level"), grpFlags);
    fFlags[kNoMaxSize] = new QBitmaskCheckBox(plBitmap::kNoMaxSize,
            tr("No Maximum Size"), grpFlags);
    fFlags[kIntensityMap] = new QBitmaskCheckBox(plBitmap::kIntensityMap,
            tr("Intensity Map"), grpFlags);
    fFlags[kHalfSize] = new QBitmaskCheckBox(plBitmap::kHalfSize,
            tr("Half Size"), grpFlags);
    fFlags[kUserOwnsBitmap] = new QBitmaskCheckBox(plBitmap::kUserOwnsBitmap,
            tr("User Owned"), grpFlags);
    fFlags[kForceRewrite] = new QBitmaskCheckBox(plBitmap::kForceRewrite,
            tr("Force Rewrite"), grpFlags);
    fFlags[kForceNonCompressed] = new QBitmaskCheckBox(plBitmap::kForceNonCompressed,
            tr("Force Non-compressed"), grpFlags);
    fFlags[kIsTexture] = new QBitmaskCheckBox(plBitmap::kIsTexture,
            tr("Is Texture"), grpFlags);
    fFlags[kIsOffscreen] = new QBitmaskCheckBox(plBitmap::kIsOffscreen,
            tr("Is Offscreen"), grpFlags);
    fFlags[kIsProjected] = new QBitmaskCheckBox(plBitmap::kIsProjected,
            tr("Is Projected"), grpFlags);
    fFlags[kIsOrtho] = new QBitmaskCheckBox(plBitmap::kIsOrtho,
            tr("Is Orthogonal"), grpFlags);
    layFlags->addWidget(fFlags[kAlphaChannelFlag], 0, 0);
    layFlags->addWidget(fFlags[kAlphaBitFlag], 1, 0);
    layFlags->addWidget(fFlags[kBumpEnvMap], 2, 0);
    layFlags->addWidget(fFlags[kForce32Bit], 3, 0);
    layFlags->addWidget(fFlags[kDontThrowAwayImage], 0, 1);
    layFlags->addWidget(fFlags[kForceOneMipLevel], 1, 1);
    layFlags->addWidget(fFlags[kNoMaxSize], 2, 1);
    layFlags->addWidget(fFlags[kForceNonCompressed], 3, 1);
    layFlags->addWidget(fFlags[kHalfSize], 0, 2);
    layFlags->addWidget(fFlags[kUserOwnsBitmap], 1, 2);
    layFlags->addWidget(fFlags[kForceRewrite], 2, 2);
    layFlags->addWidget(fFlags[kIntensityMap], 3, 2);
    layFlags->addWidget(fFlags[kIsTexture], 0, 3);
    layFlags->addWidget(fFlags[kIsOffscreen], 1, 3);
    layFlags->addWidget(fFlags[kIsProjected], 2, 3);
    layFlags->addWidget(fFlags[kIsOrtho], 3, 3);

    for (auto cb : fFlags) {
        cb->setFrom(tex->getFlags());
        connect(cb, &QBitmaskCheckBox::setBits, this, [this](unsigned int mask) {
            plMipmap* tex = plMipmap::Convert(fCreatable);
            tex->setFlags(tex->getFlags() | mask);
        });
        connect(cb, &QBitmaskCheckBox::unsetBits, this, [this](unsigned int mask) {
            plMipmap* tex = plMipmap::Convert(fCreatable);
            tex->setFlags(tex->getFlags() & ~mask);
        });
    }

    QGroupBox* grpProps = new QGroupBox(tr("Texture Propreties"), this);
    QGridLayout* layProps = new QGridLayout(grpProps);
    layProps->setVerticalSpacing(0);
    layProps->setHorizontalSpacing(8);
    layProps->addWidget(new QLabel(tr("Bits per Pixel:"), grpProps), 0, 0);
    layProps->addWidget(new QLabel(tr("%1").arg(tex->getBPP()), grpProps), 0, 1);
    layProps->addItem(new QSpacerItem(16, 0), 0, 2, 2, 1);
    layProps->addWidget(new QLabel(tr("Compression Type:"), grpProps), 0, 3);
    layProps->addWidget(new QLabel(getCompressionText(tex), grpProps), 0, 4);
    layProps->addWidget(new QLabel(tr("Texture Size:"), grpProps), 1, 0);
    layProps->addWidget(new QLabel(tr("%1x%2").arg(tex->getWidth()).arg(tex->getHeight()), grpProps), 1, 1);
    layProps->addWidget(new QLabel(tr("Color Space:"), grpProps), 1, 3);
    layProps->addWidget(new QLabel(plBitmap::kSpaceNames[tex->getSpace()], grpProps), 1, 4);

    fPreviewLink = new QCreatableLink(this, false);
    fPreviewLink->setCreatable(tex, tr("Preview"));
    fPreviewLink->setForceType(kPreviewMipmap);

    QLinkLabel* xPort = new QLinkLabel(tr("Export Image..."), this);
    QLinkLabel* iPort = new QLinkLabel(tr("Import Image..."), this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpFlags, 0, 0, 1, 2);
    layout->addWidget(grpProps, 1, 0, 1, 2);
    layout->addWidget(fPreviewLink, 2, 0, 1, 2);
    layout->addItem(new QSpacerItem(0, 8, QSizePolicy::Minimum, QSizePolicy::Minimum), 3, 0, 1, 2);
    layout->addWidget(xPort, 4, 0);
    layout->addWidget(iPort, 5, 0);

    connect(xPort, &QLinkLabel::activated, this, &QMipmap::onExportImage);
    connect(iPort, &QLinkLabel::activated, this, &QMipmap::onImportImage);
}

void QMipmap::onExportImage() {
    QString exportDir = getExportDir();
    plMipmap* tex = plMipmap::Convert(fCreatable);

    exportDir.append("/" + st2qstr(tex->getKey()->getName())
                           .replace(QRegularExpression("[?:/\\*\"<>|]"), "_"));
    QString filter = tr("DDS Files (*.dds);;JPEG Files (*.jpg *.jpeg);;PNG Files (*.png)");
    QString filename = QFileDialog::getSaveFileName(this, tr("Export Image"), exportDir, filter);

    if (filename.isEmpty())
        return;

    QString file_ext = QFileInfo(filename).suffix().toLower();
    if (file_ext == "dds") {
        hsFileStream S;
        if (!S.open(qstr2st(filename), fmCreate)) {
            QMessageBox::critical(this, tr("Error exporting DDS"),
                                  tr("Error: Could not open file %1 for writing").arg(filename),
                                  QMessageBox::Ok);
            return;
        }

        try {
            plDDSurface dds;
            dds.setFromMipmap(tex);
            dds.write(&S);
        } catch (hsException& ex) {
            QMessageBox::critical(this, tr("Error exporting DDS"),
                                  QString::fromUtf8(ex.what()), QMessageBox::Ok);
        }
    } else if (file_ext == "jpg" || file_ext == "jpeg") {
        hsFileStream S;
        if (!S.open(qstr2st(filename), fmCreate)) {
            QMessageBox::critical(this, tr("Error exporting JPEG"),
                                  tr("Error: Could not open file %1 for writing").arg(filename),
                                  QMessageBox::Ok);
            return;
        }

        if (tex->isImageJPEG()) {
            S.write(tex->getJpegSize(), tex->getJpegImage());
        } else {
            size_t image_size = tex->GetUncompressedSize(0);
            auto image_data = std::make_unique<unsigned char[]>(image_size);
            tex->DecompressImage(0, image_data.get(), image_size);
            plJPEG::CompressJPEG(&S, image_data.get(), image_size, tex->getWidth(), tex->getHeight(), tex->getBPP());
        }
    } else if (file_ext == "png") {
        hsFileStream S;
        if (!S.open(qstr2st(filename), fmCreate)) {
            QMessageBox::critical(this, tr("Error exporting PNG"),
                                  tr("Error: Could not open file %1 for writing").arg(filename),
                                  QMessageBox::Ok);
            return;
        }

        try {
            auto imageData = getTextureData(tex);
            plPNG::CompressPNG(&S, imageData.get(), tex->GetUncompressedSize(0), tex->getLevelWidth(0), tex->getLevelHeight(0), tex->getBPP());
        } catch (hsException& ex) {
            QMessageBox::critical(this, tr("Error exporting PNG"),
                                  QString::fromUtf8(ex.what()), QMessageBox::Ok);
        }
    }

    setExportDir(filename);
}

void QMipmap::onImportImage()
{
    QString importDir = getExportDir();
    plMipmap* tex = plMipmap::Convert(fCreatable);

    importDir.append("/" + st2qstr(tex->getKey()->getName())
                           .replace(QRegularExpression("[?:/\\*\"<>|]"), "_"));
    QString filter = tr("All Images (*.dds *.jpg *.jpeg *.png);;DDS Files (*.dds);;JPEG Files (*.jpg *.jpeg);;PNG Files (*.png)");
    QString filename = QFileDialog::getOpenFileName(this, tr("Import Image"), importDir, filter);

    if (filename.isEmpty())
        return;

    hsFileStream S;
    if (!S.open(qstr2st(filename), fmRead)) {
        QMessageBox::critical(this, tr("Error importing image"),
            tr("Error: Could not open file %1 for reading").arg(filename),
            QMessageBox::Ok);
        return;
    }

    try {
        QString file_ext = QFileInfo(filename).suffix().toLower();
        if (file_ext == "dds") {
            plDDSurface dds;
            dds.read(&S);
            std::unique_ptr<plMipmap> newTex(dds.createMipmap());
            tex->CopyFrom(newTex.get());
        } else if (file_ext == "jpg" || file_ext == "jpeg") {
            std::unique_ptr<plMipmap> newTex(plJPEG::DecompressJPEG(&S));
            tex->CopyFrom(newTex.get());
        } else if (file_ext == "png") {
            std::unique_ptr<plMipmap> newTex(plPNG::DecompressPNG(&S));
            tex->CopyFrom(newTex.get());
        }
    } catch (hsException& ex) {
        QMessageBox::critical(this, tr("Error importing image"),
            QString::fromUtf8(ex.what()), QMessageBox::Ok);
    }
}
