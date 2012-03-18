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

#ifndef _OPTIONSDIALOG_H
#define _OPTIONSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QFont>

#ifdef Q_OS_WIN
    #define DEFAULT_PRP_EDITOR "PrpShop.exe"
    #define DEFAULT_VAULT_EDITOR "VaultShop.exe"
#else
    #define DEFAULT_PRP_EDITOR "PrpShop"
    #define DEFAULT_VAULT_EDITOR "VaultShop"
#endif

class OptionsDialog : public QDialog {
    Q_OBJECT

public:
    OptionsDialog(QWidget* parent);

private:
    QLineEdit* fPrpEditorPath;
    QLineEdit* fVaultEditorPath;
    QLineEdit* fImageEditorPath;

    QCheckBox* fSciMargin;
    QCheckBox* fSciFolding;
    QCheckBox* fSciLineNumbers;
    QCheckBox* fSciUseSpaces;
    QCheckBox* fSciAutoIndent;
    QCheckBox* fSciIndentGuides;
    QCheckBox* fSciShowWhitespace;
    QCheckBox* fSciLongLineMark;
    QLineEdit* fSciTabWidth;
    QLineEdit* fSciLongLineSize;
    QPushButton* fSciFont;

private slots:
    void onSave();
    void onBrowsePrpEditor();
    void onBrowseVaultEditor();
    void onBrowseImageEditor();
    void onSetFont();
};

// Magic for getting special paths
QString GetPSBinPath(QString filename);
QString GetPSDataPath(QString filename);
QString GetAppDataPath();
QString GetDocumentsPath();

extern QString s_binBasePath;

#endif
