#if !defined(AFX_VVIEW_H__27521169_F994_4745_B2A4_A178C060CFD2__INCLUDED_)
#define AFX_VVIEW_H__27521169_F994_4745_B2A4_A178C060CFD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VView.h : header file
//
#include "P3.h"//包含三维点头文件
#include "Line.h"//包含直线头文件
/////////////////////////////////////////////////////////////////////////////
// CVView view

class CVView : public CView
{
protected:
	CVView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CVView)

// Attributes
public:

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CVView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CVView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VVIEW_H__27521169_F994_4745_B2A4_A178C060CFD2__INCLUDED_)
