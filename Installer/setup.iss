[Setup]
OutputDir=.
OutputBaseFilename=Setup-PS30
AppName=PlasmaShop
AppVerName=PlasmaShop 3.0
AppPublisher=Michael Hansen
AppVersion=3.0
AppID={{BF6C4F27-8C68-4DE7-8D65-C637CABD259B}
ChangesAssociations=true
DefaultDirName={pf}\PlasmaShop
AllowNoIcons=true
DefaultGroupName=PlasmaShop 3.0
UninstallDisplayIcon={app}\PrpShop.exe
UninstallDisplayName=PlasmaShop

[Types]
Name: Full; Description: Full Install
Name: Custom; Description: Custom; Flags: iscustom

[Components]
Name: PlasmaShop; Description: PlasmaShop 3.0; Types: Custom Full
Name: PrpShop; Description: PrpShop 1.0; Types: Custom Full
Name: VaultShop; Description: VaultShop 1.1; Types: Custom Full

[Files]
Source: vc_redist.x86.exe; DestDir: {tmp}; Flags: deleteafterinstall ignoreversion
Source: brotlicommon.dll; DestDir: {app}; Flags: ignoreversion
Source: brotlidec.dll; DestDir: {app}; Flags: ignoreversion
Source: bz2.dll; DestDir: {app}; Flags: ignoreversion
Source: freetype.dll; DestDir: {app}; Flags: ignoreversion
Source: harfbuzz.dll; DestDir: {app}; Flags: ignoreversion
Source: HSPlasma.dll; DestDir: {app}; Flags: ignoreversion
Source: icuin69.dll; DestDir: {app}; Flags: ignoreversion
Source: icuuc69.dll; DestDir: {app}; Flags: ignoreversion
Source: icudt69.dll; DestDir: {app}; Flags: ignoreversion
Source: jpeg62.dll; DestDir: {app}; Flags: ignoreversion
Source: libpng16.dll; DestDir: {app}; Flags: ignoreversion
Source: PlasmaShell.dll; DestDir: {app}; Flags: ignoreversion
Source: PlasmaShop.exe; DestDir: {app}; Flags: ignoreversion; Components: PlasmaShop
Source: pcre2-16.dll; DestDir: {app}; Flags: ignoreversion
Source: PrpShop.exe; DestDir: {app}; Flags: ignoreversion; Components: PrpShop
Source: VaultShop.exe; DestDir: {app}; Flags: ignoreversion; Components: VaultShop
Source: Qt5Core.dll; DestDir: {app}; Flags: ignoreversion
Source: Qt5Gui.dll; DestDir: {app}; Flags: ignoreversion
Source: Qt5Network.dll; DestDir: {app}; Flags: ignoreversion
Source: Qt5Widgets.dll; DestDir: {app}; Flags: ignoreversion
Source: Qt5OpenGL.dll; DestDir: {app}; Flags: ignoreversion; Components: PrpShop
Source: KF5SyntaxHighlighting.dll; DestDir: {app}; Flags: ignoreversion
Source: NxCooking.dll; DestDir: {app}; Flags: ignoreversion
Source: zlib1.dll; DestDir: {app}; Flags: ignoreversion
Source: zstd.dll; DestDir: {app}; Flags: ignoreversion
Source: bearer\qgenericbearer.dll; DestDir: {app}\bearer; Flags: ignoreversion
Source: iconengines\qsvgicon.dll; DestDir: {app}\iconengines; Flags: ignoreversion
Source: imageformats\qico.dll; DestDir: {app}\imageformats; Flags: ignoreversion
Source: imageformats\qjpeg.dll; DestDir: {app}\imageformats; Flags: ignoreversion
Source: imageformats\qsvg.dll; DestDir: {app}\imageformats; Flags: ignoreversion
Source: imageformats\qtiff.dll; DestDir: {app}\imageformats; Flags: ignoreversion
Source: imageformats\qwbmp.dll; DestDir: {app}\imageformats; Flags: ignoreversion
Source: platforms\qwindows.dll; DestDir: {app}\platforms; Flags: ignoreversion
Source: styles\qwindowsvistastyle.dll; DestDir: {app}\styles; Flags: ignoreversion
Source: translations\qt_ar.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_bg.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_ca.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_cs.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_da.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_de.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_en.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_es.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_fi.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_fr.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_gd.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_he.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_hu.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_it.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_ja.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_ko.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_lv.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_pl.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_ru.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_sk.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_uk.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: translations\qt_zh_TW.qm; DestDir: {app}\translations; Flags: ignoreversion
Source: ..\COPYING; DestDir: {app}

[InstallDelete]
Name: {app}\icudt52.dll; Type: files
Name: {app}\icuin52.dll; Type: files
Name: {app}\icuuc52.dll; Type: files
Name: {app}\Qt5PrintSupport.dll; Type: files
Name: {app}\platforms\qminimal.dll; Type: files

[Run]
Filename: {tmp}\vc_redist.x86.exe; Parameters: /q

[UninstallDelete]
Name: {app}\PlasmaShop.log; Type: files
Name: {app}\PrpShop.log; Type: files
Name: {app}\VaultShop.log; Type: files
Name: {app}; Type: dirifempty

[Tasks]
Name: Associations; Description: Associate Plasma Files with PlasmaShop

[Registry]
Root: HKCR; SubKey: .age; ValueType: string; ValueData: AgeFile; Flags: uninsdeletekey; Tasks: Associations; Components: PrpShop PlasmaShop
Root: HKCR; SubKey: AgeFile; ValueType: string; ValueData: Plasma Age Descriptor; Flags: uninsdeletekey; Tasks: Associations; Components: PrpShop PlasmaShop
Root: HKCR; SubKey: AgeFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PrpShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PrpShop
Root: HKCR; SubKey: AgeFile\Shell\Edit\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; Subkey: AgeFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShell.dll,0; Flags: uninsdeletevalue; Tasks: Associations; Components: PrpShop PlasmaShop
Root: HKCR; SubKey: .prp; ValueType: string; ValueData: PrpFile; Flags: uninsdeletekey; Tasks: Associations; Components: PrpShop
Root: HKCR; SubKey: PrpFile; ValueType: string; ValueData: Plasma Resource Page; Flags: uninsdeletekey; Tasks: Associations; Components: PrpShop
Root: HKCR; SubKey: PrpFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PrpShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PrpShop
Root: HKCR; Subkey: PrpFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShell.dll,6; Flags: uninsdeletevalue; Tasks: Associations; Components: PrpShop
Root: HKCR; SubKey: .p2f; ValueType: string; ValueData: P2fFile; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: P2fFile; ValueType: string; ValueData: Plasma Font; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: P2fFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; Subkey: P2fFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShell.dll,4; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: .fni; ValueType: string; ValueData: FniFile; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: FniFile; ValueType: string; ValueData: Plasma Console Script; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: FniFile\Shell\Edit\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; Subkey: FniFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShell.dll,2; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: .sdl; ValueType: string; ValueData: PlasmaSdlFile; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: PlasmaSdlFile; ValueType: string; ValueData: Plasma State Descriptors; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: PlasmaSdlFile\Shell\Edit\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; Subkey: PlasmaSdlFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShell.dll,7; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: .elf; ValueType: string; ValueData: ElfFile; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: ElfFile; ValueType: string; ValueData: Encrypted Log; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: ElfFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; Subkey: ElfFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShell.dll,1; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: .loc; ValueType: string; ValueData: LocFile; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: LocFile; ValueType: string; ValueData: Plasma Localization Data; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: LocFile\Shell\Edit\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; Subkey: LocFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShell.dll,8; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: .sub; ValueType: string; ValueData: SubFile; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: SubFile; ValueType: string; ValueData: Plasma Subtitle Data; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: SubFile\Shell\Edit\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; Subkey: SubFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShell.dll,8; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: .hex; ValueType: string; ValueData: HexFile; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: HexFile; ValueType: string; ValueData: Hex Isle Level Data; Flags: uninsdeletekey; Tasks: Associations; Components: PlasmaShop
Root: HKCR; SubKey: HexFile\Shell\Edit\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop
Root: HKCR; Subkey: HexFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShell.dll,3; Flags: uninsdeletevalue; Tasks: Associations; Components: PlasmaShop

[Icons]
Name: {group}\PlasmaShop; Filename: {app}\PlasmaShop.exe; WorkingDir: {app}; IconFilename: {app}\PlasmaShop.exe; IconIndex: 0; Components: PlasmaShop
Name: {group}\PrpShop; Filename: {app}\PrpShop.exe; WorkingDir: {app}; IconFilename: {app}\PrpShop.exe; IconIndex: 0; Components: PrpShop
Name: {group}\VaultShop; Filename: {app}\VaultShop.exe; WorkingDir: {app}; IconFilename: {app}\VaultShop.exe; Components: VaultShop
