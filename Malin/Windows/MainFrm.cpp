// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Windows.h"
#include "WindowsDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_PLAYER, OnUpdateStatus)
  ON_UPDATE_COMMAND_UI(IDC_HISTORY, OnUpdateHistory)
  
  ON_UPDATE_COMMAND_UI(IDC_PLAYER0, OnUpdatePlayer0)
  ON_UPDATE_COMMAND_UI(IDC_PLAYER1, OnUpdatePlayer1)
  ON_UPDATE_COMMAND_UI(IDC_PLAYER2, OnUpdatePlayer2)
  ON_UPDATE_COMMAND_UI(IDC_PLAYER3, OnUpdatePlayer3)

  ON_UPDATE_COMMAND_UI(IDC_TIME0, OnUpdateTime0)
  ON_UPDATE_COMMAND_UI(IDC_TIME1, OnUpdateTime1)
  ON_UPDATE_COMMAND_UI(IDC_TIME2, OnUpdateTime2)
  ON_UPDATE_COMMAND_UI(IDC_TIME3, OnUpdateTime3)

  ON_UPDATE_COMMAND_UI(IDC_COLOR0, OnUpdateColor)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_PLAYER,
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	//{{AFX_DATA_INIT(CMainFrame)
	//}}AFX_DATA_INIT
  m_nHist = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
    CBRS_RIGHT, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  // Create a window without min/max buttons or sizable border 
  cs.style = cs.style & ~FWS_ADDTOTITLE;
  // Size the window to 3/4 screen size and center it 
  cs.cy = ::GetSystemMetrics(SM_CYSCREEN) * 0.75; 
  cs.cx = ::GetSystemMetrics(SM_CXSCREEN) * 0.75; 
  cs.y = ((cs.cy / 0.75) - cs.cy) / 2;     cs.x = ((cs.cx / 0.75) - cs.cx) / 2;
  // Call the base-class version    
  return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnUpdateStatus(CCmdUI *pCmdUI)
{
  /* ? Document available */
  CWindowsDoc *pDoc = (CWindowsDoc *) GetActiveDocument();
  if (!pDoc) return;
  
  pCmdUI->SetText(pDoc->gameA().sStatus());
  pCmdUI->Enable();
}


void CMainFrame::OnUpdatePlayer0(CCmdUI *pCmdUI)
{
  //TRACE("CMainFrame::OnUpdatePlayer0\n");
  OnUpdatePlayer(pCmdUI, 0);
}
void CMainFrame::OnUpdatePlayer1(CCmdUI *pCmdUI)
{
  OnUpdatePlayer(pCmdUI, 1);
}
void CMainFrame::OnUpdatePlayer2(CCmdUI *pCmdUI)
{
  OnUpdatePlayer(pCmdUI, 2);
}
void CMainFrame::OnUpdatePlayer3(CCmdUI *pCmdUI)
{
  OnUpdatePlayer(pCmdUI, 3);
}


void CMainFrame::OnUpdatePlayer(CCmdUI *pCmdUI, int player)
{
  /* ? Document available */
  CWindowsDoc *pDoc = (CWindowsDoc *) GetActiveDocument();
  CString s;
  if (!pDoc) return;

  /* How many players */
  if (player >= pDoc->fullBoard().m_playerNum)
  {
    s.Format("Player #%d", player + 1);
    pCmdUI->SetText(s);
    pCmdUI->Enable(FALSE);
    return;
  }
    
  /* Load string */
  PlayerMap playerMap = pDoc->gameA().players(); 
  s.Format("%s (#%d)", playerMap[player]->sName().c_str(), player + 1);

  pCmdUI->SetText(s);
  pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateTime0(CCmdUI *pCmdUI)
{
  OnUpdateTime(pCmdUI, 0);
}
void CMainFrame::OnUpdateTime1(CCmdUI *pCmdUI)
{
  OnUpdateTime(pCmdUI, 1);
}
void CMainFrame::OnUpdateTime2(CCmdUI *pCmdUI)
{
  OnUpdateTime(pCmdUI, 2);
}
void CMainFrame::OnUpdateTime3(CCmdUI *pCmdUI)
{
  OnUpdateTime(pCmdUI, 3);
}


void CMainFrame::OnUpdateTime(CCmdUI *pCmdUI, int player)
{
  /* ? Document available */
  CWindowsDoc *pDoc = (CWindowsDoc *) GetActiveDocument();
  CString s;
  if (!pDoc) return;

  /* How many players */
  if (player >= pDoc->fullBoard().m_playerNum)
  {
    pCmdUI->SetText("");
    pCmdUI->Enable(FALSE);
    return;
  }
    
  /* Load string */
  time_t time = pDoc->gameA().timePlayers()[player];
  
  s.Format("%d - %d, %02d:%02d", pDoc->fullBoard().m_kicked[player], 
    pDoc->fullBoard().m_lost[player], time/60, time%60);

  pCmdUI->SetText(s);
  pCmdUI->Enable(TRUE);
}



void CMainFrame::OnUpdateHistory(CCmdUI *pCmdUI)
{
  /* ? Document available */
  CWindowsDoc *pDoc = (CWindowsDoc *) GetActiveDocument();
  CString s;
  if (!pDoc) return;

  /* Get listbox pointer */
  CListBox *pList = (CListBox*) m_wndDlgBar.GetDlgItem(IDC_HISTORY);
  
  if (!pList) return;
  if (m_nHist == pDoc->sit().m_liHist.size()) return;
  
  /* Fill in history */
  const HistoryList hist = pDoc->sit().m_liHist;
  if (m_nHist == hist.size()) return;
  else if (hist.size() == 0)
  {
    m_nHist = 0;
    pList->ResetContent();
  }
  else if (m_nHist < hist.size())
  {
    int nDiff = hist.size() - m_nHist;
    
    HistoryList::const_reverse_iterator rit = hist.rbegin();
    for (int n = 0; n < nDiff - 1; n++) rit++;

    for (n = 0; n < nDiff; n++, rit--)
    {
      s.Format("%d (#%d): %s", ++m_nHist, rit->m_playerAct + 1, 
        rit->m_fMove.string().c_str());
      pList->AddString(s);
    }
  }
  else // m_nHist > hist.size()
  {
    m_nHist = 0;
    pList->ResetContent();

    /*int nDiff = m_nHist - hist.size();

    for (int i = 0; i < nDiff; i++)
    {
      pList->DeleteString(pList->GetCount() - 1);
    }
    m_nHist = hist.size();*/
  }

  pList->SetCurSel(pList->GetCount() - 1);
}


void CMainFrame::OnUpdateColor(CCmdUI *pCmdUI)
{
  /* ? Document available */
  CWindowsDoc *pDoc = (CWindowsDoc *) GetActiveDocument();
  CString s;
  if (!pDoc) return;

  /* ? Players changed */
  PlayerMap player = pDoc->gameA().players();

  if (player.size() == m_player.size())
  {
    /* Check if players are still the same */
    for (int n = 0; n < player.size(); n++)
    {
      if (player[n] != m_player[n]) break;
    }
    /* ? All equal */
    if (n >= player.size()) return;
  }
  
  m_player = player;

  /* Invalidate rectangles */
  CRect rect;

  m_wndDlgBar.GetDlgItem(IDC_COLOR0)->GetWindowRect(rect);
  ScreenToClient(rect);
  InvalidateRect(rect);

  m_wndDlgBar.GetDlgItem(IDC_COLOR1)->GetWindowRect(rect);
  ScreenToClient(rect);
  InvalidateRect(rect);

  m_wndDlgBar.GetDlgItem(IDC_COLOR2)->GetWindowRect(rect);
  ScreenToClient(rect);
  InvalidateRect(rect);

  m_wndDlgBar.GetDlgItem(IDC_COLOR3)->GetWindowRect(rect);
  ScreenToClient(rect);
  InvalidateRect(rect);
}

void CMainFrame::OnPaint() 
{
  CClientDC cDC(this);
  CRect rect;
  
  m_wndDlgBar.GetDlgItem(IDC_COLOR0)->GetWindowRect(rect);
  ScreenToClient(rect);
  CBrush brush0;
  if (m_player.size() >= 0 + 1) brush0.CreateSolidBrush(COLOR_PLAYER0);
  else brush0.CreateSolidBrush(COLOR_LIGHTGRAY);
  cDC.FillRect(rect, &brush0);
  cDC.DrawEdge(rect, EDGE_SUNKEN, BF_RECT);	
  ValidateRect(rect);

  m_wndDlgBar.GetDlgItem(IDC_COLOR1)->GetWindowRect(rect);
  ScreenToClient(rect);
  CBrush brush1;
  if (m_player.size() >= 1 + 1) brush1.CreateSolidBrush(COLOR_PLAYER1);
  else brush1.CreateSolidBrush(COLOR_LIGHTGRAY);
  cDC.FillRect(rect, &brush1);
  cDC.DrawEdge(rect, EDGE_SUNKEN, BF_RECT);	
  ValidateRect(rect);

  m_wndDlgBar.GetDlgItem(IDC_COLOR2)->GetWindowRect(rect);
  ScreenToClient(rect);
  CBrush brush2;
  if (m_player.size() >= 2 + 1) brush2.CreateSolidBrush(COLOR_PLAYER2);
  else brush2.CreateSolidBrush(COLOR_LIGHTGRAY);
  cDC.FillRect(rect, &brush2);
  cDC.DrawEdge(rect, EDGE_SUNKEN, BF_RECT);	
  ValidateRect(rect);

  m_wndDlgBar.GetDlgItem(IDC_COLOR3)->GetWindowRect(rect);
  ScreenToClient(rect);
  CBrush brush3;
  if (m_player.size() >= 3 + 1) brush3.CreateSolidBrush(COLOR_PLAYER3);
  else brush3.CreateSolidBrush(COLOR_LIGHTGRAY);
  cDC.FillRect(rect, &brush3);
  cDC.DrawEdge(rect, EDGE_SUNKEN, BF_RECT);	
  ValidateRect(rect);

  CFrameWnd::OnPaint();
}



