// PlayerNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Windows.h"
#include "PlayerNewDlg.h"
#include "../Kernel/Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayerNewDlg dialog


CPlayerNewDlg::CPlayerNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayerNewDlg)
	//}}AFX_DATA_INIT
}


void CPlayerNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerNewDlg)
	DDX_Control(pDX, IDC_COMBO, m_comboType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayerNewDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayerNewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerNewDlg message handlers

void CPlayerNewDlg::OnOK() 
{
  UpdateData(TRUE);

  if (m_comboType.GetCurSel() != CB_ERR) 
  {
    m_type = (Simulation::Type)(m_comboType.GetCurSel() + Simulation::human);
    EndDialog(IDOK);
  }
}

BOOL CPlayerNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  /* Prepare type combobox */
  for (int i = Simulation::human; i < Simulation::last; i++)
  {
    m_comboType.AddString(Player::sSimDesc((Simulation::Type)i));
  }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

Simulation::Type CPlayerNewDlg::playerType()
{
  return m_type;
}
