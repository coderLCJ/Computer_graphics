// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"
#define Round(d) int(floor(d+0.5))//四舍五入宏定义
#define PI 3.141592
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
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_ZOOMOUT, OnZoomout)
	ON_COMMAND(IDM_ZOOMIN, OnZoomin)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
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
	nRHWidth=100;
	nRHHeight=100;
	nScale=2;
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
	Diamond(&MemDC,FALSE);//绘制金刚石背景图
	DrawRect(&MemDC,nScale);//绘制放大镜
	Diamond(&MemDC,TRUE);//绘制裁剪窗口中的金刚石
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	ReleaseDC(pDC);//释放DC
}

void CTestView::DrawRect(CDC *pDC,int nScale)//绘制裁剪窗口
{
	pDC->Rectangle(Round(nRCenter.x-nRHWidth),Round(nRCenter.y+nRHHeight),
		Round(nRCenter.x+nRHWidth),Round(nRCenter.y-nRHHeight));
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	nRCenter=Convert(point);//鼠标指针为放大镜中心
	Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}

BOOL CTestView::LBLineClip()//Liang-Barsky裁剪函数
{
	double tmax,tmin,dx,dy;
	dx=P[1].x-P[0].x;dy=P[1].y-P[0].y;tmax=0.0,tmin=1.0;
	double wxl=nRCenter.x-nRHWidth/nScale;//窗口的左边界
	double wxr=nRCenter.x+nRHWidth/nScale;//窗口的右边界
	double wyb=nRCenter.y-nRHHeight/nScale;//窗口的下边界
	double wyt=nRCenter.y+nRHHeight/nScale;//窗口的上边界
	//窗口边界的左、右、下、上顺序裁剪直线
	if(ClipTest(-dx,P[0].x-wxl,tmax,tmin))//n＝1,左边界u1＝－△x，v1＝x0－wxl
	{
		if(ClipTest(dx,wxr-P[0].x,tmax,tmin))//n＝2，右边界u2＝△x，v2＝wxr－x0
		{			
			if(ClipTest(-dy,P[0].y-wyb,tmax,tmin))//n＝3，下边界u3＝－△y，v3＝y0-wyb
			{
				if(ClipTest(dy,wyt-P[0].y,tmax,tmin))//n＝4，上边界u4＝△y，v4＝wyt-y0
				{
					if(tmin<1.0)//判断直线的终点
					{
						P[1].x=P[0].x+tmin*dx;//重新计算直线终点坐标
						P[1].y=P[0].y+tmin*dy;//x＝x0＋t(x1－x0)格式
					}
					if(tmax>0.0)//判断直线的起点
					{
						P[0].x=P[0].x+tmax*dx;//重新计算直线起点坐标
						P[0].y=P[0].y+tmax*dy;//x＝x0＋t(x1－x0)格式
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CTestView::ClipTest(double u,double v,double &tmax,double &tmin)//裁剪测试函数
{
	double t;
	BOOL ReturnValue=TRUE;
	if(u<0.0)//从裁剪窗口的外部到内部,计算起点处的tmax
	{
		t=v/u;
		if(t>tmin)
			ReturnValue=FALSE;
		else if(t>tmax)
			tmax=t;
	}
	else
	{
		if(u>0.0)//从裁剪窗口的内部到外部，计算终点处的tmin
		{
			t=v/u;
			if(t<tmax)
				ReturnValue=FALSE;
			else if(t<tmin)
				tmin=t;
		}
		else//平行于窗口边界的直线
		{
			if(v<0.0)//直线在窗口外可直接删除
				ReturnValue=FALSE;
		}
	}
	return(ReturnValue);
}

CP2 CTestView::Convert(CPoint point)//坐标系变换 
{
	CRect rect;
	GetClientRect(&rect);
	CP2 ptemp;
	ptemp.x=point.x-rect.Width()/2;
	ptemp.y=rect.Height()/2-point.y;
	return ptemp;
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景擦除 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::Diamond(CDC *pDC,BOOL bclip)
{
	CPen NewPen,*pOldPen;//定义画笔
	if(bclip)
		NewPen.CreatePen(PS_SOLID,nScale,RGB(0,0,0));//创建nScale宽度画笔
	else
		NewPen.CreatePen(PS_SOLID,1,RGB(0,0,0));//创建1像素宽度画笔
	pOldPen=pDC->SelectObject(&NewPen);//将蓝色画笔选入设备上下文
	double thta;//thta为圆的等分角	
	int n=30;//定义等分点个数
	V=new CP2[n];   
	double r=300;//定义圆的半径
	thta=2*PI/n;
	for(int i=0;i<n;i++)//计算等分点坐标
	{
		V[i].x=r*cos(i*thta);
		V[i].y=r*sin(i*thta);
	}
	for(i=0;i<=n-2;i++)//依次各连接等分点
	{
		for(int j=i+1;j<=n-1;j++)
		{
			if(!bclip)
			{
				pDC->MoveTo(Round(V[i].x),Round(V[i].y));
				pDC->LineTo(Round(V[j].x),Round(V[j].y));
			}
			else
			{		
				P[0]=V[i];P[1]=V[j];
				if(LBLineClip())					
				{
					pDC->MoveTo(ZoomX(Round(P[0].x)),ZoomY(Round(P[0].y)));
					pDC->LineTo(ZoomX(Round(P[1].x)),ZoomY(Round(P[1].y)));					
				}
			}	
		}
	}
	delete []V; 
	pDC->SelectObject(pOldPen);//恢复设备上下文原画笔
	NewPen.DeleteObject();//删除已成自由状态的新画笔 
}

int CTestView::ZoomX(int x)
{
	return Round((x-nRCenter.x)*nScale+nRCenter.x);
}

int CTestView::ZoomY(int y)
{
	return Round((y-nRCenter.y)*nScale+nRCenter.y);
}

void CTestView::OnZoomin()//放大图形菜单
{
	// TODO: Add your command handler code here
	if(nScale<=5)
		nScale=nScale+1;
	else
		MessageBox("放大倍数太大");
	Invalidate(FALSE);
}

void CTestView::OnZoomout()//缩小图形菜单 
{
	// TODO: Add your command handler code here
	if(nScale>=2)
		nScale=nScale-1;
	else
		MessageBox("放大倍数太小");
	Invalidate(FALSE);
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nScale<=5)
		nScale=nScale+1;
	else
		MessageBox("放大倍数太大");	
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nScale>=2)
		nScale=nScale-1;
	else
		MessageBox("放大倍数太小");
	Invalidate(FALSE);
	CView::OnRButtonDown(nFlags, point);
}
