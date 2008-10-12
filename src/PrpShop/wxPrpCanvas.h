#ifndef _WXPRPCANVAS_H
#define _WXPRPCANVAS_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <map>
#include <wx/glcanvas.h>
#include <PRP/Object/plSceneObject.h>
#include <PRP/Geometry/plDrawableSpans.h>

class wxPrpCanvas : public wxGLCanvas {
public:
    enum { MODE_MODEL, MODE_SCENE };

private:
    bool fInited;
    hsTArray<plKey> fObjects;
    hsTArray<plKey> fTextures;
    std::map<plKey, size_t> fTexNames;
    GLuint fList;
    GLuint* fTexList;
    
    int fMode;
    float fRotZ, fRotX, fModelDist;
    hsVector3 fViewPos;
    hsVector3 fModelMins, fModelMaxs;

public:
    wxPrpCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0, const wxString& name = wxT("AgeViewer"));
    ~wxPrpCanvas();

    void Render();
    void InitGL();

    void AddObject(plKey obj);
    void SetView(hsVector3 view, float angle=0.0f);
    void Center(plKey obj);

    void AddTexture(plKey tex);
    void Build(int mode);

protected:
    DECLARE_EVENT_TABLE()

    void OnPaint(wxPaintEvent& evt);
    void OnSize(wxSizeEvent& evt);
    void OnEraseBackground(wxEraseEvent& evt);
    void OnMouse(wxMouseEvent& evt);

    void CompileObject(plKey obj);
    void CompileTexture(plKey tex, GLuint id);

private:
    wxPoint fMouseFrom;
    int fMouseBtn;
};

#endif
