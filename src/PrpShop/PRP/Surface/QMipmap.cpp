#include "QMipmap.h"

#include <QScrollArea>
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QPainter>
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
    } else {
        if (level >= (int)tex->getNumLevels())
            level = tex->getNumLevels() - 1;
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
    }
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
