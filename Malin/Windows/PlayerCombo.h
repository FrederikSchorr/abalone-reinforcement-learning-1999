#if !defined(AFX_PLAYERCOMBO_H__504577C2_4910_11D2_90C1_925AFC51FE40__INCLUDED_)
#define AFX_PLAYERCOMBO_H__504577C2_4910_11D2_90C1_925AFC51FE40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlayerCombo window

class CPlayerCombo : public CComboBox
{
// Construction
public:
	CPlayerCombo();

// Attributes
public:
  CString m_sPlayer;
  CString sPlayerPath () const;

// Operations
public:
  void UpdateData (BOOL bSaveAndValidate = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlayerCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlayerCombo)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERCOMBO_H__504577C2_4910_11D2_90C1_925AFC51FE40__INCLUDED_)
