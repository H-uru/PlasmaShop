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

#ifndef _QPLASMAFONT_H
#define _QPLASMAFONT_H

#include "QPlasmaDocument.h"
#include <PRP/Surface/plFont.h>

#include <QAbstractScrollArea>

class QLineEdit;
class QSpinBox;
class QCheckBox;

class QPlasmaCharWidget : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit QPlasmaCharWidget(QWidget *parent);

    void setRenderFont(plFont* font) { fFont = font; }
    void reloadFont();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    plFont* fFont;
};

class QPlasmaFont : public QPlasmaDocument
{
    Q_OBJECT

public:
    explicit QPlasmaFont(QWidget* parent);

    bool loadFile(const QString& filename) override;
    bool saveTo(const QString& filename) override;

private:
    plFont fFont;
    QLineEdit* fFontName;
    QSpinBox* fFontSize;
    QCheckBox* fBold;
    QCheckBox* fItalic;
    QPlasmaCharWidget* fChars;
};

#endif
