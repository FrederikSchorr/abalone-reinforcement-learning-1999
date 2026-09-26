#if !defined(AFX_PLAYERSELDLG_H__504577C1_4910_11D2_90C1_925AFC51FE40__INCLUDED_)
#define AFX_PLAYERSELDLG_H__504577C1_4910_11D2_90C1_925AFC51FE40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerSelDlg.h : header file
//

#include "PlayerCombo.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayerSelDlg dialog

class CPlayerSelDlg : public CDialog
{
// Construction
public:
	CPlayerSelDlg(CWnd* pParent = NULL);   // standard constructor


// Attributes
public:
  CString sPlayer() { return m_combo.m_sPlayer; }
  CString sPlayerPath() { return m_combo.sPlayerPath(); }

// Dialog Data
protected:
	//{{AFX_DATA(CPlayerSelDlg)
	enum { IDD = IDD_PLAYERSELECT };
	CPlayerCombo m_combo;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayerSelDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERSELDLG_H__504577C1_4910_11D2_90C1_925AFC51FE40__INCLUDED_)
