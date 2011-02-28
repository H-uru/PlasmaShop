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

#include "NewFile.h"
#include "QPlasmaDocument.h"

#include <QLabel>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QSettings>

enum FilterValue {
    kGameAny, kGameUru, kGameMoul, kGameMyst5, kGameCT, kGameHex, kGameMQO
};

NewFileDialog::NewFileDialog(QWidget* parent) : QDialog(parent)
{
    QLabel* lblGame = new QLabel(tr("Game &Filter:"));
    fFileTypeFilter = new QComboBox(this);
    fFileTypeFilter->addItem(QIcon(), tr("All Supported Games"));
    fFileTypeFilter->addItem(QIcon(":/img/gm_URU.png"), tr("UU, Alcugs, Uru CC"));
    fFileTypeFilter->addItem(QIcon(":/img/gm_URU.png"), tr("Uru Live"));
    fFileTypeFilter->addItem(QIcon(":/img/gm_EOA.png"), tr("Myst 5"));
    fFileTypeFilter->addItem(QIcon(":/img/gm_CT.png"), tr("Crowthistle"));
    fFileTypeFilter->addItem(QIcon(":/img/gm_HI.png"), tr("Hex Isle"));
    fFileTypeFilter->addItem(QIcon(":/img/gm_MQO.png"), tr("MagiQuest Online"));
    lblGame->setBuddy(fFileTypeFilter);

    fFileTypeList = new QListWidget(this);
    //TODO

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(tr("Create"), QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setVerticalSpacing(8);
    layout->setHorizontalSpacing(4);
    layout->addWidget(lblGame, 0, 0);
    layout->addWidget(fFileTypeFilter, 0, 1);
    layout->addWidget(fFileTypeList, 1, 0, 1, 2);
    layout->addWidget(buttonBox, 2, 0, 1, 2);

    connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));
    connect(fFileTypeFilter, SIGNAL(currentIndexChanged(int)), SLOT(populateFileList(int)));
    connect(fFileTypeList, SIGNAL(itemActivated(QListWidgetItem*)), SLOT(onItemActivated(QListWidgetItem*)));

    QSettings settings("PlasmaShop", "PlasmaShop");
    fFileTypeFilter->setCurrentIndex(settings.value("NewFileFilter", kGameAny).toInt());
    populateFileList();

    setWindowTitle(tr("New File..."));
}

NewFileDialog::~NewFileDialog()
{
    QSettings settings("PlasmaShop", "PlasmaShop");
    settings.setValue("NewFileFilter", fFileTypeFilter->currentIndex());
}

void NewFileDialog::populateFileList(int)
{
    fFileTypeList->clear();

    QListWidgetItem* item;
    int filter = fFileTypeFilter->currentIndex();

    item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".age"),
                               tr("Age File"), fFileTypeList);
    item->setData(Qt::UserRole, kFileAge);
    if (filter == kGameAny || filter == kGameMyst5 || filter == kGameCT) {
        item = new QListWidgetItem(QPlasmaDocument::GetDocIcon("cursors.dat"),
                                   tr("Cursor Package"), fFileTypeList);
        item->setData(Qt::UserRole, kFileCursorPak);
    }
    item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".fni"),
                               tr("Fni File"), fFileTypeList);
    item->setData(Qt::UserRole, kFileFni);
    item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".p2f"),
                               tr("Font File"), fFileTypeList);
    item->setData(Qt::UserRole, kFileFont);
    if (filter == kGameAny || filter == kGameMyst5 || filter == kGameCT) {
        item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".pfp"),
                                   tr("Font Package"), fFileTypeList);
        item->setData(Qt::UserRole, kFileFontPak);
    }
    if (filter == kGameAny || filter == kGameHex) {
        item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".hex"),
                                   tr("Hex Isle Level"), fFileTypeList);
        item->setData(Qt::UserRole, kFileHexLevel);
    }
    if (filter != kGameUru && filter != kGameCT) {
        item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".loc"),
                                   tr("Localization File"), fFileTypeList);
        item->setData(Qt::UserRole, kFileLoc);
    }
    if (filter != kGameMoul && filter != kGameMQO) {
        item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".sub"),
                                   tr("Manifest (SUM) File"), fFileTypeList);
        item->setData(Qt::UserRole, kFileSum);
    }
    item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".py"),
                               tr("Python Script"), fFileTypeList);
    item->setData(Qt::UserRole, kFilePython);
    item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".pak"),
                               tr("Python Package"), fFileTypeList);
    item->setData(Qt::UserRole, kFilePak);
    item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".sdl"),
                               tr("SDL File"), fFileTypeList);
    item->setData(Qt::UserRole, kFileSDL);
    if (filter == kGameAny || filter == kGameHex) {
        item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".fx"),
                                   tr("Shader Source"), fFileTypeList);
        item->setData(Qt::UserRole, kFileShader);
    }
    if (filter == kGameAny || filter == kGameMyst5 || filter == kGameHex) {
        item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".sub"),
                                   tr("Subtitle File"), fFileTypeList);
        item->setData(Qt::UserRole, kFileSub);
    }
    if (filter != kGameHex) {
        item = new QListWidgetItem(QPlasmaDocument::GetDocIcon(".csv"),
                                   tr("VisRegion (CSV) File"), fFileTypeList);
        item->setData(Qt::UserRole, kFileCsv);
    }

    // Ensure an item is selected
    if (fFileTypeList->currentItem() == 0)
        fFileTypeList->setCurrentItem(fFileTypeList->item(0));
}
