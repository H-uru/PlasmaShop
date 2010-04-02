#include "QGUIDraggableMod.h"

#include <QGroupBox>
#include <QGridLayout>

/* QGUIDraggableMod */
QGUIDraggableMod::QGUIDraggableMod(plCreatable* pCre, QWidget* parent)
                : QCreatable(pCre, kGUIDraggableMod, parent)
{
    pfGUIDraggableMod* ctrl = (pfGUIDraggableMod*)fCreatable;

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setText(tr("GUI Control Common Properties"));
    fControlModLink->setCreatable(ctrl);
    fControlModLink->setForceType(kGUIControlMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUIDraggableMod::kReportDragging - kModFlagStart] = new QCheckBox(tr("Report Dragging"), grpFlags);
    fModFlags[pfGUIDraggableMod::kHideCursorWhileDragging - kModFlagStart] = new QCheckBox(tr("Hide Cursor While Dragging"), grpFlags);
    fModFlags[pfGUIDraggableMod::kAlwaysSnapBackToStart - kModFlagStart] = new QCheckBox(tr("Always Snap Back to Start"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUIDraggableMod::kReportDragging - kModFlagStart], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUIDraggableMod::kHideCursorWhileDragging - kModFlagStart], 1, 0);
    flagLayout->addWidget(fModFlags[pfGUIDraggableMod::kAlwaysSnapBackToStart - kModFlagStart], 2, 0);
    for (size_t i=0; i<kModFlagCount; i++)
        fModFlags[i]->setChecked(ctrl->getFlag(i + kModFlagStart));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(grpFlags, 1, 0, 1, 2);
}

void QGUIDraggableMod::saveDamage()
{
    pfGUIDraggableMod* ctrl = (pfGUIDraggableMod*)fCreatable;

    for (size_t i=0; i<kModFlagCount; i++)
        ctrl->setFlag(i + kModFlagStart, fModFlags[i]->isChecked());
}
