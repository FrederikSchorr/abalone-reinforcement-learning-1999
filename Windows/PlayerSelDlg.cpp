// PlayerSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Windows.h"
#include "PlayerSelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayerSelDlg dialog


CPlayerSelDlg::CPlayerSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayerSelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

  
}


void CPlayerSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerSelDlg)
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayerSelDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayerSelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerSelDlg message handlers

BOOL CPlayerSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  m_combo.UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlayerSelDlg::OnOK() 
{

  UpdateData(TRUE);
  m_combo.UpdateData(TRUE);

	if (m_combo.m_sPlayer != "") EndDialog(IDOK);
}
