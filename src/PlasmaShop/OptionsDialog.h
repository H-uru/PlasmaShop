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
