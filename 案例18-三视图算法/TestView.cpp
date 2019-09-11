// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "VView.h"//
#include "HView.h"//
#include "WView.h"//
#include "TestDoc.h"
#include "TestView.h"
#include "Transform.h"
#include "Vector.h"
#define PI 3.1415926//圆周率
#include "math.h"//数学头文件
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
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
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
	int a=80,b=140;//a三角形边长，b棱长
	P[0].x=-a/2;P[0].y=0;          P[0].z=-b/2;//P0
	P[1].x=0;   P[1].y=sqrt(3)/2*a;P[1].z=-b/2;//P1
	P[2].x=a/2; P[2].y=0;          P[2].z=-b/2;//P2
	P[3].x=-a/2;P[3].y=0;          P[3].z=b/2; //P3
	P[4].x=0;   P[4].y=sqrt(3)/2*a;P[4].z=b/2; //P4
	P[5].x=a/2; P[5].y=0;          P[5].z=b/2; //P5	
}

void CTestView::ReadFace()//面表
{   
	//面的顶点数和面的顶点索引
	F[0].SetNum(4);F[0].vI[0]=0;F[0].vI[1]=3;F[0].vI[2]=4;F[0].vI[3]=1;
	F[1].SetNum(3);F[1].vI[0]=0;F[1].vI[1]=1;F[1].vI[2]=2;
	F[2].SetNum(4);F[2].vI[0]=0;F[2].vI[1]=2;F[2].vI[2]=5;F[2].vI[3]=3;
	F[3].SetNum(4);F[3].vI[0]=1;F[3].vI[1]=4;F[3].vI[2]=5;F[3].vI[3]=2;
	F[4].SetNum(3);F[4].vI[0]=3;F[4].vI[1]=5;F[4].vI[2]=4;
}

void CTestView::Rotate()//旋转变换
{
	CTransform trans;
	trans.SetMat(P,6);
	trans.RotateX(1);
	trans.RotateY(1);
}

void CTestView::DoubleBuffer()//双缓冲
{
	CDC* pDC=GetDC();
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
	CDC MemDC[4];
	CDC* pViewDC[4];
	CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();//获得框架指针
	CVView *pVView=(CVView*)pFrame->m_wndSplitter.GetPane(0,0);//获得主视图窗格指针
	CWView *pWView=(CWView*)pFrame->m_wndSplitter.GetPane(0,1);//获得左视图窗格指针
	CHView *pHView=(CHView*)pFrame->m_wndSplitter.GetPane(1,0);//获得俯视图窗格指针
	pViewDC[0]=pDC;//多面体屏幕DC
	pViewDC[1]=pVView->GetDC();//主视图屏幕DC
	pViewDC[2]=pWView->GetDC();//左视屏幕DC
	pViewDC[3]=pHView->GetDC();//俯视图屏幕DC
	CBitmap NewBitmap[4],*pOldBitmap[4];//内存中承载图像的临时位图
    for(int i=0;i<4;i++)//创建内存DC
	{	
		pViewDC[i]->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
		pViewDC[i]->SetWindowExt(rect.Width(),rect.Height());//设置窗口范围
		pViewDC[i]->SetViewportExt(rect.Width(),-rect.Height());//x轴水平向右，y轴铅直向上
		pViewDC[i]->SetViewportOrg(rect.Width()/2,rect.Height()/2);//屏幕中心为原点
		NewBitmap[i].CreateCompatibleBitmap(pViewDC[i],rect.Width(),rect.Height());//创建兼容位图
		MemDC[i].CreateCompatibleDC(pViewDC[i]);
		pOldBitmap[i]=MemDC[i].SelectObject(&NewBitmap[i]);
		MemDC[i].FillSolidRect(&rect,pViewDC[i]->GetBkColor());//按原来背景填充客户区，否则是黑色 
		MemDC[i].SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
		MemDC[i].SetWindowExt(rect.Width(),rect.Height());
		MemDC[i].SetViewportExt(rect.Width(),-rect.Height());
		MemDC[i].SetViewportOrg(rect.Width()/2,rect.Height()/2);
	}
	DrawOblique(&MemDC[0]);//绘制斜等测图
	DrawVView(&MemDC[1]);  //绘制主视图
	DrawWView(&MemDC[2]);  //绘制左视图
	DrawHView(&MemDC[3]);  //绘制俯视图
	for(i=0;i<4;i++)
	{
		pViewDC[i]->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC[i],-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
		MemDC[i].SelectObject(pOldBitmap[i]);//恢复位图
		NewBitmap[i].DeleteObject();//删除位图
		ReleaseDC(pViewDC[i]);//释放视图DC
	}
	ReleaseDC(pDC);//释放DC
}

void CTestView::DrawObject(CDC *pDC,CP3 P[])//绘制斜等测图线框模型
{
	for(int nFace=0;nFace<5;nFace++)
	{
		CP2 t;	
		CLine *line=new CLine;
		for(int nVertex=0;nVertex<F[nFace].vN;nVertex++)//顶点循环
		{
			ScreenP=P[F[nFace].vI[nVertex]];//斜等测投影
			if(nVertex==0)
			{
				line->MoveTo(pDC,ScreenP.x,ScreenP.y);
				t=ScreenP;
			}
			else
				line->LineTo(pDC,ScreenP.x,ScreenP.y);
		} 
		line->LineTo(pDC,t.x,t.y);//闭合多边形
		delete line;		
	}
}

void CTestView::DrawTriView(CDC *pDC,CP3 P[])//绘制三视图线框模型
{
	for(int nFace=0;nFace<5;nFace++)
	{
		CP3 ScreenP,t;	
		CLine *line=new CLine;
		for(int nVertex=0;nVertex<F[nFace].vN;nVertex++)//顶点循环
		{
			ScreenP=P[F[nFace].vI[nVertex]];
			if(nVertex==0)
			{
				line->MoveTo(pDC,-ScreenP.z,ScreenP.y);
				t=ScreenP;
			}
			else
			{
				line->LineTo(pDC,-ScreenP.z,ScreenP.y);
			}
		}
		line->LineTo(pDC,-t.z,t.y);//闭合多边形
		delete line;
	}
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CTransform trans;
	trans.SetMat(P,6);	
	switch(nChar)
	{
	case VK_UP:
		trans.RotateX(1);//设定步长
		break;
	case VK_DOWN:
		trans.RotateX(-1);
		break;
	case VK_LEFT:
		trans.RotateY(-1);
		break;
	case VK_RIGHT:
		trans.RotateY(1);
		break;
	default:
		break;			
	}
	Invalidate(FALSE);		
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::TOMatrix()//斜等测变换矩阵
{
	TO[0][0]=1;         TO[0][1]=0;         TO[0][2]=0;TO[0][3]=0;
	TO[1][0]=0;         TO[1][1]=1;         TO[1][2]=0;TO[1][3]=0;
	TO[2][0]=-1/sqrt(2);TO[2][1]=-1/sqrt(2);TO[2][2]=0;TO[2][3]=0;
	TO[3][0]=0;         TO[3][1]=0;         TO[3][2]=0;TO[3][3]=1;
}

void CTestView::TVMatrix()//主视图变换矩阵
{
	TV[0][0]=0;TV[0][1]=0;TV[0][2]=0;TV[0][3]=0;
	TV[1][0]=0;TV[1][1]=1;TV[1][2]=0;TV[1][3]=0;
	TV[2][0]=0;TV[2][1]=0;TV[2][2]=1;TV[2][3]=0;
	TV[3][0]=0;TV[3][1]=0;TV[3][2]=0;TV[3][3]=1;
}

void CTestView::THMatrix()//俯视图变换矩阵
{
	TH[0][0]=0;TH[0][1]=-1;TH[0][2]=0;TH[0][3]=0;
	TH[1][0]=0;TH[1][1]=0; TH[1][2]=0;TH[1][3]=0;
	TH[2][0]=0;TH[2][1]=0; TH[2][2]=1;TH[2][3]=0;
	TH[3][0]=0;TH[3][1]=0; TH[3][2]=0;TH[3][3]=1;
}

void CTestView::TWMatrix()//侧视图变换矩阵
{
	TW[0][0]=0;TW[0][1]=0;TW[0][2]=-1;TW[0][3]=0;
	TW[1][0]=0;TW[1][1]=1;TW[1][2]=0; TW[1][3]=0;
	TW[2][0]=0;TW[2][1]=0;TW[2][2]=0; TW[2][3]=0;
	TW[3][0]=0;TW[3][1]=0;TW[3][2]=0; TW[3][3]=1;
}

void CTestView::DrawOblique(CDC *pDC)
{
	pDC->TextOut(-230,130,"立体图");
	MultiMatrix(TO);
	DrawObject(pDC,PNew);
}

void CTestView::DrawVView(CDC *pDC)
{
	pDC->TextOut(-230,130,"主视图");
	MultiMatrix(TV);
	DrawTriView(pDC,PNew);
}

void CTestView::DrawHView(CDC *pDC)
{
	pDC->TextOut(-230,130,"俯视图");
	MultiMatrix(TH);
	DrawTriView(pDC,PNew);
}

void CTestView::DrawWView(CDC *pDC)
{
	pDC->TextOut(-230,130,"侧视图");
	MultiMatrix(TW);
	DrawTriView(pDC,PNew);
}

void CTestView::MultiMatrix(double T[][4])//两个矩阵相乘
{
	for(int i=0;i<6;i++)
	{
		PNew[i].x=P[i].x*T[0][0]+P[i].y*T[1][0]+P[i].z*T[2][0]+P[i].w*T[3][0];
		PNew[i].y=P[i].x*T[0][1]+P[i].y*T[1][1]+P[i].z*T[2][1]+P[i].w*T[3][1];
		PNew[i].z=P[i].x*T[0][2]+P[i].y*T[1][2]+P[i].z*T[2][2]+P[i].w*T[3][2];
		PNew[i].w=P[i].x*T[0][3]+P[i].y*T[1][3]+P[i].z*T[2][3]+P[i].w*T[3][3];
	}		
}

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(((CMainFrame*)AfxGetMainWnd())->bPlay)
	{
		Rotate();
		Invalidate(FALSE);;	
	}
	CView::OnTimer(nIDEvent);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnInitialUpdate()//初始化 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	TOMatrix();
	TVMatrix();
	THMatrix();
	TWMatrix();
	ReadPoint();
	ReadFace();
	SetTimer(1,50,NULL);//设置定时器
}
