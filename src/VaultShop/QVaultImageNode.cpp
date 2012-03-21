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

#include "QVaultImageNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <Vault/plVaultNodeTypes.h>

/* QStaticImage */
QStaticImage::QStaticImage(QWidget* parent)
            : QWidget(parent), fImg(NULL)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QStaticImage::~QStaticImage()
{
    delete fImg;
}

void QStaticImage::set(QPixmap* img)
{
    delete fImg;
    fImg = img;
    //fScale.setWidth((400.0 / fImg->width()) * fImg->width());
    //fScale.setHeight((400.0 / fImg->width()) * fImg->height());
    //setMinimumSize(fScale.width(), fScale.height());
}

void QStaticImage::refresh()
{
    resizeEvent(0);
    repaint();
}

void QStaticImage::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    if (fImg != NULL)
        p.drawPixmap(0, 0, fScale.width(), fScale.height(), *fImg);
}

void QStaticImage::resizeEvent(QResizeEvent*)
{
    double szx = 1.0, szy = 1.0;
    if (fImg->width() > width())
        szx = (double)width() / (double)fImg->width();
    if (fImg->height() > height())
        szy = (double)height() / (double)fImg->height();
    fScale.setHeight(szx > szy ? szy * fImg->height() : szx * fImg->height());
    fScale.setWidth(szx > szy ? szy * fImg->width() : szx * fImg->width());
}


/* QVaultImageNode */
QVaultImageNode::QVaultImageNode(QWidget* parent)
               : QVaultNodeEdit(parent)
{
    static QString s_ImgTypes[] = {
        "(Invalid)", "JPEG",
    };

    fImgTitle = new QLineEdit(this);

    fImgType = new QComboBox(this);
    for (int i=0; i<=1; i++)
        fImgType->addItem(s_ImgTypes[i]);

    QWidget* imgPanel = new QWidget(this);
    fImage = new QStaticImage(imgPanel);
    QPushButton* btnSave = new QPushButton(tr("&Save..."), imgPanel);
    QPushButton* btnLoad = new QPushButton(tr("&Load..."), imgPanel);
    QGridLayout* imgLayout = new QGridLayout(imgPanel);
    imgLayout->setContentsMargins(0, 0, 0, 0);
    imgLayout->setHorizontalSpacing(8);
    imgLayout->setVerticalSpacing(8);
    imgLayout->addWidget(fImage, 0, 0, 1, 3);
    imgLayout->addWidget(btnSave, 1, 0);
    imgLayout->addWidget(btnLoad, 1, 1);
    imgLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 1, 2);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Title:"), this), 0, 0);
    layout->addWidget(fImgTitle, 0, 1);
    layout->addWidget(new QLabel(tr("Type:"), this), 1, 0);
    layout->addWidget(fImgType, 1, 1);
    layout->addWidget(imgPanel, 2, 0, 1, 2);

    connect(btnSave, SIGNAL(clicked()), this, SLOT(IExportImage()));
    connect(btnLoad, SIGNAL(clicked()), this, SLOT(IImportImage()));
}

plVaultNode QVaultImageNode::saveNode()
{
    plVaultImageNode* img = fNode.upcastToImageNode();
    if (img == NULL)
        return plVaultNode();

    img->setImageTitle(~fImgTitle->text());
    img->setImageType(fImgType->currentIndex());

    //plVaultBlob blob;
    //blob.setData((size_t)contents.size(), (const unsigned char*)contents.data());
    //img->setNoteContents(blob);

    return fNode;
}

void QVaultImageNode::IRefreshNode()
{
    plVaultImageNode* img = fNode.upcastToImageNode();
    if (img == NULL)
        return;

    fImgTitle->setText(~img->getImageTitle());
    fImgType->setCurrentIndex(img->getImageType());

    plVaultBlob blob = img->getImageData();
    if (blob.getSize() > 0) {
        QPixmap* pix = new QPixmap();
        pix->loadFromData((const uchar*)blob.getData() + 4, blob.getSize() - 4, "JPEG");
        fImage->set(pix);
    }
}

static QString ieDir;

void QVaultImageNode::IImportImage()
{
    plVaultImageNode* img = fNode.upcastToImageNode();
    if (img == NULL)
        return;

    QString fname = QFileDialog::getOpenFileName(this, tr("Import JPEG"), ieDir,
                                                 "JPEG Files (*.jpg)", 0);

    if (!fname.isEmpty()) {
        QDir ieDirTmp = QDir(fname);
        ieDirTmp.cdUp();
        ieDir = ieDirTmp.absolutePath();

        FILE* xf = fopen(fname.toUtf8().data(), "rb");
        if (xf == NULL) {
            QMessageBox::critical(this, tr("Error importing JPEG"),
                                  tr("Could not open %1 for reading").arg(fname),
                                  QMessageBox::Ok);
            return;
        }
        fseek(xf, 0, SEEK_END);
        uint size = ftell(xf);
        uchar* data = new uchar[size + 4];
        *(uint*)data = size;
        fseek(xf, 0, SEEK_SET);
        fread((uchar*)data + 4, 1, size, xf);
        fclose(xf);
        plVaultBlob blob;
        blob.setData(size + 4, data);
        img->setImageData(blob);
        delete[] data;

        IRefreshNode();
        fImage->refresh();
    }
}

void QVaultImageNode::IExportImage()
{
    plVaultImageNode* img = fNode.upcastToImageNode();
    if (img == NULL)
        return;
    plVaultBlob blob = img->getImageData();

    QString fname = fImgTitle->text() + ".jpg";
    fname.replace(QRegExp("[\\\\/:*?\"<>|]"), "_");
    if (!ieDir.isEmpty())
        fname = ieDir + "/" + fname;
    fname = QFileDialog::getSaveFileName(this, tr("Export JPEG"), fname,
                                         "JPEG Files (*.jpg)", 0);

    if (!fname.isEmpty()) {
        QDir ieDirTmp = QDir(fname);
        ieDirTmp.cdUp();
        ieDir = ieDirTmp.absolutePath();

        FILE* xf = fopen(fname.toUtf8().data(), "wb");
        if (xf == NULL) {
            QMessageBox::critical(this, tr("Error saving JPEG"),
                                  tr("Could not open %1 for writing").arg(fname),
                                  QMessageBox::Ok);
            return;
        }
        fwrite((const uchar*)blob.getData() + 4, 1, blob.getSize() - 4, xf);
        fclose(xf);
    }
}
