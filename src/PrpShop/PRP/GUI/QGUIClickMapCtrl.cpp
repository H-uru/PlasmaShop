#include "QGUIClickMapCtrl.h"

#include <QGroupBox>
#include <QGridLayout>

/* QGUIClickMapCtrl */
QGUIClickMapCtrl::QGUIClickMapCtrl(plCreatable* pCre, QWidget* parent)
                : QCreatable(pCre, kGUIClickMapCtrl, parent)
{
    pfGUIClickMapCtrl* ctrl = (pfGUIClickMapCtrl*)fCreatable;

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setText(tr("GUI Control Common Properties"));
    fControlModLink->setCreatable(ctrl);
    fControlModLink->setForceType(kGUIControlMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUIClickMapCtrl::kReportDragging - kModFlagStart] = new QCheckBox(tr("Report Dragging"), grpFlags);
    fModFlags[pfGUIClickMapCtrl::kReportHovering - kModFlagStart] = new QCheckBox(tr("Report Hovering"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUIClickMapCtrl::kReportDragging - kModFlagStart], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUIClickMapCtrl::kReportHovering - kModFlagStart], 1, 0);
    for (size_t i=0; i<kModFlagCount; i++)
        fModFlags[i]->setChecked(ctrl->getFlag(i + kModFlagStart));

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(grpFlags, 1, 0, 1, 2);
}

void QGUIClickMapCtrl::saveDamage()
{
    pfGUIClickMapCtrl* ctrl = (pfGUIClickMapCtrl*)fCreatable;

    for (size_t i=0; i<kModFlagCount; i++)
        ctrl->setFlag(i + kModFlagStart, fModFlags[i]->isChecked());
}
