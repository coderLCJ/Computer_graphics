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
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_TBACK, OnBack)
	ON_COMMAND(ID_TFRONT, OnFront)
	ON_COMMAND(ID_RXAXIS, OnRxaxis)
	ON_COMMAND(ID_RYAXIS, OnRyaxis)
	ON_COMMAND(ID_RZAXIS, OnRzaxis)
	ON_COMMAND(ID_RXOY, OnRxoy)
	ON_COMMAND(ID_RYOZ, OnRyoz)
	ON_COMMAND(ID_RXOZ, OnRzox)
	ON_COMMAND(ID_SXDIRECTIONPLUS, OnSxdirectionplus)
	ON_COMMAND(ID_SYDIRECTIONPLUS, OnSydirectionplus)
	ON_COMMAND(ID_SZDIRECTIONNEG, OnSzdirectionneg)
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
void CTestView::ReadPoint()//点表
{
	double a=100;//立方体边长为a
	//顶点的三维坐标(x,y,z)
	P[0].x=0;P[0].y=0;P[0].z=0;
	P[1].x=a;P[1].y=0;P[1].z=0;
	P[2].x=a;P[2].y=a;P[2].z=0;
	P[3].x=0;P[3].y=a;P[3].z=0;
	P[4].x=0;P[4].y=0;P[4].z=a;
	P[5].x=a;P[5].y=0;P[5].z=a;
	P[6].x=a;P[6].y=a;P[6].z=a;
	P[7].x=0;P[7].y=a;P[7].z=a;
}

void CTestView::ReadFace()//面表
{
	//面的边数、面的顶点编号
	F[0].SetNum(4);F[0].vI[0]=4;F[0].vI[1]=5;F[0].vI[2]=6;F[0].vI[3]=7;//前面
	F[1].SetNum(4);F[1].vI[0]=0;F[1].vI[1]=3;F[1].vI[2]=2;F[1].vI[3]=1;//后面
	F[2].SetNum(4);F[2].vI[0]=0;F[2].vI[1]=4;F[2].vI[2]=7;F[2].vI[3]=3;//左面
	F[3].SetNum(4);F[3].vI[0]=1;F[3].vI[1]=2;F[3].vI[2]=6;F[3].vI[3]=5;//右面
	F[4].SetNum(4);F[4].vI[0]=2;F[4].vI[1]=3;F[4].vI[2]=7;F[4].vI[3]=6;//顶面
	F[5].SetNum(4);F[5].vI[0]=0;F[5].vI[1]=1;F[5].vI[2]=5;F[5].vI[3]=4;//底面
}

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

void CTestView::ObliqueProject(CP3 p)//斜等测变换
{
	ScreenP.x=p.x-p.z/sqrt(2);
	ScreenP.y=p.y-p.z/sqrt(2);	
}

void CTestView::DrawObject(CDC *pDC)//绘制图形
{
	CLine *line=new CLine;//绘制坐标系
	line->MoveTo(pDC,0,0);//绘制x轴
	line->LineTo(pDC,rect.Width()/2,0);
	pDC->TextOut(rect.Width()/2-20,-20,"x");
	line->MoveTo(pDC,0,0);//绘制y轴
	line->LineTo(pDC,0,rect.Height()/2);
	pDC->TextOut(-20,rect.Height()/2-20,"y");
	line->MoveTo(pDC,0,0);//绘制z轴
	line->LineTo(pDC,-rect.Width()/2,-rect.Width()/2);
	pDC->TextOut(-rect.Height()/2-20,-rect.Height()/2+20,"z");
	pDC->TextOut(10,-10,"O");
	DrawPolygon(pDC,line);
	delete line;
}

void CTestView::DrawPolygon(CDC *pDC,CLine *line)//绘制立方体线框模型
{
	for(int nFace=0;nFace<6;nFace++)
	{
		CP2 t;	
		for(int nVertex=0;nVertex<F[nFace].vN;nVertex++)//顶点循环
		{
			ObliqueProject(P[F[nFace].vI[nVertex]]);//斜等测投影
			if(0==nVertex)
			{
				line->MoveTo(pDC,ScreenP.x,ScreenP.y);
				t=ScreenP;
			}
			else
				line->LineTo(pDC,ScreenP.x,ScreenP.y);
		}
		line->LineTo(pDC,t.x,t.y);//闭合多边形
	}
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	MessageBox("请使用图标按钮进行三维几何变换","提示",MB_OK);
}

void CTestView::OnLeft()//向左平移 
{
	// TODO: Add your command handler code here
	trans.Translate(-10,0,0);
	Invalidate(FALSE);
}

void CTestView::OnRight()//向右平移 
{
	// TODO: Add your command handler code here
	trans.Translate(10,0,0);
	DoubleBuffer();
}

void CTestView::OnUp()//向上平移  
{
	// TODO: Add your command handler code here
	trans.Translate(0,10,0);
	Invalidate(FALSE);	
}

void CTestView::OnDown()//向下平移 
{
	// TODO: Add your command handler code here
    trans.Translate(0,-10,0);
	Invalidate(FALSE);	
}

void CTestView::OnFront()//向前平移 
{
	// TODO: Add your command handler code here
	trans.Translate(0,0,10);
	Invalidate(FALSE);	
}

void CTestView::OnBack()//向后平移 
{
	// TODO: Add your command handler code here
	trans.Translate(0,0,-10);
	Invalidate(FALSE);	
}

void CTestView::OnIncrease()//等比放大  
{
	// TODO: Add your command handler code here
    trans.Scale(2,2,2);
	Invalidate(FALSE);	
}

void CTestView::OnDecrease()//等比缩小 
{
	// TODO: Add your command handler code here
    trans.Scale(0.5,0.5,0.5);
	Invalidate(FALSE);	
}

void CTestView::OnRxaxis()//绕x轴旋转 
{
	// TODO: Add your command handler code here
	trans.RotateX(30,P[0]);
	Invalidate(FALSE);
}

void CTestView::OnRyaxis()//绕y轴旋转  
{
	// TODO: Add your command handler code here
	trans.RotateY(30,P[0]);
	Invalidate(FALSE);
}

void CTestView::OnRzaxis()//绕z轴旋转  
{
	// TODO: Add your command handler code here
	trans.RotateZ(30,P[0]);
	Invalidate(FALSE);
}

void CTestView::OnRxoy()//关于xOy面反射 
{
	// TODO: Add your command handler code here
	trans.ReflectXOY();
	Invalidate(FALSE);
}

void CTestView::OnRyoz()//关于yOz面反射 
{
	// TODO: Add your command handler code here
	trans.ReflectYOZ();
	Invalidate(FALSE);
}

void CTestView::OnRzox()//关于zOx面反射  
{
	// TODO: Add your command handler code here
	trans.ReflectZOX();
	Invalidate(FALSE);
}

void CTestView::OnSxdirectionplus()//沿x轴正向错切 
{
	// TODO: Add your command handler code here
	trans.ShearX(1,1);
	Invalidate(FALSE);	
}

void CTestView::OnSydirectionplus()//沿y轴正向错切  
{
	// TODO: Add your command handler code here
	trans.ShearY(1,1);
	Invalidate(FALSE);	
}

void CTestView::OnSzdirectionneg()//沿z轴正向错切 
{
	// TODO: Add your command handler code here
	trans.ShearZ(1,1);
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
	ReadFace();
	trans.SetMat(P,8);
}














