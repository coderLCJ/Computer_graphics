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
#define  InMax   7  //最大输入顶点数
#define  OutMax  12 //最大输出顶点数
#define  LEFT    1
#define  RIGHT   2
#define  TOP     3
#define  BOTTOM  4
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
	bDrawRect=FALSE;
	bClip=FALSE;
	OutCount=0;
	RtCount=0;
}

CTestView::~CTestView()
{
	if(In!=NULL)
	{
		delete []In;
		In=NULL;
	}
	if(Out!=NULL)
	{
		delete []Out;
		Out=NULL;
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
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
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
	if(RtCount && !bClip)
		DrawWindowRect(&MemDC);//绘制窗口
	DrawObject(&MemDC,bClip);//绘制多边形		
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
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

void CTestView::ReadPoint()//点表
{
	In=new CP2[InMax];//输入顶点表
	In[0].x=50;  In[0].y=100;
	In[1].x=-150;In[1].y=300;
	In[2].x=-250;In[2].y=50;
	In[3].x=-150;In[3].y=-250;
	In[4].x=0;   In[4].y=-50;
	In[5].x=100; In[5].y=-250;
	In[6].x=300; In[6].y=150;
	Out=new CP2[OutMax];//输出顶点表
	for(int i=0;i<OutMax;i++)
		Out[i]=CP2(0,0);
}

void CTestView::DrawWindowRect(CDC* pDC)//绘制裁剪窗口
{
	// TODO: Add your message handler code here and/or call default
	CPen NewPen3,*pOldPen3;//定义3个像素宽度的画笔
	NewPen3.CreatePen(PS_SOLID,3,RGB(0,128,0));
	pOldPen3=pDC->SelectObject(&NewPen3);
	pDC->MoveTo(Round(Rect[0].x),Round(Rect[0].y));
	pDC->LineTo(Round(Rect[1].x),Round(Rect[0].y));
	pDC->LineTo(Round(Rect[1].x),Round(Rect[1].y));
	pDC->LineTo(Round(Rect[0].x),Round(Rect[1].y));
	pDC->LineTo(Round(Rect[0].x),Round(Rect[0].y));
	pDC->SelectObject(pOldPen3);
	NewPen3.DeleteObject();
}

void CTestView::ClipBoundary(CP2 rect0,CP2 rect1)//窗口边界赋值函数
{
	if(rect0.x>rect1.x)
	{
		Wxl=rect1.x;
		Wxr=rect0.x;
	}
	else
	{
		Wxl=rect0.x;
		Wxr=rect1.x;
	}
	if(rect0.y>rect1.y)
	{
		Wyt=rect0.y;
		Wyb=rect1.y;
	}
	else
	{
		Wyt=rect1.y;
		Wyb=rect0.y;
	}
}

void CTestView::DrawObject(CDC *pDC,BOOL bclip)//绘制多边形
{
	if(!bclip)
	{
		for(int i=0;i<InMax;i++)//绘制裁剪前的多边形
		{
			if(0==i)
				pDC->MoveTo(Round(In[i].x),Round(In[i].y));
		    else
		    	pDC->LineTo(Round(In[i].x),Round(In[i].y));
		}
    	pDC->LineTo(Round(In[0].x),Round(In[0].y));
	}
	else
	{
		ClipPolygon(In,InMax,LEFT);
		ClipPolygon(Out,OutCount,RIGHT);
		ClipPolygon(Out,OutCount,BOTTOM);
		ClipPolygon(Out,OutCount,TOP);
		for(int j=0;j<OutCount;j++)//绘制裁剪后的多边形
		{
			if(0==j)
				pDC->MoveTo(Round(Out[j].x),Round(Out[j].y));
		    else
		    	pDC->LineTo(Round(Out[j].x),Round(Out[j].y));
		}
		if(0!=OutCount)
			pDC->LineTo(Round(Out[0].x),Round(Out[0].y));
	}
}

void CTestView::ClipPolygon(CP2 *out,int Length,UINT Boundary)//裁剪多边形
{
	if(0==Length)
		return;
	CP2 *pTemp=new CP2[Length];
	for(int i=0;i<Length;i++)
		pTemp[i]=out[i];
	CP2 p0,p1,p;//p0-起点，p1-终点，p-交点
	OutCount=0;
	p0=pTemp[Length-1];
	for(i=0;i<Length;i++)
	{
		p1=pTemp[i];
		if(Inside(p0,Boundary))//起点在窗口内
		{
			if(Inside(p1,Boundary))//终点在窗口内,属于内→内
			{
				Out[OutCount]=p1;//终点在窗口内
				OutCount++;
			}
			else//属于内→外
			{
				p=Intersect(p0,p1,Boundary);//求交点
				Out[OutCount]=p;
				OutCount++;
			}
		}
		else if(Inside(p1,Boundary))//终点在窗口内,属于外→内
		{
			p=Intersect(p0,p1,Boundary);//求交点
			Out[OutCount]=p;
			OutCount++;
			Out[OutCount]=p1;
			OutCount++;
		}
		p0=p1;
	}
	delete[] pTemp;
}

BOOL CTestView::Inside(CP2 p,UINT Boundary)//判断点在窗口的内外
{
	switch(Boundary)
	{
	case LEFT:
		if(p.x>=Wxl)
			return TRUE;
		break;
	case RIGHT:
		if(p.x<=Wxr)
			return TRUE;
		break;
	case TOP:
		if(p.y<=Wyt)
			return TRUE;
		break;
	case BOTTOM:
		if(p.y>=Wyb)
			return TRUE;
		break;
	}
	return FALSE;
}

CP2 CTestView::Intersect(CP2 p0,CP2 p1,UINT Boundary)//求交点
{
	CP2 pTemp;
	double k=(p1.y-p0.y)/(p1.x-p0.x);//直线段的斜率
	switch(Boundary)
	{
	case LEFT:
		pTemp.x=Wxl;
		pTemp.y=k*(pTemp.x-p0.x)+p0.y;
		break;
	case RIGHT:		
		pTemp.x=Wxr;
		pTemp.y=k*(pTemp.x-p0.x)+p0.y;
		break;
	case TOP:
		pTemp.y=Wyt;
		pTemp.x=(pTemp.y-p0.y)/k+p0.x;
		break;
	case BOTTOM:
		pTemp.y=Wyb;
		pTemp.x=(pTemp.y-p0.y)/k+p0.x;
		break;
	}
	return pTemp;
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(bDrawRect)
	{	
		if(RtCount<2)
		{
			Rect[RtCount]=Convert(point);
    		RtCount++;
		}	
	}
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!bClip)
	{
		if(RtCount<2)
		{   
			Rect[RtCount]=Convert(point);
			Invalidate(FALSE);
		}
	}

	CView::OnMouseMove(nFlags, point);
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

void CTestView::OnDrawpic()//图形菜单 
{
	// TODO: Add your command handler code here
	RtCount=0;
	bDrawRect=TRUE;
	bClip=FALSE;
	Rect[0]=Rect[1]=CP2(0,0);
	Invalidate(FALSE);
	MessageBox("鼠标绘制窗口，剪刀进行裁剪","提示",MB_OKCANCEL);
}

void CTestView::OnClip()//裁剪菜单  
{
	// TODO: Add your command handler code here
	RtCount=0;
	bDrawRect=FALSE;
	bClip=TRUE;
	ClipBoundary(Rect[0],Rect[1]);
	Invalidate(FALSE);
}

void CTestView::OnUpdateClip(CCmdUI* pCmdUI)//剪切图标按钮状态控制 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((RtCount>=2)?TRUE:FALSE);	
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadPoint();
}
