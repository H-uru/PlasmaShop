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

#ifndef _QPLASMATEXTDOC_H
#define _QPLASMATEXTDOC_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include "QPlasmaDocument.h"
#include "QPlasmaTextEdit.h"

class QPlasmaTextDoc : public QPlasmaDocument
{
    Q_OBJECT

public:
    enum SyntaxMode
    {
        kStxNone, kStxPython, kStxSDL, kStxIni, kStxConsole, kStxXML,
        kStxHex, kStxFX,
    };

    enum EncodingMode
    {
        kTypeAnsi, kTypeUTF8, kTypeUTF16, kTypeUTF32,
    };

public:
    explicit QPlasmaTextDoc(QWidget* parent);

    bool canCut() const Q_DECL_OVERRIDE { return fEditor->haveSelection(); }
    bool canCopy() const Q_DECL_OVERRIDE { return fEditor->haveSelection(); }
    bool canPaste() const Q_DECL_OVERRIDE { return fEditor->canPaste(); }
    bool canDelete() const Q_DECL_OVERRIDE { return fEditor->haveSelection(); }
    bool canSelectAll() const Q_DECL_OVERRIDE { return true; }
    bool canUndo() const Q_DECL_OVERRIDE { return fEditor->document()->isUndoAvailable(); }
    bool canRedo() const Q_DECL_OVERRIDE { return fEditor->document()->isRedoAvailable(); }

    bool loadFile(const QString& filename) Q_DECL_OVERRIDE;
    bool saveTo(const QString& filename) Q_DECL_OVERRIDE;

    void setSyntax(SyntaxMode syn);
    void setEncoding(EncodingMode type);

    SyntaxMode syntax() const { return fSyntax; }
    EncodingMode encoding() const { return fEncoding; }

    SyntaxMode GuessIniType();

public slots:
    void updateSettings() Q_DECL_OVERRIDE;
    void performCut() Q_DECL_OVERRIDE { fEditor->cut(); }
    void performCopy() Q_DECL_OVERRIDE { fEditor->copy(); }
    void performPaste() Q_DECL_OVERRIDE { fEditor->paste(); }
    void performDelete() Q_DECL_OVERRIDE { fEditor->textCursor().removeSelectedText(); }
    void performSelectAll() Q_DECL_OVERRIDE { fEditor->selectAll(); }
    void performUndo() Q_DECL_OVERRIDE { fEditor->undo(); }
    void performRedo() Q_DECL_OVERRIDE { fEditor->redo(); }
    void textFind();
    void textFindNext() { onFindNext(); }
    void textReplace();

    bool onFind(const QString& text, bool regex, bool cs, bool wo, bool reverse);
    bool onFindNext();
    void onReplace(const QString& newText);
    bool onReplaceAll(const QString& text, bool regex, bool cs, bool wo,
                      const QString& newText);

private:
    QPlasmaTextEdit* fEditor;
    SyntaxMode fSyntax;
    EncodingMode fEncoding;
};


class TextFindDialog : public QDialog
{
    Q_OBJECT

private:
    QPlasmaTextDoc* fDocument;
    QLineEdit* fFindText;
    QLineEdit* fNewText;
    QCheckBox* fRegEx;
    QCheckBox* fCaseSensitive;
    QCheckBox* fWholeWord;
    QCheckBox* fReverse;
    QPushButton* fBtnSkip;

public:
    TextFindDialog(QPlasmaTextDoc* parent, bool replace);
    ~TextFindDialog() Q_DECL_OVERRIDE;

    bool performSearch();

private slots:
    void handleFind();
    void handleReplaceAll();
};

#endif
