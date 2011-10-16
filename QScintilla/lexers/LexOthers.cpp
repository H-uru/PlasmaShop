// Scintilla source code edit control
/** @file LexOthers.cxx
 ** Lexers for batch files, diff results, properties files, make files and error lists.
 ** Also lexer for LaTeX documents.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

static bool strstart(const char *haystack, const char *needle) {
	return strncmp(haystack, needle, strlen(needle)) == 0;
}

static bool Is0To9(char ch) {
	return (ch >= '0') && (ch <= '9');
}

static bool Is1To9(char ch) {
	return (ch >= '1') && (ch <= '9');
}

static inline bool AtEOL(Accessor &styler, unsigned int i) {
	return (styler[i] == '\n') ||
	       ((styler[i] == '\r') && (styler.SafeGetCharAt(i + 1) != '\n'));
}

static inline bool isassignchar(unsigned char ch) {
	return (ch == '=') || (ch == ':');
}

static void ColourisePropsLine(
    char *lineBuffer,
    unsigned int lengthLine,
    unsigned int startLine,
    unsigned int endPos,
    Accessor &styler,
    bool allowInitialSpaces) {

	unsigned int i = 0;
	if (allowInitialSpaces) {
		while ((i < lengthLine) && isspacechar(lineBuffer[i]))	// Skip initial spaces
			i++;
	} else {
		if (isspacechar(lineBuffer[i])) // don't allow initial spaces
			i = lengthLine;
	}

	if (i < lengthLine) {
		if (lineBuffer[i] == '#' || lineBuffer[i] == '!' || lineBuffer[i] == ';') {
			styler.ColourTo(endPos, SCE_PROPS_COMMENT);
		} else if (lineBuffer[i] == '[') {
			styler.ColourTo(endPos, SCE_PROPS_SECTION);
		} else if (lineBuffer[i] == '@') {
			styler.ColourTo(startLine + i, SCE_PROPS_DEFVAL);
			if (isassignchar(lineBuffer[i++]))
				styler.ColourTo(startLine + i, SCE_PROPS_ASSIGNMENT);
			styler.ColourTo(endPos, SCE_PROPS_DEFAULT);
		} else {
			// Search for the '=' character
			while ((i < lengthLine) && !isassignchar(lineBuffer[i]))
				i++;
			if ((i < lengthLine) && isassignchar(lineBuffer[i])) {
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

static void ColourisePropsDoc(unsigned int startPos, int length, int, WordList *[], Accessor &styler) {
	char lineBuffer[1024];
	styler.StartAt(startPos);
	styler.StartSegment(startPos);
	unsigned int linePos = 0;
	unsigned int startLine = startPos;

	// property lexer.props.allow.initial.spaces
	//	For properties files, set to 0 to style all lines that start with whitespace in the default style.
	//	This is not suitable for SciTE .properties files which use indentation for flow control but
	//	can be used for RFC2822 text where indentation is used for continuation lines.
	bool allowInitialSpaces = styler.GetPropertyInt("lexer.props.allow.initial.spaces", 1) != 0;

	for (unsigned int i = startPos; i < startPos + length; i++) {
		lineBuffer[linePos++] = styler[i];
		if (AtEOL(styler, i) || (linePos >= sizeof(lineBuffer) - 1)) {
			// End of line (or of line buffer) met, colourise it
			lineBuffer[linePos] = '\0';
			ColourisePropsLine(lineBuffer, linePos, startLine, i, styler, allowInitialSpaces);
			linePos = 0;
			startLine = i + 1;
		}
	}
	if (linePos > 0) {	// Last line does not have ending characters
		ColourisePropsLine(lineBuffer, linePos, startLine, startPos + length - 1, styler, allowInitialSpaces);
	}
}

// adaption by ksc, using the "} else {" trick of 1.53
// 030721
static void FoldPropsDoc(unsigned int startPos, int length, int, WordList *[], Accessor &styler) {
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
	styler.SetLevel(lineCurrent, lev | (flagsNext & ~SC_FOLDLEVELNUMBERMASK));
}


static void ColouriseFniLine(
    char *lineBuffer,
    unsigned int lengthLine,
    unsigned int startLine,
    unsigned int endPos,
    Accessor &styler) {

	unsigned int i = 0;
	while ((i < lengthLine) && isspacechar(lineBuffer[i]))	// Skip initial spaces
		i++;
	if (i < lengthLine) {
		if (lineBuffer[i] == '#') {
			styler.ColourTo(endPos, SCE_FNI_COMMENT);
		} else {
			// Colo(u)rize the console command
			while (i < lengthLine) {
				if (lineBuffer[i] == '.') {
					styler.ColourTo(startLine + i - 1, SCE_FNI_GROUP);
					styler.ColourTo(startLine + i, SCE_FNI_OPERATOR);
				} else if (isspacechar(lineBuffer[i])) {
					styler.ColourTo(startLine + i - 1, SCE_FNI_COMMAND);
					break;
				}
				i++;
			}
			// The argument is just default -- for now
			styler.ColourTo(endPos, SCE_FNI_DEFAULT);
		}
	} else {
		styler.ColourTo(endPos, SCE_FNI_DEFAULT);
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
        if (lineBuffer[i] == '#') {
            styler.ColourTo(endPos, SCE_PROPS_COMMENT);
        } else if (lineBuffer[i] == '[') {
            styler.ColourTo(endPos, SCE_PROPS_SECTION);
        } else {
            // Search for the '=' character
            unsigned int lnstart = i;
            while ((i < lengthLine) && (lineBuffer[i] != '='))
                i++;
            if ((i < lengthLine) && (lineBuffer[i] == '=')) {
                styler.ColourTo(startLine + i - 1, SCE_PROPS_KEY);
                styler.ColourTo(startLine + i, SCE_PROPS_ASSIGNMENT);
            } else {
                i = lnstart;
            }

            // Colourize the value of the assignment
            // Not error-proof, but should be good enough
            while (i < lengthLine) {
                if (lineBuffer[i] == '#') {
                    // In-line comment, goes to end of line
                    styler.ColourTo(endPos, SCE_PROPS_COMMENT);
                    i = lengthLine;
                } else if (Is0To9(lineBuffer[i])) {
                    // Number
                    while (Is0To9(lineBuffer[i]))
                        i++;
                    styler.ColourTo(startLine + i - 1, SCE_HEX_NUMBER);
                } else if (lineBuffer[i] == '"') {
                    // String
                    i++;
                    while ((i < lengthLine) && (lineBuffer[i] != '"'))
                        i++;
                    styler.ColourTo(startLine + i - 1, SCE_HEX_STRING);
                } else {
                    // Check for keywords...
                    // They're hard-coded; deal with it :(
                    if (styler.Match(startLine + i, "target")) {
                        i += strlen("target");
                        styler.ColourTo(startLine + i - 1, SCE_HEX_KEYWORD);
                    } else if (styler.Match(startLine + i, "ease")) {
                        i += strlen("ease");
                        styler.ColourTo(startLine + i - 1, SCE_HEX_KEYWORD);
                    } else if (styler.Match(startLine + i, "Sec")) {
                        i += strlen("Sec");
                        styler.ColourTo(startLine + i - 1, SCE_HEX_KEYWORD);
                    } else if (styler.Match(startLine + i, "sec")) {
                        i += strlen("sec");
                        styler.ColourTo(startLine + i - 1, SCE_HEX_KEYWORD);
                    } else {
                        // Nothing matched, just move along
                        styler.ColourTo(startLine + i, SCE_PROPS_DEFAULT);
                        i++;
                    }
                }
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
	unsigned int endPos = startPos + length;
	int lineCurrent = styler.GetLine(startPos);

	char chNext = styler[startPos];
	int styleNext = styler.StyleAt(startPos);
	int sectionDelta = 0;
	int lev;

	for (unsigned int i = startPos; i < endPos; i++) {
		char ch = chNext;
		chNext = styler[i+1];

		int style = styleNext;
		styleNext = styler.StyleAt(i + 1);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');

		if (style == SCE_PROPS_SECTION) {
			if (!sectionDelta)
				sectionDelta = styler.Match(i + 1, "End") ? -1 : 1;
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

			if (sectionDelta == 1) {
				lev = SC_FOLDLEVELBASE;
			} else if (sectionDelta == -1) {
				lev = SC_FOLDLEVELBASE | SC_FOLDLEVELWHITEFLAG;
			}

			if (sectionDelta == 1) {
				lev |= SC_FOLDLEVELHEADERFLAG;
			}
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}

			lineCurrent++;
			sectionDelta = 0;
		}
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

static const char * const emptyWordListDesc[] = {
	0
};

static void ColouriseNullDoc(unsigned int startPos, int length, int, WordList *[],
                            Accessor &styler) {
	// Null language means all style bytes are 0 so just mark the end - no need to fill in.
	if (length > 0) {
		styler.StartAt(startPos + length - 1);
		styler.StartSegment(startPos + length - 1);
		styler.ColourTo(startPos + length - 1, 0);
	}
}

LexerModule lmProps(SCLEX_PROPERTIES, ColourisePropsDoc, "props", FoldPropsDoc, emptyWordListDesc);
LexerModule lmHex(SCLEX_HEX, ColouriseHexDoc, "hex", FoldHexDoc, emptyWordListDesc);
LexerModule lmFni(SCLEX_FNI, ColouriseFniDoc, "fni", 0, emptyWordListDesc);
LexerModule lmNull(SCLEX_NULL, ColouriseNullDoc, "null");
