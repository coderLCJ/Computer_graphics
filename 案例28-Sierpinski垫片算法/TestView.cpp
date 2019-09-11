// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
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

void CTestView::Gasket(int n,CP2 p0,CP2 p1,CP2 p2)//垫片函数
{
	if(0==n)
	{
		FillTriangle(p0,p1,p2);
		return;
	}
	CP2 p01,p12,p20;
	p01=(p0+p1)/2;p12=(p1+p2)/2;p20=(p2+p0)/2;
	Gasket(n-1,p0,p01,p20);
	Gasket(n-1,p01,p1,p12);
	Gasket(n-1,p20,p12,p2);	
}

void CTestView::FillTriangle(CP2 p0,CP2 p1,CP2 p2)
{
	CBrush NewBrush,*pOldBrush;//创建画刷
	NewBrush.CreateSolidBrush(RGB(0,0,0));
	pOldBrush=pDC->SelectObject(&NewBrush);	
	pDC->BeginPath();
	pDC->MoveTo(Round(p0.x),Round(p0.y));//绘制三角形
	pDC->LineTo(Round(p1.x),Round(p1.y));
	pDC->LineTo(Round(p2.x),Round(p2.y));
	pDC->LineTo(Round(p0.x),Round(p0.y));
	pDC->EndPath();
	pDC->FillPath();
	pDC->SelectObject(pOldBrush);//恢复保存的画刷
	NewBrush.DeleteObject();//删除新画刷
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CInputDlg dlg;
	if(IDOK==dlg.DoModal())
	{
		n=dlg.m_n;
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
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	CP2 P0,P1,P2;//等边三角形顶点坐标
	P0=CP2(-rect.Width()/2.0+50,-rect.Height()/2.0+20);
	P1=CP2(rect.Width()/2.0-50,-rect.Height()/2.0+20);	
	P2=CP2(0,rect.Height()/2.0-20);
	Gasket(n,P0,P1,P2);
	ReleaseDC(pDC);
}
