// TestView.cpp : implementation of the CTestView class
//

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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
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
	GetClientRect(&rect);//获得客户区矩形的大小
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

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	p0.x=point.x;
	p0.y=point.y;
	p0.x=p0.x-rect.Width()/2;                           //设备坐标系向自定义坐标系系转换
	p0.y=rect.Height()/2-p0.y;
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	p1.x=point.x;
	p1.y=point.y;
	CDC *pDC=GetDC();                                   //定义设备上下文指针
	pDC->SetMapMode(MM_ANISOTROPIC);                    //自定义坐标系
	pDC->SetWindowExt(rect.Width(),rect.Height());      //设置窗口比例
	pDC->SetViewportExt(rect.Width(),-rect.Height());   //设置视区比例，且x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//设置客户区中心为坐标系原点
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);  //矩形与客户区重合
	p1.x=p1.x-rect.Width()/2;
	p1.y=rect.Height()/2-p1.y;
	double r=sqrt((p1.x-p0.x)*(p1.x-p0.x)+(p1.y-p0.y)*(p1.y-p0.y))/2.0;//计算圆的半径
	MBCircle(r,pDC);//调用圆中点Bresenham算法	
	ReleaseDC(pDC);
	CView::OnLButtonUp(nFlags, point);
}

void CTestView::MBCircle(double R,CDC *pDC)//圆中点Bresenham算法
{
	double x,y,d;	 
	d=1.25-R;x=0;y=R;
	for(x=0;x<=y;x++)
	{
		CirclePoint(x,y,pDC);//调用八分法画圆子函数
        if (d<0)
			d+=2*x+3;
        else
		{
			d+=2*(x-y)+5;
			y--;
		} 
     }
}

void CTestView::CirclePoint(double x, double y,CDC *pDC)//八分法画圆子函数
{
	CP2 pc=CP2((p0.x+p1.x)/2.0,(p0.y+p1.y)/2.0);        //圆心坐标
	COLORREF  clr=RGB(0,0,255);                         //定义圆的边界颜色
	pDC->SetPixelV(Round(x+pc.x),Round(y+pc.y),clr);     //x,y
	pDC->SetPixelV(Round(y+pc.x),Round(x+pc.y),clr);     //y,x
	pDC->SetPixelV(Round(y+pc.x),Round(-x+pc.y),clr);    //y,-x
	pDC->SetPixelV(Round(x+pc.x),Round(-y+pc.y),clr);    //x,-y
	pDC->SetPixelV(Round(-x+pc.x),Round(-y+pc.y),clr);   //-x,-y
	pDC->SetPixelV(Round(-y+pc.x),Round(-x+pc.y),clr);   //-y,-x
	pDC->SetPixelV(Round(-y+pc.x),Round(x+pc.y),clr);    //-y,x
	pDC->SetPixelV(Round(-x+pc.x),Round(y+pc.y),clr);    //-x,y
	//pDC->MoveTo(Round(p0.x),Round(p0.y));
	//pDC->LineTo(Round(p1.x),Round(p1.y));	
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	MessageBox("请按下鼠标左键绘图！","提示",MB_ICONEXCLAMATION|MB_OK);
	RedrawWindow();
}

