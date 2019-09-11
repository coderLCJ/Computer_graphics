// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "math.h"//数学头文件
#define Round(d) int(floor(d+0.5))//四舍五入宏定义
#include "TestDoc.h"
#include "TestView.h"

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
ON_WM_MOUSEMOVE()
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
	m_AbleToLeftBtn=FALSE;
	m_AbleToMove=FALSE;
	m_i=-1;
	//初始化9个控制点
	P[0].x=120;P[0].y=350;
	P[1].x=250;P[1].y=250;
	P[2].x=316;P[2].y=420;
	P[3].x=428;P[3].y=167;
	P[4].x=525;P[4].y=500;
	P[5].x=650;P[5].y=250;
	P[6].x=682;P[6].y=40;
	P[7].x=850;P[7].y=450;
	P[8].x=950;P[8].y=350;
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
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
	CDC MemDC;//内存DC
	CBitmap NewBitmap,*pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//创建兼容位图 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC 
	MemDC.FillSolidRect(rect,pDC->GetBkColor());//按原来背景填充客户区，否则是黑色 
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID,3,RGB(0,0,0));
	pOldPen=MemDC.SelectObject(&NewPen);	
	MemDC.MoveTo(P[0]);
	MemDC.Ellipse(P[0].x-2,P[0].y-2,P[0].x+2,P[0].y+2);//绘制控制多边形顶点
	for(int i=1;i<9;i++)
	{
		MemDC.LineTo(P[i]);
		MemDC.Ellipse(P[i].x-2,P[i].y-2,P[i].x+2,P[i].y+2);
	}
	if(m_i!=-1)
	{
		CString	str;
		str.Format("x=%d,y=%d",P[m_i].x,P[m_i].y);
		MemDC.TextOut(P[m_i].x+5,P[m_i].y+5,str);
	}
	B3Curves(P,&MemDC);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldPen);
	NewPen.DeleteObject();	
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemDC	
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
void CTestView::B3Curves(CPoint q[],CDC *pDC)//绘制三次B样条曲线
{
	CPoint p;
	double F03,F13,F23,F33;
	p.x=Round((q[0].x+4.0*q[1].x+q[2].x)/6.0);//t＝0的起点x坐标
	p.y=Round((q[0].y+4.0*q[1].y+q[2].y)/6.0);//t＝0的起点y坐标
	pDC->MoveTo(p);
	CPen NewPen(PS_SOLID,1,RGB(255,0,0));//红笔画B样条曲线
	CPen *pOldPen=pDC->SelectObject(&NewPen);
	for(int i=1;i<7;i++)//6段样条曲线
	{
		for(double t=0;t<=1;t+=0.01)
		{
			F03=(-t*t*t+3*t*t-3*t+1)/6;//计算F0,3(t)
			F13=(3*t*t*t-6*t*t+4)/6;//计算F1,3(t)
			F23=(-3*t*t*t+3*t*t+3*t+1)/6;//计算F2,3(t)
			F33=t*t*t/6;//计算B3,3(t)
			p.x=Round(q[i-1].x*F03+q[i].x*F13+q[i+1].x*F23+q[i+2].x*F33);
			p.y=Round(q[i-1].y*F03+q[i].y*F13+q[i+1].y*F23+q[i+2].y*F33);
			pDC->LineTo(p);
		}
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();	
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point)//鼠标移动函数 
{
	// TODO: Add your message handler code here and/or call default
	if(TRUE==m_AbleToMove)
		P[m_i]=point;
	m_i=-1;
	int i;
	for(i=0;i<9;i++)
	{
		if((point.x-P[i].x)*(point.x-P[i].x)+(point.y-P[i].y)*(point.y-P[i].y)<50)
		{
			m_i=i;
			m_AbleToLeftBtn=TRUE;
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));//改变为十字箭头光标
			break;
		}
	}
	if(10==i)
	{
		m_i=-1;
	}
	Invalidate(FALSE);	
	CView::OnMouseMove(nFlags, point);
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) //鼠标左键按下函数
{
	// TODO: Add your message handler code here and/or call default
	if(m_AbleToLeftBtn==TRUE)
		m_AbleToMove=TRUE;
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnLButtonUp(UINT nFlags, CPoint point) //鼠标左键弹起函数 
{
	// TODO: Add your message handler code here and/or call default
	m_AbleToLeftBtn=FALSE;
	m_AbleToMove=FALSE;
	m_i=0;
	CView::OnLButtonUp(nFlags, point);
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	MessageBox("请直接拖动控制多边形顶点观察效果！","信息",MB_ICONINFORMATION|MB_OK);
}
