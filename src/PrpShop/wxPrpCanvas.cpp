#include "wxPrpCanvas.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <Debug/plDebug.h>
#include <PRP/Object/plDrawInterface.h>
#include <PRP/Object/plCoordinateInterface.h>
#include <PRP/Geometry/plDrawableSpans.h>

BEGIN_EVENT_TABLE(wxPrpCanvas, wxGLCanvas)
    EVT_SIZE(wxPrpCanvas::OnSize)
    EVT_PAINT(wxPrpCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(wxPrpCanvas::OnEraseBackground)
END_EVENT_TABLE()

wxPrpCanvas::wxPrpCanvas(wxWindow* parent, wxWindowID id,
                         const wxPoint& pos, const wxSize& size,
                         long style, const wxString& name)
           : wxGLCanvas(parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name),
             fInited(false)
{ }

wxPrpCanvas::~wxPrpCanvas()
{ }

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
    gluPerspective(45.0f, (float)width/(float)height, 0.1f, 100.0f);
}

void wxPrpCanvas::OnEraseBackground(wxEraseEvent& evt)
{ }

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
    if (!fInited)
        InitGL();

    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -100.0f);
    for (size_t i=0; i<fObjects.getSize(); i++)
        DrawObject(fObjects[i]);

    SwapBuffers();
}

void wxPrpCanvas::AddObject(plKey obj)
{ fObjects.append(obj); }

void wxPrpCanvas::ClearObjects()
{ fObjects.clear(); }

void wxPrpCanvas::DrawObject(plKey key)
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

            if (coord != NULL)
                glMultMatrixf(coord->getLocalToWorld().glMatrix());
            else
                glMultMatrixf(ice->getLocalToWorld().glMatrix());

            glBegin(GL_TRIANGLES);
            for (size_t j = 0; j < indices.getSize(); j += 3) {
                hsColor32 cl[3] = { verts[indices[j+0]].fColor,
                                    verts[indices[j+1]].fColor,
                                    verts[indices[j+2]].fColor };
                glColor4ub(cl[0].r, cl[0].g, cl[0].b, cl[0].a);
                glVertex3f(verts[indices[j+0]].fPos.X, verts[indices[j+0]].fPos.Z, verts[indices[j+0]].fPos.Y);
                glColor4ub(cl[1].r, cl[1].g, cl[1].b, cl[1].a);
                glVertex3f(verts[indices[j+1]].fPos.X, verts[indices[j+1]].fPos.Z, verts[indices[j+1]].fPos.Y);
                glColor4ub(cl[2].r, cl[2].g, cl[2].b, cl[2].a);
                glVertex3f(verts[indices[j+2]].fPos.X, verts[indices[j+2]].fPos.Z, verts[indices[j+2]].fPos.Y);
            }
            glEnd();

            if (coord != NULL)
                glMultMatrixf(coord->getWorldToLocal().glMatrix());
            else
                glMultMatrixf(ice->getWorldToLocal().glMatrix());
        }
    }
}
