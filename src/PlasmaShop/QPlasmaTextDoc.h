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

    virtual bool canCut() const;
    virtual bool canCopy() const;
    virtual bool canPaste() const;
    virtual bool canDelete() const;
    virtual bool canSelectAll() const;
    virtual bool canUndo() const;
    virtual bool canRedo() const;

    virtual bool loadFile(QString filename);
    virtual bool saveTo(QString filename);

    void setSyntax(SyntaxMode syn);
    void setEncoding(EncodingMode type);

    SyntaxMode syntax() const;
    EncodingMode encoding() const;

    SyntaxMode GuessIniType();

public slots:
    virtual void updateSettings();
    virtual void performCut();
    virtual void performCopy();
    virtual void performPaste();
    virtual void performDelete();
    virtual void performSelectAll();
    virtual void performUndo();
    virtual void performRedo();
    void expandAll();
    void collapseAll();
    void textFind();
    void textFindNext();
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
    void handleSkip();
    void handleReplaceAll();
};

#endif
