// HView.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "HView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHView

IMPLEMENT_DYNCREATE(CHView, CView)

CHView::CHView()
{
}

CHView::~CHView()
{
}


BEGIN_MESSAGE_MAP(CHView, CView)
	//{{AFX_MSG_MAP(CHView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHView drawing

void CHView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CHView diagnostics

#ifdef _DEBUG
void CHView::AssertValid() const
{
	CView::AssertValid();
}

void CHView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHView message handlers
