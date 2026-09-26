#if !defined(AFX_TOURNDLG_H__686DE161_5154_11D2_90C1_968063FFED5A__INCLUDED_)
#define AFX_TOURNDLG_H__686DE161_5154_11D2_90C1_968063FFED5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlayerCombo.h"
#include "../Kernel/Player.h"

// TournDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTournDlg dialog

class CTournDlg : public CDialog
{
// Construction
public:
	CTournDlg(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
   PlayerMap players() const ;


// Dialog Data
protected:
	//{{AFX_DATA(CTournDlg)
	enum { IDD = IDD_TOURNAMENT };
	CPlayerCombo	m_player;
	CListBox	m_listPlayer;
	//}}AFX_DATA
  std::list<CString> m_listResult;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTournDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTournDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOURNDLG_H__686DE161_5154_11D2_90C1_968063FFED5A__INCLUDED_)
