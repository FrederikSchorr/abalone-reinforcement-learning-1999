// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__74FB6129_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
#define AFX_MAINFRM_H__74FB6129_465D_11D2_90C1_9D93B2FA395D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Kernel/Player.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	//CReBar      m_wndReBar;
	CDialogBar m_wndDlgBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
  
  afx_msg void OnUpdateStatus(CCmdUI *pCmdUI);
  afx_msg void OnUpdateHistory(CCmdUI *pCmdUI);

  afx_msg void OnUpdatePlayer(CCmdUI *pCmdUI, int player);
  afx_msg void OnUpdatePlayer0(CCmdUI *pCmdUI);
  afx_msg void OnUpdatePlayer1(CCmdUI *pCmdUI);
  afx_msg void OnUpdatePlayer2(CCmdUI *pCmdUI);
  afx_msg void OnUpdatePlayer3(CCmdUI *pCmdUI);

  afx_msg void OnUpdateTime(CCmdUI *pCmdUI, int player);
  afx_msg void OnUpdateTime0(CCmdUI *pCmdUI);
  afx_msg void OnUpdateTime1(CCmdUI *pCmdUI);
  afx_msg void OnUpdateTime2(CCmdUI *pCmdUI);
  afx_msg void OnUpdateTime3(CCmdUI *pCmdUI);

  afx_msg void OnUpdateColor(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
private:
	int m_nHist;
  PlayerMap m_player;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__74FB6129_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
