# Microsoft Developer Studio Generated NMAKE File, Based on Windows.dsp
!IF "$(CFG)" == ""
CFG=Windows - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Windows - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Windows - Win32 Release" && "$(CFG)" != "Windows - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Windows - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Malin.exe" ".\..\Malin.exe"


CLEAN :
	-@erase "$(INTDIR)\Aba2sim.obj"
	-@erase "$(INTDIR)\Boub.obj"
	-@erase "$(INTDIR)\fstreamExt.obj"
	-@erase "$(INTDIR)\FullBoard.obj"
	-@erase "$(INTDIR)\Game.obj"
	-@erase "$(INTDIR)\GameAsync.obj"
	-@erase "$(INTDIR)\GameNewDlg.obj"
	-@erase "$(INTDIR)\Hash.obj"
	-@erase "$(INTDIR)\HashBoard.obj"
	-@erase "$(INTDIR)\Kernel.obj"
	-@erase "$(INTDIR)\Killer.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Move.obj"
	-@erase "$(INTDIR)\Optimistic_TDlambda.obj"
	-@erase "$(INTDIR)\Perceptron_n_m_1.obj"
	-@erase "$(INTDIR)\Player.obj"
	-@erase "$(INTDIR)\PlayerCombo.obj"
	-@erase "$(INTDIR)\PlayerDlg.obj"
	-@erase "$(INTDIR)\PlayerNewDlg.obj"
	-@erase "$(INTDIR)\PlayerSelDlg.obj"
	-@erase "$(INTDIR)\Sim212.obj"
	-@erase "$(INTDIR)\SimBoub.obj"
	-@erase "$(INTDIR)\SimCaesar.obj"
	-@erase "$(INTDIR)\SimCharles.obj"
	-@erase "$(INTDIR)\SimDavid.obj"
	-@erase "$(INTDIR)\SimEmil.obj"
	-@erase "$(INTDIR)\SimFreud.obj"
	-@erase "$(INTDIR)\SimGunilla.obj"
	-@erase "$(INTDIR)\Situation.obj"
	-@erase "$(INTDIR)\Statistics.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Tournament.obj"
	-@erase "$(INTDIR)\TournDlg.obj"
	-@erase "$(INTDIR)\Training.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Windows.obj"
	-@erase "$(INTDIR)\Windows.pch"
	-@erase "$(INTDIR)\Windows.res"
	-@erase "$(INTDIR)\WindowsDoc.obj"
	-@erase "$(INTDIR)\WindowsView.obj"
	-@erase "$(OUTDIR)\Malin.exe"
	-@erase "$(OUTDIR)\Malin.map"
	-@erase ".\..\Malin.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Windows.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0xc07 /fo"$(INTDIR)\Windows.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Windows.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Malin.pdb" /map:"$(INTDIR)\Malin.map" /machine:I386 /out:"$(OUTDIR)\Malin.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Aba2sim.obj" \
	"$(INTDIR)\Boub.obj" \
	"$(INTDIR)\fstreamExt.obj" \
	"$(INTDIR)\FullBoard.obj" \
	"$(INTDIR)\Game.obj" \
	"$(INTDIR)\GameAsync.obj" \
	"$(INTDIR)\GameNewDlg.obj" \
	"$(INTDIR)\Hash.obj" \
	"$(INTDIR)\HashBoard.obj" \
	"$(INTDIR)\Kernel.obj" \
	"$(INTDIR)\Killer.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Move.obj" \
	"$(INTDIR)\Optimistic_TDlambda.obj" \
	"$(INTDIR)\Perceptron_n_m_1.obj" \
	"$(INTDIR)\Player.obj" \
	"$(INTDIR)\PlayerCombo.obj" \
	"$(INTDIR)\PlayerDlg.obj" \
	"$(INTDIR)\PlayerNewDlg.obj" \
	"$(INTDIR)\PlayerSelDlg.obj" \
	"$(INTDIR)\Sim212.obj" \
	"$(INTDIR)\SimBoub.obj" \
	"$(INTDIR)\SimCaesar.obj" \
	"$(INTDIR)\SimCharles.obj" \
	"$(INTDIR)\SimDavid.obj" \
	"$(INTDIR)\SimEmil.obj" \
	"$(INTDIR)\SimFreud.obj" \
	"$(INTDIR)\SimGunilla.obj" \
	"$(INTDIR)\Situation.obj" \
	"$(INTDIR)\Statistics.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tournament.obj" \
	"$(INTDIR)\TournDlg.obj" \
	"$(INTDIR)\Training.obj" \
	"$(INTDIR)\Windows.obj" \
	"$(INTDIR)\WindowsDoc.obj" \
	"$(INTDIR)\WindowsView.obj" \
	"$(INTDIR)\Windows.res"

"$(OUTDIR)\Malin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

InputPath=.\Release\Malin.exe
SOURCE="$(InputPath)"

"..\Malin.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy $(InputPath) .\..\Malin.exe
<< 
	

!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Malin.exe" "$(OUTDIR)\Windows.bsc" ".\..\Malin.exe"


CLEAN :
	-@erase "$(INTDIR)\Aba2sim.obj"
	-@erase "$(INTDIR)\Aba2sim.sbr"
	-@erase "$(INTDIR)\Boub.obj"
	-@erase "$(INTDIR)\Boub.sbr"
	-@erase "$(INTDIR)\fstreamExt.obj"
	-@erase "$(INTDIR)\fstreamExt.sbr"
	-@erase "$(INTDIR)\FullBoard.obj"
	-@erase "$(INTDIR)\FullBoard.sbr"
	-@erase "$(INTDIR)\Game.obj"
	-@erase "$(INTDIR)\Game.sbr"
	-@erase "$(INTDIR)\GameAsync.obj"
	-@erase "$(INTDIR)\GameAsync.sbr"
	-@erase "$(INTDIR)\GameNewDlg.obj"
	-@erase "$(INTDIR)\GameNewDlg.sbr"
	-@erase "$(INTDIR)\Hash.obj"
	-@erase "$(INTDIR)\Hash.sbr"
	-@erase "$(INTDIR)\HashBoard.obj"
	-@erase "$(INTDIR)\HashBoard.sbr"
	-@erase "$(INTDIR)\Kernel.obj"
	-@erase "$(INTDIR)\Kernel.sbr"
	-@erase "$(INTDIR)\Killer.obj"
	-@erase "$(INTDIR)\Killer.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\Move.obj"
	-@erase "$(INTDIR)\Move.sbr"
	-@erase "$(INTDIR)\Optimistic_TDlambda.obj"
	-@erase "$(INTDIR)\Optimistic_TDlambda.sbr"
	-@erase "$(INTDIR)\Perceptron_n_m_1.obj"
	-@erase "$(INTDIR)\Perceptron_n_m_1.sbr"
	-@erase "$(INTDIR)\Player.obj"
	-@erase "$(INTDIR)\Player.sbr"
	-@erase "$(INTDIR)\PlayerCombo.obj"
	-@erase "$(INTDIR)\PlayerCombo.sbr"
	-@erase "$(INTDIR)\PlayerDlg.obj"
	-@erase "$(INTDIR)\PlayerDlg.sbr"
	-@erase "$(INTDIR)\PlayerNewDlg.obj"
	-@erase "$(INTDIR)\PlayerNewDlg.sbr"
	-@erase "$(INTDIR)\PlayerSelDlg.obj"
	-@erase "$(INTDIR)\PlayerSelDlg.sbr"
	-@erase "$(INTDIR)\Sim212.obj"
	-@erase "$(INTDIR)\Sim212.sbr"
	-@erase "$(INTDIR)\SimBoub.obj"
	-@erase "$(INTDIR)\SimBoub.sbr"
	-@erase "$(INTDIR)\SimCaesar.obj"
	-@erase "$(INTDIR)\SimCaesar.sbr"
	-@erase "$(INTDIR)\SimCharles.obj"
	-@erase "$(INTDIR)\SimCharles.sbr"
	-@erase "$(INTDIR)\SimDavid.obj"
	-@erase "$(INTDIR)\SimDavid.sbr"
	-@erase "$(INTDIR)\SimEmil.obj"
	-@erase "$(INTDIR)\SimEmil.sbr"
	-@erase "$(INTDIR)\SimFreud.obj"
	-@erase "$(INTDIR)\SimFreud.sbr"
	-@erase "$(INTDIR)\SimGunilla.obj"
	-@erase "$(INTDIR)\SimGunilla.sbr"
	-@erase "$(INTDIR)\Situation.obj"
	-@erase "$(INTDIR)\Situation.sbr"
	-@erase "$(INTDIR)\Statistics.obj"
	-@erase "$(INTDIR)\Statistics.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Tournament.obj"
	-@erase "$(INTDIR)\Tournament.sbr"
	-@erase "$(INTDIR)\TournDlg.obj"
	-@erase "$(INTDIR)\TournDlg.sbr"
	-@erase "$(INTDIR)\Training.obj"
	-@erase "$(INTDIR)\Training.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Windows.obj"
	-@erase "$(INTDIR)\Windows.pch"
	-@erase "$(INTDIR)\Windows.res"
	-@erase "$(INTDIR)\Windows.sbr"
	-@erase "$(INTDIR)\WindowsDoc.obj"
	-@erase "$(INTDIR)\WindowsDoc.sbr"
	-@erase "$(INTDIR)\WindowsView.obj"
	-@erase "$(INTDIR)\WindowsView.sbr"
	-@erase "$(OUTDIR)\Malin.exe"
	-@erase "$(OUTDIR)\Malin.ilk"
	-@erase "$(OUTDIR)\Malin.pdb"
	-@erase "$(OUTDIR)\Windows.bsc"
	-@erase ".\..\Malin.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Windows.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0xc07 /fo"$(INTDIR)\Windows.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Windows.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Aba2sim.sbr" \
	"$(INTDIR)\Boub.sbr" \
	"$(INTDIR)\fstreamExt.sbr" \
	"$(INTDIR)\FullBoard.sbr" \
	"$(INTDIR)\Game.sbr" \
	"$(INTDIR)\GameAsync.sbr" \
	"$(INTDIR)\GameNewDlg.sbr" \
	"$(INTDIR)\Hash.sbr" \
	"$(INTDIR)\HashBoard.sbr" \
	"$(INTDIR)\Kernel.sbr" \
	"$(INTDIR)\Killer.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\Move.sbr" \
	"$(INTDIR)\Optimistic_TDlambda.sbr" \
	"$(INTDIR)\Perceptron_n_m_1.sbr" \
	"$(INTDIR)\Player.sbr" \
	"$(INTDIR)\PlayerCombo.sbr" \
	"$(INTDIR)\PlayerDlg.sbr" \
	"$(INTDIR)\PlayerNewDlg.sbr" \
	"$(INTDIR)\PlayerSelDlg.sbr" \
	"$(INTDIR)\Sim212.sbr" \
	"$(INTDIR)\SimBoub.sbr" \
	"$(INTDIR)\SimCaesar.sbr" \
	"$(INTDIR)\SimCharles.sbr" \
	"$(INTDIR)\SimDavid.sbr" \
	"$(INTDIR)\SimEmil.sbr" \
	"$(INTDIR)\SimFreud.sbr" \
	"$(INTDIR)\SimGunilla.sbr" \
	"$(INTDIR)\Situation.sbr" \
	"$(INTDIR)\Statistics.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Tournament.sbr" \
	"$(INTDIR)\TournDlg.sbr" \
	"$(INTDIR)\Training.sbr" \
	"$(INTDIR)\Windows.sbr" \
	"$(INTDIR)\WindowsDoc.sbr" \
	"$(INTDIR)\WindowsView.sbr"

"$(OUTDIR)\Windows.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Malin.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Malin.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Aba2sim.obj" \
	"$(INTDIR)\Boub.obj" \
	"$(INTDIR)\fstreamExt.obj" \
	"$(INTDIR)\FullBoard.obj" \
	"$(INTDIR)\Game.obj" \
	"$(INTDIR)\GameAsync.obj" \
	"$(INTDIR)\GameNewDlg.obj" \
	"$(INTDIR)\Hash.obj" \
	"$(INTDIR)\HashBoard.obj" \
	"$(INTDIR)\Kernel.obj" \
	"$(INTDIR)\Killer.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Move.obj" \
	"$(INTDIR)\Optimistic_TDlambda.obj" \
	"$(INTDIR)\Perceptron_n_m_1.obj" \
	"$(INTDIR)\Player.obj" \
	"$(INTDIR)\PlayerCombo.obj" \
	"$(INTDIR)\PlayerDlg.obj" \
	"$(INTDIR)\PlayerNewDlg.obj" \
	"$(INTDIR)\PlayerSelDlg.obj" \
	"$(INTDIR)\Sim212.obj" \
	"$(INTDIR)\SimBoub.obj" \
	"$(INTDIR)\SimCaesar.obj" \
	"$(INTDIR)\SimCharles.obj" \
	"$(INTDIR)\SimDavid.obj" \
	"$(INTDIR)\SimEmil.obj" \
	"$(INTDIR)\SimFreud.obj" \
	"$(INTDIR)\SimGunilla.obj" \
	"$(INTDIR)\Situation.obj" \
	"$(INTDIR)\Statistics.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tournament.obj" \
	"$(INTDIR)\TournDlg.obj" \
	"$(INTDIR)\Training.obj" \
	"$(INTDIR)\Windows.obj" \
	"$(INTDIR)\WindowsDoc.obj" \
	"$(INTDIR)\WindowsView.obj" \
	"$(INTDIR)\Windows.res"

"$(OUTDIR)\Malin.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

InputPath=.\Debug\Malin.exe
SOURCE="$(InputPath)"

"..\Malin.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy $(InputPath) .\..\Malin.exe
<< 
	

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Windows.dep")
!INCLUDE "Windows.dep"
!ELSE 
!MESSAGE Warning: cannot find "Windows.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Windows - Win32 Release" || "$(CFG)" == "Windows - Win32 Debug"
SOURCE=..\Kernel\Aba212\Aba2sim.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Aba2sim.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Aba2sim.obj"	"$(INTDIR)\Aba2sim.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Boub\Boub.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Boub.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Boub.obj"	"$(INTDIR)\Boub.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Utl\fstreamExt.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\fstreamExt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\fstreamExt.obj"	"$(INTDIR)\fstreamExt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\FullBoard.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\FullBoard.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\FullBoard.obj"	"$(INTDIR)\FullBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Game.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Game.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Game.obj"	"$(INTDIR)\Game.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\GameAsync.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Windows.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GameAsync.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Windows.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\GameAsync.obj"	"$(INTDIR)\GameAsync.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\GameNewDlg.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\GameNewDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\GameNewDlg.obj"	"$(INTDIR)\GameNewDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 

SOURCE=..\Kernel\Emil\Hash.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Hash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Hash.obj"	"$(INTDIR)\Hash.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Emil\HashBoard.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\HashBoard.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\HashBoard.obj"	"$(INTDIR)\HashBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Kernel.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Kernel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Kernel.obj"	"$(INTDIR)\Kernel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\David\Killer.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Killer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Killer.obj"	"$(INTDIR)\Killer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 

SOURCE=..\Kernel\Move.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Move.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Move.obj"	"$(INTDIR)\Move.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Freud\Optimistic_TDlambda.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Optimistic_TDlambda.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Optimistic_TDlambda.obj"	"$(INTDIR)\Optimistic_TDlambda.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Freud\Perceptron_n_m_1.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Perceptron_n_m_1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Perceptron_n_m_1.obj"	"$(INTDIR)\Perceptron_n_m_1.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Player.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Player.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Player.obj"	"$(INTDIR)\Player.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\PlayerCombo.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\PlayerCombo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\PlayerCombo.obj"	"$(INTDIR)\PlayerCombo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 

SOURCE=.\PlayerDlg.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\PlayerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\PlayerDlg.obj"	"$(INTDIR)\PlayerDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 

SOURCE=.\PlayerNewDlg.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\PlayerNewDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\PlayerNewDlg.obj"	"$(INTDIR)\PlayerNewDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 

SOURCE=.\PlayerSelDlg.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\PlayerSelDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\PlayerSelDlg.obj"	"$(INTDIR)\PlayerSelDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 

SOURCE=..\Kernel\Aba212\Sim212.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sim212.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Sim212.obj"	"$(INTDIR)\Sim212.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Boub\SimBoub.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SimBoub.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\SimBoub.obj"	"$(INTDIR)\SimBoub.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Caesar\SimCaesar.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SimCaesar.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\SimCaesar.obj"	"$(INTDIR)\SimCaesar.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Caesar\SimCharles.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SimCharles.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\SimCharles.obj"	"$(INTDIR)\SimCharles.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\David\SimDavid.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SimDavid.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\SimDavid.obj"	"$(INTDIR)\SimDavid.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Emil\SimEmil.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SimEmil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\SimEmil.obj"	"$(INTDIR)\SimEmil.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Freud\SimFreud.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SimFreud.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\SimFreud.obj"	"$(INTDIR)\SimFreud.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Gunilla\SimGunilla.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SimGunilla.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\SimGunilla.obj"	"$(INTDIR)\SimGunilla.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Situation.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Situation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Situation.obj"	"$(INTDIR)\Situation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Statistics.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Statistics.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Statistics.obj"	"$(INTDIR)\Statistics.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Windows.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Windows.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Windows.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Windows.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Kernel\Tournament.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Tournament.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Tournament.obj"	"$(INTDIR)\Tournament.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TournDlg.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\TournDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\TournDlg.obj"	"$(INTDIR)\TournDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 

SOURCE=..\Kernel\Training.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Training.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Training.obj"	"$(INTDIR)\Training.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Windows.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\Windows.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\Windows.obj"	"$(INTDIR)\Windows.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 

SOURCE=.\Windows.rc

"$(INTDIR)\Windows.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\WindowsDoc.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\WindowsDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\WindowsDoc.obj"	"$(INTDIR)\WindowsDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 

SOURCE=.\WindowsView.cpp

!IF  "$(CFG)" == "Windows - Win32 Release"


"$(INTDIR)\WindowsView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ELSEIF  "$(CFG)" == "Windows - Win32 Debug"


"$(INTDIR)\WindowsView.obj"	"$(INTDIR)\WindowsView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Windows.pch"


!ENDIF 


!ENDIF 

