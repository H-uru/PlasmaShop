#include "QPlasmaRender.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <Debug/plDebug.h>
#include <PRP/Surface/plLayer.h>
#include <PRP/Surface/plCubicEnvironmap.h>
#include <PRP/Object/plSceneObject.h>
#include <PRP/Object/plDrawInterface.h>
#include <PRP/Object/plCoordinateInterface.h>
#include <PRP/Geometry/plDrawableSpans.h>
#include <QGLFormat>
#include <QMessageBox>
#include <QMouseEvent>
#include <cmath>
#include "../../QPlasmaUtils.h"

PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB = NULL;

const float RADS = 0.0174532925f;
static QGLFormat s_format = QGL::DepthBuffer | QGL::StencilBuffer
                          | QGL::Rgba | QGL::AlphaChannel | QGL::DoubleBuffer;

QPlasmaRender::LayerInfo::LayerInfo()
             : fTexNameId(0), fTexTarget(0)
{ }

QPlasmaRender::LayerInfo::LayerInfo(size_t texNameId, GLuint texTarget)
             : fTexNameId(texNameId), fTexTarget(texTarget)
{ }

QPlasmaRender::QPlasmaRender(QWidget* parent)
             : QGLWidget(s_format, parent), fDrawMode(kDrawTextured),
               fNavMode(kNavModel), fMouseState(0), fRotZ(0.0f), fRotX(0.0f),
               fModelDist(0.0f), fRenderListBase(0), fTexList(NULL)
{ }

QPlasmaRender::~QPlasmaRender()
{
    makeCurrent();
    glDeleteLists(fRenderListBase, fObjects.getSize());
    if (fTexList != NULL) {
        glDeleteTextures(fLayers.size(), fTexList);
        delete[] fTexList;
    }
}

QSize QPlasmaRender::minimumSizeHint() const
{ return QSize(50, 50); }

QSize QPlasmaRender::sizeHint() const
{ return QSize(400, 400); }

void QPlasmaRender::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    if (glCompressedTexImage2DARB == NULL)
        glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)context()->getProcAddress("glCompressedTexImage2DARB");
}

void QPlasmaRender::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width/(float)height, 0.1f, 10000.0f);
}

void QPlasmaRender::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    if (fNavMode == kNavModel || fNavMode == kNavModelInScene) {
        glTranslatef(0.0f, 0.0f, -fModelDist);
        glRotatef(-90.0f + fRotX, 1.0f, 0.0f, 0.0f);
        glRotatef(fRotZ, 0.0f, 0.0f, 1.0f);
        glTranslatef(-fViewPos.X, -fViewPos.Y, -fViewPos.Z);
    } else {
        glRotatef(-90.0f + fRotX, 1.0f, 0.0f, 0.0f);
        glRotatef(fRotZ, 0.0f, 0.0f, 1.0f);
        glTranslatef(-fViewPos.X, -fViewPos.Y, -fViewPos.Z);
    }

    for (size_t i=0; i<fObjects.getSize(); i++)
        glCallList(fRenderListBase+i);
}

void QPlasmaRender::mouseMoveEvent(QMouseEvent* evt)
{
    if (fMouseState == 1) {
        if (fNavMode == kNavScene) {
            fViewPos.X += sinf(fRotZ * RADS) * (fMouseFrom.y() - evt->pos().y());
            fViewPos.Y += cosf(fRotZ * RADS) * (fMouseFrom.y() - evt->pos().y());
        }
        fRotZ += (evt->pos().x() - fMouseFrom.x());
        if (fNavMode == kNavModel || fNavMode == kNavModelInScene) {
            fRotX -= (fMouseFrom.y() - evt->pos().y());
            if (fRotX < -90.0f) fRotX = -90.0f;
            if (fRotX > 90.0f) fRotX = 90.0f;
        }
    } else if (fMouseState == 2) {
        fRotZ += (evt->pos().x() - fMouseFrom.x());
        if (fNavMode == kNavModel || fNavMode == kNavModelInScene) {
            fModelDist += (evt->pos().y() - fMouseFrom.y()) / 4.0f;
            if (fModelDist < 0.0f) fModelDist = 0.0f;
        } else {
            fRotX -= (fMouseFrom.y() - evt->pos().y());
            if (fRotX < -90.0f) fRotX = -90.0f;
            if (fRotX > 90.0f) fRotX = 90.0f;
        }
    } else if (fMouseState == 3) {
        if (fNavMode == kNavScene) {
            fViewPos.Z += (fMouseFrom.y() - evt->pos().y());
            fViewPos.X += sinf((fRotZ + 90.0f) * RADS) * (evt->pos().x() - fMouseFrom.x());
            fViewPos.Y += cosf((fRotZ + 90.0f) * RADS) * (evt->pos().x() - fMouseFrom.x());
        }
    }
    updateGL();
}

void QPlasmaRender::mousePressEvent(QMouseEvent* evt)
{
    if (fMouseState != 0)
        return;

    if ((evt->button() & Qt::LeftButton) != 0)
        fMouseState = 1;
    else if ((evt->button() & Qt::RightButton) != 0)
        fMouseState = 2;
    else if ((evt->button() & Qt::MidButton) != 0)
        fMouseState = 3;
    fMouseFrom = evt->pos();
}

void QPlasmaRender::mouseReleaseEvent(QMouseEvent* evt)
{
    if ((fMouseState == 1) && (evt->button() & Qt::LeftButton) == 0)
        fMouseState = 0;
    if ((fMouseState == 2) && (evt->button() & Qt::RightButton) == 0)
        fMouseState = 0;
    if ((fMouseState == 3) && (evt->button() & Qt::MidButton) == 0)
        fMouseState = 0;
}

void QPlasmaRender::addObject(plKey obj)
{
    fObjects.append(obj);
}

void QPlasmaRender::setView(const hsVector3& view, float angle)
{
    fViewPos = view;
    fRotZ = angle;
    fRotX = 0.0f;
    updateGL();
}

void QPlasmaRender::center(plKey obj, bool world)
{
    fCenterObj = obj;
    plSceneObject* sceneObj = plSceneObject::Convert(obj->getObj());
    if (sceneObj == NULL)
        return;

    bool mset = false;
    plDrawInterface* draw = GET_KEY_OBJECT(sceneObj->getDrawInterface(), plDrawInterface);
    plCoordinateInterface* coord = GET_KEY_OBJECT(sceneObj->getCoordInterface(), plCoordinateInterface);
    if (draw != NULL) {
        for (size_t i = 0; i < draw->getNumDrawables(); i++) {
            if (draw->getDrawableKey(i) == -1)
                continue;

            plDrawableSpans* span = plDrawableSpans::Convert(draw->getDrawable(i)->getObj());
            plDISpanIndex di = span->getDIIndex(draw->getDrawableKey(i));
            if ((di.fFlags & plDISpanIndex::kMatrixOnly) != 0)
                continue;

            for (size_t idx = 0; idx < di.fIndices.getSize(); idx++) {
                plIcicle* ice = (plIcicle*)span->getSpan(di.fIndices[idx]);
                hsBounds3Ext bounds = ice->getLocalBounds();

                if (!mset) {
                    fModelMins = bounds.getMins();
                    fModelMaxs = bounds.getMaxs();
                    mset = true;
                } else {
                    if (fModelMins.X > bounds.getMins().X)
                        fModelMins.X = bounds.getMins().X;
                    if (fModelMins.Y > bounds.getMins().Y)
                        fModelMins.Y = bounds.getMins().Y;
                    if (fModelMins.Z > bounds.getMins().Z)
                        fModelMins.Z = bounds.getMins().Z;
                    if (fModelMaxs.X < bounds.getMaxs().X)
                        fModelMaxs.X = bounds.getMaxs().X;
                    if (fModelMaxs.Y < bounds.getMaxs().Y)
                        fModelMaxs.Y = bounds.getMaxs().Y;
                    if (fModelMaxs.Z < bounds.getMaxs().Z)
                        fModelMaxs.Z = bounds.getMaxs().Z;
                }
            }
        }
    } else if (coord != NULL) {
        fModelMins = hsVector3(-0.5f, -0.5f, -0.5f);
        fModelMaxs = hsVector3( 0.5f,  0.5f,  0.5f);
    }

    if (world && coord != NULL) {
        fModelMins = fModelMins * coord->getLocalToWorld();
        fModelMaxs = fModelMaxs * coord->getLocalToWorld();
    }

    fViewPos = hsVector3((fModelMaxs.X + fModelMins.X) / 2.0f,
                         (fModelMaxs.Y + fModelMins.Y) / 2.0f,
                         (fModelMaxs.Z + fModelMins.Z) / 2.0f);
    float szX = (fModelMaxs.X - fModelMins.X),
          szY = (fModelMaxs.Y - fModelMins.Y),
          szZ = (fModelMaxs.Z - fModelMins.Z);
    fModelDist = sqrt((szX * szX) + (szY * szY) + (szZ * szZ));
}

void QPlasmaRender::build(int navMode, int drawMode)
{
    fNavMode = navMode;
    fDrawMode = drawMode;

    fLayers.clear();
    size_t layIdx = 0;
    for (size_t i=0; i<fObjects.getSize(); i++) {
        plSceneObject* obj = plSceneObject::Convert(fObjects[i]->getObj());
        plDrawInterface* draw = GET_KEY_OBJECT(obj->getDrawInterface(), plDrawInterface);
        if (draw == NULL)
            continue;

        for (size_t j=0; j<draw->getNumDrawables(); j++) {
            if (draw->getDrawableKey(j) == -1)
                continue;
            plDrawableSpans* span = plDrawableSpans::Convert(draw->getDrawable(j)->getObj());
            plDISpanIndex di = span->getDIIndex(draw->getDrawableKey(j));
            if ((di.fFlags & plDISpanIndex::kMatrixOnly) != 0)
                continue;
            for (size_t idx=0; idx<di.fIndices.getSize(); idx++) {
                plIcicle* ice = (plIcicle*)span->getSpan(di.fIndices[idx]);
                hsGMaterial* mat = hsGMaterial::Convert(span->getMaterial(ice->getMaterialIdx())->getObj());
                for (size_t lay = 0; lay < mat->getNumLayers(); lay++) {
                    if (fLayers.find(mat->getLayer(lay)) == fLayers.end()) {
                        fLayers[mat->getLayer(lay)] = LayerInfo(layIdx, 0);
                        layIdx++;
                    }
                }
            }
        }
    }

    if (fTexList != NULL)
        delete[] fTexList;
    fTexList = new GLuint[layIdx];
    glGenTextures(layIdx, fTexList);
    std::map<plKey, LayerInfo>::iterator it;
    for (it = fLayers.begin(); it != fLayers.end(); it++)
        compileTexture((*it).first, (*it).second.fTexNameId);

    fRenderListBase = glGenLists(fObjects.getSize());
    for (size_t i=0; i<fObjects.getSize(); i++) {
        glNewList(fRenderListBase+i, GL_COMPILE);
        compileObject(fObjects[i]);
        glEndList();
    }
}

void QPlasmaRender::rebuild()
{
    for (size_t i=0; i<fObjects.getSize(); i++) {
        glNewList(fRenderListBase+i, GL_COMPILE);
        compileObject(fObjects[i]);
        glEndList();
    }
}

void QPlasmaRender::rebuildObject(plKey obj)
{
    for (size_t i=0; i<fObjects.getSize(); i++) {
        if (fObjects[i] == obj) {
            glNewList(fRenderListBase+i, GL_COMPILE);
            compileObject(obj);
            glEndList();
        }
    }
}

bool QPlasmaRender::buildMipmap(plMipmap* map, GLuint id, GLuint target) {
    glBindTexture(target, id);
    //glTexParameteri(target, GL_GENERATE_MIPMAP, GL_FALSE);
    glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (map->getCompressionType() == plBitmap::kDirectXCompression) {
        GLuint dxCompression = 0;
        if (map->getDXCompression() == plBitmap::kDXT1)
            dxCompression = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        else if (map->getDXCompression() == plBitmap::kDXT3)
            dxCompression = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        else if (map->getDXCompression() == plBitmap::kDXT5)
            dxCompression = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

        if (glCompressedTexImage2DARB != NULL) {
            for (size_t i=0; i<map->getNumLevels(); i++) {
                glCompressedTexImage2DARB(target, i, dxCompression, map->getLevelWidth(i),
                                          map->getLevelHeight(i), 0, map->getLevelSize(i),
                                          map->getLevelData(i));
            }
            //glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, map->getNumLevels()-1);
        } else {
            return false;
        }
    } else if (map->getCompressionType() == plBitmap::kJPEGCompression) {
        size_t imgsize = map->getImageSize();
        unsigned char* imgbuf = new unsigned char[imgsize];
        try {
            map->DecompressImage(0, imgbuf, imgsize);
            glTexImage2D(target, 0, GL_RGBA, map->getLevelWidth(0),
                         map->getLevelHeight(0), 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, imgbuf);
        } catch (hsException& e) {
            QMessageBox msgBox(QMessageBox::Critical, tr("Error"),
                               tr("Error decompressing %1: %2")
                               .arg(~map->getKey()->getName()).arg(e.what()),
                               QMessageBox::Ok, this);
            msgBox.exec();
            delete[] imgbuf;
            return false;
        }
        delete[] imgbuf;
    } else {
        for (size_t i=0; i<map->getNumLevels(); i++) {
            glTexImage2D(target, i, GL_RGBA, map->getLevelWidth(i),
                         map->getLevelHeight(i), 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, map->getLevelData(i));
        }
        //glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, map->getNumLevels()-1);
    }
    return true;
}

void QPlasmaRender::compileTexture(plKey lay, size_t id)
{
    plLayerInterface* layer = plLayerInterface::Convert(lay->getObj());

    plLayerInterface* lp = layer;
    plKey layTex;
    while (lp != NULL) {
        if (lp->getTexture().Exists()) {
            layTex = lp->getTexture();
            lp = NULL;
        } else {
            lp = GET_KEY_OBJECT(lp->getUnderLay(), plLayerInterface);
        }
    }

    if (plMipmap* map = GET_KEY_OBJECT(layTex, plMipmap)) {
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_CUBE_MAP);
        fLayers[lay].fTexTarget = GL_TEXTURE_2D;
        if (!buildMipmap(map, fTexList[id], GL_TEXTURE_2D))
            fLayers[lay].fTexNameId = 0;
    } else if (plCubicEnvironmap* envMap = GET_KEY_OBJECT(layTex, plCubicEnvironmap)) {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_CUBE_MAP);
        fLayers[lay].fTexTarget = GL_TEXTURE_CUBE_MAP;
        if (!buildMipmap(envMap->getFace(0), fTexList[id], GL_TEXTURE_CUBE_MAP_POSITIVE_X))
            fLayers[lay].fTexNameId = 0;
        if (!buildMipmap(envMap->getFace(1), fTexList[id], GL_TEXTURE_CUBE_MAP_NEGATIVE_X))
            fLayers[lay].fTexNameId = 0;
        if (!buildMipmap(envMap->getFace(2), fTexList[id], GL_TEXTURE_CUBE_MAP_POSITIVE_Y))
            fLayers[lay].fTexNameId = 0;
        if (!buildMipmap(envMap->getFace(3), fTexList[id], GL_TEXTURE_CUBE_MAP_NEGATIVE_Y))
            fLayers[lay].fTexNameId = 0;
        if (!buildMipmap(envMap->getFace(4), fTexList[id], GL_TEXTURE_CUBE_MAP_POSITIVE_Z))
            fLayers[lay].fTexNameId = 0;
        if (!buildMipmap(envMap->getFace(5), fTexList[id], GL_TEXTURE_CUBE_MAP_NEGATIVE_Z))
            fLayers[lay].fTexNameId = 0;
    } else if (layTex.Exists()) {
        plDebug::Debug("Got unrecognized texture type for %s",
                       layTex->toString().cstr());
    }
}

void drawBounds(const hsVector3& mins, const hsVector3& maxs)
{
    glBegin(GL_LINES);
    glVertex3f(mins.X, mins.Y, mins.Z);
    glVertex3f(maxs.X, mins.Y, mins.Z);
    glVertex3f(mins.X, mins.Y, mins.Z);
    glVertex3f(mins.X, maxs.Y, mins.Z);
    glVertex3f(mins.X, mins.Y, mins.Z);
    glVertex3f(mins.X, mins.Y, maxs.Z);

    glVertex3f(maxs.X, maxs.Y, mins.Z);
    glVertex3f(mins.X, maxs.Y, mins.Z);
    glVertex3f(maxs.X, maxs.Y, mins.Z);
    glVertex3f(maxs.X, mins.Y, mins.Z);
    glVertex3f(maxs.X, maxs.Y, mins.Z);
    glVertex3f(maxs.X, maxs.Y, maxs.Z);

    glVertex3f(mins.X, maxs.Y, maxs.Z);
    glVertex3f(mins.X, mins.Y, maxs.Z);
    glVertex3f(mins.X, maxs.Y, maxs.Z);
    glVertex3f(maxs.X, maxs.Y, maxs.Z);
    glVertex3f(mins.X, maxs.Y, maxs.Z);
    glVertex3f(mins.X, maxs.Y, mins.Z);

    glVertex3f(maxs.X, mins.Y, maxs.Z);
    glVertex3f(mins.X, mins.Y, maxs.Z);
    glVertex3f(maxs.X, mins.Y, maxs.Z);
    glVertex3f(maxs.X, maxs.Y, maxs.Z);
    glVertex3f(maxs.X, mins.Y, maxs.Z);
    glVertex3f(maxs.X, mins.Y, mins.Z);
    glEnd();
}

void drawBoundCube(const hsVector3& mins, const hsVector3& maxs)
{
    glBegin(GL_QUADS);
    glVertex3f(mins.X, mins.Y, mins.Z);
    glVertex3f(maxs.X, mins.Y, mins.Z);
    glVertex3f(maxs.X, maxs.Y, mins.Z);
    glVertex3f(mins.X, maxs.Y, mins.Z);

    glVertex3f(mins.X, mins.Y, maxs.Z);
    glVertex3f(maxs.X, mins.Y, maxs.Z);
    glVertex3f(maxs.X, maxs.Y, maxs.Z);
    glVertex3f(mins.X, maxs.Y, maxs.Z);

    glVertex3f(mins.X, mins.Y, mins.Z);
    glVertex3f(maxs.X, mins.Y, mins.Z);
    glVertex3f(maxs.X, mins.Y, maxs.Z);
    glVertex3f(mins.X, mins.Y, maxs.Z);

    glVertex3f(mins.X, maxs.Y, mins.Z);
    glVertex3f(maxs.X, maxs.Y, mins.Z);
    glVertex3f(maxs.X, maxs.Y, maxs.Z);
    glVertex3f(mins.X, maxs.Y, maxs.Z);

    glVertex3f(mins.X, mins.Y, mins.Z);
    glVertex3f(mins.X, maxs.Y, mins.Z);
    glVertex3f(mins.X, maxs.Y, maxs.Z);
    glVertex3f(mins.X, mins.Y, maxs.Z);

    glVertex3f(maxs.X, mins.Y, mins.Z);
    glVertex3f(maxs.X, maxs.Y, mins.Z);
    glVertex3f(maxs.X, maxs.Y, maxs.Z);
    glVertex3f(maxs.X, mins.Y, maxs.Z);
    glEnd();
}

void QPlasmaRender::compileObject(plKey key)
{
    plSceneObject* obj = plSceneObject::Convert(key->getObj());
    if (obj == NULL)
        return;

    plDrawInterface* draw = GET_KEY_OBJECT(obj->getDrawInterface(), plDrawInterface);
    plCoordinateInterface* coord = GET_KEY_OBJECT(obj->getCoordInterface(), plCoordinateInterface);

    bool isFocused = (key == fCenterObj);   // Performance
    if (isFocused) {
        if ((draw != NULL && fNavMode == kNavModel) || fNavMode == kNavModelInScene) {
            glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
            drawBounds(fModelMins, fModelMaxs);
        }
        if (draw == NULL && coord != NULL && fNavMode == kNavModelInScene) {
            glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
            glDisable(GL_CULL_FACE);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_TEXTURE_CUBE_MAP);
            glDisable(GL_BLEND);
            drawBoundCube(fModelMins, fModelMaxs);
        }
    }

    if (draw == NULL)
        // The rest is Drawable stuff
        return;

    for (size_t i = 0; i < draw->getNumDrawables(); i++) {
        if (draw->getDrawableKey(i) == -1)
            continue;

        plDrawableSpans* span = plDrawableSpans::Convert(draw->getDrawable(i)->getObj());
        plDISpanIndex di = span->getDIIndex(draw->getDrawableKey(i));
        if ((di.fFlags & plDISpanIndex::kMatrixOnly) != 0)
            continue;

        for (size_t idx = 0; idx < di.fIndices.getSize(); idx++) {
            plIcicle* ice = (plIcicle*)span->getSpan(di.fIndices[idx]);
            hsTArray<plGBufferVertex> verts = span->getVerts(ice);
            hsTArray<unsigned short> indices = span->getIndices(ice);

            hsMatrix44 xform;
            if (fNavMode == kNavScene || fNavMode == kNavModelInScene) {
                if (coord != NULL)
                    xform = coord->getLocalToWorld();
                else
                    xform = ice->getLocalToWorld();
            }

            hsGMaterial* mat = hsGMaterial::Convert(span->getMaterial(ice->getMaterialIdx())->getObj());
            for (size_t lay = 0; lay < mat->getNumLayers(); lay++) {
            //size_t lay = 0 ; {
                plLayerInterface* layer = plLayerInterface::Convert(mat->getLayer(lay)->getObj());

                bool is2Sided = ((fDrawMode & kDrawForce2Sided) != 0)
                             || ((layer->getState().fMiscFlags & hsGMatState::kMiscTwoSided) != 0)
                             || ((mat->getCompFlags() & hsGMaterial::kCompTwoSided) != 0);
                float amb[4] = { layer->getAmbient().r, layer->getAmbient().g,
                                 layer->getAmbient().b, layer->getAmbient().b };
                float dif[4] = { layer->getRuntime().r, layer->getRuntime().g,
                                 layer->getRuntime().b, layer->getRuntime().b };
                float spec[4] = { layer->getSpecular().r, layer->getSpecular().g,
                                  layer->getSpecular().b, layer->getSpecular().b };
                glMaterialfv(is2Sided ? GL_FRONT : GL_FRONT_AND_BACK, GL_AMBIENT, amb);
                glMaterialfv(is2Sided ? GL_FRONT : GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
                glMaterialfv(is2Sided ? GL_FRONT : GL_FRONT_AND_BACK, GL_SPECULAR, spec);
                if (layer->getState().fShadeFlags & hsGMatState::kShadeEmissive)
                    glMaterialfv(is2Sided ? GL_FRONT : GL_FRONT_AND_BACK, GL_EMISSION, amb);
                glMaterialf(is2Sided ? GL_FRONT : GL_FRONT_AND_BACK, GL_SHININESS, layer->getSpecularPower());

                if (is2Sided) {
                    glDisable(GL_CULL_FACE);
                } else {
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                }

                glDisable(GL_BLEND);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
                if ((layer->getState().fBlendFlags & hsGMatState::kBlendAlpha) != 0) {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                }
                if ((layer->getState().fBlendFlags & hsGMatState::kBlendAdd) != 0) {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ONE, GL_ONE);
                    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                }
                if ((layer->getState().fBlendFlags & hsGMatState::kBlendNoTexColor) != 0) {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
                }

                /*
                if (isFocused) {
                    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                    glColor4f(1.0f, 0.3f, 1.0f, 1.0f);
                } else {
                    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                }*/

                LayerInfo linf = fLayers[layer->getKey()];
                if (linf.fTexTarget != GL_TEXTURE_2D)
                    glDisable(GL_TEXTURE_2D);
                if (linf.fTexTarget != GL_TEXTURE_CUBE_MAP)
                    glDisable(GL_TEXTURE_CUBE_MAP);
                if ((fDrawMode & kDrawModeMask) == kDrawTextured) {
                    glEnable(linf.fTexTarget);
                    glBindTexture(linf.fTexTarget, fTexList[linf.fTexNameId]);
                }

                if ((fDrawMode & kDrawModeMask) == kDrawPoints) {
                    glBegin(GL_POINTS);
                    for (size_t j = 0; j < verts.getSize(); j++) {
                        hsColor32 color(verts[j].fColor);
                        glColor4ub(color.r, color.g, color.b, color.a);

                        hsVector3 vert;
                        if (!xform.IsIdentity())
                            vert = verts[j].fPos * xform;
                        else
                            vert = verts[j].fPos;
                        glVertex3f(vert.X, vert.Y, vert.Z);
                    }
                    glEnd();
                } else if ((fDrawMode & kDrawModeMask) == kDrawWire) {
                    glBegin(GL_LINES);
                    for (size_t j = 0; j < indices.getSize(); j+=3) {
                        hsColor32 color[3] = { verts[indices[j+0]].fColor,
                                               verts[indices[j+1]].fColor,
                                               verts[indices[j+2]].fColor };
                        hsVector3 vert[3];
                        if (!xform.IsIdentity()) {
                            vert[0] = verts[indices[j+0]].fPos * xform;
                            vert[1] = verts[indices[j+1]].fPos * xform;
                            vert[2] = verts[indices[j+2]].fPos * xform;
                        } else {
                            vert[0] = verts[indices[j+0]].fPos;
                            vert[1] = verts[indices[j+1]].fPos;
                            vert[2] = verts[indices[j+2]].fPos;
                        }

                        glColor4ub(color[0].r, color[0].g, color[0].b, color[0].a);
                        glVertex3f(vert[0].X, vert[0].Y, vert[0].Z);
                        glColor4ub(color[1].r, color[1].g, color[1].b, color[1].a);
                        glVertex3f(vert[1].X, vert[1].Y, vert[1].Z);

                        glColor4ub(color[1].r, color[1].g, color[1].b, color[1].a);
                        glVertex3f(vert[1].X, vert[1].Y, vert[1].Z);
                        glColor4ub(color[2].r, color[2].g, color[2].b, color[2].a);
                        glVertex3f(vert[2].X, vert[2].Y, vert[2].Z);

                        glColor4ub(color[2].r, color[2].g, color[2].b, color[2].a);
                        glVertex3f(vert[2].X, vert[2].Y, vert[2].Z);
                        glColor4ub(color[0].r, color[0].g, color[0].b, color[0].a);
                        glVertex3f(vert[0].X, vert[0].Y, vert[0].Z);
                    }
                    glEnd();
                } else if ((fDrawMode & kDrawModeMask) == kDrawFlat) {
                    glBegin(GL_TRIANGLES);
                    for (size_t j = 0; j < indices.getSize(); j++) {
                        hsColor32 color(verts[indices[j]].fColor);
                        glColor4ub(color.r, color.g, color.b, color.a);

                        hsVector3 vert;
                        if (!xform.IsIdentity())
                            vert = verts[indices[j]].fPos * xform;
                        else
                            vert = verts[indices[j]].fPos;
                        glVertex3f(vert.X, vert.Y, vert.Z);
                    }
                    glEnd();
                } else if ((fDrawMode & kDrawModeMask) == kDrawTextured) {
                    glBegin(GL_TRIANGLES);
                    for (size_t j = 0; j < indices.getSize(); j++) {
                        hsColor32 color(verts[indices[j]].fColor);

                        size_t uvwSrc = layer->getUVWSrc() & 0xFFFF;
                        hsVector3 uvw = verts[indices[j]].fUVWs[uvwSrc] * layer->getTransform();
                        glTexCoord3f(uvw.X, uvw.Y, uvw.Z);

                        glColor4ub(color.r, color.g, color.b, color.a);
                        glNormal3f(verts[indices[j]].fNormal.X, verts[indices[j]].fNormal.Y, verts[indices[j]].fNormal.Z);

                        hsVector3 vert;
                        if (!xform.IsIdentity())
                            vert = verts[indices[j]].fPos * xform;
                        else
                            vert = verts[indices[j]].fPos;
                        glVertex3f(vert.X, vert.Y, vert.Z);
                    }
                    glEnd();
                }
                glDisable(GL_BLEND);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            }
        }
    }

    /*
    if (fMode == MODE_MODEL_IN_SCENE && isFocused) {
        glColor4f(1.0f, 0.0f, 1.0f, 0.3f);
        glDisable(GL_CULL_FACE);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_CUBE_MAP);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        DrawBoundCube(fModelMins, fModelMaxs);
        glEnable(GL_DEPTH_TEST);
    }*/
}
