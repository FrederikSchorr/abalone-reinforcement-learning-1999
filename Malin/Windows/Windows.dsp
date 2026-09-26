# Microsoft Developer Studio Project File - Name="Windows" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Windows - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Windows.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Windows.mak" CFG="Windows - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Windows - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Windows - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Malin/Windows", EEAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Windows - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386 /out:"Release/Malin.exe"
# SUBTRACT LINK32 /profile
# Begin Custom Build - Copying $(InputPath) to \Malin
InputPath=.\Release\Malin.exe
SOURCE="$(InputPath)"

".\..\Malin.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) .\..\Malin.exe

# End Custom Build

!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/Malin.exe"
# SUBTRACT LINK32 /profile /incremental:no /map
# Begin Custom Build - Copying $(InputPath) to \Malin
InputPath=.\Debug\Malin.exe
SOURCE="$(InputPath)"

".\..\Malin.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) .\..\Malin.exe

# End Custom Build

!ENDIF 

# Begin Target

# Name "Windows - Win32 Release"
# Name "Windows - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Kernel\Aba212\Aba2sim.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Boub\Boub.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Utl\fstreamExt.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\FullBoard.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Game.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GameAsync.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GameNewDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Emil\Hash.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Emil\HashBoard.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Kernel.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\David\Killer.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Move.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Freud\Optimistic_TDlambda.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Freud\Perceptron_n_m_1.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Player.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\PlayerCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerNewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerSelDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Aba212\Sim212.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Boub\SimBoub.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Caesar\SimCaesar.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Caesar\SimCharles.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\David\SimDavid.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Emil\SimEmil.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Freud\SimFreud.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Gunilla\SimGunilla.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Situation.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Kernel\Statistics.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Kernel\Tournament.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\TournDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel\Training.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Windows.cpp
# End Source File
# Begin Source File

SOURCE=.\Windows.rc
# End Source File
# Begin Source File

SOURCE=.\WindowsDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsView.cpp
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

SOURCE=..\Kernel\Boub\Boub.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Utl\fstreamExt.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\FullBoard.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Game.h
# End Source File
# Begin Source File

SOURCE=.\GameAsync.h
# End Source File
# Begin Source File

SOURCE=.\GameNewDlg.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Emil\Hash.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Emil\HashBoard.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\History.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Kernel.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\David\Killer.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Modify.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Move.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Freud\Optimistic_TDlambda.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Freud\Perceptron_n_m_1.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Player.h
# End Source File
# Begin Source File

SOURCE=.\PlayerCombo.h
# End Source File
# Begin Source File

SOURCE=.\PlayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlayerNewDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlayerSelDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Aba212\Sim212.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Boub\SimBoub.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Caesar\SimCaesar.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Caesar\SimCharles.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\David\SimDavid.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Emil\SimEmil.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Freud\SimFreud.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Gunilla\SimGunilla.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\SimHum.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Caesar\SimRandom.h
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
# Begin Source File

SOURCE=..\Kernel\Statistics.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Tournament.h
# End Source File
# Begin Source File

SOURCE=.\TournDlg.h
# End Source File
# Begin Source File

SOURCE=..\Kernel\Training.h
# End Source File
# Begin Source File

SOURCE=.\Windows.h
# End Source File
# Begin Source File

SOURCE=.\WindowsDoc.h
# End Source File
# Begin Source File

SOURCE=.\WindowsView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Windows.ico
# End Source File
# Begin Source File

SOURCE=.\res\Windows.rc2
# End Source File
# Begin Source File

SOURCE=.\res\WindowsDoc.ico
# End Source File
# End Group
# End Target
# End Project
