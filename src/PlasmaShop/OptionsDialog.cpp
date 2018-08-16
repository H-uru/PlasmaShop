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

#include "OptionsDialog.h"
#include <QCompleter>
#include <QDirModel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QTabWidget>
#include <QLabel>
#include <QDialogButtonBox>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QIntValidator>
#include <QToolButton>
#include <QPushButton>
#include <QFileDialog>
#include <QFontDialog>
#include <QStandardPaths>
#include "QPlasma.h"

#ifdef Q_OS_WIN
    // For SHGetFolderPath
    #include <shlobj.h>
#endif

OptionsDialog::OptionsDialog(QWidget* parent)
             : QDialog(parent)
{
    QTabWidget* tabs = new QTabWidget(this);
    QDirModel* dirModel = new QDirModel(QStringList("*"),
                                        QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Executable,
                                        QDir::Name | QDir::DirsLast | QDir::IgnoreCase,
                                        this);

    // "General" (programs) tab
    QWidget* tabProgs = new QWidget(tabs);
    fPrpEditorPath = new QLineEdit(tabProgs);
    fPrpEditorPath->setCompleter(new QCompleter(dirModel, fPrpEditorPath));
    QToolButton* browsePrpEditor = new QToolButton(tabProgs);
    browsePrpEditor->setText("...");
    fVaultEditorPath = new QLineEdit(tabProgs);
    fVaultEditorPath->setCompleter(new QCompleter(dirModel, fVaultEditorPath));
    QToolButton* browseVaultEditor = new QToolButton(tabProgs);
    browseVaultEditor->setText("...");
    fImageEditorPath = new QLineEdit(tabProgs);
    fImageEditorPath->setCompleter(new QCompleter(dirModel, fImageEditorPath));
    QToolButton* browseImageEditor = new QToolButton(tabProgs);
    browseImageEditor->setText("...");

    QLabel* lblPrpEditor = new QLabel(tr("&PRP Editor:"), tabProgs);
    lblPrpEditor->setBuddy(fPrpEditorPath);
    QLabel* lblVaultEditor = new QLabel(tr("&Vault Editor:"), tabProgs);
    lblVaultEditor->setBuddy(fVaultEditorPath);
    QLabel* lblImgEditor = new QLabel(tr("&Image Editor:"), tabProgs);
    lblImgEditor->setBuddy(fImageEditorPath);

    QGridLayout* layProgs = new QGridLayout(tabProgs);
    layProgs->setContentsMargins(8, 8, 8, 8);
    layProgs->addItem(new QSpacerItem(16, 0, QSizePolicy::Maximum, QSizePolicy::Minimum), 1, 0);
    layProgs->addWidget(lblPrpEditor, 0, 0, 1, 3);
    layProgs->addWidget(fPrpEditorPath, 1, 1);
    layProgs->addWidget(browsePrpEditor, 1, 2);
    layProgs->addItem(new QSpacerItem(0, 8, QSizePolicy::Minimum, QSizePolicy::Minimum), 2, 0, 1, 3);
    layProgs->addWidget(lblVaultEditor, 3, 0, 1, 3);
    layProgs->addWidget(fVaultEditorPath, 4, 1);
    layProgs->addWidget(browseVaultEditor, 4, 2);
    layProgs->addItem(new QSpacerItem(0, 8, QSizePolicy::Minimum, QSizePolicy::Minimum), 5, 0, 1, 3);
    layProgs->addWidget(lblImgEditor, 6, 0, 1, 3);
    layProgs->addWidget(fImageEditorPath, 7, 1);
    layProgs->addWidget(browseImageEditor, 7, 2);
    layProgs->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding), 8, 0, 1, 3);
    tabs->addTab(tabProgs, tr("&General"));

    // Editor tab
    QWidget* tabEditor = new QWidget(tabs);
    fSciLineNumbers = new QCheckBox(tr("&Line numbers"), tabEditor);
    fSciUseSpaces = new QCheckBox(tr("Use &spaces instead of tabs"), tabEditor);
    fSciAutoIndent = new QCheckBox(tr("&Auto-indent"), tabEditor);
    fSciIndentGuides = new QCheckBox(tr("Show in&dentation guides"), tabEditor);
    fSciShowWhitespace = new QCheckBox(tr("Show &whitespace"), tabEditor);
    fSciFont = new QPushButton(tr("Editor &Font"), tabEditor);
    fSciTabWidth = new QLineEdit(tabEditor);
    fSciLongLineMark = new QCheckBox(tr("Long line mar&ker:"), tabEditor);
    fSciLongLineSize = new QLineEdit(tabEditor);
    fSciTabWidth->setValidator(new QIntValidator(fSciTabWidth));
    ((QIntValidator*)fSciTabWidth->validator())->setRange(1, 80);
    fSciLongLineSize->setValidator(new QIntValidator(fSciLongLineSize));
    ((QIntValidator*)fSciLongLineSize->validator())->setRange(1, 9999);

    QLabel* lblTabWidth = new QLabel(tr("&Tab Width:"), tabEditor);
    lblTabWidth->setBuddy(fSciTabWidth);

    QGridLayout* layEditor = new QGridLayout(tabEditor);
    layEditor->setContentsMargins(8, 8, 8, 8);
    layEditor->setVerticalSpacing(4);
    layEditor->addWidget(fSciLineNumbers, 0, 0);
    layEditor->addWidget(fSciUseSpaces, 1, 0);
    layEditor->addWidget(fSciAutoIndent, 2, 0);
    layEditor->addWidget(fSciIndentGuides, 3, 0);
    layEditor->addWidget(fSciShowWhitespace, 4, 0);
    layEditor->addItem(new QSpacerItem(32, 0, QSizePolicy::Minimum, QSizePolicy::Minimum), 0, 1);
    layEditor->addWidget(fSciFont, 0, 2, 1, 2);
    layEditor->addItem(new QSpacerItem(0, 16, QSizePolicy::Minimum, QSizePolicy::Minimum), 1, 2, 1, 2);
    layEditor->addWidget(lblTabWidth, 2, 2);
    layEditor->addWidget(fSciTabWidth, 2, 3);
    layEditor->addWidget(fSciLongLineMark, 3, 2);
    layEditor->addWidget(fSciLongLineSize, 3, 3);
    fSciTabWidth->setMaximumWidth(40);
    fSciLongLineSize->setMaximumWidth(40);
    layEditor->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding), 6, 5);
    tabs->addTab(tabEditor, tr("Text &Editor"));

    // Lay out the dialog itself
    QGridLayout* dlgLayout = new QGridLayout(this);
    dlgLayout->setContentsMargins(4, 4, 4, 4);
    dlgLayout->setVerticalSpacing(4);
    dlgLayout->addWidget(tabs, 0, 0);
    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel,
                                                     Qt::Horizontal, this);
    dlgLayout->addWidget(buttons, 1, 0);
    setFixedSize(sizeHint());
    setWindowTitle(tr("PlasmaShop Preferences"));

    // Populate the current/default settings
    QSettings settings("PlasmaShop", "PlasmaShop");
    fPrpEditorPath->setText(settings.value("PrpEditorPath", DEFAULT_PRP_EDITOR).toString());
    fVaultEditorPath->setText(settings.value("VaultEditorPath", DEFAULT_VAULT_EDITOR).toString());
    fImageEditorPath->setText(settings.value("ImageEditorPath", "").toString());
    fSciLineNumbers->setChecked(settings.value("SciLineNumberMargin", true).toBool());
    fSciUseSpaces->setChecked(settings.value("SciUseSpaces", true).toBool());
    fSciAutoIndent->setChecked(settings.value("SciAutoIndent", true).toBool());
    fSciIndentGuides->setChecked(settings.value("SciIndentGuides", false).toBool());
    fSciShowWhitespace->setChecked(settings.value("SciShowWhitespace", false).toBool());
    fSciLongLineMark->setChecked(settings.value("SciLongLineMark", false).toBool());
    fSciLongLineSize->setText(QString("%1").arg(settings.value("SciLongLineSize", 80).toInt()));
    fSciTabWidth->setText(QString("%1").arg(settings.value("SciTabWidth", 4).toInt()));

    QFont fnt;
    fnt.setFamily(settings.value("SciFont", PLAT_FONT).toString());
    fnt.setPointSize(settings.value("SciFontSize", 10).toInt());
    fnt.setWeight(settings.value("SciFontWeight", QFont::Normal).toInt());
    fnt.setItalic(settings.value("SciFontItalic", false).toBool());
    fSciFont->setFont(fnt);

    fSciLongLineSize->setEnabled(fSciLongLineMark->isChecked());

    // Set up signals for the buttons
    connect(buttons, SIGNAL(accepted()), this, SLOT(onSave()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(browsePrpEditor, SIGNAL(clicked()), this, SLOT(onBrowsePrpEditor()));
    connect(browseVaultEditor, SIGNAL(clicked()), this, SLOT(onBrowseVaultEditor()));
    connect(browseImageEditor, SIGNAL(clicked()), this, SLOT(onBrowseImageEditor()));
    connect(fSciLongLineMark, SIGNAL(clicked(bool)), fSciLongLineSize, SLOT(setEnabled(bool)));
    connect(fSciFont, SIGNAL(clicked()), this, SLOT(onSetFont()));
}

void OptionsDialog::onSave()
{
    // Validate everything *before* saving
    if (!GetPSBinPath(fPrpEditorPath->text()).isEmpty() &&
        (!QFile::exists(GetPSBinPath(fPrpEditorPath->text())) ||
         !QFileInfo(GetPSBinPath(fPrpEditorPath->text())).isFile())) {
        QMessageBox::warning(this, tr("Invalid Path"),
                             tr("You have entered an invalid path to your PRP editor."),
                             QMessageBox::Ok);
    } else if (!GetPSBinPath(fPrpEditorPath->text()).isEmpty() &&
        (!QFile::exists(GetPSBinPath(fVaultEditorPath->text())) ||
         !QFileInfo(GetPSBinPath(fVaultEditorPath->text())).isFile())) {
        QMessageBox::warning(this, tr("Invalid Path"),
                             tr("You have entered an invalid path to your Vault editor."),
                             QMessageBox::Ok);
    } else  if (!fImageEditorPath->text().isEmpty() &&
        (!QFile::exists(GetPSBinPath(fImageEditorPath->text())) ||
         !QFileInfo(GetPSBinPath(fImageEditorPath->text())).isFile())) {
        QMessageBox::warning(this, tr("Invalid Path"),
                             tr("You have entered an invalid path to your Image editor."),
                             QMessageBox::Ok);
    }

    QSettings settings("PlasmaShop", "PlasmaShop");
    settings.setValue("PrpEditorPath", fPrpEditorPath->text());
    settings.setValue("VaultEditorPath", fVaultEditorPath->text());
    settings.setValue("ImageEditorPath", fImageEditorPath->text());
    settings.setValue("SciLineNumberMargin", fSciLineNumbers->isChecked());
    settings.setValue("SciUseSpaces", fSciUseSpaces->isChecked());
    settings.setValue("SciAutoIndent", fSciAutoIndent->isChecked());
    settings.setValue("SciIndentGuides", fSciIndentGuides->isChecked());
    settings.setValue("SciShowWhitespace", fSciShowWhitespace->isChecked());
    settings.setValue("SciLongLineMark", fSciLongLineMark->isChecked());
    settings.setValue("SciLongLineSize", fSciLongLineSize->text().toInt());
    settings.setValue("SciTabWidth", fSciTabWidth->text().toInt());
    settings.setValue("SciFont", fSciFont->font().family());
    settings.setValue("SciFontSize", fSciFont->font().pointSize());
    settings.setValue("SciFontWeight", fSciFont->font().weight());
    settings.setValue("SciFontItalic", fSciFont->font().italic());

    accept();
}

#ifdef Q_OS_WIN
    #define EXECFILTER "Executables (*.exe)"
#else
    #define EXECFILTER "All Files (*)"
#endif

void OptionsDialog::onBrowsePrpEditor()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select PRP Editor"),
                                                GetPSBinPath(fPrpEditorPath->text()),
                                                EXECFILTER);
    if (!path.isEmpty())
        fPrpEditorPath->setText(path);
}

void OptionsDialog::onBrowseVaultEditor()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select Vault Editor"),
                                                GetPSBinPath(fVaultEditorPath->text()),
                                                EXECFILTER);
    if (!path.isEmpty())
        fVaultEditorPath->setText(path);
}

void OptionsDialog::onBrowseImageEditor()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select Image Editor"),
                                                GetPSBinPath(fImageEditorPath->text()),
                                                EXECFILTER);
    if (!path.isEmpty())
        fImageEditorPath->setText(path);
}

void OptionsDialog::onSetFont()
{
    fSciFont->setFont(QFontDialog::getFont(0, fSciFont->font(), this,
                                           tr("Select Editor Font")));
}


/* Path getter magic */
QString s_binBasePath;

QString GetPSBinPath(QString filename)
{
    return QDir(s_binBasePath).absoluteFilePath(filename);
}

QString GetAppDataPath()
{
#ifdef Q_OS_WIN
    // Find the Local AppData path
    WCHAR path[MAX_PATH];
    SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path);
    return QString::fromUtf16((const ushort*)path);
#else
    // Just return the user's home dir
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#endif
}

QString GetDocumentsPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}
