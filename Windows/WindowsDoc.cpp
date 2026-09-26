// WindowsDoc.cpp : implementation of the CWindowsDoc class
//

#include "stdafx.h"
#include "Windows.h"

#include "WindowsDoc.h"
#include "PlayerDlg.h"
#include "PlayerSelDlg.h"
#include "PlayerNewDlg.h"
#include "GameNewDlg.h"
#include "TournDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SHUTDOWN_TIMEOUT  10000

/////////////////////////////////////////////////////////////////////////////
// CWindowsDoc

IMPLEMENT_DYNCREATE(CWindowsDoc, CDocument)

BEGIN_MESSAGE_MAP(CWindowsDoc, CDocument)
	//{{AFX_MSG_MAP(CWindowsDoc)
	ON_COMMAND(ID_FILE_PLAYERS, OnFilePlayers)
	ON_COMMAND(ID_PLAYER_MODIFY, OnPlayerModify)
	ON_COMMAND(ID_PLAYER_NEW, OnPlayerNew)
	ON_COMMAND(ID_FREDERIK_TOURNAMENT, OnFrederikTournament)
  ON_COMMAND(ID_FREDERIK_TRAINING, OnFrederikTraining)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowsDoc construction/destruction

CWindowsDoc::CWindowsDoc()
{
  /* Worker thread */
  m_pGameA = new CGameAsync;
  m_bStartup = true;
}

CWindowsDoc::~CWindowsDoc()
{
  TRACE("CWindowsDoc::Destructor\n");
  try
  {
    m_pGameA->cancel();
    if (m_pGameA->bError())
      AfxMessageBox(m_pGameA->sError(), MB_ICONINFORMATION);
  }
  catch (exception e)
  {
    TRACE("CWindowsDoc::Destructor %s\n", e.what());
  }

  if (m_pGameA) delete m_pGameA;
}




/////////////////////////////////////////////////////////////////////////////
// CWindowsDoc serialization

void CWindowsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWindowsDoc diagnostics

#ifdef _DEBUG
void CWindowsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWindowsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWindowsDoc commands


BOOL CWindowsDoc::SaveModified() 
{
  /* Cancel */
  try
  {
    m_pGameA->cancel();
  }
  catch(exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
  }
  if (m_pGameA->bError())
    AfxMessageBox(m_pGameA->sError(), MB_ICONINFORMATION);
	
  //return TRUE;
	return CDocument::SaveModified();
}



BOOL CWindowsDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  try
  {
  	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

    if (m_bStartup)
    {
      m_bStartup = false;
      initGameAsync();
    }

    /* Load document */
    m_pGameA->load(lpszPathName);

    /* Computer player(s) */
    m_pGameA->loop();
   }
  catch (exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
    DeleteContents();
    return TRUE;
  }
	return TRUE;
}

BOOL CWindowsDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
  try
  {
    /* Save game */
    m_pGameA->save(lpszPathName);
  }
  catch (exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
    return FALSE;
  }
	
	//return CDocument::OnSaveDocument(lpszPathName);
  return TRUE;
}


void CWindowsDoc::humanMove(Move move) 
{
    /* Move */
    m_pGameA->humanMove(move);

    /* Inform frame work */
    UpdateAllViews(NULL);
}



void CWindowsDoc::OnPlayerModify() 
{
  try
  {
    /* Select player */
    CPlayerSelDlg selDlg;
    if (selDlg.DoModal() != IDOK) return;

    /* Open player */
    CString sName = selDlg.sPlayer();
    SmartPtrPlayer spPlayer = new Player((const char *)sName);

    /* Modify player */
    CPlayerDlg dlg;
    spPlayer->modify(dlg);

    /* Select file */
    CString sFilter;
    sFilter.LoadString(IDS_PLAYER_FILES);

    CFileDialog fileDlg(FALSE, 
      EXTENSION_PLAYER,
      selDlg.sPlayerPath(),
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      sFilter);
    if (fileDlg.DoModal() != IDOK) throw CancelE();

    /* Save */
    spPlayer->save((const char *)fileDlg.GetPathName());
  }
  catch(CancelE e)
  {
    AfxMessageBox("Player was not saved", MB_ICONINFORMATION);
  }
  catch(exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
  }
}


void CWindowsDoc::DeleteContents() 
{
  m_pGameA->clear();
	
	CDocument::DeleteContents();
}


void CWindowsDoc::OnPlayerNew() 
{
  try
  {
    /* Select player */
    CPlayerNewDlg newDlg;
    if (newDlg.DoModal() != IDOK) return;

    /* Create player */
    SmartPtrSim spSim;
    SmartPtrPlayer spPlayer = new Player(newDlg.playerType());

    /* Modify player */
    CPlayerDlg dlg;
    spPlayer->modify(dlg);

    /* Select file */
    CString sFilter;
    sFilter.LoadString(IDS_PLAYER_FILES);
    CString sFile = Kernel::sDirectoryPlayer.c_str();
    sFile += spPlayer->sName().c_str();
    sFile.Replace('/', '\\');

    CFileDialog fileDlg(FALSE, 
      EXTENSION_PLAYER,
      sFile,
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      sFilter);
    if (fileDlg.DoModal() != IDOK) throw CancelE();

    /* Save */
    spPlayer->save((const char *)fileDlg.GetPathName());
  }
  catch(CancelE e)
  {
    AfxMessageBox("Player was not saved", MB_ICONINFORMATION);
  }
  catch(exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
  }
}

void CWindowsDoc::OnFilePlayers() 
{
  try
  {
    /* Change players dialog */
    CGameNewDlg dlg(m_pGameA->players());
    if (dlg.DoModal() != IDOK) return;

    /* Retrieve new players and load them to game*/
    m_pGameA->players(dlg.players());

    /* Inform frame work */
    UpdateAllViews(NULL, UPDATEVIEW_PLAYER);

    /* Computer player(s) */
    m_pGameA->loop();
  }
  catch (exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
  }
}


BOOL CWindowsDoc::OnNewDocument() 
{
  /* Base class */
  if (!CDocument::OnNewDocument()) return FALSE;

    /* While startup init. standard game */
  if (m_bStartup)
  {
    m_bStartup = false;
    initGameAsync();
    try
    {
      /* Load players */
      CString sPlayer;
      sPlayer.LoadString(IDS_NEWGAME_PLAYER0);
      PlayerMap player;
      player[0] = new Player((const char *)sPlayer);
      
      sPlayer.LoadString(IDS_NEWGAME_PLAYER1);
      player[1] = new Player((const char *)sPlayer);

      /* Load game */
      m_pGameA->load(player);

      return TRUE;
    
    }
    catch(exception e)
    {
      DeleteContents();
      return TRUE;
    }
  }

  /* Choose from dialog */
  else
  {
    try
    {
      /* Create new game */
      CGameNewDlg newDlg;
      if (newDlg.DoModal() != IDOK) 
      {
        DeleteContents();
        return TRUE;
      }

      DeleteContents();

      /* Load new game */
      m_pGameA->load(newDlg.players());

      /* Computer player(s) */
      m_pGameA->loop();

      return TRUE;
    }
    catch (exception e)
    {
      AfxMessageBox(e.what(), MB_ICONINFORMATION);
      DeleteContents();
      return TRUE;
    }
  }
}



void CWindowsDoc::initGameAsync()
{
  // Initialize the hidden window
  CRect rect(0, 0, 10, 10);
  m_pGameA->Create(NULL, 
    "Worker", 
    WS_CHILD, 
    rect, 
    AfxGetMainWnd(), 
    IDD_WORKER_WINDOW);
}

void CWindowsDoc::OnFrederikTournament() 
{
  try
  {
    /* Select players */
    CTournDlg dlg;
    if (dlg.DoModal() != IDOK) return;

    /* Start tournament */
    m_pGameA->tournament(dlg.players());

  }
  catch (exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
    DeleteContents();
  }  	  
}


void CWindowsDoc::OnFrederikTraining() 
{
  try
  {
    // Select script file
    CString sFilter;
    sFilter.LoadString(IDS_TRAINING_FILES);
    CString sFile = Kernel::sDirectoryRoot.c_str();
    sFile += ".";
    sFile.Replace('/', '\\');

    CFileDialog fileDlg(TRUE, 
      EXTENSION_TRAINING,
      sFile,
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      sFilter);
    if (fileDlg.DoModal() != IDOK) return;

    // Start training session
    m_pGameA->training(fileDlg.GetPathName(), fileDlg.GetFileName());
  }
  catch (exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
    DeleteContents();
  }  	  
}

BOOL CWindowsDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
  // Route message forward to CGameAsync* m_pGameA
	if (m_pGameA && m_pGameA->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
    return TRUE;
	
	return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
