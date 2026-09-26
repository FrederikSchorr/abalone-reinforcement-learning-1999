#if !defined(AFX_DLGMOVE_H__74FB6136_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
#define AFX_DLGMOVE_H__74FB6136_465D_11D2_90C1_9D93B2FA395D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMove.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMove dialog

class CDlgMove : public CDialog
{
// Construction
public:
	CDlgMove(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMove)
	enum { IDD = IDD_MOVE };
	CString	m_sMove;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMove)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMove)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMOVE_H__74FB6136_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
