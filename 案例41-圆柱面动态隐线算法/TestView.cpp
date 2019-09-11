// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define PI 3.1415926//圆周率
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
	ON_WM_RBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_PLAY, OnPlay)
	ON_UPDATE_COMMAND_UI(IDM_PLAY, OnUpdatePlay)
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
	bPlay=FALSE;
	R=800.0;d=1000;Phi=90;Theta=0;
	Alpha=0.0;Beta=0.0;
	V=NULL;F=NULL;
}

CTestView::~CTestView()
{
	if(V!=NULL)
	{
		delete[] V;
		V=NULL;
	}
	for(int n=0;n<N2+2;n++)//注意撤销次序,先列后行,与设置相反
	{
		delete[] F[n];
		F[n]=NULL;
	}
	delete[] F;
	F=NULL;
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

void CTestView::ReadVertex()//点表
{
	r=80;//圆柱底面半径
	h=300;//圆柱的高
	int cTheta=10;//周向夹角
	int cNum=30;//纵向间距
	N1=360/cTheta;//N1周向网格数
	N2=Round(h/cNum);//N2为纵向网格数
	V=new CP3[N1*(N2+1)+2];
	double cTheta1,cNum1;
	V[0].x=0;V[0].y=0;V[0].z=0;//底面中心
	for(int i=0;i<N2+1;i++)//纵向
	{	
		cNum1=i*cNum;
		for(int j=0;j<N1;j++)//周向
		{
			cTheta1=j*cTheta*PI/180;
			V[i*N1+j+1].x=r*cos(cTheta1);
			V[i*N1+j+1].y=cNum1;	
			V[i*N1+j+1].z=r*sin(cTheta1);
		}
	}
	V[N1*(N2+1)+1].x=0;V[N1*(N2+1)+1].y=h;V[N1*(N2+1)+1].z=0;//顶面中心	
}

void CTestView::ReadFace()//面表
{
	//设置二维动态数组
	F=new CFace *[N2+2];//纵向
	for(int n=0;n<N2+2;n++)
		F[n]=new CFace[N1];//周向
	for(int j=0;j<N1;j++)//构造底部三角形面片
	{
		int tempj=j+1;
		if(tempj==N1) tempj=0;//面片的首尾连接
		int BottomIndex[3];//底部三角形面片索引号数组
		BottomIndex[0]=0;
		BottomIndex[1]=j+1;
		BottomIndex[2]=tempj+1;
		F[0][j].SetNum(3);
		for(int k=0;k<F[0][j].vN;k++)//面片中顶点的索引
			F[0][j].vI[k]=BottomIndex[k];
	}
	for(int i=1;i<=N2;i++)//构造圆柱体四边形面片
	{
		for(int j=0;j<N1;j++)
	    {
			int tempi=i+1;
			int tempj=j+1;
			if(N1==tempj) tempj=0;
			int BodyIndex[4];//圆柱体四边形面片索引号数组
			BodyIndex[0]=(i-1)*N1+j+1;
			BodyIndex[1]=(tempi-1)*N1+j+1;
			BodyIndex[2]=(tempi-1)*N1+tempj+1;
			BodyIndex[3]=(i-1)*N1+tempj+1;
			F[i][j].SetNum(4);
			for(int k=0;k<F[i][j].vN;k++)
				F[i][j].vI[k]=BodyIndex[k];
		}
	}
	for(j=0;j<N1;j++)//构造顶部三角形面片
	{
		int tempj=j+1;
		if(tempj==N1) tempj=0;
		int TopIndex[3];//顶部三角形面片索引号数组
		TopIndex[0]=N1*(N2+1)+1;
		TopIndex[1]=N1*N2+tempj+1;
		TopIndex[2]=N1*N2+j+1;
		F[N2+1][j].SetNum(3);	
		for(int k=0;k<F[N2+1][j].vN;k++)
			F[N2+1][j].vI[k]=TopIndex[k];
	}
}

void CTestView::InitParameter()//透视变换参数初始化
{	
	k[1]=sin(PI*Theta/180);
	k[2]=sin(PI*Phi/180);
	k[3]=cos(PI*Theta/180);
	k[4]=cos(PI*Phi/180);
	k[5]=k[2]*k[3];
	k[6]=k[2]*k[1];
	k[7]=k[4]*k[3];
	k[8]=k[4]*k[1];
	ViewPoint.x=R*k[6];//用户坐标系的视点球坐标
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CTestView::PerProject(CP3 P)//透视变换
{
	CP3 ViewP;
	ViewP.x=P.x*k[3]-P.z*k[1];//观察坐标系三维坐标
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;
    ScreenP.x=d*ViewP.x/ViewP.z;//屏幕二维坐标系
	ScreenP.y=d*ViewP.y/ViewP.z;
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
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	ReleaseDC(pDC);//释放DC
}

void CTestView::DrawObject(CDC* pDC)//绘制圆柱体网格
{
	CLine *line=new CLine;
    CP2 Point3[3],t3;//顶面与底面顶点数组
	CP2 Point4[4],t4;//侧面顶点数组
	for(int i=0;i<N2+2;i++)//N2+2
	{
		for(int j=0;j<N1;j++)
		{
			CVector ViewVector(V[F[i][j].vI[0]],ViewPoint);//面的视向量
			ViewVector=ViewVector.Normalize();//单位化视向量
			F[i][j].SetFaceNormal(V[F[i][j].vI[0]],V[F[i][j].vI[1]],V[F[i][j].vI[2]]);	
			F[i][j].fNormal.Normalize();//单位化法矢量
			if(Dot(ViewVector,F[i][j].fNormal)>=0)//背面剔除
			{
				if(3==F[i][j].vN)//三角形面片
				{
					for(int m=0;m<F[i][j].vN;m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point3[m]=ScreenP;						
					}			
					for(int n=0;n<3;n++)
					{
						if(0==n)
						{
							line->MoveTo(pDC,Point3[n]);
							t3=Point3[n];
						}
						else
							line->LineTo(pDC,Point3[n]);
					}
					line->LineTo(pDC,t3);//闭合多边形
				}
				else//四边形面片
				{
					for(int m=0;m<F[i][j].vN;m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point4[m]=ScreenP;						
					}
					
					for(int n=0;n<4;n++)
					{
						if(0==n)
						{
							line->MoveTo(pDC,Point4[n]);
							t4=Point4[n];
						}
						else
						{
							line->LineTo(pDC,Point4[n]);
						}		
					}
					line->LineTo(pDC,t4);//闭合多边形
				}
			} 	
		}	
	}
	delete line;
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//鼠标左键函数
{
	// TODO: Add your message handler code here and/or call default
	R=R+100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDblClk(UINT nFlags, CPoint point)//鼠标右键函数 
{
	// TODO: Add your message handler code here and/or call default
	R=R-100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnRButtonDblClk(nFlags, point);
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(!bPlay)
	{
		switch(nChar)
		{
		case VK_UP:
			Alpha=-5;
			tran.RotateX(Alpha);
			break;
		case VK_DOWN:
			Alpha=5;
			tran.RotateX(Alpha);
			break;
		case VK_LEFT:
			Beta=-5;
			tran.RotateY(Beta);
			break;
		case VK_RIGHT:
			Beta=5;
			tran.RotateY(Beta);
			break;
		default:
			break;			
		}
		Invalidate(FALSE);		
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::OnPlay() 
{
	// TODO: Add your command handler code here
	bPlay=bPlay?FALSE:TRUE;
	if(bPlay)//设置定时器
		SetTimer(1,150,NULL);	
	else
		KillTimer(1);
}

void CTestView::OnTimer(UINT nIDEvent)//动画时间函数 
{
	// TODO: Add your message handler code here and/or call default
	Alpha=5;Beta=5;
	tran.RotateX(Alpha);
	tran.RotateY(Beta);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}

void CTestView::OnUpdatePlay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(bPlay)
	{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->SetText("停止");
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->SetText("开始");
	}
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadVertex();
	ReadFace();
	tran.SetMat(V,N1*(N2+1)+2);
	tran.Translate(0,-h/2,0);
	InitParameter();
}




