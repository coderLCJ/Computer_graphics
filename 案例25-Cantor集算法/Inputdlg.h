#if !defined(AFX_INPUTDLG_H__0E8D2950_1B42_4B91_BF75_9FB9AEFF6735__INCLUDED_)
#define AFX_INPUTDLG_H__0E8D2950_1B42_4B91_BF75_9FB9AEFF6735__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Inputdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Inputdlg dialog

class CInputDlg : public CDialog
{
// Construction
public:
	CInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Inputdlg)
	enum { IDD = IDD_DIALOG1 };
	int		m_n;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Inputdlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Inputdlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__0E8D2950_1B42_4B91_BF75_9FB9AEFF6735__INCLUDED_)
