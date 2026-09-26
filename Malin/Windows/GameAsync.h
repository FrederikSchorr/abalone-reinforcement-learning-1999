#if !defined(AFX_GAMEASYNC_H__0FB33504_4A44_11D2_90C1_E3955EAB2077__INCLUDED_)
#define AFX_GAMEASYNC_H__0FB33504_4A44_11D2_90C1_E3955EAB2077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameAsync.h : header file
//

#include "../Kernel/Game.h"
#include "../Kernel/Tournament.h"
#include "../Kernel/Training.h"

/////////////////////////////////////////////////////////////////////////////
// CGameAsync 

class CGameAsync : public CWnd, public Game
{
	DECLARE_DYNCREATE(CGameAsync)
public:
	CGameAsync();          
  virtual ~CGameAsync();

/* Game overloads */
public:
  void load (const std::string& s);
  void load (const PlayerMap &);

  void players(const PlayerMap&);
  const PlayerMap& players() const { return Game::players(); }

  void cancel();

/* Operations */
public:
  FullMove humanMove(const Move& move);
  void computerMove();
  void loop();
  void tournament(const PlayerMap &player);
  void training(const CString & sPathName, const CString & sFileName);

/* Attributes */
public:
  bool bWorking() const { return m_status != idle; }

  CString sStatus() const;

  CString sError();
  bool bError();

 /* 'loop' Implementation */
private:
  void loopA();
  void reloop();
  friend UINT CGameAsyncLoop(LPVOID pParam);
  Move m_loopMove;

/* Tournament & Training Implementation */
private:
  Tournament m_tournament;
  void tournamentA();
  friend UINT CGameAsyncTournament(LPVOID pParam);

  Training m_training;
  void trainingA();
  friend UINT CGameAsyncTraining(LPVOID pParam);


/* Implementation */
private:
  void updateView(LPARAM lHint, CObject* pHint = NULL);
  void showMove();
  Move m_showMove;

/* Data */
private:
  volatile bool m_bLoop;
  volatile bool m_bHighlight;
  volatile enum {idle = 0, game, tourn, train } m_status;

  CString m_sStatus;

  CWinThread *m_pThread;
  CCriticalSection m_cs;
  CDocument *m_pDoc;

/* Errors */
private:
  volatile bool m_bCancel;
  CString m_sError;
  void error(const CString &s);

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameAsync)
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CGameAsync)
  afx_msg void OnMoveCompute();
  afx_msg void OnMoveTakeback();
  afx_msg void OnMoveCancel();
  afx_msg void OnMoveLoop();
  afx_msg void OnMoveHighlight();
	afx_msg void OnUpdateIdle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdleGame(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBusy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLoop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHighlight(CCmdUI* pCmdUI);
  //}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEASYNC_H__0FB33504_4A44_11D2_90C1_E3955EAB2077__INCLUDED_)
