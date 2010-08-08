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

#include "QGUITextBoxMod.h"

#include <QGroupBox>
#include <QGridLayout>

class QSmallTextEdit : public QTextEdit {
public:
    QSmallTextEdit(QWidget* parent = 0)
        : QTextEdit(parent)
    { }

    virtual QSize sizeHint() const
    {
        return QSize(QTextEdit::sizeHint().width(),
                     QTextEdit::sizeHint().height() / 2);
    }

    virtual QSize minimumSizeHint() const
    {
        return sizeHint();
    }
};


/* QGUITextBoxMod */
QGUITextBoxMod::QGUITextBoxMod(plCreatable* pCre, QWidget* parent)
              : QCreatable(pCre, kGUITextBoxMod, parent)
{
    pfGUITextBoxMod* ctrl = pfGUITextBoxMod::Convert(fCreatable);

    fControlModLink = new QCreatableLink(this, false);
    fControlModLink->setText(tr("GUI Control Common Properties"));
    fControlModLink->setCreatable(ctrl);
    fControlModLink->setForceType(kGUIControlMod);

    QGroupBox* grpFlags = new QGroupBox(tr("Flags"), this);
    fModFlags[pfGUITextBoxMod::kCenterJustify - kModFlagStart] = new QCheckBox(tr("Center Justification"), grpFlags);
    fModFlags[pfGUITextBoxMod::kRightJustify - kModFlagStart] = new QCheckBox(tr("Right Justification"), grpFlags);
    QGridLayout* flagLayout = new QGridLayout(grpFlags);
    flagLayout->setContentsMargins(8, 8, 8, 8);
    flagLayout->setVerticalSpacing(0);
    flagLayout->setHorizontalSpacing(8);
    flagLayout->addWidget(fModFlags[pfGUITextBoxMod::kCenterJustify - kModFlagStart], 0, 0);
    flagLayout->addWidget(fModFlags[pfGUITextBoxMod::kRightJustify - kModFlagStart], 1, 0);
    for (size_t i=0; i<kModFlagCount; i++)
        fModFlags[i]->setChecked(ctrl->getFlag(i + kModFlagStart));

    fText = new QSmallTextEdit(this);
    fText->setPlainText(~ctrl->getText());
    fLocalizationPath = new QLineEdit(this);
    fLocalizationPath->setText(~ctrl->getLocalizationPath());

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(fControlModLink, 0, 0, 1, 2);
    layout->addWidget(grpFlags, 1, 0, 1, 2);
    layout->addWidget(new QLabel(tr("Text:"), this), 2, 0);
    layout->addWidget(fText, 2, 1);
    layout->addWidget(new QLabel(tr("Localization:"), this), 3, 0);
    layout->addWidget(fLocalizationPath, 3, 1);
}

void QGUITextBoxMod::saveDamage()
{
    pfGUITextBoxMod* ctrl = pfGUITextBoxMod::Convert(fCreatable);

    for (size_t i=0; i<kModFlagCount; i++)
        ctrl->setFlag(i + kModFlagStart, fModFlags[i]->isChecked());

    ctrl->setText(~fText->toPlainText());
    ctrl->setLocalizationPath(~fLocalizationPath->text());
}
