// GameAsync.cpp : implementation file
//
#pragma warning (disable : 4786)

#include "stdafx.h"
#include "Windows.h"
#include "GameAsync.h"
#include "WindowsDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define   CANCEL_TIMEOUT      10000
#define   LOCK_TIMEOUT        10000
#define   SHOW_MOVE_SLEEP     700


/////////////////////////////////////////////////////////////////////////////
// CGameAsync

IMPLEMENT_DYNCREATE(CGameAsync, CWnd)

#pragma warning (disable : 4355)
CGameAsync::CGameAsync()
{
  m_status = idle;
  m_bLoop = true;
  m_bHighlight = true;

  m_sStatus = "Malin initialized";

  m_pThread = NULL;
  m_bCancel = false;
}
#pragma warning (default : 4355)


CGameAsync::~CGameAsync()
{
  //TRACE("CGameAsync::Destructor\n");
}

BEGIN_MESSAGE_MAP(CGameAsync, CWnd)
	//{{AFX_MSG_MAP(CGameAsync)
  ON_COMMAND(ID_MOVE_COMPUTE, OnMoveCompute)
  ON_COMMAND(ID_MOVE_TAKEBACK, OnMoveTakeback)
  ON_COMMAND(ID_MOVE_CANCEL, OnMoveCancel)
	ON_COMMAND(ID_MOVE_LOOP, OnMoveLoop)
	ON_COMMAND(ID_MOVE_HIGHLIGHT, OnMoveHighlight)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateIdle)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateIdle)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateIdleGame)
	ON_UPDATE_COMMAND_UI(ID_FILE_PLAYERS, OnUpdateIdleGame)
	ON_UPDATE_COMMAND_UI(ID_MOVE_COMPUTE, OnUpdateIdleGame)
	ON_UPDATE_COMMAND_UI(ID_MOVE_TAKEBACK, OnUpdateIdleGame)
	ON_UPDATE_COMMAND_UI(ID_MOVE_CANCEL, OnUpdateBusy)
	ON_UPDATE_COMMAND_UI(ID_MOVE_LOOP, OnUpdateLoop)
	ON_UPDATE_COMMAND_UI(ID_MOVE_HIGHLIGHT, OnUpdateHighlight)
	ON_UPDATE_COMMAND_UI(ID_PLAYER_NEW, OnUpdateIdle)
	ON_UPDATE_COMMAND_UI(ID_PLAYER_MODIFY, OnUpdateIdle)
	ON_UPDATE_COMMAND_UI(ID_FREDERIK_TOURNAMENT, OnUpdateIdle)
  ON_UPDATE_COMMAND_UI(ID_FREDERIK_TRAINING, OnUpdateIdle)
	//}}AFX_MSG_MAP
  ON_COMMAND(ID_GAMEASYNC_RELOOP, reloop)
  ON_COMMAND(ID_GAMEASYNC_SHOWMOVE, showMove)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameAsync message handlers


/* * * * * * * * * * * * * * * * * */
void CGameAsync::computerMove()
{
  //TRACE("CGameAsync::computerMove\n");
  
  /* Launch a thread which calls loopA */
  if (bIsHuman()) throw GameE("This is a human player");

  cancel();
  m_status = game;
  m_bCancel = false;

  m_pThread = AfxBeginThread(CGameAsyncLoop, this);
  if(!m_pThread) AfxThrowResourceException();
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync::loopA()
{
  /*** Called by the secondary thread */
  //TRACE("CGameAsync::loopA\n");

  try
  {
    /* Calculate move */
    m_loopMove = Game::calcMove();

    /* Show move */
    if (m_bHighlight)
    {
      m_showMove = m_loopMove;
      PostMessage(WM_COMMAND, ID_GAMEASYNC_SHOWMOVE, 0);
      for (Count n = 0; n < SHOW_MOVE_SLEEP / 100; n++)
      {
        Sleep(100);
        if (m_bCancel) throw CancelE("Move cancelled");
      }
    }

    /* Call reloop in the main thread */
    PostMessage(WM_COMMAND, ID_GAMEASYNC_RELOOP, 0);
  }
  catch(exception e)
  {
    CString s;
    s.Format("%s (#%d):\n%s",Game::spPlayerAct()->sName().c_str(),
      Game::playerAct() + 1, e.what());
    error(s);
    m_status = idle;
  }
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync::reloop()
{
  //TRACE("CGameAsync::reloop\n");

  try
  {
    /* Error */
    if (bError()) throw GameE((const char *)sError());

    /* ? Cancel */
    if (m_bCancel) throw CancelE("Move computation cancelled");

    /* Exectue move */
    Game::move(m_loopMove);
    updateView(0);

    /* ? game over */
    if (Game::playerWon() != MARBLE_EMPTY)
    {
      cleanup();
      CString s;
      PlayerMap player = Game::players();
      s.Format("%s (#%d) has won the game !", 
        player[playerWon()]->sName().c_str(), playerWon() + 1);
      throw GameE((const char *) s);
    }

    /* Next move */
    if (m_bLoop && !bIsHuman()) 
    {
      m_pThread = AfxBeginThread(CGameAsyncLoop, this);
      if(!m_pThread) AfxThrowResourceException();
    }
    else m_status = idle;
  }
  catch(exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
    m_status = idle;
  }
}



/* * * * * * * * * * * * * * * * * */
void CGameAsync::loop()
{
  //TRACE("CGameAsync::loop\n");
  if (m_bLoop && !bIsHuman()) computerMove();
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync::cancel()
{
  //TRACE("CGameAsync::cancel\n");

  int nPriority = AfxGetThread()->GetThreadPriority();
  AfxGetThread()->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
  
  m_bCancel = true;
  Game::cancel();
  m_tournament.cancel();
  m_training.cancel();
  
  AfxGetThread()->SetThreadPriority(nPriority);

  /* Wait for the worker thread to become idle */
  for (int n = 0; n < CANCEL_TIMEOUT / 100; n++)
  {
    if (m_pThread == NULL) break;
    
    m_bCancel = true;
    Game::cancel();
    m_tournament.cancel();
    m_training.cancel();
    
    Sleep(100);
  }

  /* ? worker thread idle */
  if (m_pThread != NULL)
  {
    TRACE("CGameAsync::cancel Could not synchro\n");
    throw AsyncE("Could not synchronize with worker thread");
  }
  
  m_status = idle;
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::load (const std::string &s)
{
  cancel();

  Game::load(s);
}

/* * * * * * * * * * * * * * * * * */
void CGameAsync
::load (const PlayerMap &map)
{
  cancel();

  Game::load(map);
}


/* * * * * * * * * * * * * * * * * */
FullMove CGameAsync
::humanMove(const Move &m)
{
  cancel();

  if (!bIsHuman()) throw GameE("This player is no human");

  FullMove full = Game::move(m);
  updateView(0);

  loop();

  return full;
}

/* * * * * * * * * * * * * * * * * */
void CGameAsync
::error(const CString &s)
{
  CSingleLock lock(&m_cs);
  
  lock.Lock(LOCK_TIMEOUT);
  m_sError += "\n" + s;
}


/* * * * * * * * * * * * * * * * * */
CString CGameAsync
::sError()
{
  CSingleLock lock(&m_cs);
  lock.Lock(LOCK_TIMEOUT);

  CString s = m_sError;
  m_sError = "";

  return s;
}


/* * * * * * * * * * * * * * * * * */
bool CGameAsync
::bError()
{
  CSingleLock lock(&m_cs);
  lock.Lock(LOCK_TIMEOUT);

  if (m_sError == "") return false;
  else return true;
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::players(const PlayerMap &player)
{
  cancel();
  Game::players(player);
}

/* * * * * * * * * * * * * * * * * */
void CGameAsync
::updateView(LPARAM lHint, CObject* pHint)
{
  /* Update view */
  ((CFrameWnd*)AfxGetApp()->GetMainWnd())->
    GetActiveDocument()->UpdateAllViews(NULL, lHint, pHint);
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::showMove ()
{
  updateView(UPDATEVIEW_MOVE, (CObject *)(void *)&m_showMove);
}



/* * * * * * * * * * * * * * * * * */
void CGameAsync::tournament(const PlayerMap &players)
{
  cancel();
  clear();
  m_status = tourn;
  m_sStatus.Format("Playing tournament with %d players ...", players.size());

  m_tournament.init(players);

  /* Launch a thread which calls tournamentA */
  m_pThread = AfxBeginThread(CGameAsyncTournament, this);
  if(!m_pThread) AfxThrowResourceException();
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync::tournamentA()
{
  /*** Called by the secondary thread */
  try
  {
    m_tournament.run();
    throw GameE("Tournament finished");
  }
  catch(exception e)
  {
    error(e.what());
  }

  m_status = idle;
}




/* * * * * * * * * * * * * * * * * */
void CGameAsync::training(const CString & sPathName, const CString & sFileName)
{
  cancel();
  clear();
  m_status = train;
  m_sStatus.Format("Training neural network (%s) ...", sFileName);

  // Init training object
  m_training.init((const char *)sPathName);

  /* Launch a thread which calls neuralA */
  m_pThread = AfxBeginThread(CGameAsyncTraining, this);
  if(!m_pThread) AfxThrowResourceException();
}



/* * * * * * * * * * * * * * * * * */
void CGameAsync::trainingA()
{
  /*** Called by the secondary thread */
  try
  {
    m_training.run();
    throw GameE("Training finished");
  }
  catch(exception e)
  {
    error(e.what());
  }

  m_status = idle;
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnMoveCompute() 
{
  try
  {
    computerMove();
  }
  catch (exception e)
  {
    error(e.what());
  }
  if (bError()) AfxMessageBox(sError(), MB_ICONINFORMATION);
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnMoveTakeback() 
{
  try
  {
    cancel();
    Game::takeBackMove();
    updateView(0);
  }
  catch (exception e)
  {
    error(e.what());
  }
  if (bError()) AfxMessageBox(sError(), MB_ICONINFORMATION);
}




/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnMoveCancel() 
{
  try
  {
    cancel();
    updateView(UPDATEVIEW_CANCEL);
  }
  catch (exception e)
  {
    error(e.what());
  }
  if (bError()) AfxMessageBox(sError(), MB_ICONINFORMATION);
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnMoveLoop() 
{
  try
  {
    m_bLoop = !m_bLoop;
    loop();
  }
  catch (exception e)
  {
    error(e.what());
  }
  if (bError()) AfxMessageBox(sError(), MB_ICONINFORMATION);
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnMoveHighlight() 
{
  m_bHighlight = !m_bHighlight;
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnUpdateIdle(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_status == idle);
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnUpdateIdleGame(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(sit().m_playerNum > 0 && m_status == idle);
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnUpdateBusy(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_status != idle);
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnUpdateLoop(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
  pCmdUI->SetCheck(m_bLoop);
}


/* * * * * * * * * * * * * * * * * */
void CGameAsync
::OnUpdateHighlight(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
  pCmdUI->SetCheck(m_bHighlight);
}


/* * * * * * * * * * * * * * * * * */
CString CGameAsync
::sStatus() const
{
  CString s;

  switch(m_status)
  {
  // Ordinary game
  case game:
    // Computer player
    s.Format("%s(#%d) thinking ... %2.2d:%2.2d", 
      spPlayerAct()->sName().c_str(), sit().m_playerAct + 1, 
      timePlayerAct() / 60, timePlayerAct() % 60);
    return s;

  // Tournament
  case tourn:
  // Training
  case train:
    s.Format("%s %2.2d:%2.2d", 
      (const char *)m_sStatus, timePlayerAct() / 60, timePlayerAct() % 60);
    return s;

  // Idle
  case idle:
    // ? no game
    if (sit().m_playerNum <= 0) return "No game - idle";

    // ? human
    if (bIsHuman())
    {
      s.Format("It is your turn, %s(#%d) - %2.2d:%2.2d", 
        spPlayerAct()->sName().c_str(), sit().m_playerAct + 1,
        timePlayerAct() / 60, timePlayerAct() % 60);
      return s;
    }
    
    // Computer player
    s.Format("%s(#%d)'s turn - idle", 
      spPlayerAct()->sName().c_str(), sit().m_playerAct + 1);
    return s;

  default:
    return "Internal error";
  }
}




/***********************************/
UINT CGameAsyncTournament (LPVOID pParam)
{
  CGameAsync *pGameA = (CGameAsync*)pParam;
  pGameA->tournamentA();
  pGameA->m_pThread = NULL;
  return 0;
}



/***********************************/
UINT CGameAsyncLoop (LPVOID pParam)
{
  CGameAsync *pGameA = (CGameAsync*)pParam;
  pGameA->loopA();
  pGameA->m_pThread = NULL;
  return 0;
}



/***********************************/
UINT CGameAsyncTraining (LPVOID pParam)
{
  CGameAsync *pGameA = (CGameAsync*)pParam;
  pGameA->trainingA();
  pGameA->m_pThread = NULL;
  return 0;
}

 

