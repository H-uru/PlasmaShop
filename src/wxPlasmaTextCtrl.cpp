#include "wxPlasmaTextCtrl.h"

/* -------------------------------------------------------------------------- */

/* Plasmacode Keywords */
static const wxString kPlasmaKeywordKWs =
wxT("if else do while for switch case default break return continue ")
wxT("public protected private virtual static const new delete throw ");

static const wxString kPlasmaTypeKWs =
wxT("byte int16 int32 int64 ubyte uint16 uint32 uint64 float double ")
wxT("bool char wchar void struct class enum union ");

static const wxString kPlasmaSpecialConstKWs =
wxT("true false NULL this ");

static wxString kPlasmaClassNames = wxT("");


/* Python Keywords */
static const wxString kPythonKeywordKWs =
wxT("and assert break class continue def del elif else except exec finally ")
wxT("for from global if import in is lambda not or pass print raise return ")
wxT("try while yield ");

static const wxString kPythonSpecialConstKWs =
wxT("Ellipsis False None NotImplemented self True ");


/* SDL (Uru) Keywords */
static const wxString kSDL1KeywordKWs =
wxT("DEFAULT DEFAULTOPTION DISPLAYOPTION STATEDESC VAR VERSION ");

static const wxString kSDL1TypeKWs =
wxT("AGETIMEOFDAY BOOL BYTE CREATABLE FLOAT INT PLKEY POINT3 QUATERNION ")
wxT("RGB8 SHORT STRING32 TIME VECTOR3 ");

static const wxString kSDL1SpecialConstKWs =
wxT("false true nil VAULT");


/* SDL (Eoa) Keywords */
static const wxString kSDL2KeywordKWs =
wxT("default struct version ");

static const wxString kSDL2TypeKWs =
wxT("AgeTimeElapsed AgeTimeOfDay bool buffer byte double float ")
wxT("GameTimeElapsed int matrix44 point3 quaternion string uint uoid ")
wxT("vector3 ");

static const wxString kSDL2SpecialConstKWs =
wxT("false true nil");


/* Console Keywords */
static const wxString kFniSpecialConstKWs =
wxT("false true");


/* FX Keywords */
static const wxString kFXKeywordKWs =
wxT("asm auto break case catch compile continue decl default delete do ")
wxT("else explicit extern for friend goto if in inline inout mutable new ")
wxT("operator out private protected public return shared static switch ")
wxT("throw try typedef uniform using virtual volatile while ");

static const wxString kFXTypeKWs =
wxT("bool char class const double enum float half int long matrix namespace ")
wxT("pass pixelshader register sampler short signed string struct technique ")
wxT("template texture typename unsigned union vector vertexshader void ")
wxT("bool2 bool3 bool4 double2 double3 double4 float2 float3 float4 ")
wxT("half2 half3 half4 int2 int3 int4 bool2x2 bool2x3 bool2x4 bool3x2 ")
wxT("bool3x3 bool3x4 bool4x2 bool4x3 bool4x4 double2x2 double2x3 double2x4 ")
wxT("double3x2 double3x3 double3x4 double4x2 double4x3 double4x4 float2x2 ")
wxT("float2x3 float2x4 float3x2 float3x3 float3x4 float4x2 float4x3 float4x4 ")
wxT("half2x2 half2x3 half2x4 half3x2 half3x3 half3x4 half4x2 half4x3 half4x4 ")
wxT("int2x2 int2x3 int2x4 int3x2 int3x3 int3x4 int4x2 int4x3 int4x4 ");

static const wxString kFXSpecialConstKWs =
wxT("const_cast dynamic_cast false reinterpret_cast sizeof static_cast true ");

/* -------------------------------------------------------------------------- */

/* wxPlasmaTextCtrl */
BEGIN_EVENT_TABLE(wxPlasmaTextCtrl, wxStyledTextCtrl)
    EVT_STC_MARGINCLICK(wxID_ANY, wxPlasmaTextCtrl::OnMarginClick)
    EVT_STC_UPDATEUI(wxID_ANY, wxPlasmaTextCtrl::OnUpdateUI)
    EVT_STC_CHARADDED(wxID_ANY, wxPlasmaTextCtrl::OnCharAdded)
    EVT_STC_MODIFIED(wxID_ANY, wxPlasmaTextCtrl::OnModified)
    EVT_STC_ZOOM(wxID_ANY, wxPlasmaTextCtrl::OnZoom)
END_EVENT_TABLE()

wxPlasmaTextCtrl::wxPlasmaTextCtrl(wxWindow* parent, wxWindowID id,
                                   const wxPoint& pos, const wxSize& size,
                                   long style, const wxString& name)
    : wxStyledTextCtrl(parent, id, pos, size, style, name)
{
    SetEdgeMode(wxSTC_EDGE_LINE);
    SetEdgeColumn(80);

    SetMarginWidth(kMarginLineNumbers, 0);
    SetMarginType(kMarginLineNumbers, wxSTC_MARGIN_NUMBER);
    SetMarginWidth(kMarginFolders, 12);
    SetMarginType(kMarginFolders, wxSTC_MARGIN_SYMBOL);
    SetMarginMask(kMarginFolders, wxSTC_MASK_FOLDERS);
    SetMarginSensitive(kMarginFolders, true);
    SetFoldFlags(wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS, wxColour(0xFF, 0xFF, 0xFF), wxColour(0, 0, 0));
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS, wxColour(0xFF, 0xFF, 0xFF), wxColour(0, 0, 0));
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED, wxColour(0xFF, 0xFF, 0xFF), wxColour(0, 0, 0));
    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE, wxColour(0xFF, 0xFF, 0xFF), wxColour(0, 0, 0));
    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER, wxColour(0xFF, 0xFF, 0xFF), wxColour(0, 0, 0));
    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER, wxColour(0xFF, 0xFF, 0xFF), wxColour(0, 0, 0));
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED, wxColour(0xFF, 0xFF, 0xFF), wxColour(0, 0, 0));

    SetProperty(wxT("fold"), wxT("1"));
    SetProperty(wxT("fold.compact"), wxT("0"));
    SetProperty(wxT("fold.at.else"), wxT("1"));
    SetProperty(wxT("fold.html"), wxT("1"));

    SetIndent(4);
    SetTabIndents(true);
    SetTabWidth(4);
    SetUseTabs(false);
    SetViewWhiteSpace(false);
    SetEndAtLastLine(false);

#if defined(wxSTC_VERSION) && (wxSTC_VERSION >= 175)
    SetScrollWidth(800);
    SetScrollWidthTracking(true);
#endif

#if defined(__WXMSW__)
    fFont = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                   wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));
#elif defined(__WXMAC__)
    fFont = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                   wxFONTWEIGHT_NORMAL, false, wxT("Courier"));
#else /* Linux, UNIX, OSX, etc */
    fFont = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                   wxFONTWEIGHT_NORMAL, false, wxT("Monospace"));
#endif

    SetSyntaxMode(kSynNone);
}

wxPlasmaTextCtrl::~wxPlasmaTextCtrl()
{ }

void wxPlasmaTextCtrl::ResetSyntax()
{
    StyleResetDefault();
    StyleSetFaceName(wxSTC_STYLE_DEFAULT, fFont.GetFaceName());
    StyleSetSize(wxSTC_STYLE_DEFAULT, fFont.GetPointSize());
    StyleSetForeground(wxSTC_STYLE_BRACEBAD, wxColour(0xFF, 0, 0));
    StyleSetFontAttr(wxSTC_STYLE_BRACEBAD, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
    StyleSetForeground(wxSTC_STYLE_BRACELIGHT, wxColour(0, 0, 0xC0));
    StyleSetFontAttr(wxSTC_STYLE_BRACELIGHT, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
    UpdateLineNumberWidth();
}

void wxPlasmaTextCtrl::SetSyntaxMode(SyntaxMode mode) {
    switch (mode) {
    case kSynNone:
        ResetSyntax();
        SetLexer(wxSTC_LEX_NULL);
        break;
    case kSynPlasma:
        ResetSyntax();
        SetLexer(wxSTC_LEX_CPP);
        SetKeyWords(0, kPlasmaKeywordKWs);
        SetKeyWords(1, kPlasmaTypeKWs);
        SetKeyWords(3, kPlasmaSpecialConstKWs);
#if defined(wxSTC_VERSION) && (wxSTC_VERSION >= 175)
        SetKeyWords(4, kPlasmaClassNames);
#endif
        StyleSetForeground(wxSTC_C_CHARACTER, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_C_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTLINEDOC, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_GLOBALCLASS, wxColour(0x80, 0x80, 0));
        StyleSetForeground(wxSTC_C_NUMBER, wxColour(0, 0, 0x80));
        StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_STRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_C_STRINGEOL, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_C_WORD, wxColour(0, 0, 0));
        StyleSetFontAttr(wxSTC_C_WORD, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
        StyleSetForeground(wxSTC_C_WORD2, wxColour(0, 0, 0xFF));
#if defined(wxSTC_VERSION) && (wxSTC_VERSION >= 175)
        StyleSetForeground(wxSTC_C_WORD3, wxColour(0, 0x80, 0x80));
        StyleSetHotSpot(wxSTC_C_WORD3, true);
#endif
        break;
    case kSynPython:
        ResetSyntax();
        SetLexer(wxSTC_LEX_PYTHON);
        SetKeyWords(0, kPythonKeywordKWs);
        SetKeyWords(1, kPythonSpecialConstKWs);
        StyleSetForeground(wxSTC_P_CHARACTER, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_P_CLASSNAME, wxColour(0, 0, 0x80));
        StyleSetFontAttr(wxSTC_P_CLASSNAME, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
        StyleSetForeground(wxSTC_P_COMMENTBLOCK, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_P_COMMENTLINE, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_P_DECORATOR, wxColour(0x80, 0x80, 0x80));
        StyleSetFontAttr(wxSTC_P_DECORATOR, fFont.GetPointSize(), fFont.GetFaceName(), false, true, false);
        StyleSetForeground(wxSTC_P_DEFNAME, wxColour(0x80, 0x80, 0));
        StyleSetForeground(wxSTC_P_NUMBER, wxColour(0, 0, 0x80));
        StyleSetForeground(wxSTC_P_STRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_P_STRINGEOL, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_P_TRIPLE, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_P_TRIPLEDOUBLE, wxColour(0x80, 0, 0));
        StyleSetFontAttr(wxSTC_P_WORD, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
        StyleSetForeground(wxSTC_P_WORD2, wxColour(0, 0x80, 0x80));
        StyleSetFontAttr(wxSTC_P_WORD2, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
        break;
    case kSynSDL_Uru:
        ResetSyntax();
#ifdef wxSTC_PLASMA_STC
        SetLexer(wxSTC_LEX_SDL);
        SetKeyWords(0, kSDL1KeywordKWs);
        SetKeyWords(1, kSDL1TypeKWs);
        SetKeyWords(2, kSDL1SpecialConstKWs);
        StyleSetForeground(wxSTC_SDL_CHARACTER, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_SDL_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_SDL_COMMENTBLOCK, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_SDL_COMMENTLINE, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_SDL_NUMBER, wxColour(0, 0, 0x80));
        StyleSetForeground(wxSTC_SDL_STRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_SDL_STRINGEOL, wxColour(0x80, 0, 0));
        StyleSetFontAttr(wxSTC_SDL_WORD, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
        StyleSetForeground(wxSTC_SDL_WORD2, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_SDL_WORD3, wxColour(0x80, 0x80, 0));
#endif
        break;
    case kSynSDL_Eoa:
        ResetSyntax();
#ifdef wxSTC_PLASMA_STC
        SetLexer(wxSTC_LEX_SDL);
        SetKeyWords(0, kSDL2KeywordKWs);
        SetKeyWords(1, kSDL2TypeKWs);
        SetKeyWords(2, kSDL2SpecialConstKWs);
        StyleSetForeground(wxSTC_SDL_CHARACTER, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_SDL_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_SDL_COMMENTBLOCK, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_SDL_COMMENTLINE, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_SDL_NUMBER, wxColour(0, 0, 0x80));
        StyleSetForeground(wxSTC_SDL_STRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_SDL_STRINGEOL, wxColour(0x80, 0, 0));
        StyleSetFontAttr(wxSTC_SDL_WORD, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
        StyleSetForeground(wxSTC_SDL_WORD2, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_SDL_WORD3, wxColour(0x80, 0x80, 0));
#endif
        break;
    case kSynAgeIni:
        ResetSyntax();
        SetLexer(wxSTC_LEX_PROPERTIES);
        StyleSetForeground(wxSTC_PROPS_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_PROPS_KEY, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_PROPS_SECTION, wxColour(0x80, 0x80, 0));
        break;
    case kSynConsole:
        ResetSyntax();
#ifdef wxSTC_PLASMA_STC
        SetLexer(wxSTC_LEX_FNI);
        SetKeyWords(0, kFniSpecialConstKWs);
        StyleSetForeground(wxSTC_FNI_COMMAND, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_FNI_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_FNI_GROUP, wxColour(0x80, 0, 0x80));
        StyleSetForeground(wxSTC_FNI_NUMBER, wxColour(0, 0, 0x80));
        StyleSetForeground(wxSTC_FNI_STRING, wxColour(0x80, 0, 0));
#endif
        break;
    case kSynXML:
        ResetSyntax();
        SetLexer(wxSTC_LEX_XML);
        StyleSetForeground(wxSTC_H_ATTRIBUTE, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_H_COMMENT, wxColour(0x80, 0x80, 0x80));
        StyleSetFontAttr(wxSTC_H_COMMENT, fFont.GetPointSize(), fFont.GetFaceName(), false, true, false);
        StyleSetForeground(wxSTC_H_DOUBLESTRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_H_NUMBER, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_H_SINGLESTRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_H_TAG, wxColour(0, 0, 0x80));
        StyleSetFontAttr(wxSTC_H_TAG, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
        StyleSetForeground(wxSTC_H_TAGEND, wxColour(0, 0, 0x80));
        StyleSetFontAttr(wxSTC_H_TAGEND, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
        StyleSetForeground(wxSTC_H_XMLSTART, wxColour(0x80, 0x80, 0));
        StyleSetForeground(wxSTC_H_XMLEND, wxColour(0x80, 0x80, 0));
        break;
    case kSynHex:
        ResetSyntax();
#ifdef wxSTC_PLASMA_STC
        SetLexer(wxSTC_LEX_HEX);
#endif
        printf("TODO: kSynHex\n");
        break;
    case kSynFX:
        ResetSyntax();
        SetLexer(wxSTC_LEX_CPP);
        SetKeyWords(0, kFXKeywordKWs);
        SetKeyWords(1, kFXTypeKWs);
        SetKeyWords(3, kFXSpecialConstKWs);
        StyleSetForeground(wxSTC_C_CHARACTER, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_C_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTLINEDOC, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_GLOBALCLASS, wxColour(0x80, 0x80, 0));
        StyleSetForeground(wxSTC_C_NUMBER, wxColour(0, 0, 0x80));
        StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_STRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_C_STRINGEOL, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_C_WORD, wxColour(0, 0, 0));
        StyleSetFontAttr(wxSTC_C_WORD, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
        StyleSetForeground(wxSTC_C_WORD2, wxColour(0, 0, 0xFF));
        break;
    }

    fSyntaxMode = mode;
}

wxPlasmaTextCtrl::SyntaxMode wxPlasmaTextCtrl::GetSyntaxMode()
{
    return fSyntaxMode;
}

void wxPlasmaTextCtrl::UpdateLineNumberWidth()
{
    wxString lnLast = wxString::Format(wxT(" %d"), GetLineCount());
    int lnWidth = TextWidth(wxSTC_STYLE_LINENUMBER, lnLast);
    SetMarginWidth(kMarginLineNumbers, lnWidth);
}

void wxPlasmaTextCtrl::OnMarginClick(wxStyledTextEvent& evt)
{
    switch (evt.GetMargin()) {
    case kMarginFolders:
        ToggleFold(LineFromPosition(evt.GetPosition()));
        break;
    }
}

void wxPlasmaTextCtrl::OnUpdateUI(wxStyledTextEvent& evt)
{
    int nextCh = GetCharAt(GetCurrentPos());
    int lastCh = GetCharAt(GetCurrentPos() - 1);
    if (strchr("()[]{}", nextCh)) {
        int brace = BraceMatch(GetCurrentPos());
        if (brace >= 0)
            BraceHighlight(brace, GetCurrentPos());
        else
            BraceBadLight(GetCurrentPos());
    } else if (strchr("()[]{}", lastCh)) {
        int brace = BraceMatch(GetCurrentPos() - 1);
        if (brace >= 0)
            BraceHighlight(brace, GetCurrentPos() - 1);
        else
            BraceBadLight(GetCurrentPos() - 1);
    } else {
        BraceHighlight(-1, -1);
    }
}

void wxPlasmaTextCtrl::OnCharAdded(wxStyledTextEvent& evt)
{
    if ((evt.GetKey() == '\n' || evt.GetKey() == '\r') && (GetCurrentLine() > 0)) {
        // \r only on CRLF mode:
        if (GetEOLMode() != wxSTC_EOL_CRLF || evt.GetKey() != '\n') {
            wxString ln = GetLine(GetCurrentLine() - 1);
            int i = 0;
            while (ln[i] == ' ' || ln[i] == '\t')
                i++;
            AddText(ln.Left(i));
        }
    }
}

void wxPlasmaTextCtrl::OnModified(wxStyledTextEvent& evt)
{
    UpdateLineNumberWidth();
}

void wxPlasmaTextCtrl::OnZoom(wxStyledTextEvent& evt)
{
    UpdateLineNumberWidth();
}
