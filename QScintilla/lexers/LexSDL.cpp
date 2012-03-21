// Scintilla source code edit control
/** @file LexSDL.cxx
 ** Lexer for Plasma SDL Formats
 **/

// Scintilla Copyright 1998-2005 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

// LexSDL Copyright 2006 by Michael Hansen <zrax@comcast.net>
// Code based on LexCpp by Neil Hodgson
// Plasma, Uru, Myst are all Copyrights of Cyan Worlds, Inc.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

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

static inline bool IsAWordChar(int ch) {
	return (ch < 0x80) && (isalnum(ch) || ch == '.' || ch == '_');
}

static inline bool IsAWordStart(int ch) {
	return (ch < 0x80) && (isalpha(ch) || ch == '_');
}

static void ColouriseSDLDoc(unsigned int startPos, int length, int initStyle, WordList *keywordlists[],
                            Accessor &styler) {

	WordList &keywords = *keywordlists[0];
	WordList &keywords2 = *keywordlists[1];
	WordList &keywords3 = *keywordlists[2];

	int chPrevNonWhite = ' ';
	int visibleChars = 0;
	bool continuationLine = false;

	StyleContext sc(startPos, length, initStyle, styler);

	for (; sc.More(); sc.Forward()) {

		if (sc.atLineStart) {
			if (sc.state == SCE_SDL_STRING) {
				// Prevent SCE_SDL_STRINGEOL from leaking back to previous line which 
				// ends with a line continuation by locking in the state upto this position.
				sc.SetState(SCE_SDL_STRING);
			}
			// Reset states to begining of colourise so no surprises
			// if different sets of lines lexed.
			visibleChars = 0;
		}

		// Handle line continuation generically.
		if (sc.ch == '\\') {
			if (sc.chNext == '\n' || sc.chNext == '\r') {
				sc.Forward();
				if (sc.ch == '\r' && sc.chNext == '\n') {
					sc.Forward();
				}
				continuationLine = true;
				continue;
			}
		}

		// Determine if the current state should terminate.
		switch (sc.state) {
			case SCE_SDL_OPERATOR:
				sc.SetState(SCE_SDL_DEFAULT);
				break;
			case SCE_SDL_NUMBER:
				// We accept almost anything because of hex. and number suffixes
				if (!IsAWordChar(sc.ch)) {
					sc.SetState(SCE_SDL_DEFAULT);
				}
				break;
			case SCE_SDL_IDENTIFIER:
				if (!IsAWordChar(sc.ch) || (sc.ch == '.')) {
					char s[1000];
					sc.GetCurrent(s, sizeof(s));
					if (keywords.InList(s)) {
						sc.ChangeState(SCE_SDL_WORD);
					} else if (keywords2.InList(s)) {
						sc.ChangeState(SCE_SDL_WORD2);
					} else if (keywords3.InList(s)) {
						sc.ChangeState(SCE_SDL_WORD3);
					}
					sc.SetState(SCE_SDL_DEFAULT);
				}
				break;
			case SCE_SDL_COMMENT:
				if (sc.Match('*', '/')) {
					sc.Forward();
					sc.ForwardSetState(SCE_SDL_DEFAULT);
				}
				break;
			case SCE_SDL_COMMENTLINE:
			case SCE_SDL_COMMENTBLOCK:
				if (sc.atLineStart) {
					sc.SetState(SCE_SDL_DEFAULT);
				}
				break;
			case SCE_SDL_STRING:
				if (sc.atLineEnd) {
					sc.ChangeState(SCE_SDL_STRINGEOL);
				} else if (sc.ch == '\\') {
					if (sc.chNext == '\"' || sc.chNext == '\'' || sc.chNext == '\\') {
						sc.Forward();
					}
				} else if (sc.ch == '\"') {
					sc.ForwardSetState(SCE_SDL_DEFAULT);
				}
				break;
			case SCE_SDL_CHARACTER:
				if (sc.atLineEnd) {
					sc.ChangeState(SCE_SDL_STRINGEOL);
				} else if (sc.ch == '\\') {
					if (sc.chNext == '\"' || sc.chNext == '\'' || sc.chNext == '\\') {
						sc.Forward();
					}
				} else if (sc.ch == '\'') {
					sc.ForwardSetState(SCE_SDL_DEFAULT);
				}
				break;
			case SCE_SDL_STRINGEOL:
				if (sc.atLineStart) {
					sc.SetState(SCE_SDL_DEFAULT);
				}
				break;
		}

		// Determine if a new state should be entered.
		if (sc.state == SCE_SDL_DEFAULT) {
			if (IsADigit(sc.ch) || (sc.ch == '.' && IsADigit(sc.chNext))) {
				sc.SetState(SCE_SDL_NUMBER);
			} else if (IsAWordStart(sc.ch)) {
				sc.SetState(SCE_SDL_IDENTIFIER);
			} else if (sc.ch == '#') {
				sc.SetState(sc.chNext == '#' ? SCE_SDL_COMMENTBLOCK : SCE_SDL_COMMENTLINE);
			} else if (sc.Match('/', '*')) {
				sc.SetState(SCE_SDL_COMMENT);
				sc.Forward();	// Eat the * so it isn't used for the end of the comment
			} else if (sc.Match('/', '/')) {
				sc.SetState(SCE_SDL_COMMENTLINE);
			} else if (sc.ch == '\"') {
				sc.SetState(SCE_SDL_STRING);
			} else if (sc.ch == '\'') {
				sc.SetState(SCE_SDL_CHARACTER);
			} else if (isoperator(static_cast<char>(sc.ch))) {
				sc.SetState(SCE_SDL_OPERATOR);
			}
		}

		if (!IsASpace(sc.ch)) {
			chPrevNonWhite = sc.ch;
			visibleChars++;
		}
		continuationLine = false;
	}
	sc.Complete();
}

static bool IsCommentLine(int line, Accessor &styler) {
	int pos = styler.LineStart(line);
	int eol_pos = styler.LineStart(line + 1) - 1;
	for (int i = pos; i < eol_pos; i++) {
		char ch = styler[i];
		char chNext = styler.SafeGetCharAt(i + 1);
		if ((ch == '#') || (ch == '/' && chNext == '/'))
			return true;
		else if (ch != ' ' && ch != '\t')
			return false;
	}
	return false;
}

static void FoldSDLDoc(unsigned int startPos, int length, int initStyle,
                            WordList *[], Accessor &styler) {
	bool foldComment = styler.GetPropertyInt("fold.comment") != 0;
	bool foldCompact = styler.GetPropertyInt("fold.compact", 1) != 0;
	unsigned int endPos = startPos + length;
	int visibleChars = 0;
	int lineCurrent = styler.GetLine(startPos);
	int levelCurrent = SC_FOLDLEVELBASE;
	if (lineCurrent > 0)
		levelCurrent = styler.LevelAt(lineCurrent-1) >> 16;
	int levelNext = levelCurrent;
	char chNext = styler[startPos];
	int styleNext = styler.StyleAt(startPos);
	int style = initStyle;
	int prevComment = 0;
	if (lineCurrent > 0)
		prevComment = foldComment && IsCommentLine(lineCurrent - 1, styler);
	//char inComment = ' ';

	for (unsigned int i = startPos; i < endPos; i++) {
		char ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);
		int stylePrev = style;
		style = styleNext;
		styleNext = styler.StyleAt(i + 1);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (foldComment && (style == SCE_SDL_COMMENT)) {
			if (stylePrev != SCE_SDL_COMMENT) {
				levelNext++;
			} else if (!atEOL && (styleNext != SCE_SDL_COMMENT)) {
				levelNext--;
			}
		}
		//if (foldComment && (style == SCE_SDL_COMMENTLINE)) {
		//	if (((ch == '#') || (ch == '/' && chNext == '/')) && (inComment == ' ')) {
		//		levelNext++;
		//		inComment = ch;
		//	}
		//	if ((atEOL) && (chNext != inComment)) {
		//		levelNext--;
		//		inComment = ' ';
		//	}
		//}
		if (style == SCE_SDL_OPERATOR) {
			if (ch == '{') {
				levelNext++;
			} else if (ch == '}') {
				levelNext--;
			}
		}
		if (atEOL) {
			int levelUse = levelCurrent;
			int lev = levelUse | levelNext << 16;
			if (visibleChars == 0 && foldCompact)
				lev |= SC_FOLDLEVELWHITEFLAG;
			if (levelUse < levelNext)
				lev |= SC_FOLDLEVELHEADERFLAG;
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelCurrent = levelNext;
			visibleChars = 0;
		}
		if (!isspacechar(ch))
			visibleChars++;
	}
}

static const char * const sdlWordLists[] = {
            "Keywords",
            "Var Types",
			"Special Types",
            0,
        };

LexerModule lmSDL(SCLEX_SDL, ColouriseSDLDoc, "sdl", FoldSDLDoc, sdlWordLists);
