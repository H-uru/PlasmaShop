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

/* -------------------------------------------------------------------------- */

/* wxPlasmaTextCtrl */
BEGIN_EVENT_TABLE(wxPlasmaTextCtrl, wxStyledTextCtrl)
    EVT_STC_MARGINCLICK(wxID_ANY, wxPlasmaTextCtrl::OnMarginClick)
    EVT_STC_PAINTED(wxID_ANY, wxPlasmaTextCtrl::OnStcPainted)
    EVT_STC_CHARADDED(wxID_ANY, wxPlasmaTextCtrl::OnCharAdded)
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

    fFontName = wxT("Courier New");
    fFontSize = 10;
    SetSyntaxMode(kSynNone);

    int lnWidth, lnHeight;
    GetTextExtent(wxT("0"), &lnWidth, &lnHeight);
    fLineNumberWidth = lnWidth + 1;
}

wxPlasmaTextCtrl::~wxPlasmaTextCtrl()
{ }

void wxPlasmaTextCtrl::SetSyntaxMode(SyntaxMode mode) {
    switch (mode) {
    case kSynNone:
        SetLexer(wxSTC_LEX_NULL);
        StyleResetDefault();
        StyleSetFaceName(wxSTC_STYLE_DEFAULT, fFontName);
        StyleSetSize(wxSTC_STYLE_DEFAULT, fFontSize);
        break;
    case kSynPlasma:
        SetSyntaxMode(kSynNone);
        SetLexer(wxSTC_LEX_CPP);
        SetKeyWords(0, kPlasmaKeywordKWs);
        SetKeyWords(1, kPlasmaTypeKWs);
        SetKeyWords(4, kPlasmaSpecialConstKWs);
        StyleSetForeground(wxSTC_C_CHARACTER, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_C_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_COMMENTLINEDOC, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_GLOBALCLASS, wxColour(0, 0, 0xFF));
        StyleSetItalic(wxSTC_C_GLOBALCLASS, true);
        StyleSetForeground(wxSTC_C_NUMBER, wxColour(0, 0, 0x80));
        StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_C_STRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_C_STRINGEOL, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_C_WORD, wxColour(0, 0, 0));
        StyleSetFontAttr(wxSTC_C_WORD, fFontSize, fFontName, true, false, false);
        StyleSetForeground(wxSTC_C_WORD2, wxColour(0, 0, 0xFF));
        break;
    case kSynPython:
        SetSyntaxMode(kSynNone);
        SetLexer(wxSTC_LEX_PYTHON);
        SetKeyWords(0, kPythonKeywordKWs);
        SetKeyWords(1, kPythonSpecialConstKWs);
        StyleSetForeground(wxSTC_P_CHARACTER, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_P_CLASSNAME, wxColour(0, 0, 0x80));
        StyleSetFontAttr(wxSTC_P_CLASSNAME, fFontSize, fFontName, true, false, false);
        StyleSetForeground(wxSTC_P_COMMENTBLOCK, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_P_COMMENTLINE, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_P_DECORATOR, wxColour(0x80, 0x80, 0x80));
        StyleSetFontAttr(wxSTC_P_DECORATOR, fFontSize, fFontName, false, true, false);
        StyleSetForeground(wxSTC_P_DEFNAME, wxColour(0x80, 0x80, 0));
        StyleSetForeground(wxSTC_P_NUMBER, wxColour(0, 0, 0x80));
        StyleSetForeground(wxSTC_P_STRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_P_STRINGEOL, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_P_TRIPLE, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_P_TRIPLEDOUBLE, wxColour(0x80, 0, 0));
        StyleSetFontAttr(wxSTC_P_WORD, fFontSize, fFontName, true, false, false);
        StyleSetForeground(wxSTC_P_WORD2, wxColour(0, 0x80, 0x80));
        StyleSetFontAttr(wxSTC_P_WORD2, fFontSize, fFontName, true, false, false);
        break;
    case kSynSDL_Uru:
        SetSyntaxMode(kSynNone);
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
        StyleSetFontAttr(wxSTC_SDL_WORD, fFontSize, fFontName, true, false, false);
        StyleSetForeground(wxSTC_SDL_WORD2, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_SDL_WORD3, wxColour(0x80, 0x80, 0));
        break;
    case kSynSDL_Eoa:
        SetSyntaxMode(kSynNone);
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
        StyleSetFontAttr(wxSTC_SDL_WORD, fFontSize, fFontName, true, false, false);
        StyleSetForeground(wxSTC_SDL_WORD2, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_SDL_WORD3, wxColour(0x80, 0x80, 0));
        break;
    case kSynAgeIni:
        SetSyntaxMode(kSynNone);
        SetLexer(wxSTC_LEX_PROPERTIES);
        StyleSetForeground(wxSTC_PROPS_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_PROPS_KEY, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_PROPS_SECTION, wxColour(0x80, 0x80, 0));
        break;
    case kSynConsole:
        SetSyntaxMode(kSynNone);
        SetLexer(wxSTC_LEX_FNI);
        SetKeyWords(0, kFniSpecialConstKWs);
        StyleSetForeground(wxSTC_FNI_COMMAND, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_FNI_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_FNI_GROUP, wxColour(0x80, 0, 0x80));
        StyleSetForeground(wxSTC_FNI_NUMBER, wxColour(0, 0, 0x80));
        StyleSetForeground(wxSTC_FNI_STRING, wxColour(0x80, 0, 0));
        break;
    case kSynXML:
        SetSyntaxMode(kSynNone);
        SetLexer(wxSTC_LEX_XML);
        StyleSetForeground(wxSTC_H_ATTRIBUTE, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_H_COMMENT, wxColour(0x80, 0x80, 0x80));
        StyleSetFontAttr(wxSTC_H_COMMENT, fFontSize, fFontName, false, true, false);
        StyleSetForeground(wxSTC_H_DOUBLESTRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_H_NUMBER, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_H_SINGLESTRING, wxColour(0x80, 0, 0));
        StyleSetForeground(wxSTC_H_TAG, wxColour(0, 0, 0x80));
        StyleSetFontAttr(wxSTC_H_TAG, fFontSize, fFontName, true, false, false);
        StyleSetForeground(wxSTC_H_TAGEND, wxColour(0, 0, 0x80));
        StyleSetFontAttr(wxSTC_H_TAGEND, fFontSize, fFontName, true, false, false);
        StyleSetForeground(wxSTC_H_XMLSTART, wxColour(0x80, 0x80, 0));
        StyleSetForeground(wxSTC_H_XMLEND, wxColour(0x80, 0x80, 0));
        break;
    case kSynHex:
        printf("TODO: kSynHex\n");
        break;
    }

    fSyntaxMode = mode;
}

wxPlasmaTextCtrl::SyntaxMode wxPlasmaTextCtrl::GetSyntaxMode()
{
    return fSyntaxMode;
}

void wxPlasmaTextCtrl::OnMarginClick(wxStyledTextEvent& evt)
{
    switch (evt.GetMargin()) {
    case kMarginFolders:
        ToggleFold(LineFromPosition(evt.GetPosition()));
        break;
    }
}

void wxPlasmaTextCtrl::OnStcPainted(wxStyledTextEvent& evt)
{
    int lastLine = GetLineCount();
    int nLineChars = 2; // Add an extra char of padding
    while (lastLine > 0) {
        nLineChars++;
        lastLine /= 10;
    }

    SetMarginWidth(kMarginLineNumbers, nLineChars * fLineNumberWidth);
}

void wxPlasmaTextCtrl::OnCharAdded(wxStyledTextEvent& evt)
{
    if ((evt.GetKey() == '\n' || evt.GetKey() == '\r') && (GetCurrentLine() > 0)) {
        // \r only on CRLF mode:
        if (GetEOLMode() == wxSTC_EOL_CRLF && evt.GetKey() != '\n') {
            wxString ln = GetLine(GetCurrentLine() - 1);
            int i = 0;
            while (ln[i] == ' ' || ln[i] == '\t')
                i++;
            AddText(ln.Left(i));
        }
    }
}
