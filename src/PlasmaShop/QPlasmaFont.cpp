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
#include <QGroupBox>
#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <algorithm>

/* QPlasmaCharWidget */
QPlasmaCharWidget::QPlasmaCharWidget(QWidget* parent)
    : QAbstractScrollArea(parent), fFont(), fSelected(), fCharsPerLine(1)
{
    setFocusPolicy(Qt::StrongFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void QPlasmaCharWidget::reloadFont()
{
    // Recompute layout
    const QSize viewportSize = viewport()->size();
    fCharsPerLine = std::max<int>(viewportSize.width() / (fFont->getWidth() + 1), 1);
    const int lines = (fFont->getNumCharacters() + (fCharsPerLine - 1)) / fCharsPerLine;
    const int totalHeight = lines * (fFont->getMaxCharHeight() + 1);
    verticalScrollBar()->setRange(0, totalHeight - viewportSize.height());
    verticalScrollBar()->setPageStep(fFont->getMaxCharHeight() * 2);
    verticalScrollBar()->setSingleStep(fFont->getMaxCharHeight() / 2);
}

static QImage::Format fontImageFormat(plFont* font)
{
    switch (font->getBPP()) {
    case 1:
        return QImage::Format_Mono;
    case 8:
        return QImage::Format_Alpha8;
    default:
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Unsupported bitmap format"));
        return QImage::Format_Invalid;
    }
}

void QPlasmaCharWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(viewport());
    QPalette pal = palette();
    painter.fillRect(event->rect(), pal.brush(QPalette::Base));
    if (!fFont)
        return;

    painter.setPen(pal.color(QPalette::Midlight));

    const size_t stride = fFont->getStride();
    for (size_t c = 0; c < fFont->getNumCharacters(); ++c) {
        QRect rGlyph((c % fCharsPerLine) * (fFont->getWidth() + 1),
                     (c / fCharsPerLine) * (fFont->getMaxCharHeight() + 1),
                     fFont->getWidth(), fFont->getMaxCharHeight());
        rGlyph.translate(0, -verticalScrollBar()->value());

        if (static_cast<int>(c) == fSelected && hasFocus())
            painter.fillRect(rGlyph, pal.color(QPalette::Highlight));

        const plFont::plCharacter& ch = fFont->getCharacter(c);
        QImage cimg(fFont->getGlyph(c), fFont->getWidth(), ch.getHeight(),
                    stride, fontImageFormat(fFont));
        painter.drawImage(rGlyph.topLeft(), cimg);
        painter.drawLine(rGlyph.right() + 1, rGlyph.top(),
                         rGlyph.right() + 1, rGlyph.bottom() + 1);
        painter.drawLine(rGlyph.left(), rGlyph.bottom() + 1,
                         rGlyph.right() + 1, rGlyph.bottom() + 1);
    }
}

void QPlasmaCharWidget::resizeEvent(QResizeEvent* event)
{
    (void)event;
    reloadFont();
}

void QPlasmaCharWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        const int charX = event->x() / (fFont->getWidth() + 1);
        const int charY = (event->y() + verticalScrollBar()->value())
                          / (fFont->getMaxCharHeight() + 1);
        if (charX >= fCharsPerLine
                || size_t(charY * fCharsPerLine + charX) >= fFont->getNumCharacters())
            return;

        fSelected = charY * fCharsPerLine + charX;
        viewport()->repaint();

        emit characterSelected(fSelected);
    }
}

void QPlasmaCharWidget::keyPressEvent(QKeyEvent* event)
{
    int newSelection = fSelected;
    if (event->matches(QKeySequence::MoveToNextLine))
        newSelection += fCharsPerLine;
    else if (event->matches(QKeySequence::MoveToPreviousLine))
        newSelection -= fCharsPerLine;
    else if (event->matches(QKeySequence::MoveToNextChar))
        newSelection += 1;
    else if (event->matches(QKeySequence::MoveToPreviousChar))
        newSelection -= 1;
    else
        QAbstractScrollArea::keyPressEvent(event);

    if (newSelection != fSelected && newSelection >= 0
            && size_t(newSelection) < fFont->getNumCharacters()) {
        fSelected = newSelection;
        // TODO: Scroll so the selected character is fully in view
        viewport()->repaint();

        emit characterSelected(fSelected);
    }
}


/* QPlasmaCharRender */
QPlasmaCharRender::QPlasmaCharRender(QWidget* parent)
    : QWidget(parent), fFont(), fCharIdx()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void QPlasmaCharRender::reloadFont()
{
    setFixedSize(fFont->getWidth() * 3, fFont->getMaxCharHeight() * 3);
}

void QPlasmaCharRender::setCharacter(int which)
{
    fCharIdx = which;
    update();
}

void QPlasmaCharRender::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), QColor(255, 255, 224));
    if (!fFont)
        return;

    const auto& ch = fFont->getCharacter(fCharIdx);
    const size_t stride = fFont->getStride();

    painter.setPen(QColor(255, 0, 0));
    const int basePos = fFont->getMaxCharHeight() * 2;
    painter.drawLine(0, basePos, width(), basePos);

    if (ch.getHeight() == 0)
        return;

    const qreal adjustedWidth = -ch.getLeftKern() + fFont->getWidth() + ch.getRightKern();
    const qreal startX = (width() - (adjustedWidth * 2)) / 2;

    painter.setPen(QColor(128, 128, 255));
    painter.drawLine(QPointF(startX - ch.getLeftKern() * 2, 0),
                     QPointF(startX - ch.getLeftKern() * 2, height()));
    painter.drawLine(QPointF(startX + (fFont->getWidth() + ch.getRightKern()) * 2, 0),
                     QPointF(startX + (fFont->getWidth() + ch.getRightKern()) * 2, height()));

    QImage cimg(fFont->getGlyph(fCharIdx), fFont->getWidth(), ch.getHeight(),
                stride, fontImageFormat(fFont));
    QRectF destRect(0, 0, fFont->getWidth() * 2, ch.getHeight() * 2);
    destRect.translate(startX, basePos - ch.getBaseline() * 2);
    painter.drawImage(destRect, cimg);
}


/* QPlasmaFont */
QPlasmaFont::QPlasmaFont(QWidget* parent)
    : QPlasmaDocument(kDocFont, parent), fCharacter(), fBlockSpinBoxes()
{
    auto lblName = new QLabel(tr("&Face:"), this);
    fFontName = new QLineEdit(this);
    lblName->setBuddy(fFontName);

    auto lblSize = new QLabel(tr("&Size:"), this);
    fFontSize = new QSpinBox(this);
    fFontSize->setRange(0, 255);
    lblSize->setBuddy(fFontSize);

    fBold = new QCheckBox(tr("&Bold"), this);
    fItalic = new QCheckBox(tr("&Italic"), this);

    fChars = new QPlasmaCharWidget(this);
    fChars->setRenderFont(&fFont);

    fCharDetail = new QGroupBox(tr("Character Details"), this);
    fRender = new QPlasmaCharRender(this);
    fRender->setRenderFont(&fFont);

    auto lblLeftKern = new QLabel(tr("&Left Kern:"), this);
    fLeftKern = new QDoubleSpinBox(this);
    fLeftKern->setDecimals(2);
    fLeftKern->setRange(-1024.0, 1024.0);
    lblLeftKern->setBuddy(fLeftKern);

    auto lblRightKern = new QLabel(tr("&Right Kern:"), this);
    fRightKern = new QDoubleSpinBox(this);
    fRightKern->setDecimals(2);
    fRightKern->setRange(-1024.0, 1024.0);
    lblRightKern->setBuddy(fRightKern);

    auto lblBaseLine = new QLabel(tr("Base &Height:"), this);
    fBaseLine = new QSpinBox(this);
    fBaseLine->setRange(-1024, 1024);
    lblBaseLine->setBuddy(fBaseLine);

    auto charGroupLayout = new QGridLayout(fCharDetail);
    charGroupLayout->setContentsMargins(4, 4, 4, 4);
    charGroupLayout->addWidget(fRender, 0, 0, 3, 1);
    charGroupLayout->addWidget(lblLeftKern, 0, 1);
    charGroupLayout->addWidget(fLeftKern, 0, 2);
    charGroupLayout->addWidget(lblRightKern, 1, 1);
    charGroupLayout->addWidget(fRightKern, 1, 2);
    charGroupLayout->addWidget(lblBaseLine, 2, 1);
    charGroupLayout->addWidget(fBaseLine, 2, 2);
    charGroupLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 0, 3);

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
    layout->addWidget(fCharDetail, 3, 0, 1, 6);

    connect(fFontName, &QLineEdit::textEdited, this, &QPlasmaDocument::makeDirty);
    connect(fFontSize, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &QPlasmaDocument::makeDirty);
    connect(fBold, &QCheckBox::clicked, this, &QPlasmaDocument::makeDirty);
    connect(fItalic, &QCheckBox::clicked, this, &QPlasmaDocument::makeDirty);

    connect(fChars, &QPlasmaCharWidget::characterSelected,
            this, &QPlasmaFont::showCharacter);

    connect(fLeftKern, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &QPlasmaFont::updateCharacter);
    connect(fRightKern, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &QPlasmaFont::updateCharacter);
    connect(fBaseLine, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &QPlasmaFont::updateCharacter);
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
    fRender->reloadFont();

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

void QPlasmaFont::showCharacter(int which)
{
    fCharacter = which;
    fCharDetail->setTitle(tr("Character Details: \'%1\' (U+%2)")
            .arg(QChar(which)).arg(which, 4, 16, QLatin1Char('0')));
    fRender->setCharacter(which);

    const plFont::plCharacter& ch = fFont.getCharacter(which);
    fBlockSpinBoxes = true;
    fLeftKern->setValue(ch.getLeftKern());
    fRightKern->setValue(ch.getRightKern());
    fBaseLine->setValue(ch.getBaseline());
    fBlockSpinBoxes = false;
}

void QPlasmaFont::updateCharacter()
{
    if (!fBlockSpinBoxes) {
        plFont::plCharacter &ch = fFont.getCharacter(fCharacter);
        ch.setLeftKern(fLeftKern->value());
        ch.setRightKern(fRightKern->value());
        ch.setBaseline(fBaseLine->value());
        fRender->setCharacter(fCharacter);
        makeDirty();
    }
}
