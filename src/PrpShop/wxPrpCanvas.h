#ifndef _WXPRPCANVAS_H
#define _WXPRPCANVAS_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/glcanvas.h>
#include <PRP/Object/plSceneObject.h>

class wxPrpCanvas : public wxGLCanvas {
private:
    bool fInited;
    hsTArray<plKey> fObjects;

public:
    wxPrpCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0, const wxString& name = wxT("AgeViewer"));
    ~wxPrpCanvas();

    void Render();
    void InitGL();

    void AddObject(plKey obj);
    void ClearObjects();

protected:
    DECLARE_EVENT_TABLE()

    void OnPaint(wxPaintEvent& evt);
    void OnSize(wxSizeEvent& evt);
    void OnEraseBackground(wxEraseEvent& evt);

    void DrawObject(plKey obj);
};

#endif
