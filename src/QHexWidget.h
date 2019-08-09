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

#ifndef _QHEXWIDGET_H
#define _QHEXWIDGET_H

#include <QAbstractScrollArea>

class QHexWidget : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit QHexWidget(QWidget* parent = 0);

    QSize sizeHint() const Q_DECL_OVERRIDE;
    int currentAddress() const { return fCurrentAddress; }
    int firstVisibleAddress() const { return fViewportAddress; }
    int lastVisibleAddress() const;
    QPair<int, int> selection() const;

    void loadFromData(const QByteArray& data);
    bool loadFromFile(const QString& fileName);  // WARNING: Loads entire file into RAM

    int dataSize() const { return fData.size(); }
    uchar byteAt(int address) const { return (uchar)fData.at(address); }

signals:
    void currentAddressChanged(int address);

public slots:
    void setCurrentAddress(int address);

protected:
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent*) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent*) Q_DECL_OVERRIDE;
    void scrollContentsBy(int dx, int dy) Q_DECL_OVERRIDE;

private:
    QByteArray fData;
    int fCurrentAddress;
    int fViewportAddress;
    int fSelectionStart;

    int visibleLines(const QFontMetrics& fm, int viewportHeight) const;
    void getRenderMetrics(const QFontMetrics& fm, int maxAddress,
                          int& byteOffset, int& charOffset, int& rightMargin) const;
    void render(QPainter *painter);

    int addressAt(int x, int y) const;
};

#endif
