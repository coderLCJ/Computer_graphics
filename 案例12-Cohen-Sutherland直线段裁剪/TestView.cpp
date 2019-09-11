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
#define LEFT   1   //代表:0001
#define RIGHT  2   //代表:0010
#define BOTTOM 4   //代表:0100
#define TOP    8   //代表:1000
/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_DRAWPIC, OnDrawpic)
	ON_COMMAND(IDM_CLIP, OnClip)
	ON_UPDATE_COMMAND_UI(IDM_CLIP, OnUpdateClip)
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
	Wxl=-300;Wyt=100;Wxr=300;Wyb=-100;
	PtCount=0;
	bDrawLine=FALSE;
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
	DoubleBuffer();
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
void CTestView::DoubleBuffer()//双缓冲
{
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
	CDC *pDC=GetDC();
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(),rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(),-rect.Height());//x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//屏幕中心为原点
	CDC MemDC;//内存DC
	CBitmap NewBitmap,*pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//创建兼容位图 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC 
	MemDC.FillSolidRect(&rect,pDC->GetBkColor());//按原来背景填充客户区，否则是黑色 
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	DrawWindowRect(&MemDC);//绘制窗口
	if(PtCount>=1)
	{
		MemDC.MoveTo(Round(P[0].x),Round(P[0].y));
		MemDC.LineTo(Round(P[1].x),Round(P[1].y));			
	}
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
}

void CTestView::DrawWindowRect(CDC* pDC)//绘制裁剪窗口
{
	// TODO: Add your message handler code here and/or call default
	pDC->SetTextColor(RGB(128,0,0));
	pDC->TextOut(-10,Wyt+20,"窗口");
	CPen NewPen3,*pOldPen3;//定义3个像素宽度的画笔
	NewPen3.CreatePen(PS_SOLID,3,RGB(0,128,0));
	pOldPen3=pDC->SelectObject(&NewPen3);
    pDC->Rectangle(Wxl,Wyt,Wxr,Wyb);
	pDC->SelectObject(pOldPen3);
	NewPen3.DeleteObject();
}

void CTestView::Cohen()//Cohen-Sutherland算法
{
	CP2 p;//交点坐标
	EnCode(P[0]);//起点编码
	EnCode(P[1]);//终点编码
	while(P[0].rc!=0 || P[1].rc!=0)//处理至少一个顶点在窗口之外的情况
	{
		if((P[0].rc & P[1].rc)!=0)//简弃之
		{
			PtCount=0;
			return;
		}
		if(0==P[0].rc)//确保P[0]位于窗口之外
		{
			CP2 Temp;
			Temp=P[0];
			P[0]=P[1];
			P[1]=Temp;
		}
		UINT RC=P[0].rc;
		double k=(P[1].y-P[0].y)/(P[1].x-P[0].x);//直线段的斜率
		//窗口边界按左、右、下、上的顺序裁剪直线段
		if(RC & LEFT)//P[0]点位于窗口的左侧
		{
			p.x=Wxl;//计算交点y坐标
			p.y=k*(p.x-P[0].x)+P[0].y;
		}
		else if(RC & RIGHT)//P[0]点位于窗口的右侧
		{
			p.x=Wxr;//计算交点y坐标
			p.y=k*(p.x-P[0].x)+P[0].y;
		}
		else if(RC & BOTTOM)//P[0]点位于窗口的下侧
		{
			p.y=Wyb;//计算交点x坐标
			p.x=(p.y-P[0].y)/k+P[0].x;
		}			
		else if(RC & TOP)//P[0]点位于窗口的上侧
		{
			p.y=Wyt;//计算交点x坐标
			p.x=(p.y-P[0].y)/k+P[0].x;
		}
		EnCode(p);
		P[0]=p;
	}
}

void CTestView::EnCode(CP2 &pt)//端点编码函数
{
	pt.rc=0;
	if(pt.x<Wxl)
		pt.rc=pt.rc|LEFT;
	else if(pt.x>Wxr)
		pt.rc=pt.rc|RIGHT;
	if(pt.y<Wyb)
		pt.rc=pt.rc|BOTTOM;
	else if(pt.y>Wyt)
		pt.rc=pt.rc|TOP;
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(bDrawLine)
	{	if(PtCount<2)
		{
			P[PtCount]=Convert(point);
			PtCount++;
		}
	}
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(bDrawLine)
	{
		if(PtCount<2)
		{   
			P[PtCount]=Convert(point);
			Invalidate(FALSE);
		}
	}
	CView::OnMouseMove(nFlags, point);
}

CP2 CTestView::Convert(CPoint point)//设备坐标系向自定义坐标系转换
{
	CRect rect;
	GetClientRect(&rect);
	CP2 ptemp;
	ptemp.x=point.x-rect.Width()/2;
	ptemp.y=rect.Height()/2-point.y;
	return ptemp;
}

void CTestView::OnDrawpic()//图形菜单  
{
	// TODO: Add your command handler code here
	PtCount=0;
	bDrawLine=TRUE;
	MessageBox("鼠标画线，剪刀裁剪","提示",MB_OKCANCEL);
	Invalidate(FALSE);
}

void CTestView::OnClip()//裁剪菜单 
{
	// TODO: Add your command handler code here
	Cohen();
	bDrawLine=FALSE;
	Invalidate(FALSE);
}

void CTestView::OnUpdateClip(CCmdUI* pCmdUI)//剪切图标按钮状态控制 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((PtCount>=2)?TRUE:FALSE);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景擦除 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}


