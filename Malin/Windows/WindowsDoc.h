// WindowsDoc.h : interface of the CWindowsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSDOC_H__74FB612B_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
#define AFX_WINDOWSDOC_H__74FB612B_465D_11D2_90C1_9D93B2FA395D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameAsync.h"

enum {UPDATEVIEW_SELECTION = 1, UPDATEVIEW_PLAYER, UPDATEVIEW_MOVE,
  UPDATEVIEW_CANCEL};

class CWindowsDoc : public CDocument
{
protected: // create from serialization only
  CWindowsDoc();
  DECLARE_DYNCREATE(CWindowsDoc)
    
    // Attributes
public:
private:
  CGameAsync* m_pGameA;
  
  // Operations
public:
  const CGameAsync & gameA() const { return *m_pGameA; }
  const Situation &sit() const { return m_pGameA->sit(); }
  const FullBoard &fullBoard() const { return m_pGameA->fullBoard(); }
  CString sError() { return m_pGameA->sError(); }
  bool bError() { return m_pGameA->bError(); }
  void computerMove() { m_pGameA->computerMove(); }
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CWindowsDoc)
	public:
  virtual void Serialize(CArchive& ar);
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual BOOL OnNewDocument();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
  virtual BOOL SaveModified();
	//}}AFX_VIRTUAL
  
  // Implementation
public:
  void humanMove(Move move);
	void initGameAsync();

  virtual ~CWindowsDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  
private:
  bool m_bStartup;

protected:
  
  // Generated message map functions
protected:
  //{{AFX_MSG(CWindowsDoc)
	afx_msg void OnFilePlayers();
	afx_msg void OnPlayerModify();
	afx_msg void OnPlayerNew();
	afx_msg void OnFrederikTournament();
  afx_msg void OnFrederikTraining();
	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWSDOC_H__74FB612B_465D_11D2_90C1_9D93B2FA395D__INCLUDED_)
