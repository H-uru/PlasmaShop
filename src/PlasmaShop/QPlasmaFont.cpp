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

#include "QPlasmaFont.h"
#include "QPlasma.h"

#include <Stream/hsRAMStream.h>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollBar>
#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>

/* QPlasmaCharWidget */
QPlasmaCharWidget::QPlasmaCharWidget(QWidget* parent)
    : QAbstractScrollArea(parent), fFont()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void QPlasmaCharWidget::reloadFont()
{
    // Recompute layout
    const QSize viewportSize = viewport()->size();
    const size_t charsPerLine = viewportSize.width() / (fFont->getWidth() + 1);
    const size_t lines = (fFont->getNumCharacters() + (charsPerLine - 1)) / charsPerLine;
    const size_t totalHeight = lines * (fFont->getMaxCharHeight() + 1);
    verticalScrollBar()->setRange(0, totalHeight - viewportSize.height());
    verticalScrollBar()->setPageStep(fFont->getMaxCharHeight() * 2);
    verticalScrollBar()->setSingleStep(fFont->getMaxCharHeight() / 2);
}

void QPlasmaCharWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(viewport());
    QPalette pal = palette();
    painter.fillRect(event->rect(), pal.brush(QPalette::Base));
    if (!fFont)
        return;

    QImage::Format imgFormat;
    switch (fFont->getBPP()) {
    case 1:
        imgFormat = QImage::Format_Mono;
        break;
    case 8:
        imgFormat = QImage::Format_Alpha8;
        break;
    default:
        QMessageBox::critical(this, tr("Error"), tr("Unsupported bitmap format"));
        return;
    }

    painter.setPen(pal.color(QPalette::Midlight));

    const QSize viewportSize = viewport()->size();
    const size_t charsPerLine = viewportSize.width() / (fFont->getWidth() + 1);
    const size_t stride = fFont->getStride();
    for (size_t c = 0; c < fFont->getNumCharacters(); ++c) {
        QRect rGlyph((c % charsPerLine) * (fFont->getWidth() + 1),
                     (c / charsPerLine) * (fFont->getMaxCharHeight() + 1),
                     fFont->getWidth(), fFont->getMaxCharHeight());
        rGlyph.translate(0, -verticalScrollBar()->value());

        const plFont::plCharacter& ch = fFont->getCharacter(c);
        QImage cimg(fFont->getGlyph(c), fFont->getWidth(), ch.getHeight(),
                    stride, imgFormat);
        painter.drawImage(rGlyph.topLeft(), cimg);
        painter.drawLine(rGlyph.right() + 1, rGlyph.top(),
                         rGlyph.right() + 1, rGlyph.bottom() + 1);
        painter.drawLine(rGlyph.left(), rGlyph.bottom() + 1,
                         rGlyph.right() + 1, rGlyph.bottom() + 1);
    }
}

void QPlasmaCharWidget::resizeEvent(QResizeEvent *event)
{
    (void)event;
    reloadFont();
}


/* QPlasmaFont */
QPlasmaFont::QPlasmaFont(QWidget* parent)
    : QPlasmaDocument(kDocFont, parent)
{
    auto lblName = new QLabel(tr("&Face:"), this);
    fFontName = new QLineEdit(this);
    lblName->setBuddy(fFontName);

    auto lblSize = new QLabel(tr("&Size:"), this);
    fFontSize = new QSpinBox(this);
    fFontSize->setMinimum(0);
    fFontSize->setMaximum(255);
    lblSize->setBuddy(fFontSize);

    fBold = new QCheckBox(tr("&Bold"), this);
    fItalic = new QCheckBox(tr("&Italic"), this);

    fChars = new QPlasmaCharWidget(this);
    fChars->setRenderFont(&fFont);

    auto layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->addWidget(lblName, 0, 0);
    layout->addWidget(fFontName, 0, 1, 1, 5);
    layout->addWidget(lblSize, 1, 0);
    layout->addWidget(fFontSize, 1, 1);
    layout->addItem(new QSpacerItem(10, 0), 1, 2);
    layout->addWidget(fBold, 1, 3);
    layout->addWidget(fItalic, 1, 4);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 1, 5);
    layout->addWidget(fChars, 2, 0, 1, 6);
    setLayout(layout);

    connect(fFontName, &QLineEdit::textEdited, this, &QPlasmaDocument::makeDirty);
    connect(fFontSize, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this] { makeDirty(); });
    connect(fBold, &QCheckBox::clicked, this, &QPlasmaDocument::makeDirty);
    connect(fItalic, &QCheckBox::clicked, this, &QPlasmaDocument::makeDirty);
}

bool QPlasmaFont::loadFile(const QString& filename)
{
    ST::string st_filename = qstr2st(filename);
    hsFileStream S;
    if (!S.open(st_filename, fmRead)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open %1 for reading").arg(filename));
        return false;
    }

    fFont.readP2F(&S);
    fFontName->setText(st2qstr(fFont.getName()));
    fFontSize->setValue(fFont.getSize());
    fBold->setChecked(fFont.isBold());
    fItalic->setChecked(fFont.isItalic());
    fChars->reloadFont();

    return QPlasmaDocument::loadFile(filename);
}

bool QPlasmaFont::saveTo(const QString& filename)
{
    ST::string st_filename = qstr2st(filename);
    hsFileStream S;
    if (!S.open(st_filename, fmCreate)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open %1 for writing").arg(filename));
        return false;
    }

    fFont.setName(qstr2st(fFontName->text()));
    fFont.setSize(fFontSize->value());
    fFont.setBold(fBold->isChecked());
    fFont.setItalic(fItalic->isChecked());
    fFont.writeP2F(&S);

    return QPlasmaDocument::saveTo(filename);
}
