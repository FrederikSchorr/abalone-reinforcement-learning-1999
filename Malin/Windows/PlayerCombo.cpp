// PlayerCombo.cpp : implementation file
//

#include "stdafx.h"
#include "Windows.h"
#include "PlayerCombo.h"
#include "../Kernel/Kernel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayerCombo

CPlayerCombo::CPlayerCombo()
{
  
}

CPlayerCombo::~CPlayerCombo()
{
}


BEGIN_MESSAGE_MAP(CPlayerCombo, CComboBox)
	//{{AFX_MSG_MAP(CPlayerCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerCombo message handlers


CString CPlayerCombo::sPlayerPath() const
{
  /* Return complete path of player */
  CString sPath = Kernel::sDirectoryPlayer.c_str();
  sPath += m_sPlayer + "." + EXTENSION_PLAYER;
  sPath.Replace('/', '\\');
  return sPath;
}


void CPlayerCombo::UpdateData(BOOL bRetrieve)
{
  /* Retrieve player from combo */
  if (bRetrieve)
  {
    int sel = GetCurSel();
    if (sel == CB_ERR) m_sPlayer = "";
    else 
    {
      GetLBText(sel, m_sPlayer);
      m_sPlayer = m_sPlayer.Left(m_sPlayer.Find("  ("));
    }
  }
  /* Initialize combo */
  else
  {
    ResetContent();

    /* Find files in player directory */
    CFileFind finder;
    BOOL bWorking = finder.FindFile((Kernel::sDirectoryPlayer + 
      "*." + EXTENSION_PLAYER).c_str());
    while (bWorking)
    {
      bWorking = finder.FindNextFile();
      try
      {
        /* Read player desc */
        CStdioFile file(finder.GetFilePath(), CFile::modeRead);
        CString sDesc;
        file.ReadString(sDesc);

        /* Add to combo */
        AddString(finder.GetFileTitle() + "  (" + sDesc + ")");   
      }
      catch(CFileException e)
      {
      }
    }

    /* Select actual player */
    SelectString(0, m_sPlayer);
  }
}
