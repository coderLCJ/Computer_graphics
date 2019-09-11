#if !defined(AFX_WVIEW_H__590A3608_ED77_4CE4_85A3_64182A5E3F52__INCLUDED_)
#define AFX_WVIEW_H__590A3608_ED77_4CE4_85A3_64182A5E3F52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWView view

class CWView : public CView
{
protected:
	CWView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WVIEW_H__590A3608_ED77_4CE4_85A3_64182A5E3F52__INCLUDED_)
