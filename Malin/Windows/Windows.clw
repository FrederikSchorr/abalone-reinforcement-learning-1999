; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWindowsApp
LastTemplate=CHtmlView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Windows.h"
LastPage=0

ClassCount=13
Class1=CWindowsApp
Class2=CWindowsDoc
Class3=CWindowsView
Class4=CMainFrame

ResourceCount=9
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_ABOUTBOX (English (U.S.))
Class6=CDlgMove
Resource4=IDD_PLAYERNEW
Class7=CPlayerDlg
Resource5=IDR_MAINFRAME (English (U.S.))
Class8=CPlayerSelDlg
Class9=CPlayerCombo
Class10=CGameAsync
Resource6=IDD_GAMENEW
Class11=CPlayerNewDlg
Resource7=IDD_PLAYERSELECT
Class12=CGameNewDlg
Resource8=IDD_PLAYERMODIFY
Class13=CTournDlg
Resource9=IDD_TOURNAMENT

[CLS:CWindowsApp]
Type=0
HeaderFile=Windows.h
ImplementationFile=Windows.cpp
Filter=N
LastObject=ID_HELP_USERGUIDE
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CWindowsDoc]
Type=0
HeaderFile=WindowsDoc.h
ImplementationFile=WindowsDoc.cpp
Filter=W
BaseClass=CDocument
VirtualFilter=DC
LastObject=CWindowsDoc

[CLS:CWindowsView]
Type=0
HeaderFile=WindowsView.h
ImplementationFile=WindowsView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CWindowsView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=Windows.cpp
ImplementationFile=Windows.cpp
Filter=D
LastObject=ID_APP_ABOUT

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
CommandCount=16
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command16=ID_APP_ABOUT

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
CommandCount=14
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE_AS
Command4=ID_FILE_PLAYERS
Command5=ID_APP_EXIT
Command6=ID_MOVE_COMPUTE
Command7=ID_MOVE_TAKEBACK
Command8=ID_MOVE_CANCEL
Command9=ID_MOVE_LOOP
Command10=ID_MOVE_HIGHLIGHT
Command11=ID_FREDERIK_TOURNAMENT
Command12=ID_PLAYER_NEW
Command13=ID_PLAYER_MODIFY
Command14=ID_FREDERIK_TRAINING
Command15=ID_HELP_USERGUIDE
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342177283

[DLG:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
ControlCount=15
Control1=IDC_PLAYER0,static,1342308352
Control2=IDC_PLAYER1,static,1342308352
Control3=IDC_PLAYER2,static,1342308352
Control4=IDC_PLAYER3,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_HISTORY,listbox,1352728832
Control8=IDC_TIME0,static,1342308352
Control9=IDC_COLOR0,static,1073872896
Control10=IDC_COLOR1,static,1342308352
Control11=IDC_COLOR2,static,1073872896
Control12=IDC_COLOR3,static,1342308352
Control13=IDC_TIME1,static,1342308352
Control14=IDC_TIME2,static,1342308352
Control15=IDC_TIME3,static,1342308352

[CLS:CDlgMove]
Type=0
HeaderFile=DlgMove.h
ImplementationFile=DlgMove.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgMove

[DLG:IDD_PLAYERMODIFY]
Type=1
Class=CPlayerDlg
ControlCount=5
Control1=IDC_INPUT,edit,1350631552
Control2=IDC_CONTINUE,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_HISTORY,edit,1352665156
Control5=IDC_OUTPUT,edit,1350567936

[CLS:CPlayerDlg]
Type=0
HeaderFile=PlayerDlg.h
ImplementationFile=PlayerDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_RICHEDIT1

[DLG:IDD_PLAYERSELECT]
Type=1
Class=CPlayerSelDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO1,combobox,1344340227
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:CPlayerSelDlg]
Type=0
HeaderFile=PlayerSelDlg.h
ImplementationFile=PlayerSelDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPlayerSelDlg
VirtualFilter=dWC

[CLS:CPlayerCombo]
Type=0
HeaderFile=PlayerCombo.h
ImplementationFile=PlayerCombo.cpp
BaseClass=CComboBox
Filter=D
LastObject=CPlayerCombo
VirtualFilter=cWC

[CLS:CGameAsync]
Type=0
HeaderFile=GameAsync.h
ImplementationFile=GameAsync.cpp
BaseClass=CWnd
Filter=N
VirtualFilter=TC
LastObject=ID_MOVE_HIGHLIGHT

[DLG:IDD_PLAYERNEW]
Type=1
Class=CPlayerNewDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO,combobox,1344339971
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:CPlayerNewDlg]
Type=0
HeaderFile=PlayerNewDlg.h
ImplementationFile=PlayerNewDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPlayerNewDlg
VirtualFilter=dWC

[DLG:IDD_GAMENEW]
Type=1
Class=CGameNewDlg
ControlCount=12
Control1=IDC_NUM,edit,1350631552
Control2=IDC_COMBO2,combobox,1344340227
Control3=IDC_COMBO3,combobox,1344340227
Control4=IDC_COMBO4,combobox,1344340227
Control5=IDC_COMBO5,combobox,1344340227
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[CLS:CGameNewDlg]
Type=0
HeaderFile=GameNewDlg.h
ImplementationFile=GameNewDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CGameNewDlg

[DLG:IDD_TOURNAMENT]
Type=1
Class=CTournDlg
ControlCount=5
Control1=IDC_PLAYER,combobox,1344340227
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_LIST,listbox,1352745217
Control5=IDC_STATIC,static,1342308352

[CLS:CTournDlg]
Type=0
HeaderFile=TournDlg.h
ImplementationFile=TournDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CTournDlg

