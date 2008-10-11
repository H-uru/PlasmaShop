#include "wxPrpCanvas.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <Debug/plDebug.h>
#include <PRP/Object/plDrawInterface.h>
#include <PRP/Object/plCoordinateInterface.h>

const float RADS = 0.0174532925f;

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
           : wxGLCanvas(parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name),
             fInited(false), fRotZ(0.0f), fRotX(0.0f), fModelDist(0.0f),
             fMouseBtn(0)
{ }

wxPrpCanvas::~wxPrpCanvas()
{
    glDeleteLists(fList, fObjects.getSize());
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
                fViewPos.X += cosf(fRotZ * RADS) * (evt.GetPosition().x - fMouseFrom.x);
                fViewPos.Y += sinf(fRotZ * RADS) * (evt.GetPosition().x - fMouseFrom.x);
            }
        }
    } else {
        evt.Skip();
        return;
    }
    fMouseFrom = evt.GetPosition();
    Render();
}

void wxPrpCanvas::InitGL()
{
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    fInited = true;
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
    
    //*
    for (size_t i=0; i<fObjects.getSize(); i++)
        glCallList(fList+i);
    /*/
    for (size_t i=0; i<fObjects.getSize(); i++)
        CompileObject(fObjects[i]);
    //*/

    SwapBuffers();
}

void wxPrpCanvas::AddObject(plKey obj)
{
    fObjects.append(obj);
}

void wxPrpCanvas::ClearObjects()
{
    fObjects.clear();
}

void wxPrpCanvas::Build(int mode)
{
    SetCurrent();
    if (!fInited)
        InitGL();

    fMode = mode;
    fList = glGenLists(fObjects.getSize());
    
    for (size_t i=0; i<fObjects.getSize(); i++) {
        glNewList(fList+i, GL_COMPILE);
        CompileObject(fObjects[i]);
        glEndList();
    }
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
    if (!sceneObj->getDrawInterface().isLoaded()) {
        plDebug::Warning("Could not get DrawInterface for %s", sceneObj->getKey()->getName().cstr());
        return;
    }

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

void wxPrpCanvas::CompileObject(plKey key)
{
    plSceneObject* obj = plSceneObject::Convert(key->getObj());
    if (obj == NULL || !obj->getDrawInterface().Exists())
        return;
    if (!obj->getDrawInterface().isLoaded()) {
        plDebug::Warning("Could not get DrawInterface for %s", obj->getKey()->getName().cstr());
        return;
    }

    plDrawInterface* draw = plDrawInterface::Convert(obj->getDrawInterface()->getObj());
    plCoordinateInterface* coord = NULL;
    if (obj->getCoordInterface().Exists())
        coord = plCoordinateInterface::Convert(obj->getCoordInterface()->getObj());

    for (size_t i = 0; i < draw->getNumDrawables(); i++) {
        if (draw->getDrawableKey(i) == -1)
            continue;

        plDrawableSpans* span = plDrawableSpans::Convert(draw->getDrawable(i)->getObj());
        plDISpanIndex di = span->getDIIndex(draw->getDrawableKey(i));
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
                glColor3f(0.0f, 0.0f, 0.0f);

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

            glBegin(GL_TRIANGLES);
            for (size_t j = 0; j < indices.getSize(); j++) {
                hsColor32 color(verts[indices[j]].fColor);
                glTexCoord3f(verts[indices[j]].fUVWs[0].X, verts[indices[j]].fUVWs[0].Y, verts[indices[j]].fUVWs[0].Z);
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
    }
}
