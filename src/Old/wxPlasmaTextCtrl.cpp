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

#include "wxPlasmaTextCtrl.h"
#include <Stream/hsElfStream.h>
#include <Debug/plDebug.h>

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
    fEncryptionType = plEncryptedStream::kEncNone;
}

wxPlasmaTextCtrl::~wxPlasmaTextCtrl()
{ }

void wxPlasmaTextCtrl::DoLoad(const wxString& filename)
{
    hsStream* S;
    fEncryptionType = plEncryptedStream::kEncNone;
    plString fn = (const char*)filename.mb_str(wxConvFile);
    if (plEncryptedStream::IsFileEncrypted(fn)) {
        S = new plEncryptedStream();
        fEncryptionType = plEncryptedStream::kEncAuto;
        ((plEncryptedStream*)S)->open(fn, fmRead, fEncryptionType);
    } else {
        S = new hsFileStream();
        ((hsFileStream*)S)->open(fn, fmRead);
    }

    unsigned int bom = S->readInt();
    fEncoding = kUniNone;
    if (bom == 0x0000FEFF) {
        fEncoding = kUniUTF32LE;
    } else if (bom == 0xFFFE0000) {
        fEncoding = kUniUTF32BE;
    } else if ((bom & 0xFFFF) == 0xFEFF) {
        fEncoding = kUniUTF16LE;
    } else if ((bom & 0xFFFF) == 0xFFFE) {
        fEncoding = kUniUTF16BE;
    } else if ((bom & 0xFFFFFF) == 0xBFBBEF) {
        fEncoding = kUniUTF8;
    }
    S->rewind();

    size_t bufSize = S->size();
    if (fEncoding == kUniUTF32LE) {
        S->skip(4); // BOM
        bufSize -= 4;
        hsUint32* buf = new hsUint32[(bufSize / 4)];
        S->read(bufSize, buf);
        wchar_t* wbuf = new wchar_t[(bufSize / 4) + 1];
        for (size_t i=0; i<(bufSize / 4); i++)
            wbuf[i] = (wchar_t)buf[i];
        wbuf[(bufSize / 4)] = 0;
        wxMBConvUTF32LE conv;
        SetText(wxString(wbuf, conv));
        delete[] buf;
        delete[] wbuf;
    } else if (fEncoding == kUniUTF32BE) {
        S->skip(4); // BOM
        bufSize -= 4;
        hsUint32* buf = new hsUint32[(bufSize / 4)];
        S->read(bufSize, buf);
        wchar_t* wbuf = new wchar_t[(bufSize / 4) + 1];
        for (size_t i=0; i<(bufSize / 4); i++)
            wbuf[i] = (wchar_t)buf[i];
        wbuf[(bufSize / 4)] = 0;
        wxMBConvUTF32BE conv;
        SetText(wxString(wbuf, conv));
        delete[] buf;
        delete[] wbuf;
    } else if (fEncoding == kUniUTF16LE) {
        S->skip(2); // BOM
        bufSize -= 2;
        hsUint16* buf = new hsUint16[(bufSize / 2)];
        S->read(bufSize, buf);
        wchar_t* wbuf = new wchar_t[(bufSize / 2) + 1];
        for (size_t i=0; i<(bufSize / 2); i++)
            wbuf[i] = (wchar_t)buf[i];
        wbuf[(bufSize / 2)] = 0;
        wxMBConvUTF16LE conv;
        SetText(wxString(wbuf, conv));
        delete[] buf;
        delete[] wbuf;
    } else if (fEncoding == kUniUTF16BE) {
        S->skip(2); // BOM
        bufSize -= 2;
        hsUint16* buf = new hsUint16[(bufSize / 2)];
        S->read(bufSize, buf);
        wchar_t* wbuf = new wchar_t[(bufSize / 2) + 1];
        for (size_t i=0; i<(bufSize / 2); i++)
            wbuf[i] = (wchar_t)buf[i];
        wbuf[(bufSize / 2)] = 0;
        wxMBConvUTF16BE conv;
        SetText(wxString(wbuf, conv));
        delete[] buf;
        delete[] wbuf;
    } else if (fEncoding == kUniUTF8) {
        S->skip(3); // BOM
        bufSize -= 3;
        char* buf = new char[bufSize + 1];
        S->read(bufSize, buf);
        buf[bufSize] = 0;
        SetText(wxString(buf, wxConvUTF8));
        delete[] buf;
    } else {
        char* buf = new char[bufSize + 1];
        S->read(bufSize, buf);
        buf[bufSize] = 0;
        SetText(wxString(buf, wxConvISO8859_1));
        delete[] buf;
    }

    fFileName = filename;
    if (fEncryptionType != plEncryptedStream::kEncNone)
        fEncryptionType = ((plEncryptedStream*)S)->getEncType();
    delete S;

    EmptyUndoBuffer();
    SetSavePoint();
}

void wxPlasmaTextCtrl::DoSave(const wxString& filename)
{
    hsStream* S;
    wxString saveFilename = (filename == wxEmptyString) ? fFileName : filename;
    plString fn = (const char*)saveFilename.mb_str(wxConvFile);
    if (fEncryptionType == plEncryptedStream::kEncNone) {
        S = new hsFileStream();
        ((hsFileStream*)S)->open(fn, fmCreate);
    } else {
        S = new plEncryptedStream();
        ((plEncryptedStream*)S)->open(fn, fmCreate, fEncryptionType);
    }

    wxString src = GetText();
    if (fEncoding == kUniUTF32LE) {
        S->writeInt(0x0000FEFF);    // BOM
        wxMBConvUTF32LE conv;
        size_t bufSize = wcslen(conv.cWX2WC(src));
        wchar_t* wbuf = new wchar_t[bufSize + 1];
        memcpy(wbuf, conv.cWX2WC(src), bufSize);
        wbuf[bufSize] = 0;
        hsUint32* buf = new hsUint32[bufSize];
        for (size_t i=0; i<bufSize; i++)
            buf[i] = (hsUint32)wbuf[i];
        S->write(bufSize * 4, buf);
        delete[] buf;
        delete[] wbuf;
    } else if (fEncoding == kUniUTF32BE) {
        S->writeInt(0xFFFE0000);    // BOM
        wxMBConvUTF32BE conv;
        size_t bufSize = wcslen(conv.cWX2WC(src));
        wchar_t* wbuf = new wchar_t[bufSize + 1];
        memcpy(wbuf, conv.cWX2WC(src), bufSize);
        wbuf[bufSize] = 0;
        hsUint32* buf = new hsUint32[bufSize];
        for (size_t i=0; i<bufSize; i++)
            buf[i] = (hsUint32)wbuf[i];
        S->write(bufSize * 4, buf);
        delete[] buf;
        delete[] wbuf;
    } else if (fEncoding == kUniUTF16LE) {
        S->writeShort(0xFEFF);  // BOM
        wxMBConvUTF16LE conv;
        size_t bufSize = wcslen(conv.cWX2WC(src));
        wchar_t* wbuf = new wchar_t[bufSize + 1];
        memcpy(wbuf, conv.cWX2WC(src), bufSize);
        wbuf[bufSize] = 0;
        hsUint16* buf = new hsUint16[bufSize];
        for (size_t i=0; i<bufSize; i++)
            buf[i] = (hsUint16)wbuf[i];
        S->write(bufSize * 2, buf);
        delete[] buf;
        delete[] wbuf;
    } else if (fEncoding == kUniUTF16BE) {
        S->writeShort(0xFFFE);  // BOM
        wxMBConvUTF16BE conv;
        size_t bufSize = wcslen(conv.cWX2WC(src));
        wchar_t* wbuf = new wchar_t[bufSize + 1];
        memcpy(wbuf, conv.cWX2WC(src), bufSize);
        wbuf[bufSize] = 0;
        hsUint16* buf = new hsUint16[bufSize];
        for (size_t i=0; i<bufSize; i++)
            buf[i] = (hsUint16)wbuf[i];
        S->write(bufSize * 2, buf);
        delete[] buf;
        delete[] wbuf;
    } else if (fEncoding == kUniUTF8) {
        S->writeByte(0xEF); // BOM: EF BB BF
        S->writeByte(0xBB);
        S->writeByte(0xBF);
        size_t bufSize = strlen(wxConvUTF8.cWX2MB(src));
        S->write(bufSize, wxConvUTF8.cWX2MB(src));
    } else {
        size_t bufSize = strlen(wxConvISO8859_1.cWX2MB(src));
        S->write(bufSize, wxConvISO8859_1.cWX2MB(src));
    }

    delete S;
    SetSavePoint();
}

void wxPlasmaTextCtrl::LoadElf(const wxString& filename)
{
    hsElfStream* S = new hsElfStream();
    S->open(filename.mb_str(wxConvFile), fmRead);

    while (!S->eof())
        AddText(wxString::FromUTF8(S->readLine()) + wxT("\n"));

    fFileName = filename;
    fEncryptionType = plEncryptedStream::kEncNone;
    delete S;

    EmptyUndoBuffer();
    SetSavePoint();
}

void wxPlasmaTextCtrl::SaveElf(const wxString& filename)
{
    plDebug::Error("PlasmaShop: [ERR] ELF writing not supported");
}

void wxPlasmaTextCtrl::ResetSyntax()
{
    ClearDocumentStyle();
    StyleResetDefault();
    StyleSetFaceName(wxSTC_STYLE_DEFAULT, fFont.GetFaceName());
    StyleSetSize(wxSTC_STYLE_DEFAULT, fFont.GetPointSize());
    StyleSetForeground(wxSTC_STYLE_BRACEBAD, wxColour(0xFF, 0, 0));
    StyleSetFontAttr(wxSTC_STYLE_BRACEBAD, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
    StyleSetForeground(wxSTC_STYLE_BRACELIGHT, wxColour(0, 0, 0xC0));
    StyleSetFontAttr(wxSTC_STYLE_BRACELIGHT, fFont.GetPointSize(), fFont.GetFaceName(), true, false, false);
    StyleClearAll();
    UpdateLineNumberWidth();
}

wxPlasmaTextCtrl::SyntaxMode wxPlasmaTextCtrl::GetSyntaxMode()
{
    return fSyntaxMode;
}

void wxPlasmaTextCtrl::SetSyntaxMode(SyntaxMode mode)
{
    ResetSyntax();

    switch (mode) {
    case kSynNone:
        SetLexer(wxSTC_LEX_NULL);
        break;
    case kSynPlasma:
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
        SetLexer(wxSTC_LEX_PROPERTIES);
        StyleSetForeground(wxSTC_PROPS_COMMENT, wxColour(0, 0x80, 0));
        StyleSetForeground(wxSTC_PROPS_KEY, wxColour(0, 0, 0xFF));
        StyleSetForeground(wxSTC_PROPS_SECTION, wxColour(0x80, 0x80, 0));
        break;
    case kSynConsole:
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
#ifdef wxSTC_PLASMA_STC
        SetLexer(wxSTC_LEX_HEX);
#endif
        plDebug::Debug("TODO: kSynHex");
        break;
    case kSynFX:
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
    Colourise(0, -1);
}

plEncryptedStream::EncryptionType wxPlasmaTextCtrl::GetEncryptionType()
{
    return fEncryptionType;
}

void wxPlasmaTextCtrl::SetEncryptionType(plEncryptedStream::EncryptionType enc)
{
    fEncryptionType = enc;
}

wxPlasmaTextCtrl::EncodingType wxPlasmaTextCtrl::GetEncoding()
{
    return fEncoding;
}

void wxPlasmaTextCtrl::SetEncoding(wxPlasmaTextCtrl::EncodingType uni)
{
    fEncoding = uni;
}

wxString wxPlasmaTextCtrl::GetFilename()
{
    return fFileName;
}

void wxPlasmaTextCtrl::SetFilename(const wxString& filename)
{
    fFileName = filename;
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
