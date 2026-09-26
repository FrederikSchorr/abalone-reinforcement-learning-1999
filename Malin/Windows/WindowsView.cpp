// WindowsView.cpp : implementation of the CWindowsView class
//
    
#include "stdafx.h"
#include "Windows.h"

#include "WindowsDoc.h"
#include "WindowsView.h"

#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define   TIMER_UPDATE_VIEW   500


/////////////////////////////////////////////////////////////////////////////
// CWindowsView

IMPLEMENT_DYNCREATE(CWindowsView, CView)

BEGIN_MESSAGE_MAP(CWindowsView, CView)
//{{AFX_MSG_MAP(CWindowsView)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowsView construction/destruction

CWindowsView::CWindowsView()
{
  /* Init data */
  for (Pos n = 0; n <= POS_NUM; n++) m_stateMarble[n] = unselectedUp;
  m_nHist = 0;

  /* Load pens */
  m_penSelected.CreatePen(PS_SOLID | PS_INSIDEFRAME, 12, RGB(  0,  0,128));
  m_penUnselected.CreatePen(PS_SOLID | PS_INSIDEFRAME, 1, RGB(0,0,0));
    
  /* Load brushes */
  m_brushEmpty.CreateSolidBrush(COLOR_LIGHTGRAY);
  m_brushPlayer[0].CreateSolidBrush(COLOR_PLAYER0);
  m_brushPlayer[1].CreateSolidBrush(COLOR_PLAYER1);
  m_brushPlayer[2].CreateSolidBrush(COLOR_PLAYER2);
  m_brushPlayer[3].CreateSolidBrush(COLOR_PLAYER3);
}

CWindowsView::~CWindowsView()
{
}

BOOL CWindowsView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs
  
  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWindowsView drawing

void CWindowsView::OnDraw(CDC* pDC)
{
  TRACE("CWindowsView::OnDraw\n");

  /* Prepare drawing */
  pDC->SetMapMode(MM_ANISOTROPIC);
  pDC->SetWindowExt(1000 * m_fDPtoLPx, 1000 * m_fDPtoLPy);
  pDC->SetViewportExt(1000, 1000);

  CPen *pOldPen = pDC->SelectObject(&m_penUnselected);
  CBrush *pOldBrush = pDC->SelectObject(&m_brushEmpty);
  
  /* Draw marbles */
  for (Count n = 1; n <= POS_NUM; n++)
  {
    if (m_fullBoard.m_marble[n] == MARBLE_EMPTY)
      pDC->SelectObject(&m_brushEmpty);
    else 
      pDC->SelectObject(&m_brushPlayer[m_fullBoard.m_marble[n]]);
    
    if (m_stateMarble[n] == unselectedUp)
      pDC->SelectObject(m_penUnselected);
    else
      pDC->SelectObject(&m_penSelected);
    
    pDC->Ellipse(m_rectMarble[n]);
  }
    
  /* Restore CDC */
  pDC->SelectObject(pOldPen);
  pDC->SelectObject(pOldBrush);
}

void CWindowsView::OnInitialUpdate()
{
  try
  {
    //TRACE("CWindowsView::OnInitalUpdate\n");
    CView::OnInitialUpdate();

    /* Install timer to update view */
    SetTimer(2, TIMER_UPDATE_VIEW, NULL);
   
    /* Calculate marble positions */
    CSize total(1000, 1000);
    
    CPoint center;
    center.x = total.cx / 2; center.y = total.cy / 2;
    
    CSize step;
    step.cx= total.cx / 20.0; step.cy= total.cy / 10.0;
    
    CPoint marble[POS_NUM + 1];
    Count nPos, i, nLine;
    
    for (nPos=27,i=-4; nPos<=35; nPos++,i++)
    {
      marble[nPos].x= center.x+ (i*2*step.cx);
      marble[nPos].y= center.y;
    }
    
    for (nLine=1,nPos=1; nLine<=4; nLine++)
    {
      for (i=0; i<nLine+4; i++,nPos++)
      {
        marble[nPos].x= center.x- ((nLine+3)*step.cx)+ (i*2*step.cx);
        marble[nPos].y= center.y- ((5-nLine)*step.cy);
      }
    }
    for (nLine=1,nPos=61; nLine<=4; nLine++)
    {
      for (i=0; i<nLine+4; i++,nPos--)
      {
        marble[nPos].x= center.x+ ((nLine+3)*step.cx)- (i*2*step.cx);
        marble[nPos].y= center.y+ ((5-nLine)*step.cy);
      }
    }
    
    /* Store marble rectangles */
    int radius = 0.9 * step.cx;
    for (nPos = 1; nPos <= POS_NUM; nPos++)
    {
      m_rectMarble[nPos].left = marble[nPos].x - radius;
      m_rectMarble[nPos].top = marble[nPos].y - radius;
      m_rectMarble[nPos].right = marble[nPos].x + radius;
      m_rectMarble[nPos].bottom = marble[nPos].y + radius;
    }
  }
  catch (exception e)
  {
    AfxMessageBox(e.what(), MB_ICONEXCLAMATION);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CWindowsView printing

BOOL CWindowsView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CWindowsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CWindowsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWindowsView diagnostics

#ifdef _DEBUG
void CWindowsView::AssertValid() const
{
  CView::AssertValid();
}

void CWindowsView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CWindowsDoc* CWindowsView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWindowsDoc)));
  return (CWindowsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWindowsView message handlers

void CWindowsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
  /*** Only invalidate client area if changes occured ***/
  const CGameAsync & gameA = GetDocument()->gameA();

  /* ? Error message */
  if (GetDocument()->bError())
    AfxMessageBox(GetDocument()->sError(), MB_ICONINFORMATION);

  int n;
  switch(lHint)
  {
  case UPDATEVIEW_SELECTION:
    InvalidateRect(NULL, FALSE);
    break;

  case UPDATEVIEW_PLAYER:
    m_player = gameA.players();
    break;

  case UPDATEVIEW_CANCEL:
    for (n = 1; n <= POS_NUM; n++) m_stateMarble[n] = unselectedUp;
    InvalidateRect(NULL, FALSE);
    break;
    
  case UPDATEVIEW_MOVE:
    {
    Move *pMove = (Move*) pHint;
    for (n = 0; n < pMove->m_count; n++)
      m_stateMarble[pMove->m_pos[n]] = selectedUp;
    InvalidateRect(NULL, FALSE);
    break;
    }

  default:
    /* Retrieve all if board changed */
    if ( !((Board)m_fullBoard).equals(gameA.fullBoard()) )
    {
      for (n = 1; n <= POS_NUM; n++) m_stateMarble[n] = unselectedUp;
      m_fullBoard = gameA.fullBoard();
      m_player = gameA.players();
      InvalidateRect(NULL, FALSE);
    }
  }
}

void CWindowsView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  /* ? worker thread active */
  if (GetDocument()->gameA().bWorking()) 
  {
    AfxMessageBox("Currently computing next move ...", 
      MB_ICONINFORMATION);
    return;
  }

  try
  {
    /* Transform device units to logical units */
    point.x *= m_fDPtoLPx;
    point.y *= m_fDPtoLPy;

    /* Store position */
    m_pointLButtonDown = point;

    /* Loop through all marbles */
    for (Pos n = 1; n <= POS_NUM; n++)
    {
      /* ? over which marble was button pressed */
      if (m_rectMarble[n].PtInRect(point))
      {
        Count player;
        switch (m_stateMarble[n])
        {
        case selectedUp:
        case selectedDown:
        case reSelectedDown:
          m_stateMarble[n] = reSelectedDown;
          break;

        case unselectedUp:
          /* Get player */
          player = m_fullBoard.m_playerAct;

          /* ? My marble */
          if (m_fullBoard.m_marble[n] == player)
          {
            /* ? computer player */
            if (!m_player[player]->bIsHuman())
            {
              if (AfxMessageBox("This is a computer player, \ndo you want to compute the next move ?",
                MB_YESNO | MB_ICONQUESTION) == IDYES)
              {
                GetDocument()->computerMove();
              }
              return;
            }

            /* Select marble */
            m_stateMarble[n] = selectedDown;
            OnUpdate(NULL, UPDATEVIEW_SELECTION, NULL);
          }
          /* Ignore empty marbles */
          else if (m_fullBoard.m_marble[n] == MARBLE_EMPTY) ;
          /* ? Enemy marble */
          else
          {
            throw exception("It's not your turn");
          }
          break;
          
        default:
          ASSERT(0);
        }
        break;
      }
    }
  }
  catch (exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
  }
    
  CView::OnLButtonDown(nFlags, point);
}

void CWindowsView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  if (GetDocument()->gameA().bWorking()) return;

  try
  {
    /* Transform device units to logical units */
    point.x *= m_fDPtoLPx;
    point.y *= m_fDPtoLPy;
    
    /* Loop through all marbles */
    for (Pos n = 1; n <= POS_NUM; n++)
    {
      /* ? over which marble was button pressed */
      if (m_rectMarble[n].PtInRect(m_pointLButtonDown))
      {
        /* ? was button released over the same marble */
        if (m_rectMarble[n].PtInRect(point))
        {
          switch(m_stateMarble[n])
          {
          case selectedDown:
            m_stateMarble[n] = selectedUp;
            break;

          case reSelectedDown:
            m_stateMarble[n] = unselectedUp;
            OnUpdate(NULL, UPDATEVIEW_SELECTION, NULL);
            break;

          default:
            ;
          }
        }
        /* Button not released over the same marble */
        else
        {
          switch(m_stateMarble[n])
          {
          case selectedDown:
          case reSelectedDown:
            {
            /* Move marbles */
            double angle = atan2(-1.0 * (point.y - m_pointLButtonDown.y), 
              point.x - m_pointLButtonDown.x);

            double fDir = angle * 3.0 / 3.1514926;
            if (fDir < 0.0) fDir += 6.49;
            else fDir += 0.49;
            int nDir = (int)(fDir) % 6;

            moveMarble(n, nDir);
            break;
            }
          default:
            break;
          }

        } 
        m_pointLButtonDown = CPoint(0,0); 
        break;
      }
    }
  }
  catch (exception e)
  {
    AfxMessageBox(e.what(), MB_ICONINFORMATION);
  }
  
  /* Call base class */
  CView::OnLButtonUp(nFlags, point);
}



void CWindowsView::moveMarble(Pos nPos, Dir dir)
{
  
  /* Collect all selected marbles */
  Pos n;
  Move move;
  for (n = 1, move.m_count = 0; n <= POS_NUM; n++)
  {
    if (m_stateMarble[n] != unselectedUp)
    {
      /* ? still enough place */
      if (move.m_count >= SEL_MAX) 
        throw exception("You have selected more than 3 marbles");
      
      /* Add to 'move' */
      move.m_pos[move.m_count++] = n;
    }
  }
  
  /* Set direction */
  move.m_dir = dir;
  
  /* Move */
  GetDocument()->humanMove(move);
}



void CWindowsView::OnTimer(UINT nIDEvent) 
{
	OnUpdate(NULL, NULL, NULL);
}

void CWindowsView::OnSize(UINT nType, int cx, int cy) 
{
  //TRACE("CWindowsView::OnSize\n");
	CView::OnSize(nType, cx, cy);

  if (cx <= 0) cx = 20;
  if (cy <= 0) cy = 20;

	CClientDC cDC(this);
  CSize m(cx, cy);

  cDC.DPtoHIMETRIC(&m);

  if (m.cx > m.cy)
  {
    m_fDPtoLPy = 1000.0 / (float)cy;
    m_fDPtoLPx = 1000.0 * ((float)m.cx / (float)m.cy) / (float)cx;
  }
  else
  {
    m_fDPtoLPx = 1000.0 / (float)cx;
    m_fDPtoLPy = 1000.0 * ((float)m.cy / (float)m.cx) / (float)cy;
  }
}
