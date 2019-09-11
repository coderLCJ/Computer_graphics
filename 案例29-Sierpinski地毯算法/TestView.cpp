// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
#include "InputDlg.h"//对话框头文件
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

void CTestView::Carpet(int n,CP2 p0,CP2 p1 )//地毯函数
{
	if(0==n)
	{
		FillRectangle(p0,p1);
		return;
	}
	double w=p1.x-p0.x,h=p1.y-p0.y;
	CP2 p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13;
	p2=CP2(p0.x+w/3,p0.y);
	p3=CP2(p0.x+2*w/3,p0.y);
	p4=CP2(p1.x,p0.y+h/3);
	p5=CP2(p1.x,p0.y+2*h/3);
	p6=CP2(p0.x+2*w/3,p1.y);
	p7=CP2(p0.x+w/3,p1.y);
	p8=CP2(p0.x,p0.y+2*h/3);
	p9=CP2(p0.x,p0.y+h/3);
	p10=CP2(p0.x+w/3,p0.y+h/3);
	p11=CP2(p0.x+2*w/3,p0.y+h/3);
    p12=CP2(p0.x+2*w/3,p0.y+2*h/3);
	p13=CP2(p0.x+w/3,p0.y+2*h/3);
	Carpet(n-1,p0,p10);
	Carpet(n-1,p2,p11);
	Carpet(n-1,p3,p4);
	Carpet(n-1,p9,p13);
	Carpet(n-1,p11,p5);
	Carpet(n-1,p8,p7);
	Carpet(n-1,p13,p6);
	Carpet(n-1,p12,p1);
}

void CTestView::FillRectangle(CP2 p0,CP2 p1)
{
	CBrush NewBrush,*pOldBrush;//声明画刷
	NewBrush.CreateSolidBrush(RGB(0,0,0));//创建黑色画刷
	pOldBrush=pDC->SelectObject(&NewBrush);//选入画刷

	pDC->BeginPath();
	pDC->MoveTo(Round(p0.x),Round(p0.y));//绘制正方形
	pDC->LineTo(Round(p1.x),Round(p0.y));
	pDC->LineTo(Round(p1.x),Round(p1.y));
	pDC->LineTo(Round(p0.x),Round(p1.y));
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
	CRect rect;
	GetClientRect(&rect);	
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	CP2 P0,P1;//正方形对角点坐标
	P0=CP2(-rect.Height()/2.0+20,-rect.Height()/2.0+20);
	P1=CP2(rect.Height()/2.0-20,rect.Height()/2.0-20);	
	Carpet(n,P0,P1);
}
