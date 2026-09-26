# Microsoft Developer Studio Generated NMAKE File, Based on Text.dsp
!IF "$(CFG)" == ""
CFG=Text - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Text - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Text - Win32 Release" && "$(CFG)" != "Text - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Text - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Text.exe" ".\..\MalinTxt.exe"


CLEAN :
	-@erase "$(INTDIR)\Aba2sim.obj"
	-@erase "$(INTDIR)\Boub.obj"
	-@erase "$(INTDIR)\fstreamExt.obj"
	-@erase "$(INTDIR)\Game.obj"
	-@erase "$(INTDIR)\Kernel.obj"
	-@erase "$(INTDIR)\Menudos.obj"
	-@erase "$(INTDIR)\Move.obj"
	-@erase "$(INTDIR)\Player.obj"
	-@erase "$(INTDIR)\Sim212.obj"
	-@erase "$(INTDIR)\SimBoub.obj"
	-@erase "$(INTDIR)\Situation.obj"
	-@erase "$(INTDIR)\Text.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Text.exe"
	-@erase ".\..\MalinTxt.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "e:\eigene dateien\c\source\utl" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Text.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\Text.pdb" /machine:I386 /out:"$(OUTDIR)\Text.exe" 
LINK32_OBJS= \
	"$(INTDIR)\fstreamExt.obj" \
	"$(INTDIR)\Game.obj" \
	"$(INTDIR)\Kernel.obj" \
	"$(INTDIR)\Menudos.obj" \
	"$(INTDIR)\Move.obj" \
	"$(INTDIR)\Player.obj" \
	"$(INTDIR)\Sim212.obj" \
	"$(INTDIR)\Situation.obj" \
	"$(INTDIR)\Text.obj" \
	"$(INTDIR)\Aba2sim.obj" \
	"$(INTDIR)\SimBoub.obj" \
	"$(INTDIR)\Boub.obj"

"$(OUTDIR)\Text.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

InputPath=.\Release\Text.exe
SOURCE="$(InputPath)"

"..\MalinTxt.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy $(InputPath) .\..\MalinTxt.exe
<< 
	

!ELSEIF  "$(CFG)" == "Text - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Text.exe" "$(OUTDIR)\Text.bsc" ".\..\MalinTxt.exe"


CLEAN :
	-@erase "$(INTDIR)\Aba2sim.obj"
	-@erase "$(INTDIR)\Aba2sim.sbr"
	-@erase "$(INTDIR)\Boub.obj"
	-@erase "$(INTDIR)\Boub.sbr"
	-@erase "$(INTDIR)\fstreamExt.obj"
	-@erase "$(INTDIR)\fstreamExt.sbr"
	-@erase "$(INTDIR)\Game.obj"
	-@erase "$(INTDIR)\Game.sbr"
	-@erase "$(INTDIR)\Kernel.obj"
	-@erase "$(INTDIR)\Kernel.sbr"
	-@erase "$(INTDIR)\Menudos.obj"
	-@erase "$(INTDIR)\Menudos.sbr"
	-@erase "$(INTDIR)\Move.obj"
	-@erase "$(INTDIR)\Move.sbr"
	-@erase "$(INTDIR)\Player.obj"
	-@erase "$(INTDIR)\Player.sbr"
	-@erase "$(INTDIR)\Sim212.obj"
	-@erase "$(INTDIR)\Sim212.sbr"
	-@erase "$(INTDIR)\SimBoub.obj"
	-@erase "$(INTDIR)\SimBoub.sbr"
	-@erase "$(INTDIR)\Situation.obj"
	-@erase "$(INTDIR)\Situation.sbr"
	-@erase "$(INTDIR)\Text.obj"
	-@erase "$(INTDIR)\Text.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Text.bsc"
	-@erase "$(OUTDIR)\Text.exe"
	-@erase "$(OUTDIR)\Text.ilk"
	-@erase "$(OUTDIR)\Text.pdb"
	-@erase ".\..\MalinTxt.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "e:\eigene dateien\c\source\utl" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Text.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\fstreamExt.sbr" \
	"$(INTDIR)\Game.sbr" \
	"$(INTDIR)\Kernel.sbr" \
	"$(INTDIR)\Menudos.sbr" \
	"$(INTDIR)\Move.sbr" \
	"$(INTDIR)\Player.sbr" \
	"$(INTDIR)\Sim212.sbr" \
	"$(INTDIR)\Situation.sbr" \
	"$(INTDIR)\Text.sbr" \
	"$(INTDIR)\Aba2sim.sbr" \
	"$(INTDIR)\SimBoub.sbr" \
	"$(INTDIR)\Boub.sbr"

"$(OUTDIR)\Text.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\Text.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Text.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\fstreamExt.obj" \
	"$(INTDIR)\Game.obj" \
	"$(INTDIR)\Kernel.obj" \
	"$(INTDIR)\Menudos.obj" \
	"$(INTDIR)\Move.obj" \
	"$(INTDIR)\Player.obj" \
	"$(INTDIR)\Sim212.obj" \
	"$(INTDIR)\Situation.obj" \
	"$(INTDIR)\Text.obj" \
	"$(INTDIR)\Aba2sim.obj" \
	"$(INTDIR)\SimBoub.obj" \
	"$(INTDIR)\Boub.obj"

"$(OUTDIR)\Text.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

InputPath=.\Debug\Text.exe
SOURCE="$(InputPath)"

"..\MalinTxt.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	copy $(InputPath) .\..\MalinTxt.exe
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
!IF EXISTS("Text.dep")
!INCLUDE "Text.dep"
!ELSE 
!MESSAGE Warning: cannot find "Text.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Text - Win32 Release" || "$(CFG)" == "Text - Win32 Debug"
SOURCE=..\Kernel\Aba212\Aba2sim.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Aba2sim.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Aba2sim.obj"	"$(INTDIR)\Aba2sim.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Kernel\Boub\Boub.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Boub.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Boub.obj"	"$(INTDIR)\Boub.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Kernel\Utl\fstreamExt.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\fstreamExt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\fstreamExt.obj"	"$(INTDIR)\fstreamExt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Kernel\Game.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Game.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Game.obj"	"$(INTDIR)\Game.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Kernel\Kernel.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Kernel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Kernel.obj"	"$(INTDIR)\Kernel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\C\Source\Utl\Menudos.c

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Menudos.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Menudos.obj"	"$(INTDIR)\Menudos.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Kernel\Move.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Move.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Move.obj"	"$(INTDIR)\Move.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Kernel\Player.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Player.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Player.obj"	"$(INTDIR)\Player.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Kernel\Aba212\Sim212.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Sim212.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Sim212.obj"	"$(INTDIR)\Sim212.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Kernel\Boub\SimBoub.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\SimBoub.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\SimBoub.obj"	"$(INTDIR)\SimBoub.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Kernel\Situation.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Situation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Situation.obj"	"$(INTDIR)\Situation.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Text.cpp

!IF  "$(CFG)" == "Text - Win32 Release"


"$(INTDIR)\Text.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Text - Win32 Debug"


"$(INTDIR)\Text.obj"	"$(INTDIR)\Text.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

