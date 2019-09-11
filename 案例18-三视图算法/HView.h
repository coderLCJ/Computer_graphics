#if !defined(AFX_HVIEW_H__84DE8581_6F4C_40F4_890A_B7191F53412A__INCLUDED_)
#define AFX_HVIEW_H__84DE8581_6F4C_40F4_890A_B7191F53412A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHView view

class CHView : public CView
{
protected:
	CHView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HVIEW_H__84DE8581_6F4C_40F4_890A_B7191F53412A__INCLUDED_)
