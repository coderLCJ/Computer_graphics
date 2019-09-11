// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
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

void CTestView::OnDraw(CDC* pDC)//绘制坐标系
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
void CTestView::ReadPoint()//读入控制多边形16个顶点坐标
{
	P3[0][0].x=20;  P3[0][0].y=0;  P3[0][0].z=200;//P00
	P3[0][1].x=0;   P3[0][1].y=100;P3[0][1].z=150;//P01
	P3[0][2].x=-130;P3[0][2].y=100;P3[0][2].z=50; //P02
	P3[0][3].x=-250;P3[0][3].y=50; P3[0][3].z=0;  //P03

	P3[1][0].x=100; P3[1][0].y=100;P3[1][0].z=150;//P10
	P3[1][1].x=30;  P3[1][1].y=100;P3[1][1].z=100;//p11
	P3[1][2].x=-40; P3[1][2].y=100;P3[1][2].z=50; //p12
	P3[1][3].x=-110;P3[1][3].y=100;P3[1][3].z=0;  //p13

	P3[2][0].x=280; P3[2][0].y=90; P3[2][0].z=140;//P20
	P3[2][1].x=110; P3[2][1].y=120;P3[2][1].z=80; //P21
	P3[2][2].x=30;  P3[2][2].y=130;P3[2][2].z=30; //P22
	P3[2][3].x=-100;P3[2][3].y=150;P3[2][3].z=-50;//P23

	P3[3][0].x=350; P3[3][0].y=30; P3[3][0].z=150;//P30
	P3[3][1].x=200; P3[3][1].y=150;P3[3][1].z=50; //P31
	P3[3][2].x=50;  P3[3][2].y=200;P3[3][2].z=0;  //P32
	P3[3][3].x=0;   P3[3][3].y=100;P3[3][3].z=-70;//P33	
}

void CTestView::DoubleBuffer()//双缓冲
{
	CDC* pDC=GetDC();
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
	DrawCtrlPolygon(&MemDC);//绘制控制多边形
	SignCtrPoint(&MemDC);//标注控制点编号
	DrawObject(&MemDC);//绘制B样条曲面
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	ReleaseDC(pDC);//释放DC
}

void CTestView::ObliqueProjection()//斜等测投影
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			P2[i][j].x=P3[i][j].x-P3[i][j].z/sqrt(2);
			P2[i][j].y=P3[i][j].y-P3[i][j].z/sqrt(2);
		}
}

void CTestView::DrawCtrlPolygon(CDC *pDC)//绘制控制多边形
{
	ReadPoint();
	ObliqueProjection();
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID,3,RGB(0,0,0));
	pOldPen=pDC->SelectObject(&NewPen);
	for(int i=0;i<4;i++)
	{
		pDC->MoveTo(Round(P2[i][0].x),Round(P2[i][0].y));
		for(int j=1;j<4;j++)
			pDC->LineTo(Round(P2[i][j].x),Round(P2[i][j].y));
	}
	for(int j=0;j<4;j++)
	{
		pDC->MoveTo(Round(P2[0][j].x),Round(P2[0][j].y));
		for(int i=1;i<4;i++)
			pDC->LineTo(Round(P2[i][j].x),Round(P2[i][j].y));
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

void CTestView::DrawObject(CDC *pDC)//绘制双三次B样条曲面	
{
	double x,y,u,v,u1,u2,u3,u4,v1,v2,v3,v4;
	double M[4][4];
	M[0][0]=-1;M[0][1]=3;M[0][2]=-3;M[0][3]=1;
	M[1][0]=3;M[1][1]=-6;M[1][2]=3;M[1][3]=0;
	M[2][0]=-3;M[2][1]=0;M[2][2]=3;M[2][3]=0;
	M[3][0]=1;M[3][1]=4;M[3][2]=1;M[3][3]=0;
	LeftMultiMatrix(M,P3);//数字矩阵左乘三维点矩阵
	TransposeMatrix(M);//计算转置矩阵
	RightMultiMatrix(P3,MT);//数字矩阵右乘三维点矩阵
	ObliqueProjection();//斜等测投影
	for(u=0;u<=1;u+=0.1)
		for(v=0;v<=1;v+=0.1)
		{
			u1=u*u*u;u2=u*u;u3=u;u4=1;v1=v*v*v;v2=v*v;v3=v;v4=1;
			x=(u1*P2[0][0].x+u2*P2[1][0].x+u3*P2[2][0].x+u4*P2[3][0].x)*v1
			 +(u1*P2[0][1].x+u2*P2[1][1].x+u3*P2[2][1].x+u4*P2[3][1].x)*v2
			 +(u1*P2[0][2].x+u2*P2[1][2].x+u3*P2[2][2].x+u4*P2[3][2].x)*v3
			 +(u1*P2[0][3].x+u2*P2[1][3].x+u3*P2[2][3].x+u4*P2[3][3].x)*v4;
			y=(u1*P2[0][0].y+u2*P2[1][0].y+u3*P2[2][0].y+u4*P2[3][0].y)*v1
			 +(u1*P2[0][1].y+u2*P2[1][1].y+u3*P2[2][1].y+u4*P2[3][1].y)*v2
			 +(u1*P2[0][2].y+u2*P2[1][2].y+u3*P2[2][2].y+u4*P2[3][2].y)*v3
			 +(u1*P2[0][3].y+u2*P2[1][3].y+u3*P2[2][3].y+u4*P2[3][3].y)*v4;
			x=x/36;y=y/36;
			if(v==0)
				pDC->MoveTo(Round(x),Round(y));
			else
				pDC->LineTo(Round(x),Round(y));
		}		  
	for(v=0;v<=1;v+=0.1)
		for(u=0;u<=1;u+=0.1)
		{
			u1=u*u*u;u2=u*u;u3=u;u4=1;v1=v*v*v;v2=v*v;v3=v;v4=1;
			x=(u1*P2[0][0].x+u2*P2[1][0].x+u3*P2[2][0].x+u4*P2[3][0].x)*v1
			 +(u1*P2[0][1].x+u2*P2[1][1].x+u3*P2[2][1].x+u4*P2[3][1].x)*v2
			 +(u1*P2[0][2].x+u2*P2[1][2].x+u3*P2[2][2].x+u4*P2[3][2].x)*v3
			 +(u1*P2[0][3].x+u2*P2[1][3].x+u3*P2[2][3].x+u4*P2[3][3].x)*v4;
			y=(u1*P2[0][0].y+u2*P2[1][0].y+u3*P2[2][0].y+u4*P2[3][0].y)*v1
			 +(u1*P2[0][1].y+u2*P2[1][1].y+u3*P2[2][1].y+u4*P2[3][1].y)*v2
			 +(u1*P2[0][2].y+u2*P2[1][2].y+u3*P2[2][2].y+u4*P2[3][2].y)*v3
			 +(u1*P2[0][3].y+u2*P2[1][3].y+u3*P2[2][3].y+u4*P2[3][3].y)*v4;
			x=x/36;y=y/36;
			if(u==0)
				pDC->MoveTo(Round(x),Round(y));
			else
				pDC->LineTo(Round(x),Round(y));
		}	
}

void CTestView::LeftMultiMatrix(double M0[][4],CP3 P0[][4])//左乘矩阵M*P
{
	CP3 T[4][4];//临时矩阵
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{	
			T[i][j].x=M0[i][0]*P0[0][j].x+M0[i][1]*P0[1][j].x+M0[i][2]*P0[2][j].x+M0[i][3]*P0[3][j].x;	
			T[i][j].y=M0[i][0]*P0[0][j].y+M0[i][1]*P0[1][j].y+M0[i][2]*P0[2][j].y+M0[i][3]*P0[3][j].y;
			T[i][j].z=M0[i][0]*P0[0][j].z+M0[i][1]*P0[1][j].z+M0[i][2]*P0[2][j].z+M0[i][3]*P0[3][j].z;
		}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			P3[i][j]=T[i][j];
}

void CTestView::RightMultiMatrix(CP3 P0[][4],double M1[][4])//右乘矩阵P*M
{
	CP3 T[4][4];//临时矩阵
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{	
			T[i][j].x=P0[i][0].x*M1[0][j]+P0[i][1].x*M1[1][j]+P0[i][2].x*M1[2][j]+P0[i][3].x*M1[3][j];
			T[i][j].y=P0[i][0].y*M1[0][j]+P0[i][1].y*M1[1][j]+P0[i][2].y*M1[2][j]+P0[i][3].y*M1[3][j];
			T[i][j].z=P0[i][0].z*M1[0][j]+P0[i][1].z*M1[1][j]+P0[i][2].z*M1[2][j]+P0[i][3].z*M1[3][j];
		}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			P3[i][j]=T[i][j];
}

void CTestView::SignCtrPoint(CDC *pDC)//标注控制点的编号
{
	CString str;
    pDC->SetTextColor(RGB(0,0,255));
   	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			str.Format("P%d,%d",i,j);
			pDC->TextOut(Round(P2[i][j].x+10),Round(P2[i][j].y),str);				
		}		
	}
}

void CTestView::TransposeMatrix(double M0[][4])//转置矩阵
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{	
			MT[j][i]=M0[i][j];
		}
	}
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadPoint();
}


