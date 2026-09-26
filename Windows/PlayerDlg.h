#if !defined(AFX_PLAYERDLG_H__7DEBA821_48A7_11D2_90C1_FFB7DD0C2446__INCLUDED_)
#define AFX_PLAYERDLG_H__7DEBA821_48A7_11D2_90C1_FFB7DD0C2446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerDlg.h : header file
//
#include "../Kernel/Modify.h"


/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog

class CPlayerDlg : public CDialog, public Modify
{
// Construction
public:
	void modify(const std::string &sOutput, float &fInput, 
    const float &fLower, const float &fUpper, const int nDeci);
  void modify(const std::string &sOuput, std::string &sInput);
  void println(const std::string &sOutput);

	CPlayerDlg(CWnd* pParent = NULL);   // standard constructor

private:
  float m_fLower;
  float m_fUpper;
  bool m_bFloat;
  CString m_sHistory;

// Dialog Data
	//{{AFX_DATA(CPlayerDlg)
	enum { IDD = IDD_PLAYERMODIFY };
	CEdit	m_editHistory;
	CString	m_sInput;
	CString	m_sOutput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayerDlg)
	afx_msg void OnContinue();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERDLG_H__7DEBA821_48A7_11D2_90C1_FFB7DD0C2446__INCLUDED_)
