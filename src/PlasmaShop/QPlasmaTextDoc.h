#ifndef _QPLASMATEXTDOC_H
#define _QPLASMATEXTDOC_H

#include <Qsci/qsciscintilla.h>
#include "QPlasmaDocument.h"

class QPlasmaTextDoc : public QPlasmaDocument {
    Q_OBJECT

public:
    enum SyntaxMode {
        kStxNone, kStxPython, kStxSDL1, kStxSDL2, kStxIni, kStxConsole,
        kStxXML, kStxHex, kStxFX,
    };

    enum EncryptionMode {
        kEncNone, kEncXtea, kEncAes, kEncDroid,
    };

    enum EncodingMode {
        kTypeAnsi, kTypeUTF8, kTypeUTF16, kTypeUTF32,
    };

    QPlasmaTextDoc(QWidget* parent);

    virtual void loadFile(QString filename);
    virtual void saveTo(QString filename);

private:
    QsciScintilla* fEditor;
    SyntaxMode fSyntax;
    EncryptionMode fEncryption;
    EncodingMode fEncoding;
};

#endif
