// WView.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "WView.h"
#include "TestDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWView

IMPLEMENT_DYNCREATE(CWView, CView)

CWView::CWView()
{
}

CWView::~CWView()
{
}


BEGIN_MESSAGE_MAP(CWView, CView)
	//{{AFX_MSG_MAP(CWView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWView drawing

void CWView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CWView diagnostics

#ifdef _DEBUG
void CWView::AssertValid() const
{
	CView::AssertValid();
}

void CWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWView message handlers

