// TestView.cpp : implementation of the CTestView class
#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"
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

void CTestView::Peano_Hilbert(int n,int s,CP2 p0,CP2 p1)//Peano-Hilbert函数
{
	double w,h;	
	if(0==n)
  	{
		CP2 p2,p3,p4,p5;
		w=p1.x-p0.x;h=p1.y-p0.y;
		p2=CP2(p0.x+w/4,p0.y+h/4);
		p3=CP2(p0.x+(2-s)*w/4,p0.y+(2+s)*h/4);
		p4=CP2(p0.x+3*w/4,p0.y+3*h/4);
		p5=CP2(p0.x+(2+s)*w/4,p0.y+(2-s)*h/4);
		pDC->MoveTo(Round(P0.x),Round(P0.y));
		pDC->LineTo(Round(p2.x),Round(p2.y));
		pDC->LineTo(Round(p3.x),Round(p3.y));
		pDC->LineTo(Round(p4.x),Round(p4.y));
		pDC->LineTo(Round(p5.x),Round(p5.y));
		P0=p5;
		return;
     }
	 CP2 p2,p3,p4,p5,p6;
	 p2=CP2((p0.x+p1.x)/2,p0.y);
	 p3=CP2(p1.x,(p0.y+p1.y)/2);
	 p4=CP2((p0.x+p1.x)/2,p1.y);
	 p5=CP2(p0.x,(p0.y+p1.y)/2);
	 p6=(p0+p1)/2;
     if(s>0)
     {
		 Peano_Hilbert(n-1,-1,p0,p6);//左
		 Peano_Hilbert(n-1,1,p5,p4);//下
         Peano_Hilbert(n-1,1,p6,p1);//下
         Peano_Hilbert(n-1,-1,p3,p2);//左
	 }
     else
     {
		 Peano_Hilbert(n-1,1,p0,p6);//下
         Peano_Hilbert(n-1,-1,p2,p3);//左
         Peano_Hilbert(n-1,-1,p6,p1);//左
         Peano_Hilbert(n-1,1,p4,p5);//下
     }
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CInputDlg dlg;
	if (IDOK==dlg.DoModal())
	{
		n=dlg.m_n;
		s=dlg.m_s;
	}
	else
		return;
	RedrawWindow();	
	pDC=GetDC();
	CRect rect;//客户区矩形对象
	GetClientRect(&rect);	
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);
	P0=CP2(-rect.Width()/2.0,-rect.Height()/2.0);
	P1=CP2(rect.Width()/2.0,rect.Height()/2.0);
	Peano_Hilbert(n,s,P0,P1);
	ReleaseDC(pDC);
}
