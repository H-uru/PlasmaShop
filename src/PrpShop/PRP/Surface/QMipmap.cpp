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
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <Util/plDDSurface.h>
#include "QLinkLabel.h"
#include "QPlasmaUtils.h"

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

/* QTextureBox */
QTextureBox::~QTextureBox()
{
    delete fImage;
    delete[] fImageData;
}

void QTextureBox::setTexture(plMipmap* tex, int level)
{
    delete fImage;
    delete[] fImageData;

    if (tex == NULL) {
        fImage = NULL;
        fImageData = NULL;
        update();
        emit textureChanged(false);
        return;
    }

    if (level >= (int)tex->getNumLevels())
        level = tex->getNumLevels() - 1;
    if (level < 0) {
        fImage = NULL;
        fImageData = NULL;
        update();
        emit textureChanged(false);
        return;
    }

    size_t size = tex->GetUncompressedSize(level);
    fImageData = new unsigned char[size];
    tex->DecompressImage(level, fImageData, size);

    if (tex->getCompressionType() != plMipmap::kUncompressed) {
        // Manipulate the data from RGBA to BGRA
        unsigned int* dp = (unsigned int*)fImageData;
        for (size_t i=0; i<size; i += 4) {
            //unsigned int alpha = doAlpha ? (*dp & 0xFF000000) : 0xFF000000;
            *dp = (*dp & 0xFF000000)
                | (*dp & 0x00FF0000) >> 16
                | (*dp & 0x0000FF00)
                | (*dp & 0x000000FF) << 16;
            dp++;
        }
    }
    fImage = new QImage(fImageData, tex->getLevelWidth(level),
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

    QWidget* levelWidget = new QWidget(this);
    QGridLayout* levelLayout = new QGridLayout(levelWidget);
    levelLayout->setContentsMargins(4, 4, 4, 4);
    levelLayout->setHorizontalSpacing(8);
    QSpinBox* levelSel = new QSpinBox(levelWidget);
    if (tex->getCompressionType() == plBitmap::kJPEGCompression)
        levelSel->setRange(0, 0);
    else
        levelSel->setRange(0, tex->getNumLevels() - 1);
    connect(levelSel, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &QMipmap_Preview::setLevel);
    levelLayout->addWidget(new QLabel(tr("Level:"), levelWidget), 0, 0);
    levelLayout->addWidget(levelSel, 0, 1);
    levelSel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    QLinkLabel* saveAsLink = new QLinkLabel(tr("Save As..."), levelWidget);
    levelLayout->addWidget(saveAsLink, 1, 0, 1, 2);
    connect(fTexture, &QTextureBox::textureChanged, saveAsLink, &QWidget::setEnabled);
    connect(saveAsLink, &QLinkLabel::activated, fTexture, &QTextureBox::saveAs);
    fTexture->setTexture(tex);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setVerticalSpacing(0);
    layout->addWidget(levelWidget, 0, 0);
    layout->addWidget(scroll, 1, 0);
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

    QLinkLabel* xDDSLink = new QLinkLabel(tr("Export DDS..."), this);
    QLinkLabel* iDDSLink = new QLinkLabel(tr("Import DDS..."), this);
    QLinkLabel* xJPGLink = new QLinkLabel(tr("Export Jpeg..."), this);
    QLinkLabel* iJPGLink = new QLinkLabel(tr("Import Jpeg..."), this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpFlags, 0, 0, 1, 2);
    layout->addWidget(grpProps, 1, 0, 1, 2);
    layout->addWidget(fPreviewLink, 2, 0, 1, 2);
    layout->addItem(new QSpacerItem(0, 8, QSizePolicy::Minimum, QSizePolicy::Minimum), 3, 0, 1, 2);
    layout->addWidget(xDDSLink, 4, 0);
    layout->addWidget(iDDSLink, 5, 0);
    layout->addWidget(xJPGLink, 4, 1);
    layout->addWidget(iJPGLink, 5, 1);

    if (tex->getCompressionType() == plBitmap::kJPEGCompression) {
        xDDSLink->setEnabled(false);
        xJPGLink->setEnabled(true);
    } else {
        xDDSLink->setEnabled(true);
        xJPGLink->setEnabled(false);
    }

    connect(xDDSLink, &QLinkLabel::activated, this, &QMipmap::onExportDDS);
    connect(xJPGLink, &QLinkLabel::activated, this, &QMipmap::onExportJPEG);
    connect(iDDSLink, &QLinkLabel::activated, this, &QMipmap::onImportDDS);
    connect(iJPGLink, &QLinkLabel::activated, this, &QMipmap::onImportJPEG);
}

static void makeJColorSurface(const plMipmap* tex, hsStream* S)
{
    if (tex->getCompressionType() != plBitmap::kJPEGCompression) {
        QMessageBox::critical(NULL, QObject::tr("Error exporting JPEG"),
                              QObject::tr("Texture is not in a supported export format"),
                              QMessageBox::Ok);
        return;
    }

    plDDSurface dds;
    dds.fFlags = plDDSurface::DDSD_CAPS | plDDSurface::DDSD_HEIGHT
               | plDDSurface::DDSD_WIDTH | plDDSurface::DDSD_PIXELFORMAT
               | plDDSurface::DDSD_LINEARSIZE;
    dds.fCaps = plDDSurface::DDSCAPS_TEXTURE;
    dds.fHeight = tex->getHeight();
    dds.fWidth = tex->getWidth();
    dds.fLinearSize = dds.fHeight * dds.fWidth * 3;
    dds.fPixelFormat.fFlags = plDDSurface::DDPF_RGB;
    dds.fPixelFormat.fBitDepth = 24;
    dds.fPixelFormat.fRBitMask = 0xFF0000;
    dds.fPixelFormat.fGBitMask = 0x00FF00;
    dds.fPixelFormat.fBBitMask = 0x0000FF;

    // Strip down data to 24 bit color
    unsigned char* data = new unsigned char[dds.fLinearSize];
    tex->extractColorData(data, dds.fLinearSize);
    dds.setData(dds.fLinearSize, data);
    delete[] data;

    dds.write(S);
}

static void makeJAlphaSurface(const plMipmap* tex, hsStream* S)
{
    if (tex->getCompressionType() != plBitmap::kJPEGCompression) {
        QMessageBox::critical(NULL, QObject::tr("Error exporting JPEG"),
                              QObject::tr("Texture is not in a supported export format"),
                              QMessageBox::Ok);
        return;
    }

    plDDSurface dds;
    dds.fFlags = plDDSurface::DDSD_CAPS | plDDSurface::DDSD_HEIGHT
               | plDDSurface::DDSD_WIDTH | plDDSurface::DDSD_PIXELFORMAT
               | plDDSurface::DDSD_LINEARSIZE;
    dds.fCaps = plDDSurface::DDSCAPS_TEXTURE;
    dds.fHeight = tex->getHeight();
    dds.fWidth = tex->getWidth();
    dds.fLinearSize = dds.fHeight * dds.fWidth;
    dds.fPixelFormat.fFlags = plDDSurface::DDPF_LUMINANCE;
    dds.fPixelFormat.fBitDepth = 8;
    dds.fPixelFormat.fLuminanceBitMask = 0xFF;

    // Strip down data to alpha luminance
    unsigned char* data = new unsigned char[dds.fLinearSize];
    tex->extractAlphaData(data, dds.fLinearSize);
    dds.setData(dds.fLinearSize, data);
    delete[] data;

    dds.write(S);
}

static bool getJColorSurface(const plDDSurface& dds, plMipmap* tex)
{
    if ((dds.fFlags & plDDSurface::DDSD_HEIGHT) == 0 ||
        (dds.fFlags & plDDSurface::DDSD_WIDTH) == 0 ||
        (dds.fFlags & plDDSurface::DDSD_PIXELFORMAT) == 0) {
        QMessageBox::critical(NULL, QObject::tr("Error importing JPEG"),
                              QObject::tr("DDSurface does not contain required fields"),
                              QMessageBox::Ok);
        return false;
    }
    if ((dds.fPixelFormat.fFlags & plDDSurface::DDPF_RGB) == 0 ||
        (dds.fPixelFormat.fBitDepth != 24) ||
        (dds.fPixelFormat.fRBitMask != 0xFF0000) ||
        (dds.fPixelFormat.fGBitMask != 0x00FF00) ||
        (dds.fPixelFormat.fBBitMask != 0x0000FF)) {
        QMessageBox::critical(NULL, QObject::tr("Error importing JPEG"),
                              QObject::tr("DDS file should be in RGB888 format"));
        return false;
    }

    tex->Create(dds.fWidth, dds.fHeight, 0, plBitmap::kJPEGCompression, plBitmap::kRGB8888);
    tex->setColorData(dds.getData(), dds.getDataSize());
    return true;
}

static bool getJAlphaSurface(const plDDSurface& dds, plMipmap* tex)
{
    if ((dds.fFlags & plDDSurface::DDSD_HEIGHT) == 0 ||
        (dds.fFlags & plDDSurface::DDSD_WIDTH) == 0 ||
        (dds.fFlags & plDDSurface::DDSD_PIXELFORMAT) == 0) {
        QMessageBox::critical(NULL, QObject::tr("Error importing JPEG"),
                              QObject::tr("DDSurface does not contain required fields"),
                              QMessageBox::Ok);
        return false;
    }
    if ((dds.fPixelFormat.fFlags & plDDSurface::DDPF_LUMINANCE) == 0 ||
        (dds.fPixelFormat.fBitDepth != 8) ||
        (dds.fPixelFormat.fLuminanceBitMask != 0xFF)) {
        QMessageBox::critical(NULL, QObject::tr("Error importing JPEG"),
                              QObject::tr("DDS file should be in Luminance 8-bit format"),
                              QMessageBox::Ok);
        return false;
    }

    if (tex->getWidth() != dds.fWidth || tex->getHeight() != dds.fHeight) {
        QMessageBox::critical(NULL, QObject::tr("Error importing JPEG"),
                              QObject::tr("Alpha DDS size does not match image size"),
                              QMessageBox::Ok);
        return false;
    }
    tex->setAlphaData(dds.getData(), dds.getDataSize());
    return true;
}

void QMipmap::onExportDDS()
{
    plMipmap* tex = plMipmap::Convert(fCreatable);
    QString filename = st2qstr(tex->getKey()->getName())
                       .replace(QRegularExpression("[?:/\\*\"<>|]"), "_");
    filename = QFileDialog::getSaveFileName(this, tr("Export DDS"), getExportDir() + "/" + filename,
                                            "DDS Files (*.dds)");
    if (filename.isEmpty())
        return;

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
    S.close();

    setExportDir(filename);
}

void QMipmap::onExportJPEG()
{
    QString exportDir = getExportDir();

    plMipmap* tex = plMipmap::Convert(fCreatable);
    exportDir.append("/" + st2qstr(tex->getKey()->getName())
                           .replace(QRegularExpression("[?:/\\*\"<>|]"), "_"));
    QString filter = tex->isImageJPEG() ? "JPEG Files (*.jpg)" : "DDS Files (*.dds)";
    QString filename = QFileDialog::getSaveFileName(this, tr("Export JPEG Image"),
                                                    exportDir, filter);
    if (filename.isEmpty())
        return;

    exportDir.append("_ALPHA");
    filter = tex->isAlphaJPEG() ? "JPEG Files (*.jpg)" : "DDS Files (*.dds)";
    QString alphaFname = QFileDialog::getSaveFileName(this, tr("Export JPEG Alpha"),
                                                      exportDir, filter);
    if (alphaFname.isEmpty())
        return;

    hsFileStream S;
    if (!S.open(qstr2st(filename), fmCreate)) {
        QMessageBox::critical(this, tr("Error exporting JPEG"),
                              tr("Error: Could not open file %1 for writing").arg(filename),
                              QMessageBox::Ok);
        return;
    }
    if (tex->isImageJPEG())
        S.write(tex->getJpegSize(), tex->getJpegImage());
    else
        makeJColorSurface(tex, &S);
    S.close();

    if (!S.open(qstr2st(alphaFname), fmCreate)) {
        QMessageBox::critical(this, tr("Error exporting JPEG"),
                              tr("Error: Could not open file %1 for writing").arg(alphaFname),
                              QMessageBox::Ok);
        return;
    }
    if (tex->isAlphaJPEG())
        S.write(tex->getJpegAlphaSize(), tex->getJpegAlpha());
    else
        makeJAlphaSurface(tex, &S);
    S.close();

    setExportDir(filename);
}

void QMipmap::onImportDDS()
{
    plMipmap* tex = plMipmap::Convert(fCreatable);
    QString filename = QFileDialog::getOpenFileName(this, tr("Import DDS"), getExportDir(),
                                                    "DDS Files (*.dds)");
    if (filename.isEmpty())
        return;

    hsFileStream S;
    if (!S.open(qstr2st(filename), fmRead)) {
        QMessageBox::critical(this, tr("Error importing DDS"),
                              tr("Error: Could not open file %1 for reading").arg(filename),
                              QMessageBox::Ok);
        return;
    }
    try {
        plDDSurface dds;
        dds.read(&S);
        plMipmap* newTex = dds.createMipmap();
        tex->CopyFrom(newTex);
        delete newTex;
    } catch (hsException& ex) {
        QMessageBox::critical(this, tr("Error importing DDS"),
                              QString::fromUtf8(ex.what()), QMessageBox::Ok);
    }
    S.close();

    setExportDir(filename);
}

void QMipmap::onImportJPEG()
{
    QString exportDir = getExportDir();

    plMipmap* tex = plMipmap::Convert(fCreatable);
    QString filename = QFileDialog::getOpenFileName(this, tr("Import JPEG"), exportDir,
                                                    "JPEG Files (*.jpg *.jpeg *.dds)");
    if (filename.isEmpty())
        return;
    QString alphaFname = QFileDialog::getOpenFileName(this, tr("Import JPEG Alpha"), exportDir,
                                                      "JPEG Files (*.jpg *.jpeg *.dds)");
    if (alphaFname.isEmpty())
        return;

    plMipmap newTex;
    hsFileStream S;
    bool valid = true;
    if (!S.open(qstr2st(filename), fmRead)) {
        QMessageBox::critical(this, tr("Error importing JPEG"),
                              tr("Error: Could not open file %1 for reading").arg(filename),
                              QMessageBox::Ok);
        valid = false;
    }
    if (filename.toLower().endsWith(".dds")) {
        try {
            plDDSurface dds;
            dds.read(&S);
            if (!getJColorSurface(dds, &newTex))
                valid = false;
        } catch (hsException& ex) {
            QMessageBox::critical(this, tr("Error importing JPEG"),
                                  QString::fromUtf8(ex.what()), QMessageBox::Ok);
            valid = false;
        }
    } else {
        unsigned char* data = new unsigned char[S.size()];
        S.read(S.size(), data);
        QImage imgTemp(filename);
        newTex.Create(imgTemp.width(), imgTemp.height(), 0, plBitmap::kJPEGCompression,
                      plBitmap::kRGB8888);
        newTex.setImageJPEG(data, S.size());
        delete[] data;
    }
    S.close();

    if (!S.open(qstr2st(alphaFname), fmRead)) {
        QMessageBox::critical(this, tr("Error importing JPEG"),
                              tr("Error: Could not open file %1 for reading").arg(alphaFname),
                              QMessageBox::Ok);
        valid = false;
    }
    if (alphaFname.toLower().endsWith(".dds")) {
        try {
            plDDSurface dds;
            dds.read(&S);
            if (!getJAlphaSurface(dds, &newTex))
                valid = false;
        } catch (hsException& ex) {
            QMessageBox::critical(this, tr("Error importing JPEG"),
                                  QString::fromUtf8(ex.what()), QMessageBox::Ok);
            valid = false;
        }
    } else {
        unsigned char* data = new unsigned char[S.size()];
        S.read(S.size(), data);
        newTex.setAlphaJPEG(data, S.size());
        delete[] data;
    }
    S.close();

    if (valid)
        tex->CopyFrom(&newTex);

    setExportDir(filename);
}
