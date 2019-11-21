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
class QDoubleSpinBox;
class QCheckBox;
class QGroupBox;

class QPlasmaCharWidget : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit QPlasmaCharWidget(QWidget *parent);

    void setRenderFont(plFont* font) { fFont = font; }
    void reloadFont();

signals:
    void characterSelected(int which);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    void focusInEvent(QFocusEvent*) override { viewport()->update(); }
    void focusOutEvent(QFocusEvent*) override { viewport()->update(); }

private:
    plFont* fFont;
    int fSelected;
    int fCharsPerLine;
};

class QPlasmaCharRender : public QWidget
{
    Q_OBJECT

public:
    QPlasmaCharRender(QWidget* parent);

    void setRenderFont(plFont* font) { fFont = font; }
    void reloadFont();

    void setCharacter(int which);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    plFont* fFont;
    int fCharIdx;
};

class QPlasmaFont : public QPlasmaDocument
{
    Q_OBJECT

public:
    explicit QPlasmaFont(QWidget* parent);

    bool loadFile(const QString& filename) override;
    bool saveTo(const QString& filename) override;

public slots:
    void showCharacter(int which);
    void updateCharacter();

private:
    plFont fFont;
    QLineEdit* fFontName;
    QSpinBox* fFontSize;
    QCheckBox* fBold;
    QCheckBox* fItalic;
    QPlasmaCharWidget* fChars;

    int fCharacter;
    QGroupBox* fCharDetail;
    QPlasmaCharRender* fRender;
    QDoubleSpinBox* fLeftKern;
    QDoubleSpinBox* fRightKern;
    QSpinBox* fBaseLine;
    bool fBlockSpinBoxes;
};

#endif
