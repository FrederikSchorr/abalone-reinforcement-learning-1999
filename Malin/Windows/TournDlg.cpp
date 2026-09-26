// TournDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Windows.h"
#include "TournDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTournDlg dialog


CTournDlg::CTournDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTournDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTournDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTournDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTournDlg)
	DDX_Control(pDX, IDC_PLAYER, m_player);
	DDX_Control(pDX, IDC_LIST, m_listPlayer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTournDlg, CDialog)
	//{{AFX_MSG_MAP(CTournDlg)
	ON_CBN_SELCHANGE(IDC_PLAYER, OnAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTournDlg message handlers

void CTournDlg::OnAdd() 
{
	UpdateData(TRUE);
  m_player.UpdateData(TRUE);

  if (m_player.m_sPlayer != "")
    m_listPlayer.AddString(m_player.m_sPlayer);	
}

void CTournDlg::OnOK() 
{
	UpdateData(TRUE);
  m_player.UpdateData(TRUE);

  if (m_listPlayer.GetCount() < 2) return;

  /* Save players list */
  m_listResult.clear();
  CString s;
  for(int n = 0; n < m_listPlayer.GetCount(); n++)
  {
    m_listPlayer.GetText(n, s);
    m_listResult.push_back(s);
  }
	
	CDialog::OnOK();
}

BOOL CTournDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_player.UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

PlayerMap CTournDlg::players() const
{
  PlayerMap player;

  std::list<CString>::const_iterator it = m_listResult.begin();
  int n;
  for (n = 0; it != m_listResult.end(); n++, it++)
  {
    player[n] = new Player((const char*)*it);
  }

  return player;
}
