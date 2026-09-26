// DlgMove.cpp : implementation file
//

#include "stdafx.h"
#include "Windows.h"
#include "DlgMove.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMove dialog


CDlgMove::CDlgMove(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMove::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMove)
	m_sMove = _T("");
	//}}AFX_DATA_INIT
}


void CDlgMove::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMove)
	DDX_Text(pDX, IDC_EDIT1, m_sMove);
	DDV_MaxChars(pDX, m_sMove, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMove, CDialog)
	//{{AFX_MSG_MAP(CDlgMove)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMove message handlers
