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

#ifndef _NEWFILE_H
#define _NEWFILE_H

#include <QDialog>
#include <QComboBox>
#include <QListWidget>

enum FileClass
{
    kFileAge, kFileFni, kFileSum, kFileCsv, kFileFont, kFilePython, kFilePak,
    kFileSDL, kFileLoc, kFileSub, kFileCursorPak, kFileFontPak, kFileHexLevel,
    kFileShader,
};

class NewFileDialog : public QDialog
{
    Q_OBJECT

public:
    NewFileDialog(QWidget* parent);
    ~NewFileDialog();

    FileClass fileType() const
    {
        return (FileClass)fFileTypeList->currentItem()->data(Qt::UserRole).toUInt();
    }

private:
    QComboBox* fFileTypeFilter;
    QListWidget* fFileTypeList;

private slots:
    void populateFileList(int dummy = 0);
    void onItemActivated(QListWidgetItem*) { accept(); }
};

#endif
