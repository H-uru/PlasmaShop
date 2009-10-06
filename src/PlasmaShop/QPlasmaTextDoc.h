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
#include "QPlasmaDocument.h"

class QPlasmaTextDoc : public QPlasmaDocument {
    Q_OBJECT

public:
    enum SyntaxMode {
        kStxNone, kStxPython, kStxSDL, kStxIni, kStxConsole, kStxXML,
        kStxHex, kStxFX,
    };

    enum EncryptionMode {
        kEncNone, kEncXtea, kEncAes, kEncDroid,
    };

    enum EncodingMode {
        kTypeAnsi, kTypeUTF8, kTypeUTF16, kTypeUTF32,
    };

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
    void setEncryption(EncryptionMode enc);
    void setEncoding(EncodingMode type);

    SyntaxMode syntax() const;
    EncryptionMode encryption() const;
    EncodingMode encoding() const;

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

private:
    QsciScintilla* fEditor;
    SyntaxMode fSyntax;
    EncryptionMode fEncryption;
    EncodingMode fEncoding;
    bool fDoLineNumbers;
    bool fLexersInited;
    bool fPersistDirty;
    unsigned int fDroidKey[4];

    QsciLexerFni* fLexerFNI;
    QsciLexerFX* fLexerFX;
    QsciLexerHexIsle* fLexerHEX;
    QsciLexerProperties* fLexerINI;
    QsciLexerPython* fLexerPY;
    QsciLexerSDL* fLexerSDL;
    QsciLexerXML* fLexerXML;

private slots:
    void adjustLineNumbers();
    void maybeClean();
};

#endif
