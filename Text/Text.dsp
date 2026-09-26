# Microsoft Developer Studio Project File - Name="Text" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Text - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Text.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Text.mak" CFG="Text - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Text - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Text - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Malin/Text", FBAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Text - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "e:\eigene dateien\c\source\utl" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# Begin Custom Build - Copying $(InputPath) to \Malin
InputPath=.\Release\Text.exe
SOURCE="$(InputPath)"

".\..\MalinTxt.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) .\..\MalinTxt.exe

# End Custom Build

!ELSEIF  "$(CFG)" == "Text - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "e:\eigene dateien\c\source\utl" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Copying $(InputPath) to \Malin
InputPath=.\Debug\Text.exe
SOURCE="$(InputPath)"

".\..\MalinTxt.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) .\..\MalinTxt.exe

# End Custom Build

!ENDIF 

# Begin Target

# Name "Text - Win32 Release"
# Name "Text - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Kernel\Aba212\Aba2sim.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Boub\Boub.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Utl\fstreamExt.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Game.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Kernel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\C\Source\Utl\Menudos.c
# End Source File
# Begin Source File

SOURCE=..\Kernel\Move.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Player.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Aba212\Sim212.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Boub\SimBoub.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Situation.cpp
# End Source File
# Begin Source File

SOURCE=.\Text.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Kernel\Aba212\Aba2.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Board.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Utl\fstreamExt.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Game.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\History.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Kernel.h
# End Source File
# Begin Source File

SOURCE=..\..\C\Source\Utl\Menudos.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Move.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Player.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Aba212\Sim212.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\SimHum.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Simulation.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Situation.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\SmartPtr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
