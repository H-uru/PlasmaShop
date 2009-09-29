[Setup]
OutputDir=.
OutputBaseFilename=Setup-PS30
AppName=PlasmaShop
AppVerName=PlasmaShop 3.0
LicenseFile=..\COPYING
ChangesAssociations=true
DefaultDirName={pf}\PlasmaShop
AllowNoIcons=true
DefaultGroupName=PlasmaShop 3.0
AppVersion=3.0
AppID={{BF6C4F27-8C68-4DE7-8D65-C637CABD259B}
UninstallDisplayIcon={app}\PrpShop.exe
UninstallDisplayName=PlasmaShop

[Types]
Name: Full; Description: Full Install
Name: Custom; Description: Custom; Flags: iscustom

[Components]
Name: PlasmaShop; Description: PlasmaShop 3.0; Types: Custom Full
Name: PrpShop; Description: PrpShop 1.0; Types: Custom Full
Name: VaultShop; Description: VaultShop 1.0; Types: Custom Full

[Files]
Source: ..\bin\Plasma.dll; DestDir: {app}
Source: ..\bin\zlib1.dll; DestDir: {app}
Source: ..\bin\PlasmaShop.exe; DestDir: {app}; Components: PlasmaShop
Source: ..\bin\PrpShop.exe; DestDir: {app}; Components: PrpShop
Source: ..\bin\VaultShop.exe; DestDir: {app}; Components: VaultShop
Source: ..\bin\QtCore4.dll; DestDir: {app}; Components: " VaultShop PrpShop PlasmaShop"
Source: ..\bin\QtGui4.dll; DestDir: {app}; Components: " VaultShop PrpShop PlasmaShop"
Source: ..\bin\QtOpenGL4.dll; DestDir: {app}; Components: PrpShop

[Tasks]
Name: Associations; Description: Associate Plasma Files with PlasmaShop

[Registry]
Root: HKCR; SubKey: .age; ValueType: string; ValueData: AgeFile; Flags: uninsdeletekey; Tasks: " Associations"; Components: PrpShop
Root: HKCR; SubKey: AgeFile; ValueType: string; ValueData: Plasma Age Descriptor; Flags: uninsdeletekey; Tasks: " Associations"; Components: PrpShop
Root: HKCR; SubKey: AgeFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PrpShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PrpShop
Root: HKCR; Subkey: AgeFile\DefaultIcon; ValueType: string; ValueData: {app}\PrpShop.exe,1; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PrpShop
Root: HKCR; SubKey: .prp; ValueType: string; ValueData: PrpFile; Flags: uninsdeletekey; Tasks: " Associations"; Components: PrpShop
Root: HKCR; SubKey: PrpFile; ValueType: string; ValueData: Plasma Resource Page; Flags: uninsdeletekey; Tasks: " Associations"; Components: PrpShop
Root: HKCR; SubKey: PrpFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PrpShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PrpShop
Root: HKCR; Subkey: PrpFile\DefaultIcon; ValueType: string; ValueData: {app}\PrpShop.exe,2; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PrpShop
Root: HKCR; SubKey: .p2f; ValueType: string; ValueData: P2fFile; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: P2fFile; ValueType: string; ValueData: Plasma Font; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: P2fFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; Subkey: P2fFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShop.exe,3; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: .fni; ValueType: string; ValueData: FniFile; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: FniFile; ValueType: string; ValueData: Plasma Console Script; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: FniFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; Subkey: FniFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShop.exe,2; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: .sdl; ValueType: string; ValueData: PlasmaSdlFile; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: PlasmaSdlFile; ValueType: string; ValueData: Plasma State Descriptors; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: PlasmaSdlFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; Subkey: PlasmaSdlFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShop.exe,5; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: .elf; ValueType: string; ValueData: ElfFile; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: ElfFile; ValueType: string; ValueData: Encrypted Log; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: ElfFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; Subkey: ElfFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShop.exe,2; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: .pak; ValueType: string; ValueData: PlasmaPakFile; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: PlasmaPakFile; ValueType: string; ValueData: Python Package; Flags: uninsdeletekey; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; SubKey: PlasmaPakFile\Shell\Open\Command; ValueType: string; ValueData: """{app}\PlasmaShop.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop
Root: HKCR; Subkey: PlasmaPakFile\DefaultIcon; ValueType: string; ValueData: {app}\PlasmaShop.exe,4; Flags: uninsdeletevalue; Tasks: " Associations"; Components: PlasmaShop

[Icons]
Name: {group}\PlasmaShop; Filename: {app}\PlasmaShop.exe; WorkingDir: {app}; IconFilename: {app}\PlasmaShop.exe; IconIndex: 0; Components: PlasmaShop
Name: {group}\PrpShop; Filename: {app}\PrpShop.exe; WorkingDir: {app}; IconFilename: {app}\PrpShop.exe; IconIndex: 0; Components: PrpShop
Name: {group}\VaultShop; Filename: {app}\VaultShop.exe; WorkingDir: {app}; IconFilename: {app}\VaultShop.exe; Components: " VaultShop"
