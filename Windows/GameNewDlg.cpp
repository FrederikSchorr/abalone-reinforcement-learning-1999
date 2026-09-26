// GameNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Windows.h"
#include "GameNewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameNewDlg dialog


CGameNewDlg::CGameNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameNewDlg)
	m_playerNum = 2;
	//}}AFX_DATA_INIT
  m_bNew = true;
  m_player0.m_sPlayer.LoadString(IDS_NEWGAME_PLAYER0);
  m_player1.m_sPlayer.LoadString(IDS_NEWGAME_PLAYER1);
}

CGameNewDlg::CGameNewDlg(const PlayerMap &map, CWnd* pParent /*=NULL*/)
	: CDialog(CGameNewDlg::IDD, pParent)
{
  m_bNew = false;
	m_playerNum = map.size();
  PlayerMap::const_iterator it = map.begin();
  
  m_player0.m_sPlayer = (*it).second->sName().c_str(); 
  if (++it == map.end()) return;
  
  m_player1.m_sPlayer = (*it).second->sName().c_str(); 
  if (++it == map.end()) return;
  
  m_player2.m_sPlayer = (*it).second->sName().c_str(); 
  if (++it == map.end()) return;
  
  m_player3.m_sPlayer = (*it).second->sName().c_str(); 
  if (++it == map.end()) return;
}


void CGameNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameNewDlg)
	DDX_Control(pDX, IDC_NUM, m_editPlayerNum);
	DDX_Text(pDX, IDC_NUM, m_playerNum);
	DDV_MinMaxInt(pDX, m_playerNum, 2, 4);
	DDX_Control(pDX, IDC_COMBO2, m_player0);
	DDX_Control(pDX, IDC_COMBO3, m_player1);
	DDX_Control(pDX, IDC_COMBO4, m_player2);
	DDX_Control(pDX, IDC_COMBO5, m_player3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameNewDlg, CDialog)
	//{{AFX_MSG_MAP(CGameNewDlg)
	ON_EN_CHANGE(IDC_NUM, OnChangeNum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameNewDlg message handlers

void CGameNewDlg::OnChangeNum() 
{
  enablePlayers();
  UpdateWindow();
}

void CGameNewDlg::enablePlayers() 
{
  UpdateData(TRUE);
  m_player2.EnableWindow(m_playerNum >= 3);
  m_player3.EnableWindow(m_playerNum >= 4);
}

BOOL CGameNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  if (!m_bNew)
  {
    m_editPlayerNum.EnableWindow(FALSE);
    SetWindowText("Change players");
  }

  m_player0.UpdateData(FALSE);
  m_player1.UpdateData(FALSE);
  m_player2.UpdateData(FALSE);
  m_player3.UpdateData(FALSE);
  
  enablePlayers();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGameNewDlg::OnOK() 
{
  UpdateData(TRUE);

  m_player0.UpdateData(TRUE);
  if (m_player0.m_sPlayer == "") return;

  m_player1.UpdateData(TRUE);
  if (m_player1.m_sPlayer == "") return;

  if (m_playerNum >= 3)
  {
    m_player2.UpdateData(TRUE);
    if (m_player2.m_sPlayer == "") return;
  }

  if (m_playerNum >= 4)
  {
    m_player3.UpdateData(TRUE);
    if (m_player3.m_sPlayer == "") return;
  }

  /* Ok */
  EndDialog(IDOK);
}


PlayerMap CGameNewDlg::players() const
{
  PlayerMap map;

  SmartPtrPlayer spPlayer = new Player((const char *)m_player0.sPlayerPath());
  map[0] = spPlayer;

  spPlayer = new Player((const char *)m_player1.sPlayerPath());
  map[1] = spPlayer;
  if (m_playerNum <= 2) return map;

  spPlayer = new Player((const char *)m_player2.sPlayerPath());
  map[2] = spPlayer;
  if (m_playerNum <= 3) return map;

  spPlayer = new Player((const char *)m_player3.sPlayerPath());
  map[3] = spPlayer;

  return map;
}
