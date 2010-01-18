#include "QMipmap.h"

#include <QScrollArea>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QPainter>
#include "../QLinkLabel.h"
#include "../../QPlasmaUtils.h"

/* QTextureBox */
QTextureBox::QTextureBox(QWidget* parent)
           : QWidget(parent), fImage(NULL), fImageData(NULL)
{ }

QTextureBox::~QTextureBox()
{
    if (fImage != NULL)
        delete fImage;
    if (fImageData != NULL)
        delete[] fImageData;
}

void QTextureBox::setTexture(plMipmap* tex, int level)
{
    if (fImage != NULL)
        delete fImage;
    if (fImageData != NULL)
        delete[] fImageData;

    if (tex == NULL) {
        fImage = NULL;
        fImageData = NULL;
        update();
        return;
    }

    if (level >= (int)tex->getNumLevels())
        level = tex->getNumLevels() - 1;
    if (level < 0) {
        fImage = NULL;
        fImageData = NULL;
        update();
        return;
    }

    size_t size = tex->GetUncompressedSize(level);
    fImageData = new unsigned char[size];
    tex->DecompressImage(level, fImageData, size);

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
    fImage = new QImage(fImageData, tex->getLevelWidth(level),
                        tex->getLevelHeight(level), QImage::Format_ARGB32);
    resize(tex->getLevelWidth(level), tex->getLevelHeight(level));
    update();
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
               : QCreatable(pCre, kPreviewMipmap, parent), fLevel(0)
{
    plMipmap* tex = (plMipmap*)fCreatable;

    QScrollArea* scroll = new QScrollArea(this);
    fTexture = new QTextureBox(scroll);
    fTexture->setTexture(tex);
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
    connect(levelSel, SIGNAL(valueChanged(int)), this, SLOT(setLevel(int)));
    levelLayout->addWidget(new QLabel(tr("Level:"), levelWidget), 0, 0);
    levelLayout->addWidget(levelSel, 0, 1);
    levelSel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setVerticalSpacing(0);
    layout->addWidget(levelWidget, 0, 0);
    layout->addWidget(scroll, 1, 0);
}

void QMipmap_Preview::saveDamage()
{ }

void QMipmap_Preview::setLevel(int level)
{
    fLevel = level;
    fTexture->setTexture((plMipmap*)fCreatable, fLevel);
}

int QMipmap_Preview::level() const
{ return fLevel; }


/* QMipmap */
QString getCompressionText(plBitmap* tex)
{
    if (tex->getCompressionType() == plBitmap::kDirectXCompression) {
        switch (tex->getDXCompression()) {
        case plBitmap::kDXT1:
            return "DXT1";
        case plBitmap::kDXT2:
            return "DXT2";
        case plBitmap::kDXT3:
            return "DXT3";
        case plBitmap::kDXT4:
            return "DXT4";
        case plBitmap::kDXT5:
            return "DXT5";
        }
    } else if (tex->getCompressionType() == plBitmap::kJPEGCompression) {
        switch (tex->getARGBType()) {
        case plBitmap::kRGB8888:
            return "JPEG (RGB8888)";
        case plBitmap::kRGB4444:
            return "JPEG (RGB4444)";
        case plBitmap::kRGB1555:
            return "JPEG (RGB1555)";
        case plBitmap::kInten8:
            return "JPEG (Greyscale)";
        case plBitmap::kAInten88:
            return "JPEG (Alpha+Greyscale)";
        }
    } else {
        switch (tex->getARGBType()) {
        case plBitmap::kRGB8888:
            return "Uncompressed (RGB8888)";
        case plBitmap::kRGB4444:
            return "Uncompressed (RGB4444)";
        case plBitmap::kRGB1555:
            return "Uncompressed (RGB1555)";
        case plBitmap::kInten8:
            return "Uncompressed (Greyscale)";
        case plBitmap::kAInten88:
            return "Uncompressed (Alpha+Greyscale)";
        }
    }
    return "(Invalid)";
}

QMipmap::QMipmap(plCreatable* pCre, QWidget* parent)
       : QCreatable(pCre, kMipmap, parent)
{
    plMipmap* tex = (plMipmap*)fCreatable;

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    QGridLayout* layFlags = new QGridLayout(grpFlags);
    layFlags->setVerticalSpacing(0);
    layFlags->setHorizontalSpacing(8);
    fFlags[kAlphaChannelFlag] = new QCheckBox(tr("Alpha Channel"), grpFlags);
    fFlags[kAlphaBitFlag] = new QCheckBox(tr("Alpha Bit"), grpFlags);
    fFlags[kBumpEnvMap] = new QCheckBox(tr("Bump Env Map"), grpFlags);
    fFlags[kForce32Bit] = new QCheckBox(tr("Force 32-bit"), grpFlags);
    fFlags[kDontThrowAwayImage] = new QCheckBox(tr("Don't Throw Away"), grpFlags);
    fFlags[kForceOneMipLevel] = new QCheckBox(tr("Force One Level"), grpFlags);
    fFlags[kNoMaxSize] = new QCheckBox(tr("No Maximum Size"), grpFlags);
    fFlags[kIntensityMap] = new QCheckBox(tr("Intensity Map"), grpFlags);
    fFlags[kHalfSize] = new QCheckBox(tr("Half Size"), grpFlags);
    fFlags[kUserOwnsBitmap] = new QCheckBox(tr("User Owned"), grpFlags);
    fFlags[kForceRewrite] = new QCheckBox(tr("Force Rewrite"), grpFlags);
    fFlags[kForceNonCompressed] = new QCheckBox(tr("Force Non-compressed"), grpFlags);
    fFlags[kIsTexture] = new QCheckBox(tr("Is Texture"), grpFlags);
    fFlags[kIsOffscreen] = new QCheckBox(tr("Is Offscreen"), grpFlags);
    fFlags[kIsProjected] = new QCheckBox(tr("Is Projected"), grpFlags);
    fFlags[kIsOrtho] = new QCheckBox(tr("Is Orthogonal"), grpFlags);
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
    fFlags[kAlphaChannelFlag]->setChecked(tex->getFlags() & plBitmap::kAlphaChannelFlag);
    fFlags[kAlphaBitFlag]->setChecked(tex->getFlags() & plBitmap::kAlphaBitFlag);
    fFlags[kBumpEnvMap]->setChecked(tex->getFlags() & plBitmap::kBumpEnvMap);
    fFlags[kForce32Bit]->setChecked(tex->getFlags() & plBitmap::kForce32Bit);
    fFlags[kDontThrowAwayImage]->setChecked(tex->getFlags() & plBitmap::kDontThrowAwayImage);
    fFlags[kForceOneMipLevel]->setChecked(tex->getFlags() & plBitmap::kForceOneMipLevel);
    fFlags[kNoMaxSize]->setChecked(tex->getFlags() & plBitmap::kNoMaxSize);
    fFlags[kIntensityMap]->setChecked(tex->getFlags() & plBitmap::kIntensityMap);
    fFlags[kHalfSize]->setChecked(tex->getFlags() & plBitmap::kHalfSize);
    fFlags[kUserOwnsBitmap]->setChecked(tex->getFlags() & plBitmap::kUserOwnsBitmap);
    fFlags[kForceRewrite]->setChecked(tex->getFlags() & plBitmap::kForceRewrite);
    fFlags[kForceNonCompressed]->setChecked(tex->getFlags() & plBitmap::kForceNonCompressed);
    fFlags[kIsTexture]->setChecked(tex->getFlags() & plBitmap::kIsTexture);
    fFlags[kIsOffscreen]->setChecked(tex->getFlags() & plBitmap::kIsOffscreen);
    fFlags[kIsProjected]->setChecked(tex->getFlags() & plBitmap::kIsProjected);
    fFlags[kIsOrtho]->setChecked(tex->getFlags() & plBitmap::kIsOrtho);

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
    fPreviewLink->setText(tr("Preview"));
    fPreviewLink->setCreatable(tex);
    fPreviewLink->setForceType(kPreviewMipmap);

    QLinkLabel* xDDSLink = new QLinkLabel(tr("Export DDS..."), this);
    QLinkLabel* iDDSLink = new QLinkLabel(tr("Import DDS..."), this);
    QLinkLabel* xBMPLink = new QLinkLabel(tr("Export Bitmap..."), this);
    QLinkLabel* iBMPLink = new QLinkLabel(tr("Import Bitmap..."), this);
    QLinkLabel* xJPGLink = new QLinkLabel(tr("Export Jpeg..."), this);
    QLinkLabel* iJPGLink = new QLinkLabel(tr("Import Jpeg..."), this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(grpFlags, 0, 0, 1, 3);
    layout->addWidget(grpProps, 1, 0, 1, 3);
    layout->addWidget(fPreviewLink, 2, 0, 1, 3);
    layout->addItem(new QSpacerItem(0, 8, QSizePolicy::Minimum, QSizePolicy::Minimum), 3, 0, 1, 3);
    layout->addWidget(xDDSLink, 4, 0);
    layout->addWidget(iDDSLink, 5, 0);
    layout->addWidget(xBMPLink, 4, 1);
    layout->addWidget(iBMPLink, 5, 1);
    layout->addWidget(xJPGLink, 4, 2);
    layout->addWidget(iJPGLink, 5, 2);

    if (tex->getCompressionType() == plBitmap::kJPEGCompression) {
        xDDSLink->setEnabled(false);
        xBMPLink->setEnabled(false);
        xJPGLink->setEnabled(true);
    } else if (tex->getCompressionType() == plBitmap::kDirectXCompression) {
        xDDSLink->setEnabled(true);
        xBMPLink->setEnabled(false);
        xJPGLink->setEnabled(false);
    } else {
        xDDSLink->setEnabled(false);
        xBMPLink->setEnabled(true);
        xJPGLink->setEnabled(false);
    }
}

void QMipmap::saveDamage()
{
    plMipmap* tex = (plMipmap*)fCreatable;

    tex->setFlags((fFlags[kAlphaChannelFlag]->isChecked() ? plBitmap::kAlphaChannelFlag : 0)
                | (fFlags[kAlphaBitFlag]->isChecked() ? plBitmap::kAlphaBitFlag : 0)
                | (fFlags[kBumpEnvMap]->isChecked() ? plBitmap::kBumpEnvMap : 0)
                | (fFlags[kForce32Bit]->isChecked() ? plBitmap::kForce32Bit : 0)
                | (fFlags[kDontThrowAwayImage]->isChecked() ? plBitmap::kDontThrowAwayImage : 0)
                | (fFlags[kForceOneMipLevel]->isChecked() ? plBitmap::kForceOneMipLevel : 0)
                | (fFlags[kNoMaxSize]->isChecked() ? plBitmap::kNoMaxSize : 0)
                | (fFlags[kIntensityMap]->isChecked() ? plBitmap::kIntensityMap : 0)
                | (fFlags[kHalfSize]->isChecked() ? plBitmap::kHalfSize : 0)
                | (fFlags[kUserOwnsBitmap]->isChecked() ? plBitmap::kUserOwnsBitmap : 0)
                | (fFlags[kForceRewrite]->isChecked() ? plBitmap::kForceRewrite : 0)
                | (fFlags[kForceNonCompressed]->isChecked() ? plBitmap::kForceNonCompressed : 0)
                | (fFlags[kIsTexture]->isChecked() ? plBitmap::kIsTexture : 0)
                | (fFlags[kIsOffscreen]->isChecked() ? plBitmap::kIsOffscreen : 0)
                | (fFlags[kIsProjected]->isChecked() ? plBitmap::kIsProjected : 0)
                | (fFlags[kIsOrtho]->isChecked() ? plBitmap::kIsOrtho : 0));
}
