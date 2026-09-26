// Windows.h : main header file for the WINDOWS application
//

#if !defined(AFX_WINDOWS_H__74FB6125_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
#define AFX_WINDOWS_H__74FB6125_465D_11D2_90C1_9D93B2FA395D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

const COLORREF COLOR_LIGHTGRAY  = RGB(192,192,192);
const COLORREF COLOR_PLAYER0    = RGB(128,128,255);
//const COLORREF COLOR_PLAYER0    = RGB(255,255,255);
const COLORREF COLOR_PLAYER1    = RGB(255,  0,128);
const COLORREF COLOR_PLAYER2    = RGB(255,255,128);
const COLORREF COLOR_PLAYER3    = RGB(  0,255,128);

/////////////////////////////////////////////////////////////////////////////
// CWindowsApp:
// See Windows.cpp for the implementation of this class
//

class CWindowsApp : public CWinApp
{
public:
	CWindowsApp();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWindowsApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpUserguide();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWS_H__74FB6125_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
