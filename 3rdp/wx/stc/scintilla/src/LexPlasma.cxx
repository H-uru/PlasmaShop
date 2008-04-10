// Scintilla source code edit control
/** @file LexPlasma.cxx
 ** Lexers for generic Plasma files (Hex, Fni).
 **/
// Copyright 1998-2008 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

static inline bool AtEOL(Accessor &styler, unsigned int i) {
	return (styler[i] == '\n') ||
	       ((styler[i] == '\r') && (styler.SafeGetCharAt(i + 1) != '\n'));
}

static inline bool IsOperator(char ch) {
    return (ch == '=' || ch == '[' || ch == ']' || ch == '{' ||
            ch == '}' || ch == '(' || ch == ')' || ch == ';' ||
            ch == '+' || ch == '-' || ch == '/' || ch == '*');
}

static bool Is0To9(char ch) {
	return (ch >= '0') && (ch <= '9');
}

static void ColouriseFniLine(
    char *lineBuffer,
    unsigned int lengthLine,
    unsigned int startLine,
    unsigned int endPos,
    Accessor &styler) {

	unsigned int i = 0;
    unsigned int area = 0;
    unsigned int state = SCE_FNI_DEFAULT;
	while ((i < lengthLine) && isspacechar(lineBuffer[i])) {
        // Skip initial spaces
        styler.ColourTo(startLine + i, SCE_FNI_DEFAULT);
		i++;
    }
    while (i < lengthLine) {
		if (lineBuffer[i] == '#') {
			styler.ColourTo(endPos, SCE_FNI_COMMENT);
            break;
		} else if (lineBuffer[i] == '.' && area == 0) {
            styler.ColourTo(startLine + i - 1, SCE_FNI_GROUP);
            styler.ColourTo(startLine + i, SCE_FNI_OPERATOR);
        } else if (isspacechar(lineBuffer[i]) && area == 0) {
		    styler.ColourTo(startLine + i - 1, SCE_FNI_COMMAND);
		    styler.ColourTo(startLine + i, SCE_FNI_DEFAULT);
            area = 1;
        } else if (lineBuffer[i] == '"' && area == 1) {
            if (state == SCE_FNI_STRING) {
                styler.ColourTo(startLine + i, SCE_FNI_STRING);
                state = SCE_FNI_DEFAULT;
            } else {
                styler.ColourTo(startLine + i - 1, state);
                state = SCE_FNI_STRING;
            }
        } else if (Is0To9(lineBuffer[i]) && area == 1) {
            if (state == SCE_FNI_DEFAULT)
                state = SCE_FNI_NUMBER;
        } else if (isspacechar(lineBuffer[i]) && area == 1) {
            if (state == SCE_FNI_NUMBER || state == SCE_FNI_IDENTIFIER) {
                styler.ColourTo(startLine + i - 1, state);
                state = SCE_FNI_DEFAULT;
            } else {
                styler.ColourTo(startLine + i, state);
            }
        } else if (IsOperator(lineBuffer[i]) && area == 1) {
            if (state != SCE_FNI_STRING) {
                styler.ColourTo(startLine + i - 1, state);
                styler.ColourTo(startLine + i, SCE_FNI_OPERATOR);
                state = SCE_FNI_DEFAULT;
            }
        } else if (lineBuffer[i] == '.' && area == 1) {
            if (state != SCE_FNI_STRING && state != SCE_FNI_NUMBER) {
                if ((i+1) < lengthLine && Is0To9(lineBuffer[i+1])) {
                    state = SCE_FNI_NUMBER;
                } else {
                    styler.ColourTo(startLine + i - 1, state);
                    styler.ColourTo(startLine + i, SCE_FNI_OPERATOR);
                }
            }
        } else if (area == 1) {
            if (state != SCE_FNI_STRING)
                state = SCE_FNI_IDENTIFIER;
        }
        i++;
	}
}

static void ColouriseFniDoc(
    unsigned int startPos,
    int length,
    int /*initStyle*/,
    WordList *[],
    Accessor &styler) {

	char lineBuffer[1024];
	styler.StartAt(startPos);
	styler.StartSegment(startPos);
	unsigned int linePos = 0;
	unsigned int startLine = startPos;
	for (unsigned int i = startPos; i < startPos + length; i++) {
		lineBuffer[linePos++] = styler[i];
		if (AtEOL(styler, i) || (linePos >= sizeof(lineBuffer) - 1)) {
			// End of line (or of line buffer) met, colourise it
			lineBuffer[linePos] = '\0';
			ColouriseFniLine(lineBuffer, linePos, startLine, i, styler);
			linePos = 0;
			startLine = i + 1;
		}
	}
	if (linePos > 0) {	// Last line does not have ending characters
		ColouriseFniLine(lineBuffer, linePos, startLine, startPos + length - 1, styler);
	}
}

static void ColouriseHexLine(
    char *lineBuffer,
    unsigned int lengthLine,
    unsigned int startLine,
    unsigned int endPos,
    Accessor &styler) {

	unsigned int i = 0;
	while ((i < lengthLine) && isspacechar(lineBuffer[i]))	// Skip initial spaces
		i++;
	if (i < lengthLine) {
		if (lineBuffer[i] == '#' || lineBuffer[i] == '!' || lineBuffer[i] == ';') {
			styler.ColourTo(endPos, SCE_PROPS_COMMENT);
		} else if (lineBuffer[i] == '[') {
			styler.ColourTo(endPos, SCE_PROPS_SECTION);
		} else if (lineBuffer[i] == '@') {
			styler.ColourTo(startLine + i, SCE_PROPS_DEFVAL);
			if (lineBuffer[++i] == '=')
				styler.ColourTo(startLine + i, SCE_PROPS_ASSIGNMENT);
			styler.ColourTo(endPos, SCE_PROPS_DEFAULT);
		} else {
			// Search for the '=' character
			while ((i < lengthLine) && (lineBuffer[i] != '='))
				i++;
			if ((i < lengthLine) && (lineBuffer[i] == '=')) {
				styler.ColourTo(startLine + i - 1, SCE_PROPS_KEY);
				styler.ColourTo(startLine + i, SCE_PROPS_ASSIGNMENT);
				styler.ColourTo(endPos, SCE_PROPS_DEFAULT);
			} else {
				styler.ColourTo(endPos, SCE_PROPS_DEFAULT);
			}
		}
	} else {
		styler.ColourTo(endPos, SCE_PROPS_DEFAULT);
	}
}

static void ColouriseHexDoc(unsigned int startPos, int length, int, WordList *[], Accessor &styler) {
	char lineBuffer[1024];
	styler.StartAt(startPos);
	styler.StartSegment(startPos);
	unsigned int linePos = 0;
	unsigned int startLine = startPos;
	for (unsigned int i = startPos; i < startPos + length; i++) {
		lineBuffer[linePos++] = styler[i];
		if (AtEOL(styler, i) || (linePos >= sizeof(lineBuffer) - 1)) {
			// End of line (or of line buffer) met, colourise it
			lineBuffer[linePos] = '\0';
			ColouriseHexLine(lineBuffer, linePos, startLine, i, styler);
			linePos = 0;
			startLine = i + 1;
		}
	}
	if (linePos > 0) {	// Last line does not have ending characters
		ColouriseHexLine(lineBuffer, linePos, startLine, startPos + length - 1, styler);
	}
}

static void FoldHexDoc(unsigned int startPos, int length, int, WordList *[], Accessor &styler) {
	bool foldCompact = styler.GetPropertyInt("fold.compact", 1) != 0;

	unsigned int endPos = startPos + length;
	int visibleChars = 0;
	int lineCurrent = styler.GetLine(startPos);

	char chNext = styler[startPos];
	int styleNext = styler.StyleAt(startPos);
	bool headerPoint = false;
	int lev;

	for (unsigned int i = startPos; i < endPos; i++) {
		char ch = chNext;
		chNext = styler[i+1];

		int style = styleNext;
		styleNext = styler.StyleAt(i + 1);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');

		if (style == SCE_PROPS_SECTION) {
			headerPoint = true;
		}

		if (atEOL) {
			lev = SC_FOLDLEVELBASE;

			if (lineCurrent > 0) {
				int levelPrevious = styler.LevelAt(lineCurrent - 1);

				if (levelPrevious & SC_FOLDLEVELHEADERFLAG) {
					lev = SC_FOLDLEVELBASE + 1;
				} else {
					lev = levelPrevious & SC_FOLDLEVELNUMBERMASK;
				}
			}

			if (headerPoint) {
				lev = SC_FOLDLEVELBASE;
			}
			if (visibleChars == 0 && foldCompact)
				lev |= SC_FOLDLEVELWHITEFLAG;

			if (headerPoint) {
				lev |= SC_FOLDLEVELHEADERFLAG;
			}
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}

			lineCurrent++;
			visibleChars = 0;
			headerPoint = false;
		}
		if (!isspacechar(ch))
			visibleChars++;
	}

	if (lineCurrent > 0) {
		int levelPrevious = styler.LevelAt(lineCurrent - 1);
		if (levelPrevious & SC_FOLDLEVELHEADERFLAG) {
			lev = SC_FOLDLEVELBASE + 1;
		} else {
			lev = levelPrevious & SC_FOLDLEVELNUMBERMASK;
		}
	} else {
		lev = SC_FOLDLEVELBASE;
	}
	int flagsNext = styler.LevelAt(lineCurrent);
	styler.SetLevel(lineCurrent, lev | flagsNext & ~SC_FOLDLEVELNUMBERMASK);
}

static const char * const hexWordListDesc[] = {
    /* TODO */
	0
};

static const char * const emptyWordListDesc[] = {
	0
};

LexerModule lmHex(SCLEX_HEX, ColouriseHexDoc, "hex", FoldHexDoc, hexWordListDesc);
LexerModule lmFni(SCLEX_FNI, ColouriseFniDoc, "fni", 0, emptyWordListDesc);
