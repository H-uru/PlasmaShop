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

#ifndef _QPRCEDITOR_H
#define _QPRCEDITOR_H

#include "PRP/QCreatable.h"

#include <QsciPS3/qsciscintilla.h>
#include <QsciPS3/qscilexerxml.h>
#include "QPlasmaUtils.h"

class QStatusBar;

class QPrcEditor : public QCreatable {
    Q_OBJECT

protected:
    QsciScintilla* fEditor;
    QsciLexerXML* fLexerXML;
    QAction* fSaveAction;
    QStatusBar *fStatusBar;
    bool fDirty;
    bool fLexersInited;
    bool fDoLineNumbers;

public:
    QPrcEditor(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage() { }   // Handled in compilation

    QSize sizeHint() const Q_DECL_OVERRIDE;

    virtual bool canCut() const { return fEditor->hasSelectedText(); }
    virtual bool canCopy() const { return fEditor->hasSelectedText(); }
    virtual bool canPaste() const { return fEditor->isPasteAvailable(); }
    virtual bool canDelete() const { return fEditor->hasSelectedText(); }
    virtual bool canSelectAll() const { return true; }
    virtual bool canUndo() const { return fEditor->isUndoAvailable(); }
    virtual bool canRedo() const { return fEditor->isRedoAvailable(); }

public slots:
    virtual void updateSettings();
    virtual void performCut() { fEditor->cut(); }
    virtual void performCopy() { fEditor->copy(); }
    virtual void performPaste() { fEditor->paste(); }
    virtual void performDelete() { fEditor->removeSelectedText(); }
    virtual void performSelectAll() { fEditor->selectAll(true); }
    virtual void performUndo() { fEditor->undo(); }
    virtual void performRedo() { fEditor->redo(); }

    void compilePrc();

signals:
    void statusChanged();

private slots:
    void adjustLineNumbers();
    void showCursorPosition(int line, int column);
    void onDirty();
    void onClean();

protected:
    virtual void closeEvent(QCloseEvent* event);
    void loadPrcData();
};

#endif
