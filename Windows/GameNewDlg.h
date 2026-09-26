#if !defined(AFX_GAMENEWDLG_H__B2C23E43_4B10_11D2_90C1_AB085643925E__INCLUDED_)
#define AFX_GAMENEWDLG_H__B2C23E43_4B10_11D2_90C1_AB085643925E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlayerCombo.h"

// GameNewDlg.h : header file
//

#include "../Kernel/Player.h"

/////////////////////////////////////////////////////////////////////////////
// CGameNewDlg dialog

class CGameNewDlg : public CDialog
{
// Construction
public:
	CGameNewDlg(CWnd* pParent = NULL);   // standard constructor
  CGameNewDlg(const PlayerMap &map, CWnd* pParent = NULL);   

// Operations
public:
  PlayerMap players() const;

// Data
private:
  bool m_bNew;

// Dialog Data
protected:
	//{{AFX_DATA(CGameNewDlg)
	enum { IDD = IDD_GAMENEW };
	CEdit	m_editPlayerNum;
	int		m_playerNum;
  CPlayerCombo m_player0;
  CPlayerCombo m_player1;
  CPlayerCombo m_player2;
  CPlayerCombo m_player3;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  void enablePlayers();

	// Generated message map functions
	//{{AFX_MSG(CGameNewDlg)
	afx_msg void OnChangeNum();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMENEWDLG_H__B2C23E43_4B10_11D2_90C1_AB085643925E__INCLUDED_)
