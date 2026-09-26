// WindowsView.h : interface of the CWindowsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSVIEW_H__74FB612D_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
#define AFX_WINDOWSVIEW_H__74FB612D_465D_11D2_90C1_9D93B2FA395D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWindowsView : public CView
{
protected: // create from serialization only
	CWindowsView();
	DECLARE_DYNCREATE(CWindowsView)

// Attributes
public:
	CWindowsDoc* GetDocument();

// Data:
private:
	PlayerMap m_player;
  // Marbles
  CRect m_rectMarble[POS_NUM + 1];      // Holds the logical rectangles for all marbles
  
  enum MarbleState {unselectedUp, selectedUp, selectedDown, reSelectedDown};
  MarbleState m_stateMarble[POS_NUM + 1];     // Marble selected ?

  // Left mouse button
  CPoint m_pointLButtonDown;

  // Pens & Brushes
  CPen m_penUnselected;
  CPen m_penSelected;
  CBrush m_brushEmpty;
  CBrush m_brushPlayer[PLAYERS_MAX];


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowsView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
private:
	void moveMarble(Pos n, Dir dir);

private:
	bool m_bLoop;
  FullBoard m_fullBoard;
  float m_fDPtoLPx;
  float m_fDPtoLPy;
	int m_nHist;

public:
	virtual ~CWindowsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWindowsView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WindowsView.cpp
inline CWindowsDoc* CWindowsView::GetDocument()
   { return (CWindowsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWSVIEW_H__74FB612D_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
