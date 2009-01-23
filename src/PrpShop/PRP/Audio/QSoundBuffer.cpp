#include "QSoundBuffer.h"

#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include "../../../QPlasma.h"

QSoundBuffer::QSoundBuffer(plCreatable* pCre, QWidget* parent)
            : QCreatable(pCre, kSoundBuffer, parent)
{
    plSoundBuffer* obj = (plSoundBuffer*)fCreatable;

    QGroupBox* flagGroup = new QGroupBox(tr("Flags"), this);
    fFlags[kIsExternal] = new QCheckBox(tr("External"), flagGroup);
    fFlags[kAlwaysExternal] = new QCheckBox(tr("Always External"), flagGroup);
    fFlags[kStreamCompressed] = new QCheckBox(tr("Stream Compressed"), flagGroup);
    fFlags[kOnlyLeftChannel] = new QCheckBox(tr("Left Channel Only"), flagGroup);
    fFlags[kOnlyRightChannel] = new QCheckBox(tr("Right Channel Only"), flagGroup);
    QGridLayout* flagLayout = new QGridLayout(flagGroup);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fFlags[kIsExternal], 0, 0);
    flagLayout->addWidget(fFlags[kAlwaysExternal], 1, 0);
    flagLayout->addWidget(fFlags[kStreamCompressed], 2, 0);
    flagLayout->addWidget(fFlags[kOnlyLeftChannel], 0, 1);
    flagLayout->addWidget(fFlags[kOnlyRightChannel], 1, 1);

    fFlags[kIsExternal]->setChecked((obj->getFlags() & plSoundBuffer::kIsExternal) != 0);
    fFlags[kAlwaysExternal]->setChecked((obj->getFlags() & plSoundBuffer::kAlwaysExternal) != 0);
    fFlags[kStreamCompressed]->setChecked((obj->getFlags() & plSoundBuffer::kStreamCompressed) != 0);
    fFlags[kOnlyLeftChannel]->setChecked((obj->getFlags() & plSoundBuffer::kOnlyLeftChannel) != 0);
    fFlags[kOnlyRightChannel]->setChecked((obj->getFlags() & plSoundBuffer::kOnlyRightChannel) != 0);

    QGroupBox* headerGroup = new QGroupBox(tr("Header"), this);
    fFormat = new QComboBox(headerGroup);
    fFormat->addItems(QStringList() << "(Invalid)" << "PCM");
    fBitRate = new QSpinBox(headerGroup);
    fBitRate->setRange(0, 0xFFFF);
    fNumChannels = new QSpinBox(headerGroup);
    fNumChannels->setRange(0, 0xFFFF);
    fBlockAlign = new QSpinBox(headerGroup);
    fBlockAlign->setRange(0, 0xFFFF);
    fSampleRate = new QSpinBox(headerGroup);
    fSampleRate->setRange(0, 0x7FFFFFFF);
    fAvgBytesPerSec = new QSpinBox(headerGroup);
    fAvgBytesPerSec->setRange(0, 0x7FFFFFFF);
    QGridLayout* headerLayout = new QGridLayout(headerGroup);
    headerLayout->setVerticalSpacing(4);
    headerLayout->setHorizontalSpacing(8);
    headerLayout->addItem(new QSpacerItem(8, 0, QSizePolicy::Minimum, QSizePolicy::Minimum), 2, 2);
    headerLayout->addWidget(new QLabel(tr("Format:"), headerGroup), 0, 0);
    headerLayout->addWidget(fFormat, 0, 1);
    headerLayout->addWidget(new QLabel(tr("Bit Rate:"), headerGroup), 0, 3);
    headerLayout->addWidget(fBitRate, 0, 4);
    headerLayout->addWidget(new QLabel(tr("Channels:"), headerGroup), 1, 0);
    headerLayout->addWidget(fNumChannels, 1, 1);
    headerLayout->addWidget(new QLabel(tr("Block Align:"), headerGroup), 1, 3);
    headerLayout->addWidget(fBlockAlign, 1, 4);
    headerLayout->addWidget(new QLabel(tr("Sample Rate:"), headerGroup), 2, 0, 1, 3);
    headerLayout->addWidget(fSampleRate, 2, 3, 1, 2);
    headerLayout->addWidget(new QLabel(tr("Avg. Bytes/Second:"), headerGroup), 3, 0, 1, 3);
    headerLayout->addWidget(fAvgBytesPerSec, 3, 3, 1, 2);

    fFormat->setCurrentIndex(obj->getHeader().getFormatTag());
    fBlockAlign->setValue(obj->getHeader().getBlockAlign());
    fNumChannels->setValue(obj->getHeader().getNumChannels());
    fBitRate->setValue(obj->getHeader().getBitsPerSample());
    fSampleRate->setValue(obj->getHeader().getNumSamplesPerSec());
    fAvgBytesPerSec->setValue(obj->getHeader().getAvgBytesPerSec());

    fFilename = new QLineEdit(this);
    fFilename->setText(~obj->getFileName());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(flagGroup, 0, 0, 1, 2);
    layout->addWidget(headerGroup, 1, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Filename:"), this), 2, 0);
    layout->addWidget(fFilename, 2, 1);
}

void QSoundBuffer::saveDamage()
{
    plSoundBuffer* obj = (plSoundBuffer*)fCreatable;

    obj->setFlags((fFlags[kIsExternal]->isChecked() ? plSoundBuffer::kIsExternal : 0)
                | (fFlags[kAlwaysExternal]->isChecked() ? plSoundBuffer::kAlwaysExternal : 0)
                | (fFlags[kStreamCompressed]->isChecked() ? plSoundBuffer::kStreamCompressed : 0)
                | (fFlags[kOnlyLeftChannel]->isChecked() ? plSoundBuffer::kOnlyLeftChannel : 0)
                | (fFlags[kOnlyRightChannel]->isChecked() ? plSoundBuffer::kOnlyRightChannel : 0));
    obj->getHeader().setFormatTag(fFormat->currentIndex());
    obj->getHeader().setBlockAlign(fBlockAlign->value());
    obj->getHeader().setNumChannels(fNumChannels->value());
    obj->getHeader().setBitsPerSample(fBitRate->value());
    obj->getHeader().setNumSamplesPerSec(fSampleRate->value());
    obj->getHeader().setAvgBytesPerSec(fAvgBytesPerSec->value());
    obj->setFileName(~fFilename->text());
}
