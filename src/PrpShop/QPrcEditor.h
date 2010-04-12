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
#include <ResManager/plResManager.h>
#include <Stream/hsRAMStream.h>
#include <PRP/KeyedObject/hsKeyedObject.h>
#include "QPlasmaUtils.h"

class QPrcEditor : public QCreatable {
    Q_OBJECT

protected:
    QsciScintilla* fEditor;
    QsciLexerXML* fLexerXML;
    bool fDirty;
    bool fLexersInited;
    bool fDoLineNumbers;

public:
    QPrcEditor(plCreatable* pCre, QWidget* parent = NULL);
    virtual void saveDamage();

    virtual QSize sizeHint() const;

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

signals:
    void statusChanged();

private slots:
    void adjustLineNumbers();
};

#endif
