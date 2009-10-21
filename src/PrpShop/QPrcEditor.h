#ifndef _QPRCEDITOR_H
#define _QPRCEDITOR_H

#include <QWidget>
#include <QsciPS3/qsciscintilla.h>
#include <QsciPS3/qscilexerxml.h>
#include <ResManager/plResManager.h>
#include <Stream/hsRAMStream.h>
#include <PRP/KeyedObject/hsKeyedObject.h>
#include "QPlasmaUtils.h"

class QPrcEditor : public QWidget {
    Q_OBJECT

protected:
    QsciScintilla* fEditor;
    QsciLexerXML* fLexerXML;
    bool fDirty;
    bool fLexersInited;
    bool fPersistDirty;
    bool fDoLineNumbers;
    plCreatable* fCreatable;

public:
    QPrcEditor(plCreatable* pCre, QWidget* parent = NULL);
    bool isMatch(plCreatable* pCre);
    bool isDirty() const;
    
    virtual bool canCut() const;
    virtual bool canCopy() const;
    virtual bool canPaste() const;
    virtual bool canDelete() const;
    virtual bool canSelectAll() const;
    virtual bool canUndo() const;
    virtual bool canRedo() const;

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

signals:
    void becameDirty();
    void becameClean();
    void statusChanged();

private slots:
    void makeDirty();
    void makeClean();
    void adjustLineNumbers();
    void maybeClean();
};

#endif
