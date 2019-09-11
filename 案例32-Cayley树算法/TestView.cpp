// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define PI 3.1415926//圆周率
#include "math.h"//数学头文件
#define Round(d) int(floor(d+0.5))//四舍五入宏定义
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_COMMAND(IDM_DRAWPIC, OnDrawpic)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView construction/destruction

CTestView::CTestView()
{
	// TODO: add construction code here
	pDC=NULL;
}

CTestView::~CTestView()
{
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers

void CTestView::Cayley(int n, CP2 p0, double height, double theta)//Cayley函数
{
	CP2 p1,p2,p3;
	double Scale=2.0/3.0;
	if(1==n) 
		return;
	p1.x=p0.x+height*cos(theta);
	p1.y=p0.y+height*sin(theta);
	p2.x=p1.x+Scale*height*cos(theta+Alpha);
	p2.y=p1.y+Scale*height*sin(theta+Alpha);
	p3.x=p1.x+Scale*height*cos(theta-Alpha);
	p3.y=p1.y+Scale*height*sin(theta-Alpha);
	pDC->MoveTo(Round(p0.x),Round(p0.y));
	pDC->LineTo(Round(p1.x),Round(p1.y));
	pDC->LineTo(Round(p2.x),Round(p2.y));
	pDC->MoveTo(Round(p1.x),Round(p1.y));
	pDC->LineTo(Round(p3.x),Round(p3.y));	
	Cayley(n-1,p1,Scale*height,theta+Alpha);
	Cayley(n-1,p1,Scale*height,theta-Alpha);
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CInputDlg dlg;
	if(IDOK==dlg.DoModal())
	{
		n=dlg.m_n;
		Height=dlg.m_height;
		Alpha=dlg.m_alpha;
	}
	else
		return;
	RedrawWindow();
	pDC=GetDC();	
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	P.x=0;P.y=-250;Theta=PI/2;
	Alpha=Alpha*PI/180;
	Cayley(n,P,Height,Theta);
	ReleaseDC(pDC);
}
