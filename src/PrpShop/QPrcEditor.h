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

#include "QPlasmaTextEdit.h"
#include "QPlasmaUtils.h"

class QStatusBar;

class QPrcEditor : public QCreatable
{
    Q_OBJECT

protected:
    QPlasmaTextEdit* fEditor;
    QAction* fSaveAction;
    QStatusBar *fStatusBar;
    bool fDirty;
    bool fLexersInited;
    bool fDoLineNumbers;

public:
    explicit QPrcEditor(plCreatable* pCre, QWidget* parent = Q_NULLPTR);

    QSize sizeHint() const Q_DECL_OVERRIDE;

    virtual bool canCut() const { return fEditor->textCursor().hasSelection(); }
    virtual bool canCopy() const { return fEditor->textCursor().hasSelection(); }
    virtual bool canPaste() const { return fEditor->canPaste(); }
    virtual bool canDelete() const { return fEditor->textCursor().hasSelection(); }
    virtual bool canSelectAll() const { return true; }
    virtual bool canUndo() const { return fEditor->document()->isUndoAvailable(); }
    virtual bool canRedo() const { return fEditor->document()->isRedoAvailable(); }

public slots:
    virtual void updateSettings();
    virtual void performCut() { fEditor->cut(); }
    virtual void performCopy() { fEditor->copy(); }
    virtual void performPaste() { fEditor->paste(); }
    virtual void performDelete() { fEditor->textCursor().removeSelectedText(); }
    virtual void performSelectAll() { fEditor->selectAll(); }
    virtual void performUndo() { fEditor->undo(); }
    virtual void performRedo() { fEditor->redo(); }

    void compilePrc();

signals:
    void statusChanged();

private slots:
    void showCursorPosition();
    void onModificationChanged(bool changed);

protected:
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
    void loadPrcData();
};

#endif
