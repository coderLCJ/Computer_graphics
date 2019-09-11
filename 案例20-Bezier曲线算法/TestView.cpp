// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define N_MAX_POINT 21//控制多边形的最大顶点数
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
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
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
	P=new CPoint[N_MAX_POINT];
	bFlag=FALSE;
	CtrlPointNum=0;
}

CTestView::~CTestView()
{
	if(P!=NULL)
	{
		delete []P;
		P=NULL;
	}
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

void CTestView::DrawBezier()//绘制Bezier曲线
{
	CDC *pDC=GetDC();
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID,1,RGB(0,0,255));//曲线颜色为蓝色
	pOldPen=pDC->SelectObject(&NewPen);	
	pDC->MoveTo(P[0]);
	for(double t=0.0;t<=1.0;t+=0.01)
	{		
		double x=0,y=0;
		for(int i=0;i<=n;i++)
		{
			x+=P[i].x*Cni(n,i)*pow(t,i)*pow(1-t,n-i);
			y+=P[i].y*Cni(n,i)*pow(t,i)*pow(1-t,n-i);
		}
		pDC->LineTo(Round(x),Round(y));
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
	ReleaseDC(pDC);
}

double CTestView::Cni(const int &n, const int &i)//Bernstein第一项组合
{
	return double(Fac(n))/(Fac(i)*Fac(n-i));
}

long CTestView::Fac(int m)//阶乘函数
{
	long f;
	if(m==0||m==1)
		f=1;
	else
		f=m*Fac(m-1);
	return f;
}

void CTestView::DrawCtrlPolygon()//绘制控制多边形
{
	CDC *pDC=GetDC();
	CBrush NewBrush,*pOldBrush;
	pOldBrush=(CBrush*)pDC->SelectStockObject(GRAY_BRUSH);//灰色实心圆绘制控制点
	for(int i=0;i<=n;i++)
	{
		if(0==i)
		{
			pDC->MoveTo(P[i]);
			pDC->Ellipse(P[i].x-2,P[i].y-2,P[i].x+2,P[i].y+2);
		}
		else
		{
			pDC->LineTo(P[i]);
			pDC->Ellipse(P[i].x-2,P[i].y-2,P[i].x+2,P[i].y+2);
		}
	}
	pDC->SelectObject(pOldBrush);
	ReleaseDC(pDC);
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//获得屏幕控制点坐标 
{
	// TODO: Add your message handler code here and/or call default	
	if(bFlag)
	{
		P[CtrlPointNum].x=point.x;
		P[CtrlPointNum].y=point.y;
		if(CtrlPointNum<N_MAX_POINT-1)//N_MAX_POINT为控制点个数的最大值
			CtrlPointNum++;
		else
			bFlag=FALSE;
		n=CtrlPointNum-1;
		DrawCtrlPolygon();
	}
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDown(UINT nFlags, CPoint point)//调用曲线绘制函数 
{
	// TODO: Add your message handler code here and/or call default
	bFlag=FALSE;
	if(0!=CtrlPointNum)
		DrawBezier();
	CView::OnRButtonDown(nFlags, point);
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	MessageBox("左键绘制控制多边形，右键绘制曲线","提示",MB_OK);
	RedrawWindow();
	bFlag=TRUE;
	CtrlPointNum=0;
}
