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

#include "QKeyDialog.h"

#include <QLabel>
#include <QDialogButtonBox>
#include <QGridLayout>
#include "QPlasmaUtils.h"

/* QNewKeyDialog */
QNewKeyDialog::QNewKeyDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("New Object..."));

    fLocationBox = new QComboBox(this);
    fTypeBox = new QComboBox(this);
    fName = new QLineEdit(this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel |
                                  QDialogButtonBox::Ok);

    QGridLayout* layout = new QGridLayout(this);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Location:"), this), 0, 0);
    layout->addWidget(fLocationBox, 0, 1);
    layout->addWidget(new QLabel(tr("Type:"), this), 1, 0);
    layout->addWidget(fTypeBox, 1, 1);
    layout->addWidget(new QLabel(tr("Name:"), this), 2, 0);
    layout->addWidget(fName, 2, 1);
    layout->addWidget(buttonBox, 3, 0, 1, 2);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void QNewKeyDialog::init(plResManager* mgr, plLocation loc, short type)
{
    fLocationBox->clear();
    fTypeBox->clear();
    fName->clear();

    fLocations = mgr->getLocations();
    for (size_t i=0; i<fLocations.size(); i++) {
        plPageInfo* page = mgr->FindPage(fLocations[i]);
        fLocationBox->addItem(QIcon(":/img/page.png"),
            st2qstr(ST::format("[{}] {}", page->getAge(), page->getPage())));
    }

    fTypes = pqGetValidKOTypes();
    for (size_t i=0; i<fTypes.size(); i++)
        fTypeBox->addItem(pqGetTypeIcon(fTypes[i]), pqGetFriendlyClassName(fTypes[i]));

    if (loc.isValid()) {
        for (size_t i=0; i<fLocations.size(); i++) {
            if (loc == fLocations[i])
                fLocationBox->setCurrentIndex(i);
        }
    }
    if (type >= 0) {
        for (size_t i=0; i<fTypes.size(); i++) {
            if (type == fTypes[i])
                fTypeBox->setCurrentIndex(i);
        }
    }
}


/* QFindKeyDialog */
QFindKeyDialog::QFindKeyDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Find Object..."));

    fLocationBox = new QComboBox(this);
    fTypeBox = new QComboBox(this);
    fKeyBox = new QComboBox(this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel |
                                  QDialogButtonBox::Ok);

    QGridLayout* layout = new QGridLayout(this);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);
    layout->addWidget(new QLabel(tr("Location:"), this), 0, 0);
    layout->addWidget(fLocationBox, 0, 1);
    layout->addWidget(new QLabel(tr("Type:"), this), 1, 0);
    layout->addWidget(fTypeBox, 1, 1);
    layout->addWidget(new QLabel(tr("Object:"), this), 2, 0);
    layout->addWidget(fKeyBox, 2, 1);
    layout->addWidget(buttonBox, 3, 0, 1, 2);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(fLocationBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &QFindKeyDialog::locationSelected);
    connect(fTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &QFindKeyDialog::typeSelected);
}

void QFindKeyDialog::init(plResManager* mgr, plLocation loc, short type)
{
    fResMgr = mgr;
    fLocationBox->clear();

    fLocations = fResMgr->getLocations();
    for (size_t i=0; i<fLocations.size(); i++) {
        plPageInfo* page = fResMgr->FindPage(fLocations[i]);
        fLocationBox->addItem(QIcon(":/img/page.png"),
            st2qstr(ST::format("[{}] {}", page->getAge(), page->getPage())));
    }

    if (loc.isValid()) {
        for (size_t i=0; i<fLocations.size(); i++) {
            if (loc == fLocations[i])
                fLocationBox->setCurrentIndex(i);
        }
    }
    if (type >= 0) {
        for (size_t i=0; i<fTypes.size(); i++) {
            if (type == fTypes[i])
                fTypeBox->setCurrentIndex(i);
        }
    }
}

void QFindKeyDialog::init(plResManager* mgr, plKey current)
{
    fResMgr = mgr;
    fLocationBox->clear();

    fLocations = fResMgr->getLocations();
    for (size_t i=0; i<fLocations.size(); i++) {
        plPageInfo* page = fResMgr->FindPage(fLocations[i]);
        fLocationBox->addItem(QIcon(":/img/page.png"),
            st2qstr(ST::format("[{}] {}", page->getAge(), page->getPage())));
    }

    if (current.Exists()) {
        for (size_t i=0; i<fLocations.size(); i++) {
            if (fLocations[i] == current->getLocation())
                fLocationBox->setCurrentIndex(i);
        }
        for (size_t i=0; i<fTypes.size(); i++) {
            if (fTypes[i] == current->getType())
                fTypeBox->setCurrentIndex(i);
        }
        for (size_t i=0; i<fKeys.size(); i++) {
            if (fKeys[i] == current)
                fKeyBox->setCurrentIndex(i);
        }
    }
}

plKey QFindKeyDialog::selection() const
{
    if (fKeyBox->currentIndex() < 0)
        return plKey();
    return fKeys[fKeyBox->currentIndex()];
}

void QFindKeyDialog::locationSelected(int idx)
{
    fTypeBox->clear();
    if (idx < 0)
        return;

    fTypes = fResMgr->getTypes(fLocations[idx]);
    for (size_t i=0; i<fTypes.size(); i++)
        fTypeBox->addItem(pqGetTypeIcon(fTypes[i]), pqGetFriendlyClassName(fTypes[i]));
}

void QFindKeyDialog::typeSelected(int idx)
{
    fKeyBox->clear();
    if (idx < 0)
        return;

    fKeys = fResMgr->getKeys(fLocations[fLocationBox->currentIndex()], fTypes[idx]);
    for (size_t i=0; i<fKeys.size(); i++)
        fKeyBox->addItem(pqFormatKeyName(fKeys[i]));
}
