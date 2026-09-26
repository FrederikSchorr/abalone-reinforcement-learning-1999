#if !defined(AFX_PLAYERNEWDLG_H__B2C23E41_4B10_11D2_90C1_AB085643925E__INCLUDED_)
#define AFX_PLAYERNEWDLG_H__B2C23E41_4B10_11D2_90C1_AB085643925E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// PlayerNewDlg.h : header file
//

#include "../Kernel/Simulation.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayerNewDlg dialog

class CPlayerNewDlg : public CDialog
{
// Construction
public:
	CPlayerNewDlg(CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
  Simulation::Type playerType();

private:
  Simulation::Type m_type;

// Dialog Data
protected:
	//{{AFX_DATA(CPlayerNewDlg)
	enum { IDD = IDD_PLAYERNEW };
	CComboBox	m_comboType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayerNewDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERNEWDLG_H__B2C23E41_4B10_11D2_90C1_AB085643925E__INCLUDED_)
