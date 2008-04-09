CXX = g++
CXXFLAGSSHARED = -O2 -fPIC -Wall -D__WX__ -DSCI_LEXER -DLINK_LEXERS -fno-strict-aliasing
CXXFLAGSWX = $(CXXFLAGSSHARED) -Iscintilla/include -Iscintilla/src `wx-config --cppflags`
CXXFLAGSSCI = $(CXXFLAGSSHARED) -Iscintilla/include

OBJDIR=../../../out/wx
OBJFILES=\
	$(OBJDIR)/PlatWX.o \
	$(OBJDIR)/ScintillaWX.o \
	$(OBJDIR)/stc.o \
	$(OBJDIR)/AutoComplete.o \
	$(OBJDIR)/CallTip.o \
	$(OBJDIR)/CellBuffer.o \
	$(OBJDIR)/CharClassify.o \
	$(OBJDIR)/ContractionState.o \
	$(OBJDIR)/Decoration.o \
	$(OBJDIR)/Document.o \
	$(OBJDIR)/DocumentAccessor.o \
	$(OBJDIR)/Editor.o \
	$(OBJDIR)/ExternalLexer.o \
	$(OBJDIR)/Indicator.o \
	$(OBJDIR)/KeyMap.o \
	$(OBJDIR)/KeyWords.o \
	$(OBJDIR)/LexAbaqus.o \
	$(OBJDIR)/LexAda.o \
	$(OBJDIR)/LexAPDL.o \
	$(OBJDIR)/LexAsm.o \
	$(OBJDIR)/LexAsn1.o \
	$(OBJDIR)/LexASY.o \
	$(OBJDIR)/LexAU3.o \
	$(OBJDIR)/LexAVE.o \
	$(OBJDIR)/LexBaan.o \
	$(OBJDIR)/LexBash.o \
	$(OBJDIR)/LexBasic.o \
	$(OBJDIR)/LexBullant.o \
	$(OBJDIR)/LexCaml.o \
	$(OBJDIR)/LexCLW.o \
	$(OBJDIR)/LexCmake.o \
	$(OBJDIR)/LexConf.o \
	$(OBJDIR)/LexCPP.o \
	$(OBJDIR)/LexCrontab.o \
	$(OBJDIR)/LexCsound.o \
	$(OBJDIR)/LexCSS.o \
	$(OBJDIR)/LexD.o \
	$(OBJDIR)/LexEiffel.o \
	$(OBJDIR)/LexErlang.o \
	$(OBJDIR)/LexEScript.o \
	$(OBJDIR)/LexFlagship.o \
	$(OBJDIR)/LexForth.o \
	$(OBJDIR)/LexFortran.o \
	$(OBJDIR)/LexGAP.o \
	$(OBJDIR)/LexGui4Cli.o \
	$(OBJDIR)/LexHaskell.o \
	$(OBJDIR)/LexHTML.o \
	$(OBJDIR)/LexInno.o \
	$(OBJDIR)/LexKix.o \
	$(OBJDIR)/LexLisp.o \
	$(OBJDIR)/LexLout.o \
	$(OBJDIR)/LexLua.o \
	$(OBJDIR)/LexMagik.o \
	$(OBJDIR)/LexMatlab.o \
	$(OBJDIR)/LexMetapost.o \
	$(OBJDIR)/LexMMIXAL.o \
	$(OBJDIR)/LexMPT.o \
	$(OBJDIR)/LexMSSQL.o \
	$(OBJDIR)/LexNsis.o \
	$(OBJDIR)/LexOpal.o \
	$(OBJDIR)/LexOthers.o \
	$(OBJDIR)/LexPascal.o \
	$(OBJDIR)/LexPB.o \
	$(OBJDIR)/LexPerl.o \
	$(OBJDIR)/LexPlasma.o \
	$(OBJDIR)/LexPLM.o \
	$(OBJDIR)/LexPOV.o \
	$(OBJDIR)/LexPowerShell.o \
	$(OBJDIR)/LexProgress.o \
	$(OBJDIR)/LexPS.o \
	$(OBJDIR)/LexPython.o \
	$(OBJDIR)/LexR.o \
	$(OBJDIR)/LexRebol.o \
	$(OBJDIR)/LexRuby.o \
	$(OBJDIR)/LexScriptol.o \
	$(OBJDIR)/LexSDL.o \
	$(OBJDIR)/LexSmalltalk.o \
	$(OBJDIR)/LexSpecman.o \
	$(OBJDIR)/LexSpice.o \
	$(OBJDIR)/LexSQL.o \
	$(OBJDIR)/LexTADS3.o \
	$(OBJDIR)/LexTCL.o \
	$(OBJDIR)/LexTeX.o \
	$(OBJDIR)/LexVB.o \
	$(OBJDIR)/LexVerilog.o \
	$(OBJDIR)/LexVHDL.o \
	$(OBJDIR)/LexYAML.o \
	$(OBJDIR)/LineMarker.o \
	$(OBJDIR)/PositionCache.o \
	$(OBJDIR)/PropSet.o \
	$(OBJDIR)/RESearch.o \
	$(OBJDIR)/RunStyles.o \
	$(OBJDIR)/ScintillaBase.o \
	$(OBJDIR)/Style.o \
	$(OBJDIR)/StyleContext.o \
	$(OBJDIR)/UniConversion.o \
	$(OBJDIR)/ViewStyle.o \
	$(OBJDIR)/WindowAccessor.o \
	$(OBJDIR)/XPM.o

LIB=libwxstc.a

all: $(OBJDIR)/.keep $(LIB)

clean:
	rm -f $(OBJFILES)
	rm -f $(LIB)

$(LIB): $(OBJFILES)
	$(AR) cr $(LIB) $(OBJFILES)

$(OBJDIR)/.keep:
	touch $(OBJDIR)/.keep

$(OBJDIR)/PlatWX.o: PlatWX.h PlatWX.cpp
	$(CXX) $(CXXFLAGSWX) -c PlatWX.cpp -o $@

$(OBJDIR)/ScintillaWX.o: ScintillaWX.h ScintillaWX.cpp
	$(CXX) $(CXXFLAGSWX) -c ScintillaWX.cpp -o $@

$(OBJDIR)/stc.o: stc.h stc.cpp
	$(CXX) $(CXXFLAGSWX) -c stc.cpp -o $@

$(OBJDIR)/AutoComplete.o: scintilla/src/AutoComplete.h scintilla/src/AutoComplete.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/AutoComplete.cxx -o $@

$(OBJDIR)/CallTip.o: scintilla/src/CallTip.h scintilla/src/CallTip.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/CallTip.cxx -o $@

$(OBJDIR)/CellBuffer.o: scintilla/src/CellBuffer.h scintilla/src/CellBuffer.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/CellBuffer.cxx -o $@

$(OBJDIR)/CharClassify.o: scintilla/src/CharClassify.h scintilla/src/CharClassify.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/CharClassify.cxx -o $@

$(OBJDIR)/ContractionState.o: scintilla/src/ContractionState.h scintilla/src/ContractionState.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/ContractionState.cxx -o $@

$(OBJDIR)/Decoration.o: scintilla/src/Decoration.h scintilla/src/Decoration.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/Decoration.cxx -o $@

$(OBJDIR)/Document.o: scintilla/src/Document.h scintilla/src/Document.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/Document.cxx -o $@

$(OBJDIR)/DocumentAccessor.o: scintilla/src/DocumentAccessor.h scintilla/src/DocumentAccessor.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/DocumentAccessor.cxx -o $@

$(OBJDIR)/Editor.o: scintilla/src/Editor.h scintilla/src/Editor.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/Editor.cxx -o $@

$(OBJDIR)/ExternalLexer.o: scintilla/src/ExternalLexer.h scintilla/src/ExternalLexer.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/ExternalLexer.cxx -o $@

$(OBJDIR)/Indicator.o: scintilla/src/Indicator.h scintilla/src/Indicator.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/Indicator.cxx -o $@

$(OBJDIR)/KeyMap.o: scintilla/src/KeyMap.h scintilla/src/KeyMap.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/KeyMap.cxx -o $@

$(OBJDIR)/KeyWords.o: scintilla/src/KeyWords.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/KeyWords.cxx -o $@

$(OBJDIR)/LexAbaqus.o: scintilla/src/LexAbaqus.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexAbaqus.cxx -o $@

$(OBJDIR)/LexAda.o: scintilla/src/LexAda.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexAda.cxx -o $@

$(OBJDIR)/LexAPDL.o: scintilla/src/LexAPDL.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexAPDL.cxx -o $@

$(OBJDIR)/LexAsm.o: scintilla/src/LexAsm.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexAsm.cxx -o $@

$(OBJDIR)/LexAsn1.o: scintilla/src/LexAsn1.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexAsn1.cxx -o $@

$(OBJDIR)/LexASY.o: scintilla/src/LexASY.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexASY.cxx -o $@

$(OBJDIR)/LexAU3.o: scintilla/src/LexAU3.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexAU3.cxx -o $@

$(OBJDIR)/LexAVE.o: scintilla/src/LexAVE.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexAVE.cxx -o $@

$(OBJDIR)/LexBaan.o: scintilla/src/LexBaan.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexBaan.cxx -o $@

$(OBJDIR)/LexBash.o: scintilla/src/LexBash.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexBash.cxx -o $@

$(OBJDIR)/LexBasic.o: scintilla/src/LexBasic.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexBasic.cxx -o $@

$(OBJDIR)/LexBullant.o: scintilla/src/LexBullant.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexBullant.cxx -o $@

$(OBJDIR)/LexCaml.o: scintilla/src/LexCaml.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexCaml.cxx -o $@

$(OBJDIR)/LexCLW.o: scintilla/src/LexCLW.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexCLW.cxx -o $@

$(OBJDIR)/LexCmake.o: scintilla/src/LexCmake.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexCmake.cxx -o $@

$(OBJDIR)/LexConf.o: scintilla/src/LexConf.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexConf.cxx -o $@

$(OBJDIR)/LexCPP.o: scintilla/src/LexCPP.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexCPP.cxx -o $@

$(OBJDIR)/LexCrontab.o: scintilla/src/LexCrontab.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexCrontab.cxx -o $@

$(OBJDIR)/LexCsound.o: scintilla/src/LexCsound.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexCsound.cxx -o $@

$(OBJDIR)/LexCSS.o: scintilla/src/LexCSS.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexCSS.cxx -o $@

$(OBJDIR)/LexD.o: scintilla/src/LexD.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexD.cxx -o $@

$(OBJDIR)/LexEiffel.o: scintilla/src/LexEiffel.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexEiffel.cxx -o $@

$(OBJDIR)/LexErlang.o: scintilla/src/LexErlang.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexErlang.cxx -o $@

$(OBJDIR)/LexEScript.o: scintilla/src/LexEScript.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexEScript.cxx -o $@

$(OBJDIR)/LexFlagship.o: scintilla/src/LexFlagship.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexFlagship.cxx -o $@

$(OBJDIR)/LexForth.o: scintilla/src/LexForth.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexForth.cxx -o $@

$(OBJDIR)/LexFortran.o: scintilla/src/LexFortran.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexFortran.cxx -o $@

$(OBJDIR)/LexGAP.o: scintilla/src/LexGAP.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexGAP.cxx -o $@

$(OBJDIR)/LexGui4Cli.o: scintilla/src/LexGui4Cli.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexGui4Cli.cxx -o $@

$(OBJDIR)/LexHaskell.o: scintilla/src/LexHaskell.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexHaskell.cxx -o $@

$(OBJDIR)/LexHTML.o: scintilla/src/LexHTML.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexHTML.cxx -o $@

$(OBJDIR)/LexInno.o: scintilla/src/LexInno.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexInno.cxx -o $@

$(OBJDIR)/LexKix.o: scintilla/src/LexKix.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexKix.cxx -o $@

$(OBJDIR)/LexLisp.o: scintilla/src/LexLisp.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexLisp.cxx -o $@

$(OBJDIR)/LexLout.o: scintilla/src/LexLout.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexLout.cxx -o $@

$(OBJDIR)/LexLua.o: scintilla/src/LexLua.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexLua.cxx -o $@

$(OBJDIR)/LexMagik.o: scintilla/src/LexMagik.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexMagik.cxx -o $@

$(OBJDIR)/LexMatlab.o: scintilla/src/LexMatlab.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexMatlab.cxx -o $@

$(OBJDIR)/LexMetapost.o: scintilla/src/LexMetapost.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexMetapost.cxx -o $@

$(OBJDIR)/LexMMIXAL.o: scintilla/src/LexMMIXAL.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexMMIXAL.cxx -o $@

$(OBJDIR)/LexMPT.o: scintilla/src/LexMPT.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexMPT.cxx -o $@

$(OBJDIR)/LexMSSQL.o: scintilla/src/LexMSSQL.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexMSSQL.cxx -o $@

$(OBJDIR)/LexNsis.o: scintilla/src/LexNsis.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexNsis.cxx -o $@

$(OBJDIR)/LexOpal.o: scintilla/src/LexOpal.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexOpal.cxx -o $@

$(OBJDIR)/LexOthers.o: scintilla/src/LexOthers.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexOthers.cxx -o $@

$(OBJDIR)/LexPascal.o: scintilla/src/LexPascal.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexPascal.cxx -o $@

$(OBJDIR)/LexPB.o: scintilla/src/LexPB.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexPB.cxx -o $@

$(OBJDIR)/LexPerl.o: scintilla/src/LexPerl.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexPerl.cxx -o $@

$(OBJDIR)/LexPlasma.o: scintilla/src/LexPlasma.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexPlasma.cxx -o $@

$(OBJDIR)/LexPLM.o: scintilla/src/LexPLM.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexPLM.cxx -o $@

$(OBJDIR)/LexPOV.o: scintilla/src/LexPOV.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexPOV.cxx -o $@

$(OBJDIR)/LexPowerShell.o: scintilla/src/LexPowerShell.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexPowerShell.cxx -o $@

$(OBJDIR)/LexProgress.o: scintilla/src/LexProgress.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexProgress.cxx -o $@

$(OBJDIR)/LexPS.o: scintilla/src/LexPS.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexPS.cxx -o $@

$(OBJDIR)/LexPython.o: scintilla/src/LexPython.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexPython.cxx -o $@

$(OBJDIR)/LexR.o: scintilla/src/LexR.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexR.cxx -o $@

$(OBJDIR)/LexRebol.o: scintilla/src/LexRebol.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexRebol.cxx -o $@

$(OBJDIR)/LexRuby.o: scintilla/src/LexRuby.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexRuby.cxx -o $@

$(OBJDIR)/LexScriptol.o: scintilla/src/LexScriptol.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexScriptol.cxx -o $@

$(OBJDIR)/LexSDL.o: scintilla/src/LexSDL.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexSDL.cxx -o $@

$(OBJDIR)/LexSmalltalk.o: scintilla/src/LexSmalltalk.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexSmalltalk.cxx -o $@

$(OBJDIR)/LexSpecman.o: scintilla/src/LexSpecman.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexSpecman.cxx -o $@

$(OBJDIR)/LexSpice.o: scintilla/src/LexSpice.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexSpice.cxx -o $@

$(OBJDIR)/LexSQL.o: scintilla/src/LexSQL.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexSQL.cxx -o $@

$(OBJDIR)/LexTADS3.o: scintilla/src/LexTADS3.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexTADS3.cxx -o $@

$(OBJDIR)/LexTCL.o: scintilla/src/LexTCL.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexTCL.cxx -o $@

$(OBJDIR)/LexTeX.o: scintilla/src/LexTeX.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexTeX.cxx -o $@

$(OBJDIR)/LexVB.o: scintilla/src/LexVB.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexVB.cxx -o $@

$(OBJDIR)/LexVerilog.o: scintilla/src/LexVerilog.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexVerilog.cxx -o $@

$(OBJDIR)/LexVHDL.o: scintilla/src/LexVHDL.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexVHDL.cxx -o $@

$(OBJDIR)/LexYAML.o: scintilla/src/LexYAML.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LexYAML.cxx -o $@

$(OBJDIR)/LineMarker.o: scintilla/src/LineMarker.h scintilla/src/LineMarker.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/LineMarker.cxx -o $@

$(OBJDIR)/PositionCache.o: scintilla/src/PositionCache.h scintilla/src/PositionCache.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/PositionCache.cxx -o $@

$(OBJDIR)/PropSet.o: scintilla/src/PropSet.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/PropSet.cxx -o $@

$(OBJDIR)/RESearch.o: scintilla/src/RESearch.h scintilla/src/RESearch.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/RESearch.cxx -o $@

$(OBJDIR)/RunStyles.o: scintilla/src/RunStyles.h scintilla/src/RunStyles.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/RunStyles.cxx -o $@

$(OBJDIR)/ScintillaBase.o: scintilla/src/ScintillaBase.h scintilla/src/ScintillaBase.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/ScintillaBase.cxx -o $@

$(OBJDIR)/Style.o: scintilla/src/Style.h scintilla/src/Style.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/Style.cxx -o $@

$(OBJDIR)/StyleContext.o: scintilla/src/StyleContext.h scintilla/src/StyleContext.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/StyleContext.cxx -o $@

$(OBJDIR)/UniConversion.o: scintilla/src/UniConversion.h scintilla/src/UniConversion.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/UniConversion.cxx -o $@

$(OBJDIR)/ViewStyle.o: scintilla/src/ViewStyle.h scintilla/src/ViewStyle.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/ViewStyle.cxx -o $@

$(OBJDIR)/WindowAccessor.o: scintilla/src/WindowAccessor.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/WindowAccessor.cxx -o $@

$(OBJDIR)/XPM.o: scintilla/src/XPM.h scintilla/src/XPM.cxx
	$(CXX) $(CXXFLAGSSCI) -c scintilla/src/XPM.cxx -o $@
