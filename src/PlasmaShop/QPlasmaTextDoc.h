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

#include <QsciPS3/qsciscintilla.h>
#include <QsciPS3/qscilexerfni.h>
#include <QsciPS3/qscilexerfx.h>
#include <QsciPS3/qscilexerhexisle.h>
#include <QsciPS3/qscilexerproperties.h>
#include <QsciPS3/qscilexerpython.h>
#include <QsciPS3/qscilexersdl.h>
#include <QsciPS3/qscilexerxml.h>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include "QPlasmaDocument.h"

class QPlasmaTextDoc : public QPlasmaDocument {
    Q_OBJECT

public:
    enum SyntaxMode {
        kStxNone, kStxPython, kStxSDL, kStxIni, kStxConsole, kStxXML,
        kStxHex, kStxFX,
    };

    enum EncodingMode {
        kTypeAnsi, kTypeUTF8, kTypeUTF16, kTypeUTF32,
    };

public:
    QPlasmaTextDoc(QWidget* parent);

    virtual bool canCut() const { return fEditor->hasSelectedText(); }
    virtual bool canCopy() const { return fEditor->hasSelectedText(); }
    virtual bool canPaste() const { return fEditor->isPasteAvailable(); }
    virtual bool canDelete() const { return fEditor->hasSelectedText(); }
    virtual bool canSelectAll() const { return true; }
    virtual bool canUndo() const { return fEditor->isUndoAvailable(); }
    virtual bool canRedo() const { return fEditor->isRedoAvailable(); }

    virtual bool loadFile(QString filename);
    virtual bool saveTo(QString filename);

    void setSyntax(SyntaxMode syn);
    void setEncoding(EncodingMode type);

    SyntaxMode syntax() const { return fSyntax; }
    EncodingMode encoding() const { return fEncoding; }

    SyntaxMode GuessIniType();

public slots:
    virtual void updateSettings();
    virtual void performCut() { fEditor->cut(); }
    virtual void performCopy() { fEditor->copy(); }
    virtual void performPaste() { fEditor->paste(); }
    virtual void performDelete() { fEditor->removeSelectedText(); }
    virtual void performSelectAll() { fEditor->selectAll(true); }
    virtual void performUndo() { fEditor->undo(); }
    virtual void performRedo() { fEditor->redo(); }
    void expandAll() { fEditor->setFoldAll(false); }
    void collapseAll() { fEditor->setFoldAll(true); }
    void textFind();
    void textFindNext() { fEditor->findNext(); }
    void textReplace();

    bool onFind(QString text, bool regex, bool cs, bool wo, bool reverse);
    void onReplace(QString newText);
    bool onReplaceAll(QString text, bool regex, bool cs, bool wo, QString newText);

private:
    QsciScintilla* fEditor;
    SyntaxMode fSyntax;
    EncodingMode fEncoding;
    bool fDoLineNumbers;
    bool fLexersInited;

    QsciLexerFni* fLexerFNI;
    QsciLexerFX* fLexerFX;
    QsciLexerHexIsle* fLexerHEX;
    QsciLexerProperties* fLexerINI;
    QsciLexerPython* fLexerPY;
    QsciLexerSDL* fLexerSDL;
    QsciLexerXML* fLexerXML;

private slots:
    void adjustLineNumbers();
};


class TextFindDialog : public QDialog {
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
    virtual ~TextFindDialog();

private slots:
    void handleFind();
    void handleSkip() { fDocument->textFindNext(); }
    void handleReplaceAll();
};

#endif
