// VView.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "VView.h"
#include "TestDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVView

IMPLEMENT_DYNCREATE(CVView, CView)

CVView::CVView()
{
}

CVView::~CVView()
{
}


BEGIN_MESSAGE_MAP(CVView, CView)
	//{{AFX_MSG_MAP(CVView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVView drawing

void CVView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CVView diagnostics

#ifdef _DEBUG
void CVView::AssertValid() const
{
	CView::AssertValid();
}

void CVView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVView message handlers
