// PlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Windows.h"
#include "PlayerDlg.h"
#include "../Kernel/Kernel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog


CPlayerDlg::CPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayerDlg)
	m_sInput = _T("");
	m_sOutput = _T("");
	//}}AFX_DATA_INIT
}


void CPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerDlg)
	DDX_Control(pDX, IDC_HISTORY, m_editHistory);
	DDX_Text(pDX, IDC_INPUT, m_sInput);
	DDX_Text(pDX, IDC_OUTPUT, m_sOutput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayerDlg)
	ON_BN_CLICKED(IDC_CONTINUE, OnContinue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg message handlers

void CPlayerDlg::modify(const std::string &sOutput, float &fInput, 
  const float &fLower, const float &fUpper, const int nDecimals)
{
  /* Load in & output */
  m_sOutput = sOutput.c_str();
  m_sInput.Format("%.*f", nDecimals, fInput);
  
  /* Read */
  m_fLower = fLower;
  m_fUpper = fUpper;
  m_bFloat = true;
  if (DoModal() != IDOK) throw CancelE();

  /* Retrieve and store to history */
  fInput = atof(m_sInput); 
  m_sHistory += m_sOutput + ": " + m_sInput + _T("\r\n");
}


void CPlayerDlg::modify(const std::string &sOutput, std::string &sInput)
{
  /* Load in & output */
  m_sOutput = sOutput.c_str();
  m_sInput = sInput.c_str();
  
  /* Read */
  m_fLower = m_fUpper = 0.0;
  m_bFloat = false;
  if (DoModal() != IDOK) throw CancelE();

  /* Retrieve and store to history */
  sInput = (const char *)m_sInput;
  m_sHistory += m_sOutput + ": " + m_sInput + _T("\r\n");
}


void CPlayerDlg::println(const std::string &sOutput)
{
  m_sHistory += sOutput.c_str();
  m_sHistory += _T("\r\n");
}

void CPlayerDlg::OnContinue() 
{
  UpdateData(TRUE);

  /* If float is read, check bounds */
  if (m_bFloat)
  {
    float f = atof(m_sInput);
    if (f < m_fLower || f > m_fUpper)
    {
      AfxMessageBox("Input value must respect bounds", MB_ICONINFORMATION);
      return;
    }
  }

  /* Return to DoModal caller */
  EndDialog(IDOK);
}

BOOL CPlayerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
  
  m_editHistory.SetWindowText(m_sHistory);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


