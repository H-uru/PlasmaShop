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

#ifndef _QPLASMARENDER_H
#define _QPLASMARENDER_H

#include <QGLWidget>
#include <Math/hsGeometry3.h>
#include <PRP/KeyedObject/plKey.h>
#include <PRP/Surface/plMipmap.h>

class QPlasmaRender : public QGLWidget {
protected:
    struct LayerInfo {
        size_t fTexNameId;
        GLuint fTexTarget;

        LayerInfo();
        LayerInfo(size_t texNameId, GLuint texTarget);
    };

    int fDrawMode, fNavMode;
    int fMouseState;
    QPoint fMouseFrom;
    float fRotZ, fRotX, fModelDist;
    hsVector3 fViewPos;
    hsVector3 fModelMins, fModelMaxs;
    hsTArray<plKey> fObjects;
    plKey fCenterObj;
    std::map<plKey, LayerInfo> fLayers;
    GLuint fRenderListBase;
    GLuint* fTexList;

public:
    enum DrawMode {
        kDrawPoints, kDrawWire, kDrawFlat, kDrawTextured,
        kDrawModeMask = 0x0000000F,
        kDrawForce2Sided = 1<<4,
        kDrawNo2Sided = 1<<5,
    };

    enum NavigationMode {
        kNavModel, kNavScene, kNavModelInScene,
    };

public:
    QPlasmaRender(QWidget* parent);
    virtual ~QPlasmaRender();

    virtual QSize minimumSizeHint() const { return QSize(50, 50); }
    virtual QSize sizeHint() const { return QSize(400, 400); }

    void addObject(plKey obj) { fObjects.append(obj); }
    void setView(const hsVector3& view, float angle = 0.0f);
    void center(plKey obj, bool world);
    void build(int navMode, int drawMode);
    void rebuild();
    void rebuildObject(plKey obj);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    virtual void mouseMoveEvent(QMouseEvent* evt);
    virtual void mousePressEvent(QMouseEvent* evt);
    virtual void mouseReleaseEvent(QMouseEvent* evt);

    bool buildMipmap(plMipmap* map, GLuint id, GLuint target);
    void compileTexture(plKey layer, size_t id);
    void compileObject(plKey obj);
};

#endif
