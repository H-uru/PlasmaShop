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

#include "QHexViewer.h"
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStatusBar>
#include <QFile>
#include <Stream/hsRAMStream.h>
#include "../QHexWidget.h"

class SelectableLabel : public QLabel
{
public:
    SelectableLabel(QWidget* parent) : QLabel(parent)
    {
        setTextInteractionFlags(Qt::TextSelectableByMouse);
        //setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        //setLineWidth(0);
    }
};

QHexViewer::QHexViewer(plCreatable* pCre, QWidget* parent)
    : QCreatable(pCre, kHex_Type | pCre->ClassIndex(), parent)
{
    fViewer = new QHexWidget(this);
    connect(fViewer, SIGNAL(currentAddressChanged(int)),
            this, SLOT(cursorChanged(int)));

    QLabel* i32Label = new QLabel(tr("32-bit int: "), this);
    fI32Val = new SelectableLabel(this);
    QLabel* i16Label = new QLabel(tr("16-bit int: "), this);
    fI16Val = new SelectableLabel(this);
    QLabel* i8Label = new QLabel(tr("byte: "), this);
    fI8Val = new SelectableLabel(this);
    fSigned = new QCheckBox(tr("Signed Ints"), this);
    fSigned->setChecked(true);
    QLabel* f32Label = new QLabel(tr("float: "), this);
    fFloatVal = new SelectableLabel(this);
    QLabel* f64Label = new QLabel(tr("double: "), this);
    fDoubleVal = new SelectableLabel(this);
    connect(fSigned, SIGNAL(toggled(bool)), this, SLOT(signedChanged(bool)));
    QLabel* stringLabel = new QLabel(tr("Encoded string:"), this);
    fStringVal = new SelectableLabel(this);

    QFrame *decoder = new QFrame(this);
    decoder->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    QGridLayout* decoderLayout = new QGridLayout(decoder);
    decoderLayout->setContentsMargins(4, 4, 4, 4);
    decoderLayout->setSpacing(4);
    decoderLayout->addWidget(i32Label, 0, 0, Qt::AlignRight);
    decoderLayout->addWidget(fI32Val, 0, 1);
    decoderLayout->addWidget(i16Label, 0, 2, Qt::AlignRight);
    decoderLayout->addWidget(fI16Val, 0, 3);
    decoderLayout->addWidget(i8Label, 0, 4, Qt::AlignRight);
    decoderLayout->addWidget(fI8Val, 0, 5);
    decoderLayout->addWidget(fSigned, 1, 0, 1, 2, Qt::AlignHCenter);
    decoderLayout->addWidget(f32Label, 1, 2, Qt::AlignRight);
    decoderLayout->addWidget(fFloatVal, 1, 3);
    decoderLayout->addWidget(f64Label, 1, 4, Qt::AlignRight);
    decoderLayout->addWidget(fDoubleVal, 1, 5);
    decoderLayout->addWidget(stringLabel, 2, 0, Qt::AlignRight);
    decoderLayout->addWidget(fStringVal, 2, 1, 1, 5);

    fStatusBar = new QStatusBar(this);
    fStatusBar->setSizeGripEnabled(true);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);
    layout->addWidget(fViewer);
    layout->addWidget(decoder);
    layout->addWidget(fStatusBar);
    setLayout(layout);
}

void QHexViewer::loadObject(const QString& filename, uint32_t offset, uint32_t size)
{
    QFile fin(filename);
    if (fin.open(QIODevice::ReadOnly)) {
        fin.seek(offset);
        QByteArray data = fin.read(size);
        fViewer->loadFromData(data);
    }
    fin.close();
}

void QHexViewer::cursorChanged(int address)
{
    if (address < fViewer->dataSize()) {
        uchar byte = fViewer->byteAt(address);
        if (fSigned->isChecked())
            fI8Val->setText(QString::number((int)(signed char)byte));
        else
            fI8Val->setText(QString::number((unsigned int)byte));
    } else {
        fI8Val->setText(tr("N/A"));
    }
    if (address + 1 < fViewer->dataSize()) {
        uint16_t u16val = fViewer->byteAt(address);
        u16val |= ((uint16_t)fViewer->byteAt(address + 1)) << 8;
        if (fSigned->isChecked())
            fI16Val->setText(QString::number((int)(int16_t)u16val));
        else
            fI16Val->setText(QString::number((unsigned int)u16val));
    } else {
        fI16Val->setText(tr("N/A"));
    }
    if (address + 3 < fViewer->dataSize()) {
        uint32_t u32val = fViewer->byteAt(address);
        u32val |= ((uint32_t)fViewer->byteAt(address + 1)) << 8;
        u32val |= ((uint32_t)fViewer->byteAt(address + 2)) << 16;
        u32val |= ((uint32_t)fViewer->byteAt(address + 3)) << 24;
        if (fSigned->isChecked())
            fI32Val->setText(QString::number((int)u32val));
        else
            fI32Val->setText(QString::number(u32val));

        float fval;
        memcpy(&fval, &u32val, sizeof(fval));
        fFloatVal->setText(QString::number(fval));
    } else {
        fI32Val->setText(tr("N/A"));
        fFloatVal->setText(tr("N/A"));
    }
    if (address + 7 < fViewer->dataSize()) {
        uint64_t u64val = fViewer->byteAt(address);
        u64val |= ((uint64_t)fViewer->byteAt(address + 1)) << 8;
        u64val |= ((uint64_t)fViewer->byteAt(address + 2)) << 16;
        u64val |= ((uint64_t)fViewer->byteAt(address + 3)) << 24;
        u64val |= ((uint64_t)fViewer->byteAt(address + 4)) << 32;
        u64val |= ((uint64_t)fViewer->byteAt(address + 5)) << 40;
        u64val |= ((uint64_t)fViewer->byteAt(address + 6)) << 48;
        u64val |= ((uint64_t)fViewer->byteAt(address + 7)) << 56;

        double fval;
        memcpy(&fval, &u64val, sizeof(fval));
        fDoubleVal->setText(QString::number(fval));
    } else {
        fDoubleVal->setText(tr("N/A"));
    }

    QPair<int, int> selection = fViewer->selection();
    if (selection.first >= 0 && selection.second >= 0) {
        QByteArray sdata;
        sdata.reserve(selection.second - selection.first + 1);
        for (int i = selection.first; i <= selection.second; ++i)
            sdata.append(~fViewer->byteAt(i));

        fStringVal->setText(QString::fromLatin1(sdata));
    } else {
        fStringVal->setText(tr("N/A"));
    }

    QString addrText;
    if (selection.first >= 0 && selection.second >= 0) {
        fStatusBar->showMessage(tr("Selection: %1 - %2 (%3 bytes)")
                        .arg((unsigned int)selection.first, 8, 16, QChar('0'))
                        .arg((unsigned int)selection.second, 8, 16, QChar('0'))
                        .arg(selection.second - selection.first + 1));
    } else {
        fStatusBar->showMessage(tr("Cursor Address: %1")
                        .arg((unsigned int)address, 8, 16, QChar('0')));
    }
}

void QHexViewer::signedChanged(bool)
{
    cursorChanged(fViewer->currentAddress());
}
