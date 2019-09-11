// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog


CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(InputDlg)
	m_n = 4;
	m_s=1;
	//}}AFX_DATA_INIT
}


void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(InputDlg)
	DDX_Text(pDX, IDC_EDIT1, m_n);
	DDV_MinMaxInt(pDX, m_n, 0, 10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	//{{AFX_MSG_MAP(InputDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InputDlg message handlers

void CInputDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);//设置radio默认
	GetDlgItem(IDC_EDIT1)->SetFocus();//设置edit焦点
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetSel(0,-1);//设置edit选中
}

void CInputDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_s=1;
}

void CInputDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_s=-1;
}
