// This module implements the QsciLexerFX class.
//
// Copyright (c) 2010 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// Please review the following information to ensure GNU General
// Public Licensing requirements will be met:
// http://trolltech.com/products/qt/licenses/licensing/opensource/. If
// you are unsure which license is appropriate for your use, please
// review the following information:
// http://trolltech.com/products/qt/licenses/licensing/licensingoverview
// or contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "QsciPS3/qscilexerfx.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerFX::QsciLexerFX(QObject *parent)
    : QsciLexerCPP(parent)
{
}


// The dtor.
QsciLexerFX::~QsciLexerFX()
{
}


// Returns the language name.
const char *QsciLexerFX::language() const
{
    return "FX";
}


// Returns the set of keywords.
const char *QsciLexerFX::keywords(int set) const
{
    if (set == 1)
        return "tex1D tex2D tex3D texRECT texCUBE tex1Dproj tex2Dproj "
               "tex3Dproj texRECTproj texCUBEproj tex1D_proj tex2D_proj "
               "tex3D_proj texRECT_proj texCUBE_proj tex1D_bias tex2D_bias "
               "tex3D_bias texRECT_bias texCUBE_bias offsettex2D offsettexRECT "
               "offsettex2DScaleBias offsettexRECTScaleBias tex1D_dp3 "
               "tex2D_dp3x2 texRECT_dp3x2 tex3D_dp3x3 texCUBE_dp3x3 "
               "tex_dp3x2_depth texCUBE_reflect_dp3x3 discard arbfp1 arbvp1 "
               "texCUBE_reflect_eye_dp3x3 compile "
               "and and_eq asm bitand bitor break case compl continue default "
               "do else extern for goto if not not_eq or or_eq return "
               "switch typedef while xor xor_eq ";

    if (set == 2)
        return "bool bool2 bool3 bool4 bool1x2 bool1x3 bool1x4 bool2x2 bool2x3 "
               "bool2x4 bool3x2 bool3x3 bool3x4 bool4x2 bool4x3 bool4x4 half "
               "half2 half3 half4 half1x2 half1x3 half1x4 half2x2 half2x3 "
               "half2x4 half3x2 half3x3 half3x4 half4x2 half4x3 half4x4 fixed "
               "fixed2 fixed3 fixed4 fixed1x2 fixed1x3 fixed1x4 fixed2x2 "
               "fixed2x3 fixed2x4 fixed3x2 fixed3x3 fixed3x4 fixed4x2 fixed4x3 "
               "fixed4x4 float float2 float3 float4 float1x2 float1x3 float1x4 "
               "float2x2 float2x3 float2x4 float3x2 float3x3 float3x4 float4x2 "
               "float4x3 float4x4 int int2 int3 int4 int1x2 int1x3 int1x4 "
               "int2x2 int2x3 int2x4 int3x2 int3x3 int3x4 int4x2 int4x3 int4x4 "
               "sampler1D sampler2D sampler3D samplerCUBE samplerRECT cint "
               "cfloat string texture technique pass in out inout uniform "
               "packed sampler "
               "const char double auto enum inline long register wchar_t "
               "short signed static struct union unsigned void volatile ";

    if (set == 4)
        return "Space UIDesc UIName UIObject UIType UIMin UIMax UIStep "
               "UIWidget Texture MinFilter MagFilter MipFilter AddressU "
               "AddressV AddressW usage width height levels DepthBuffer "
               "format geometry cleardepth clearcolor rendertarget "
               "ps_1_1 ps_1_2 ps_1_3 vs_1_1 vs_2_0 vs_2_x ps_2_0 ps_2_x vs_3_0 "
               "ps_3_0 fp20 vp20 fp30 vp30 AddressU AddressV AddressW "
               "true false none sizeof ";

    return 0;
}

QColor QsciLexerFX::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Comment:
    case CommentLine:
        return QColor(0x00,0x7f,0x00);

    case CommentDoc:
    case CommentLineDoc:
        return QColor(0x00,0x7f,0x00);

    case Number:
        return QColor(0x00,0x00,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x00);

    case DoubleQuotedString:
    case SingleQuotedString:
        return QColor(0x7f,0x00,0x00);

    case PreProcessor:
        return QColor(0x00,0x7f,0x00);

    case Operator:
    case UnclosedString:
        return QColor(0x00,0x00,0x00);

    case KeywordSet2:
        return QColor(0x00,0x00,0xff);

    case GlobalClass:
        return QColor(0x00,0x7f,0x7f);
    }

    return QsciLexer::defaultColor(style);
}

// Returns the font of the text for a style.
QFont QsciLexerFX::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Keyword:
    case GlobalClass:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}
