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
	ON_COMMAND(ID_TLEFT, OnLeft)
	ON_COMMAND(ID_TRIGHT, OnRight)
	ON_COMMAND(ID_TUP, OnUp)
	ON_COMMAND(ID_TDOWN, OnDown)
	ON_COMMAND(ID_SINCREASE, OnIncrease)
	ON_COMMAND(ID_SDECREASE, OnDecrease)
	ON_COMMAND(ID_RCLOCKWISE, OnCounterClockwise)
	ON_COMMAND(ID_RCOUNTERCLOCKWISE, OnClockwise)
	ON_COMMAND(ID_RXAXIS, OnXaxis)
	ON_COMMAND(ID_RYAXIS, OnYaxis)
	ON_COMMAND(ID_RORG, OnOrg)
	ON_COMMAND(ID_SYDIRECTIONPLUS, OnYDirectionPlus)
	ON_COMMAND(ID_SYDIRECTIONNEG, OnXDirectionNeg)
	ON_COMMAND(ID_SXDIRECTIONPLUS, OnXDirectionPlus)
	ON_COMMAND(ID_SXDIRECTIONNEG, OnYDirectionNeg)
	ON_COMMAND(ID_RESET, OnReset)
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
	CDC* pDC=GetDC();
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
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	ReleaseDC(pDC);//释放DC	
}

void CTestView::ReadPoint()//点表
{
	P[0].x=-100;P[0].y=-50;
	P[1].x=100; P[1].y=-50;
	P[2].x=100; P[2].y=50;
	P[3].x=-100;P[3].y=50;
}

void CTestView::DrawObject(CDC *pDC)//绘制图形
{
	CLine *line=new CLine;//绘制坐标系
	line->MoveTo(pDC,0,-rect.Height()/2);//绘制x轴
	line->LineTo(pDC,0,rect.Height()/2);
	line->MoveTo(pDC,-rect.Width()/2,0);//绘制y轴
	line->LineTo(pDC,rect.Width()/2,0);
	DrawPolygon(pDC);
	delete line;
}

void CTestView::DrawPolygon(CDC *pDC)//绘制多边形线框模型
{
	CLine *line=new CLine;
	CP2 t;
	for(int i=0;i<4;i++)//绘制多边形
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

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	MessageBox("请使用图标按钮进行二维几何变换","提示",MB_OK);
}

void CTestView::OnLeft()//向左平移 
{
	// TODO: Add your command handler code here
	trans.Translate(-10,0);
	Invalidate(FALSE);
}

void CTestView::OnRight()//向右平移 
{
	// TODO: Add your command handler code here
	trans.Translate(10,0);
	DoubleBuffer();
}

void CTestView::OnUp()//向上平移  
{
	// TODO: Add your command handler code here
	trans.Translate(0,10);
	Invalidate(FALSE);	
}

void CTestView::OnDown()//向下平移 
{
	// TODO: Add your command handler code here
    trans.Translate(0,-10);
	Invalidate(FALSE);	
}

void CTestView::OnIncrease()//等比放大  
{
	// TODO: Add your command handler code here
    trans.Scale(2,2);
	Invalidate(FALSE);	
}

void CTestView::OnDecrease()//等比缩小 
{
	// TODO: Add your command handler code here
    trans.Scale(0.5,0.5);
	Invalidate(FALSE);	
}

void CTestView::OnClockwise()//顺时针复合旋转  
{
	// TODO: Add your command handler code here
	CP2 p=(P[0]+P[2])/2;
	trans.Rotate(-30,p);
	Invalidate(FALSE);
}

void CTestView::OnCounterClockwise()//逆时针复合旋转 
{
	// TODO: Add your command handler code here
	CP2 p=(P[0]+P[2])/2;
    trans.Rotate(30,p);
	Invalidate(FALSE);		
}

void CTestView::OnXaxis()//关于x轴反射 
{
	// TODO: Add your command handler code here
    trans.ReflectX();
	Invalidate(FALSE);	
}

void CTestView::OnYaxis()//关于y轴反射 
{
	// TODO: Add your command handler code here
    trans.ReflectY();
	Invalidate(FALSE);	
}

void CTestView::OnOrg()//关于原点反射 
{
	// TODO: Add your command handler code here
    trans.ReflectOrg();
	Invalidate(FALSE);	
}

void CTestView::OnXDirectionPlus()//沿x轴正向错切  
{
	// TODO: Add your command handler code here
	trans.Shear(0,1);
	Invalidate(FALSE);	
}

void CTestView::OnXDirectionNeg()//沿x轴负向错切
{
	// TODO: Add your command handler code here
	trans.Shear(0,-1);
	Invalidate(FALSE);	
}

void CTestView::OnYDirectionPlus()//沿y轴正向错切 
{
	// TODO: Add your command handler code here
	trans.Shear(1,0);
	Invalidate(FALSE);	
}

void CTestView::OnYDirectionNeg()//沿y轴负向错切 
{
	// TODO: Add your command handler code here
	trans.Shear(-1,0);
	Invalidate(FALSE);	
}

void CTestView::OnReset()//复位 
{
	// TODO: Add your command handler code here
	ReadPoint();
	Invalidate(FALSE);	
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadPoint();
	trans.SetMat(P,4);
}
