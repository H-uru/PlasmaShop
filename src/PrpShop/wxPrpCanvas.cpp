#include "wxPrpCanvas.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#ifdef WIN32
  #include <GL/wglext.h>
#else
  #include <GL/glxext.h>
#endif

#include <Debug/plDebug.h>
#include <PRP/Object/plDrawInterface.h>
#include <PRP/Object/plCoordinateInterface.h>
#include <PRP/Surface/hsGMaterial.h>
#include <PRP/Surface/plLayerAnimation.h>
#include <PRP/Surface/plMipmap.h>
#include <PRP/Surface/plCubicEnvironmap.h>

PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB = NULL;

const float RADS = 0.0174532925f;


LayerInfo::LayerInfo()
         : fTexNameId(0), fTexTarget(0)
{ }

LayerInfo::LayerInfo(size_t nameId, GLuint target)
         : fTexNameId(nameId), fTexTarget(target)
{ }


BEGIN_EVENT_TABLE(wxPrpCanvas, wxGLCanvas)
    EVT_SIZE(wxPrpCanvas::OnSize)
    EVT_PAINT(wxPrpCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(wxPrpCanvas::OnEraseBackground)
    EVT_MOUSE_EVENTS(wxPrpCanvas::OnMouse)
    //EVT_MOUSEWHEEL(wxPrpCanvas::OnMouseWheel)
END_EVENT_TABLE()

wxPrpCanvas::wxPrpCanvas(wxWindow* parent, wxWindowID id,
                         const wxPoint& pos, const wxSize& size,
                         long style, const wxString& name)
           : wxGLCanvas(parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE,
                        name, NULL),
             fInited(false), fRenderListBase(0), fTexList(NULL),
             fRotZ(0.0f), fRotX(0.0f), fModelDist(0.0f), fMouseBtn(0)
{ }

wxPrpCanvas::~wxPrpCanvas()
{
    glDeleteLists(fRenderListBase, fObjects.getSize());
    if (fTexList != NULL) {
        glDeleteTextures(fLayers.size(), fTexList);
        delete[] fTexList;
    }
}

void wxPrpCanvas::InitGL()
{
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

#ifdef WIN32
    glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)wglGetProcAddress("glCompressedTexImage2DARB");
#else
    glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)glXGetProcAddress((const GLubyte*)"glCompressedTexImage2DARB");
#endif

    fInited = true;
}

void wxPrpCanvas::OnPaint(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    Render();
}

void wxPrpCanvas::OnSize(wxSizeEvent& evt)
{
    wxGLCanvas::OnSize(evt);

    int width, height;
    GetClientSize(&width, &height);

    SetCurrent();
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width/(float)height, 0.1f, 10000.0f);
}

void wxPrpCanvas::OnEraseBackground(wxEraseEvent& evt)
{ }

void wxPrpCanvas::OnMouse(wxMouseEvent& evt)
{
    if (evt.ButtonDown(wxMOUSE_BTN_LEFT) && fMouseBtn == 0) {
        fMouseBtn = 1;
    } else if (evt.ButtonUp(wxMOUSE_BTN_LEFT) && fMouseBtn == 1) {
        fMouseBtn = 0;
    } else if (evt.ButtonDown(wxMOUSE_BTN_RIGHT) && fMouseBtn == 0) {
        fMouseBtn = 2;
    } else if (evt.ButtonUp(wxMOUSE_BTN_RIGHT) && fMouseBtn == 2) {
        fMouseBtn = 0;
    } else if (evt.ButtonDown(wxMOUSE_BTN_MIDDLE) && fMouseBtn == 0) {
        fMouseBtn = 3;
    } else if (evt.ButtonUp(wxMOUSE_BTN_MIDDLE) && fMouseBtn == 3) {
        fMouseBtn = 0;
    } else if (evt.Dragging()) {
        if (fMouseBtn == 1) {
            if (fMode == MODE_SCENE) {
                fViewPos.X += sinf(fRotZ * RADS) * (fMouseFrom.y - evt.GetPosition().y);
                fViewPos.Y += cosf(fRotZ * RADS) * (fMouseFrom.y - evt.GetPosition().y);
            }
            fRotZ += (evt.GetPosition().x - fMouseFrom.x);
            if (fMode == MODE_MODEL) {
                fRotX -= (fMouseFrom.y - evt.GetPosition().y);
                if (fRotX < -90.0f) fRotX = -90.0f;
                if (fRotX > 90.0f) fRotX = 90.0f;
            }
        } else if (fMouseBtn == 2) {
            fRotZ += (evt.GetPosition().x - fMouseFrom.x);
            if (fMode == MODE_MODEL) {
                fModelDist += (evt.GetPosition().y - fMouseFrom.y) / 4.0f;
                if (fModelDist < 0.0f) fModelDist = 0.0f;
            } else {
                fRotX -= (fMouseFrom.y - evt.GetPosition().y);
                if (fRotX < -90.0f) fRotX = -90.0f;
                if (fRotX > 90.0f) fRotX = 90.0f;
            }
        } else if (fMouseBtn == 3) {
            if (fMode == MODE_SCENE) {
                fViewPos.Z += (fMouseFrom.y - evt.GetPosition().y);
                fViewPos.X += sinf((fRotZ + 90.0f) * RADS) * (evt.GetPosition().x - fMouseFrom.x);
                fViewPos.Y += cosf((fRotZ + 90.0f) * RADS) * (evt.GetPosition().x - fMouseFrom.x);
            }
        }
    } else {
        evt.Skip();
        return;
    }
    fMouseFrom = evt.GetPosition();
    Render();
}

void wxPrpCanvas::Render()
{
    SetCurrent();

    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    if (fMode == MODE_MODEL) {
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

    SwapBuffers();
}

void wxPrpCanvas::AddObject(plKey obj)
{
    fObjects.append(obj);
}

void wxPrpCanvas::SetView(hsVector3 view, float angle)
{
    fViewPos = view;
    fRotZ = angle;
    fRotX = 0.0f;
    Render();
}

void wxPrpCanvas::Center(plKey obj)
{
    plSceneObject* sceneObj = plSceneObject::Convert(obj->getObj());
    if (sceneObj == NULL || !sceneObj->getDrawInterface().Exists())
        return;

    bool mset = false;
    plDrawInterface* draw = plDrawInterface::Convert(sceneObj->getDrawInterface()->getObj());
    for (size_t i = 0; i < draw->getNumDrawables(); i++) {
        if (draw->getDrawableKey(i) == -1)
            continue;

        plDrawableSpans* span = plDrawableSpans::Convert(draw->getDrawable(i)->getObj());
        plDISpanIndex di = span->getDIIndex(draw->getDrawableKey(i));
        for (size_t idx = 0; idx < di.fIndices.getSize(); idx++) {
            plIcicle* ice = (plIcicle*)span->getSpan(di.fIndices[idx]);
            if (!mset) {
                fModelMins = ice->getLocalBounds().getMins();
                fModelMaxs = ice->getLocalBounds().getMaxs();
                mset = true;
            } else {
                if (fModelMins.X > ice->getLocalBounds().getMins().X)
                    fModelMins.X = ice->getLocalBounds().getMins().X;
                if (fModelMins.Y > ice->getLocalBounds().getMins().Y)
                    fModelMins.Y = ice->getLocalBounds().getMins().Y;
                if (fModelMins.Z > ice->getLocalBounds().getMins().Z)
                    fModelMins.Z = ice->getLocalBounds().getMins().Z;
                if (fModelMaxs.X < ice->getLocalBounds().getMaxs().X)
                    fModelMaxs.X = ice->getLocalBounds().getMaxs().X;
                if (fModelMaxs.Y < ice->getLocalBounds().getMaxs().Y)
                    fModelMaxs.Y = ice->getLocalBounds().getMaxs().Y;
                if (fModelMaxs.Z < ice->getLocalBounds().getMaxs().Z)
                    fModelMaxs.Z = ice->getLocalBounds().getMaxs().Z;
            }
        }
    }
    fViewPos = hsVector3((fModelMaxs.X + fModelMins.X) / 2.0f,
                         (fModelMaxs.Y + fModelMins.Y) / 2.0f,
                         (fModelMaxs.Z + fModelMins.Z) / 2.0f);
    float szX = (fModelMaxs.X - fModelMins.X),
          szY = (fModelMaxs.Y - fModelMins.Y),
          szZ = (fModelMaxs.Z - fModelMins.Z);
    fModelDist = sqrt((szX * szX) + (szY * szY) + (szZ * szZ));
}

void wxPrpCanvas::Build(int mode)
{
    SetCurrent();
    if (!fInited)
        InitGL();
    fMode = mode;

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

    assert(fLayers.size() == layIdx);
    if (fTexList != NULL)
        delete[] fTexList;
    fTexList = new GLuint[layIdx];
    glGenTextures(layIdx, fTexList);
    std::map<plKey, LayerInfo>::iterator it;
    for (it = fLayers.begin(); it != fLayers.end(); it++)
        CompileTexture((*it).first, (*it).second.fTexNameId);
    
    fRenderListBase = glGenLists(fObjects.getSize());
    for (size_t i=0; i<fObjects.getSize(); i++) {
        glNewList(fRenderListBase+i, GL_COMPILE);
        CompileObject(fObjects[i]);
        glEndList();
    }
}

void wxPrpCanvas::CompileObject(plKey key)
{
    plSceneObject* obj = plSceneObject::Convert(key->getObj());
    if (obj == NULL || !obj->getDrawInterface().Exists())
        return;

    plDrawInterface* draw = plDrawInterface::Convert(obj->getDrawInterface()->getObj());
    plCoordinateInterface* coord = NULL;
    if (obj->getCoordInterface().Exists())
        coord = plCoordinateInterface::Convert(obj->getCoordInterface()->getObj());

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
            if (fMode == MODE_SCENE) {
                if (coord != NULL)
                    xform = coord->getLocalToWorld();
                else
                    xform = ice->getLocalToWorld();
            }

            if (fMode == MODE_MODEL) {
                glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

                glBegin(GL_LINES);
                glVertex3f(fModelMins.X, fModelMins.Y, fModelMins.Z);
                glVertex3f(fModelMaxs.X, fModelMins.Y, fModelMins.Z);
                glVertex3f(fModelMins.X, fModelMins.Y, fModelMins.Z);
                glVertex3f(fModelMins.X, fModelMaxs.Y, fModelMins.Z);
                glVertex3f(fModelMins.X, fModelMins.Y, fModelMins.Z);
                glVertex3f(fModelMins.X, fModelMins.Y, fModelMaxs.Z);

                glVertex3f(fModelMaxs.X, fModelMaxs.Y, fModelMins.Z);
                glVertex3f(fModelMins.X, fModelMaxs.Y, fModelMins.Z);
                glVertex3f(fModelMaxs.X, fModelMaxs.Y, fModelMins.Z);
                glVertex3f(fModelMaxs.X, fModelMins.Y, fModelMins.Z);
                glVertex3f(fModelMaxs.X, fModelMaxs.Y, fModelMins.Z);
                glVertex3f(fModelMaxs.X, fModelMaxs.Y, fModelMaxs.Z);
                
                glVertex3f(fModelMins.X, fModelMaxs.Y, fModelMaxs.Z);
                glVertex3f(fModelMins.X, fModelMins.Y, fModelMaxs.Z);
                glVertex3f(fModelMins.X, fModelMaxs.Y, fModelMaxs.Z);
                glVertex3f(fModelMaxs.X, fModelMaxs.Y, fModelMaxs.Z);
                glVertex3f(fModelMins.X, fModelMaxs.Y, fModelMaxs.Z);
                glVertex3f(fModelMins.X, fModelMaxs.Y, fModelMins.Z);
                
                glVertex3f(fModelMaxs.X, fModelMins.Y, fModelMaxs.Z);
                glVertex3f(fModelMins.X, fModelMins.Y, fModelMaxs.Z);
                glVertex3f(fModelMaxs.X, fModelMins.Y, fModelMaxs.Z);
                glVertex3f(fModelMaxs.X, fModelMaxs.Y, fModelMaxs.Z);
                glVertex3f(fModelMaxs.X, fModelMins.Y, fModelMaxs.Z);
                glVertex3f(fModelMaxs.X, fModelMins.Y, fModelMins.Z);
                glEnd();
            }

            hsGMaterial* mat = hsGMaterial::Convert(span->getMaterial(ice->getMaterialIdx())->getObj());
            for (size_t lay = 0; lay < mat->getNumLayers(); lay++) {
            //size_t lay = 0 ; {
                plLayerInterface* layer = plLayerInterface::Convert(mat->getLayer(lay)->getObj());

                bool is2Sided = ((layer->getState().fMiscFlags & hsGMatState::kMiscTwoSided) != 0)
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

                LayerInfo linf = fLayers[layer->getKey()];
                if (linf.fTexTarget != GL_TEXTURE_2D)
                    glDisable(GL_TEXTURE_2D);
                if (linf.fTexTarget != GL_TEXTURE_CUBE_MAP)
                    glDisable(GL_TEXTURE_CUBE_MAP);
                glEnable(linf.fTexTarget);
                glBindTexture(linf.fTexTarget, fTexList[linf.fTexNameId]);

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
                glDisable(GL_BLEND);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            }
        }
    }
}

bool BuildMipmap(plMipmap* map, GLuint id, GLuint target) {
    glBindTexture(target, id);
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
        } else {
            return false;
        }
    } else if (map->getCompressionType() == plBitmap::kJPEGCompression) {
        size_t imgsize = map->GetUncompressedSize();
        unsigned char* imgbuf = new unsigned char[imgsize];
        try {
            map->DecompressImage(imgbuf, imgsize);
            glTexImage2D(target, 0, GL_RGBA, map->getLevelWidth(0),
                         map->getLevelHeight(0), 0, GL_RGBA,
                         GL_UNSIGNED_INT_8_8_8_8, imgbuf);
        } catch (hsException& e) {
            wxMessageBox(wxT("Error decompressing ") + wxString(map->getKey()->getName().cstr(), wxConvUTF8) +
                         wxT(": ") + wxString(e.what(), wxConvUTF8));
            delete[] imgbuf;
            return false;
        }
        delete[] imgbuf;
    } else {
        for (size_t i=0; i<map->getNumLevels(); i++) {
            glTexImage2D(target, i, GL_RGBA, map->getLevelWidth(i),
                         map->getLevelHeight(i), 0, GL_RGBA,
                         GL_UNSIGNED_INT_8_8_8_8, map->getLevelData(i));
        }
    }
    return true;
}

void wxPrpCanvas::CompileTexture(plKey lay, size_t id)
{
    plLayerInterface* layer = plLayerInterface::Convert(lay->getObj());
    glPixelTransferf(GL_ALPHA_SCALE, layer->getOpacity());

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
        if (!BuildMipmap(map, fTexList[id], GL_TEXTURE_2D))
            fLayers[lay].fTexNameId = 0;
    } else if (plCubicEnvironmap* envMap = GET_KEY_OBJECT(layTex, plCubicEnvironmap)) {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_CUBE_MAP);
        fLayers[lay].fTexTarget = GL_TEXTURE_CUBE_MAP;
        if (!BuildMipmap(envMap->getFace(0), fTexList[id], GL_TEXTURE_CUBE_MAP_POSITIVE_X))
            fLayers[lay].fTexNameId = 0;
        if (!BuildMipmap(envMap->getFace(1), fTexList[id], GL_TEXTURE_CUBE_MAP_NEGATIVE_X))
            fLayers[lay].fTexNameId = 0;
        if (!BuildMipmap(envMap->getFace(2), fTexList[id], GL_TEXTURE_CUBE_MAP_POSITIVE_Y))
            fLayers[lay].fTexNameId = 0;
        if (!BuildMipmap(envMap->getFace(3), fTexList[id], GL_TEXTURE_CUBE_MAP_NEGATIVE_Y))
            fLayers[lay].fTexNameId = 0;
        if (!BuildMipmap(envMap->getFace(4), fTexList[id], GL_TEXTURE_CUBE_MAP_POSITIVE_Z))
            fLayers[lay].fTexNameId = 0;
        if (!BuildMipmap(envMap->getFace(5), fTexList[id], GL_TEXTURE_CUBE_MAP_NEGATIVE_Z))
            fLayers[lay].fTexNameId = 0;
    } else if (layTex.Exists()) {
        plDebug::Debug("Got unrecognized texture type for %s",
                       layTex->toString().cstr());
    }
}
