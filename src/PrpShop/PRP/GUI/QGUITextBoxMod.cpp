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
    pfGUITextBoxMod* ctrl = (pfGUITextBoxMod*)fCreatable;

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
    fLocalizationPath->setText(WStrToQStr(ctrl->getLocalizationPath()));

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
    pfGUITextBoxMod* ctrl = (pfGUITextBoxMod*)fCreatable;

    for (size_t i=0; i<kModFlagCount; i++) {
        if (fModFlags[i]->isChecked())
            ctrl->setFlag(i + kModFlagStart);
        else
            ctrl->clearFlag(i + kModFlagStart);
    }

    ctrl->setText(~fText->toPlainText());
    ctrl->setLocalizationPath(QStrToWStr(fLocalizationPath->text()));
}
