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

#include "QHexWidget.h"

#include <QPainter>
#include <QStyle>
#include <QScrollBar>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFile>
#include <QMessageBox>

#define HEX_PADDING     (2)
#define BYTES_PER_LINE  (16)

QHexWidget::QHexWidget(QWidget* parent)
    : QAbstractScrollArea(parent), fCurrentAddress(), fViewportAddress(),
      fSelectionStart(-1)
{
    // Should be enough to get a reasonable fixed-width font on all
    // supported platforms...
    QFont defaultFont("Monospace", 10);
    defaultFont.setStyleHint(QFont::TypeWriter);
    setFont(defaultFont);

    setFocusPolicy(Qt::StrongFocus);
}

QSize QHexWidget::sizeHint() const
{
    QFontMetrics fm(font());
    int byteOffset, charOffset, rightMargin;
    getRenderMetrics(fm, 0x10000000, byteOffset, charOffset, rightMargin);
    const int scrollWidth = style()->pixelMetric(QStyle::PM_ScrollBarExtent);

    // Full view width * 25 visible lines
    return QSize(rightMargin + scrollWidth + frameWidth() + HEX_PADDING,
                 (fm.height() * 25) + 5);
}

int QHexWidget::lastVisibleAddress() const
{
    QFontMetrics fm(font());
    return fViewportAddress + (visibleLines(fm, viewport()->height()) * BYTES_PER_LINE);
}

QPair<int, int> QHexWidget::selection() const
{
    int loSelect = fSelectionStart;
    int hiSelect = loSelect >= 0 ? fCurrentAddress : -1;
    if (loSelect > hiSelect)
        qSwap(loSelect, hiSelect);
    return qMakePair(loSelect, hiSelect);
}

void QHexWidget::loadFromData(const QByteArray& data)
{
    fData = data;
    fViewportAddress = 0;
    fCurrentAddress = 0;
    fSelectionStart = -1;
    resizeEvent(Q_NULLPTR);
    viewport()->update();
    emit currentAddressChanged(fCurrentAddress);
}

bool QHexWidget::loadFromFile(const QString& fileName)
{
    QFile fin(fileName);
    if (!fin.open(QIODevice::ReadOnly)) {
        loadFromData(QByteArray());
        return false;
    }

    loadFromData(fin.readAll());
    fin.close();
    return true;
}

void QHexWidget::setCurrentAddress(int address)
{
    if (address == fCurrentAddress)
        return;

    if (address < 0)
        fCurrentAddress = 0;
    else if (address >= fData.size())
        fCurrentAddress = fData.size() - 1;
    else
        fCurrentAddress = address;

    QFontMetrics fm(font());
    const int lines = visibleLines(fm, viewport()->height());
    if (    fCurrentAddress < fViewportAddress
         || fCurrentAddress >= fViewportAddress + (lines * BYTES_PER_LINE)) {
        int line = fCurrentAddress / BYTES_PER_LINE;
        line -= lines / 2;
        verticalScrollBar()->setValue(line);
    }

    viewport()->update();
    emit currentAddressChanged(fCurrentAddress);
}

void QHexWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(viewport());
    render(&painter);
}

void QHexWidget::resizeEvent(QResizeEvent* event)
{
    if (event)
        QAbstractScrollArea::resizeEvent(event);

    QFontMetrics fm(font());
    int lines = fData.size();
    lines = (lines + (BYTES_PER_LINE - 1)) / BYTES_PER_LINE;
    lines -= visibleLines(fm, viewport()->height());

    if (lines < 0) {
        verticalScrollBar()->setMaximum(0);
        return;
    }
    verticalScrollBar()->setMaximum(lines);
}

void QHexWidget::mousePressEvent(QMouseEvent* event)
{
    int clickAddr = addressAt(event->x(), event->y());
    if (clickAddr >= 0) {
        if (event->button() == Qt::LeftButton)
            fSelectionStart = -1;

        if (fCurrentAddress != clickAddr) {
            fCurrentAddress = clickAddr;
            viewport()->update();
            emit currentAddressChanged(fCurrentAddress);
        }
    }
}

void QHexWidget::mouseMoveEvent(QMouseEvent* event)
{
    int clickAddr = addressAt(event->x(), event->y());
    if (clickAddr >= 0) {
        if (event->buttons() & Qt::LeftButton) {
            if (fSelectionStart < 0 && clickAddr != fCurrentAddress)
                fSelectionStart = fCurrentAddress;
        }

        if (fCurrentAddress != clickAddr) {
            fCurrentAddress = clickAddr;
            viewport()->update();
            emit currentAddressChanged(fCurrentAddress);
        }
    }
}

void QHexWidget::mouseReleaseEvent(QMouseEvent*)
{
    // TODO: Show context menu
}

void QHexWidget::keyPressEvent(QKeyEvent* event)
{
    int addr = fCurrentAddress;
    QFontMetrics fm(font());
    const int lines = visibleLines(fm, viewport()->height());

    // Includes some vi keys!
    if (event->modifiers() == Qt::NoModifier) {
        switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_K:
            addr -= BYTES_PER_LINE;
            break;
        case Qt::Key_Down:
        case Qt::Key_J:
            addr += BYTES_PER_LINE;
            break;
        case Qt::Key_Left:
        case Qt::Key_H:
            addr -= 1;
            break;
        case Qt::Key_Right:
        case Qt::Key_L:
            addr += 1;
            break;
        case Qt::Key_PageUp:
            addr -= BYTES_PER_LINE * lines;
            break;
        case Qt::Key_PageDown:
            addr += BYTES_PER_LINE * lines;
            break;
        default:
            event->ignore();
            return;
        }
    }

    if (addr != fCurrentAddress) {
        fSelectionStart = -1;

        // This handles scrolling and bounds checking for us
        setCurrentAddress(addr);
    }
}

void QHexWidget::scrollContentsBy(int, int dy)
{
    fViewportAddress -= (dy * BYTES_PER_LINE);
    viewport()->update();
}

int QHexWidget::visibleLines(const QFontMetrics& fm, int viewportHeight) const
{
    return viewportHeight / fm.height();
}

void QHexWidget::getRenderMetrics(const QFontMetrics& fm, int maxAddress,
                                  int& byteOffset, int& charOffset,
                                  int& rightMargin) const
{
    if (maxAddress >= 0x10000)
        byteOffset = fm.width("0000 0000") + (HEX_PADDING * 2);
    else
        byteOffset = fm.width("0000") + (HEX_PADDING * 2);

    const int bwidth = fm.width("00_");
    const int swidth = fm.width(QChar('_'));
    charOffset = byteOffset + HEX_PADDING + (bwidth * BYTES_PER_LINE)
                            + (swidth * ((BYTES_PER_LINE / 4) + 1));
    rightMargin = charOffset + (swidth * (BYTES_PER_LINE + 1));
}

static QString _formatAddress(int address, int maxAddress)
{
    if (maxAddress > 0x10000) {
        return QString("%1 %2").arg((address >> 16) & 0xFFFF, 4, 16, QChar('0'))
                               .arg((address      ) & 0xFFFF, 4, 16, QChar('0'));
    } else {
        return QString("%1").arg(address & 0xFFFF, 4, 16, QChar('0'));
    }
}

static bool _isNonPrintable(uchar ch)
{
    return (ch < 0x20) || (ch >= 0x7f && ch <= 0x9f);
}

static QString _printableChar(uchar ch)
{
    if (_isNonPrintable(ch))
        return QString(QChar(0x00B7));
    return QString(QLatin1Char(ch));
}

int _fetchLine(const QByteArray& data, int address, uchar (&out)[BYTES_PER_LINE])
{
    Q_ASSERT(address >= 0);

    int end = address + BYTES_PER_LINE;
    if (end > data.size())
        end = data.size();

    const char* cdata = data.constData() + address;
    const char* cend = data.constData() + end;
    char *cout = reinterpret_cast<char *>(&out[0]);
    while (cdata < cend)
        *cout++ = *cdata++;

    return end - address;
}

void QHexWidget::render(QPainter* painter)
{
    QPalette pal(palette());
    QFontMetrics fm(font());
    painter->setFont(font());

    int maxAddress = fData.size();
    int byteOffset, charOffset, rightMargin;
    getRenderMetrics(fm, maxAddress, byteOffset, charOffset, rightMargin);
    const int bwidth = fm.width("00_");
    const int swidth = fm.width(QChar('_'));

    painter->fillRect(0, 0, byteOffset, viewport()->height(),
                      pal.color(QPalette::Window));
    painter->fillRect(byteOffset, 0, rightMargin, viewport()->height(),
                      pal.color(QPalette::Base));
    painter->fillRect(rightMargin, 0, viewport()->width() - rightMargin,
                      viewport()->height(), pal.color(QPalette::Window));

    // Mark off the separation between the view areas
    painter->setPen(pal.color(QPalette::Dark));
    painter->drawLine(byteOffset, 0, byteOffset, viewport()->height());
    painter->drawLine(charOffset - swidth, 0,
                      charOffset - swidth, viewport()->height());
    painter->drawLine(rightMargin, 0, rightMargin, viewport()->height());

    // Left header
    const int lines = visibleLines(fm, viewport()->height());
    painter->setPen(pal.color(QPalette::Text));
    {
        int hy = HEX_PADDING + fm.ascent();
        int baseAddr = fViewportAddress;
        for (int i = 0; i < lines; ++i) {
            painter->drawText(HEX_PADDING, hy, _formatAddress(baseAddr, maxAddress));
            baseAddr += BYTES_PER_LINE;
            if (baseAddr >= maxAddress)
                break;
            hy += fm.height();
        }
    }

    // Selection
    int loSelect = fSelectionStart;
    int hiSelect = loSelect >= 0 ? fCurrentAddress : -1;
    if (loSelect > hiSelect)
        qSwap(loSelect, hiSelect);

    // Hex values and chars
    int ly = HEX_PADDING + fm.ascent();
    int bytes, bx, cx;
    int baseAddr = fViewportAddress;
    uchar buffer[BYTES_PER_LINE];
    for (int i = 0; i < lines; ++i) {
        bytes = _fetchLine(fData, baseAddr, buffer);
        if (!bytes)
            break;

        bx = byteOffset + HEX_PADDING + swidth;
        cx = charOffset;
        for (int b = 0; b < bytes; ++b) {
            bool inSelect = loSelect <= baseAddr + b && baseAddr + b <= hiSelect;
            if (inSelect) {
                int hlBytes = 2;
                if (b < 15 && baseAddr + b + 1 <= hiSelect) {
                    hlBytes += 1;
                    if ((b % 4) == 3)
                        hlBytes += 1;
                }

                painter->fillRect(bx, ly - fm.ascent(),
                                  swidth * hlBytes, fm.height(),
                                  pal.color(QPalette::Highlight));
                painter->fillRect(cx, ly - fm.ascent(), swidth, fm.height(),
                                  pal.color(QPalette::Highlight));
            }

            uchar bvalue = buffer[b];
            if (inSelect) {
                painter->setPen(pal.color(QPalette::HighlightedText));
            } else if (_isNonPrintable(bvalue)) {
                painter->setPen(QColor(115, 10, 125));
            } else {
                painter->setPen(pal.color(QPalette::Text));
            }

            painter->drawText(bx, ly, QString("%1").arg(bvalue, 2, 16, QChar('0')));
            painter->drawText(cx, ly, _printableChar(bvalue));

            bx += bwidth;
            cx += swidth;
            if ((b % 4) == 3)
                bx += swidth;
        }

        baseAddr += BYTES_PER_LINE;
        ly += fm.height();
    }

    // Paint selection box last so it doesn't get overpainted by selection
    // highlight
    int endAddr = baseAddr;
    if (fCurrentAddress >= fViewportAddress && fCurrentAddress < endAddr) {
        ly = HEX_PADDING + fm.ascent();
        int relAddress = fCurrentAddress - fViewportAddress;
        ly += (relAddress / BYTES_PER_LINE) * fm.height();
        int bOffset = (relAddress % BYTES_PER_LINE);
        bx = byteOffset + HEX_PADDING + swidth;
        bx += bwidth * bOffset + ((bOffset / 4) * swidth);
        cx = charOffset + (swidth * bOffset);
        painter->setPen(QPalette::Shadow);
        painter->drawRect(bx - 1, ly - fm.ascent() - 1,
                          swidth * 2 + 1, fm.height() + 1);
        painter->drawRect(cx - 1, ly - fm.ascent() - 1,
                          swidth + 1, fm.height() + 1);
    }

}

int QHexWidget::addressAt(int x, int y) const
{
    QFontMetrics fm(font());
    const int bwidth = fm.width("00_");
    const int cwidth = fm.width(QChar('0'));

    int byteOffset, charOffset, rightMargin;
    getRenderMetrics(fm, fData.size(), byteOffset, charOffset, rightMargin);
    const int firstByteOffsetChar = byteOffset + HEX_PADDING;

    int lineAddr = (y - fm.descent() + 1) / fm.height();
    lineAddr = lineAddr * BYTES_PER_LINE + fViewportAddress;
    int addr = -1;

    if (x < byteOffset) {
        // Address header clicked -- just get the address of the first byte
        // in the line.
        addr = lineAddr;
    } else if (x >= firstByteOffsetChar + (cwidth / 2) && x < charOffset) {
        // Byte selection -- round to nearest group
        int fbox = x - firstByteOffsetChar;

        // Remove extra space between words
        if (fbox > 4 * bwidth)
            fbox -= cwidth;
        if (fbox > 8 * bwidth)
            fbox -= cwidth;
        if (fbox > 12 * bwidth)
            fbox -= cwidth;

        int selChar = (fbox - (cwidth / 2)) / bwidth;
        if (selChar < BYTES_PER_LINE)
            addr = lineAddr + selChar;
    } else if (x >= charOffset && x < rightMargin - cwidth) {
        // Char selection
        addr = lineAddr + (x - charOffset) / cwidth;
    }

    return (addr < fData.size()) ? addr : -1;
}
