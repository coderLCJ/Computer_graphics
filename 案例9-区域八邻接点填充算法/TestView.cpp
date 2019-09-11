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
	ON_COMMAND(IDM_DRAWPIC, OnDrawpic)
	ON_WM_LBUTTONDOWN()
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
	DrawGraph();
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
	P[0].x=1;   P[0].y=-1;
	P[1].x=300; P[1].y=-1;
	P[2].x=300; P[2].y=300;
	P[3].x=0;   P[3].y=300;
	P[4].x=0;   P[4].y=0;
	P[5].x=-300;P[5].y=0;
	P[6].x=-300;P[6].y=-300;
	P[7].x=1;   P[7].y=-300;
}

void CTestView::DrawGraph()//绘制图形
{
	GetClientRect(&rect);                               //获得客户区的大小
	CDC *pDC=GetDC();                                   //定义设备上下文指针              
	pDC->SetMapMode(MM_ANISOTROPIC);                    //自定义坐标系
	pDC->SetWindowExt(rect.Width(),rect.Height());      //设置窗口比例
	pDC->SetViewportExt(rect.Width(),-rect.Height());   //设置视区比例，且x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//设置客户区中心为坐标系原点
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);  //矩形与客户区重合
	if(!bFill)
		DrawPolygon(pDC);//绘制多边形
	else
		FillPolygon(pDC);//填充多边形
	ReleaseDC(pDC);//释放DC
}

void CTestView::DrawPolygon(CDC *pDC)//绘制多边形边界
{
	CLine *line=new CLine;
	CP2 t;
	for(int i=0;i<8;i++)//绘制多边形
	{
		if(i==0)
		{
			line->MoveTo(pDC,P[i]);
			t=P[i];
		}
		else
		{
			line->LineTo(pDC,P[i]);
		}		
	}
	line->LineTo(pDC,t);//闭合多边形
	delete line;
}

void CTestView::FillPolygon(CDC *pDC)//填充多边形
{
	COLORREF BoundaryClr=RGB(0,0,0);//边界色
	COLORREF PixelClr;//当前像素点的颜色
	pHead=new CStackNode;//建立头结点
	pHead->pNext=NULL;//栈头结点的指针域总为空
	Push(Seed);	
	int x,y,x0=Round(Seed.x),y0=Round(Seed.y);//x，y用于判断种子与图形的位置关系
	x=x0-1;
	while(BoundaryClr!=pDC->GetPixel(x,y0) && SeedClr!=pDC->GetPixel(x,y0))//左方判断
	{
		x--;
		if(x<=-rect.Width()/2)//到达客户区最左端
		{
			MessageBox("种子不在图形之内","警告");//如果出现此对话框弹出错误，请注释
			return;
		}
	}
	y=y0+1;
	while(BoundaryClr!=pDC->GetPixel(x0,y) && SeedClr!=pDC->GetPixel(x0,y))//上方判断
	{
		y++;
		if(y>=rect.Height()/2)//到达客户区最上端
		{
			MessageBox("种子不在图形之内","警告");//如果出现此对话框弹出错误，请注释
			return;
		}
	}
	x=x0+1;
	while(BoundaryClr!=pDC->GetPixel(x,y0) && SeedClr!=pDC->GetPixel(x,y0))//右方判断
	{
		x++;
		if(x>=rect.Width()/2)//到达客户区最右端
		{
			MessageBox("种子不在图形之内","警告");//如果出现此对话框弹出错误，请注释		
			return;
		}
	}
	y=y0-1;
	while(BoundaryClr!=pDC->GetPixel(x0,y) && SeedClr!=pDC->GetPixel(x0,y))//下方判断
	{
		y--;
		if(y<=-rect.Height()/2)//到达客户区最下端
		{
			MessageBox("种子不在图形之内","警告");//如果出现此对话框弹出错误，请注释
			return;
		}
	}
	while(pHead->pNext!=NULL)//如果栈不为空
	{
		CP2 PopPoint;
		Pop(PopPoint);
		if(SeedClr==pDC->GetPixel(Round(PopPoint.x),Round(PopPoint.y)))
			continue;
		pDC->SetPixelV(Round(PopPoint.x),Round(PopPoint.y),SeedClr);
		Left.x=PopPoint.x-1;//搜索出栈结点的左方像素
		Left.y=PopPoint.y;
		PixelClr=pDC->GetPixel(Round(Left.x),Round(Left.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)//不是边界色并且未置成填充色
			Push(Left);//左方像素入栈
		LeftTop.x=PopPoint.x-1;
		LeftTop.y=PopPoint.y+1;//搜索出栈结点的左上方像素
		PixelClr=pDC->GetPixel(Round(LeftTop.x),Round(LeftTop.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(LeftTop);	//左上方像素入栈
		Top.x=PopPoint.x;
		Top.y=PopPoint.y+1;//搜索出栈结点的上方像素
		PixelClr=pDC->GetPixel(Round(Top.x),Round(Top.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(Top);	//上方像素入栈
		RightTop.x=PopPoint.x+1;
		RightTop.y=PopPoint.y+1;//搜索出栈结点的右上方像素
		PixelClr=pDC->GetPixel(Round(RightTop.x),Round(RightTop.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(RightTop);	//右上方像素入栈
		Right.x=PopPoint.x+1;//搜索出栈结点的右方像素
		Right.y=PopPoint.y;
		PixelClr=pDC->GetPixel(Round(Right.x),Round(Right.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(Right);//右方像素入栈	
		RightBottom.x=PopPoint.x+1;//搜索出栈结点的右下方像素
		RightBottom.y=PopPoint.y-1;
		PixelClr=pDC->GetPixel(Round(RightBottom.x),Round(RightBottom.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(RightBottom);//右下方像素入栈	
		Bottom.x=PopPoint.x;
		Bottom.y=PopPoint.y-1;//搜索出栈结点的下方像素
		PixelClr=pDC->GetPixel(Round(Bottom.x),Round(Bottom.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(Bottom);//下方像素入栈
		LeftBottom.x=PopPoint.x-1;
		LeftBottom.y=PopPoint.y-1;//搜索出栈结点的左下方像素
		PixelClr=pDC->GetPixel(Round(LeftBottom.x),Round(LeftBottom.y));
		if(BoundaryClr!=PixelClr && SeedClr!=PixelClr)
			Push(LeftBottom);//左下方像素入栈
	}
	pDC->TextOut(rect.left+50,rect.bottom-20,"填充完毕");
	delete pHead;
	pHead = NULL;
}

void CTestView::Push(CP2 point)//入栈函数
{
	pTop=new CStackNode;
	pTop->PixelPoint=point;
	pTop->pNext=pHead->pNext;
	pHead->pNext=pTop;
}

void CTestView::Pop(CP2 &point)//出栈函数
{
	if(pHead->pNext!=NULL)
	{
		pTop=pHead->pNext;
		pHead->pNext=pTop->pNext;
		point=pTop->PixelPoint;
		delete pTop;
	}
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CColorDialog ccd(SeedClr,CC_SOLIDCOLOR);
	if(IDOK==ccd.DoModal())//调用颜色对话框选取填充色
		SeedClr=ccd.GetColor();	
	else
		return;	
	if(IDOK==MessageBox("请在区域内部选取种子点","提示",MB_OKCANCEL))
	    bFill=TRUE;
	else
		return;	
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(TRUE==bFill)
	{
		Seed=CP2(point.x-rect.Width()/2,rect.Height()/2-point.y);//选择种子位置
		Invalidate(FALSE);
	}
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	bFill=FALSE;
	SeedClr=RGB(0,0,255);
	ReadPoint();
}


