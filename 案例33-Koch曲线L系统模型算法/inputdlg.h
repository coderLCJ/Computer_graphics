#if !defined(AFX_INPUTDLG1_H__D6FB32C6_BBD4_4750_8C3F_A1592D3F713B__INCLUDED_)
#define AFX_INPUTDLG1_H__D6FB32C6_BBD4_4750_8C3F_A1592D3F713B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// InputDlg dialog

class CInputDlg : public CDialog
{
// Construction
public:
	CInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(InputDlg)
	enum { IDD = IDD_DIALOG1 };
	int		m_n;
	int		m_theta;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(InputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(InputDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG1_H__D6FB32C6_BBD4_4750_8C3F_A1592D3F713B__INCLUDED_)
