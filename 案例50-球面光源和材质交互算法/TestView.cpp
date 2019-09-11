// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "Mainfrm.h"//
#include "math.h"
#define Round(d) int(floor(d+0.5))//四舍五入宏定义
#define  PI 3.14159265//圆周率
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
	V=NULL;F=NULL;
    R=1000.0,d=800.0,Phi=90.0,Theta=0.0;//视点位于正前方
	Near=200.0,Far=1200.0;//近剪切面与远剪切面	
 	LightNum=1;//光源个数
 	pLight=new CLighting(LightNum);//一维光源动态数组
	pLight->Light[0].SetPosition(800,800,800);//设置光源位置坐标	
 	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0);//光源的漫反射颜色	
		pLight->Light[i].L_Specular=CRGB(1.0,1.0,1.0);//光源镜面高光颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.0000001;//线性衰减系数
		pLight->Light[i].L_C2=0.00000001;//二次衰减系数
		pLight->Light[i].L_OnOff=TRUE;//光源开启
	}
	pMaterial=new CMaterial;//一维材质动态数组
	pMaterial->SetAmbient(CRGB(0.175,0.012,0.012));//材质对环境光光的反射率
	pMaterial->SetDiffuse(CRGB(0.614,0.041,0.041));//材质对漫反射光的反射率
	pMaterial->SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
	pMaterial->SetEmit(CRGB(0.2,0.0,0.0));//材质自身发散的颜色
	pMaterial->SetExp(30);//高光指数	
}

CTestView::~CTestView()
{	
	if(pLight!=NULL)
	{
		delete pLight;
		pLight=NULL;
	}
	if(pMaterial!=NULL)
	{
		delete pMaterial;
		pMaterial=NULL;
	}
	if(V!=NULL)
	{
		delete[] V;
		V=NULL;
	}
	for(int n=0;n<N1;n++)//注意撤销次序,先列后行,与设置相反
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
	CTestDoc* pDoc=GetDocument();
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
void CTestView::ReadVertex()//读入顶点坐标
{
	int gAlpha=4,gBeta=4;//面片夹角
	N1=180/gAlpha,N2=360/gBeta;//N1为纬度区域,N2为经度区域
	V=new CP3[(N1-1)*N2+2];//V为球的顶点
	//纬度方向除南北极点外有"N1－1"个点，"2"代表南北极两个点
	double gAlpha1,gBeta1,r=300;//r为球体半径
	//计算北极点坐标
	V[0].x=0,V[0].y=r,V[0].z=0;
	//按行循环计算球体上的点坐标
	for(int i=0;i<N1-1;i++)
	{
		gAlpha1=(i+1)*gAlpha*PI/180;
		for(int j=0;j<N2;j++)
	    {
			gBeta1=j*gBeta*PI/180;
			V[i*N2+j+1].x=r*sin(gAlpha1)*sin(gBeta1);
			V[i*N2+j+1].y=r*cos(gAlpha1);	
			V[i*N2+j+1].z=r*sin(gAlpha1)*cos(gBeta1);
		}
	}
	//计算南极点坐标
	V[(N1-1)*N2+1].x=0,V[(N1-1)*N2+1].y=-r,V[(N1-1)*N2+1].z=0;	
}

void CTestView::ReadFace()//读入面表
{
	//设置二维动态数组
	F=new CFace *[N1];//设置行
	for(int n=0;n<N1;n++)
		F[n]=new CFace[N2];//设置列
	for(int j=0;j<N2;j++)//构造北极三角形面片
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;//面片的首尾连接
		int NorthIndex[3];//北极三角形面片索引号数组
		NorthIndex[0]=0;
		NorthIndex[1]=j+1;
		NorthIndex[2]=tempj+1;
		F[0][j].SetNum(3);
		for(int k=0;k<F[0][j].vN;k++)
			F[0][j].vI[k]=NorthIndex[k];
	}
	for(int i=1;i<N1-1;i++)//构造球面四边形面片
	{
		for(int j=0;j<N2;j++)
	    {
			int tempi=i+1;
			int tempj=j+1;
			if(tempj==N2) tempj=0;
			int BodyIndex[4];//球面四边形面片索引号数组
			BodyIndex[0]=(i-1)*N2+j+1;
			BodyIndex[1]=(tempi-1)*N2+j+1;
			BodyIndex[2]=(tempi-1)*N2+tempj+1;
			BodyIndex[3]=(i-1)*N2+tempj+1;
			F[i][j].SetNum(4);
			for(int k=0;k<F[i][j].vN;k++)
				F[i][j].vI[k]=BodyIndex[k];
		}
	}
	for(j=0;j<N2;j++)//构造南极三角形面片
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;
		int SouthIndex[3];//南极三角形面片索引号数组
		SouthIndex[0]=(N1-2)*N2+j+1;
		SouthIndex[1]=(N1-1)*N2+1;
		SouthIndex[2]=(N1-2)*N2+tempj+1;
		F[N1-1][j].SetNum(3);	
		for(int k=0;k<F[N1-1][j].vN;k++)
			F[N1-1][j].vI[k]=SouthIndex[k];
	}
}

void CTestView::InitParameter()
{
	k[1]=sin(PI*Theta/180);
	k[2]=sin(PI*Phi/180);
	k[3]=cos(PI*Theta/180);
	k[4]=cos(PI*Phi/180);
	k[5]=k[2]*k[3];
	k[6]=k[2]*k[1];
	k[7]=k[4]*k[3];
	k[8]=k[4]*k[1];
	ViewPoint.x=R*k[6];
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CTestView::PerProject(CP3 P)
{
	CP3 ViewP;
	ViewP.x=P.x*k[3]-P.z*k[1];//观察坐标系三维坐标
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;
	ViewP.c=P.c;
    ScreenP.x=d*ViewP.x/ViewP.z;//屏幕坐标系三维坐标
	ScreenP.y=Round(d*ViewP.y/ViewP.z);
	ScreenP.z=Far*(1-Near/ViewP.z)/(Far-Near);
	ScreenP.c=ViewP.c;
}

void CTestView::DoubleBuffer()
{
	CDC* pDC=GetDC();
	CRect Rect;//定义客户区
	GetClientRect(&Rect);//获得客户区的大小
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(Rect.Width(),Rect.Height());//设置窗口范围
	pDC->SetViewportExt(Rect.Width(),-Rect.Height());//x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(Rect.Width()/2,Rect.Height()/2);//屏幕中心为原点
	CDC MemDC;//内存DC
	CBitmap NewBitmap,*pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,Rect.Width(),Rect.Height());//创建兼容位图 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC 
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
	MemDC.SetWindowExt(Rect.Width(),Rect.Height());
	MemDC.SetViewportExt(Rect.Width(),-Rect.Height());
	MemDC.SetViewportOrg(Rect.Width()/2,Rect.Height()/2);	
	DrawObject(&MemDC);	
	pDC->BitBlt(-Rect.Width()/2,-Rect.Height()/2,Rect.Width(),Rect.Height(),&MemDC,-Rect.Width()/2,-Rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	ReleaseDC(pDC);//释放DC
}

void CTestView::CalVertexColor()//计算顶点颜色
{
	for(int nVertex=0;nVertex<(N1-1)*N2+2;nVertex++)//遍历所有点
	{
		CVector VNormal(V[nVertex]);//点的位置矢量代表共享该点的所有面的平均法矢量
		V[nVertex].c=pLight->Lighting(ViewPoint,V[nVertex],VNormal,pMaterial);//调用光照函数
	}
}

void CTestView::DrawObject(CDC *pDC)//绘制球面
{
	CalVertexColor();
	CZBuffer *zbuf=new CZBuffer;//申请内存
	zbuf->InitDeepBuffer(800,800,1000);//初始化深度缓冲器
    CPi3 Point3[3];//南北极顶点数组
	CPi3 Point4[4];//球体顶点数组	
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			CVector ViewVector(V[F[i][j].vI[0]],ViewPoint);//面的视矢量
			ViewVector=ViewVector.Normalize();//单位化视矢量
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
					zbuf->SetPoint(Point3,3);//设置顶点			
					zbuf->CreateBucket();//建立桶表
					zbuf->CreateEdge();//建立边表
					zbuf->Gouraud(pDC);//填充三角形	
					zbuf->ClearMemory();//内存清理					
				}
				else//四边形面片
				{
					for(int m=0;m<F[i][j].vN;m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point4[m]=ScreenP;						
					}
					zbuf->SetPoint(Point4,4);//设置顶点			
					zbuf->CreateBucket();//建立桶表
					zbuf->CreateEdge();//建立边表
					zbuf->Gouraud(pDC);//填充四边形	
					zbuf->ClearMemory();//内存清理
				}
			} 	
		}	
	}
	delete zbuf;//释放内存
}

void CTestView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(lHint)
	{
	case 1://环境光
		pLight->Light[0].b_Ambient=!pLight->Light[0].b_Ambient;
		break;
	case 2://漫反射光
		pLight->Light[0].b_Diffuse=!pLight->Light[0].b_Diffuse;
		break;
	case 3://镜面反射光
		pLight->Light[0].b_Specular=!pLight->Light[0].b_Specular;
		break;
	case 4://金
		pMaterial->SetAmbient(CRGB(0.247,0.2,0.075));//材质对环境光的反射率
		pMaterial->SetDiffuse(CRGB(0.752,0.606,0.226));//材质对环境光和漫反射光的反射率相等
		pMaterial->SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
		pMaterial->SetEmit(CRGB(0.2,0.2,0.0));//材质自身发散的颜色
		pMaterial->SetExp(50);//高光指数
		break;
	case 5://银
		pMaterial->SetAmbient(CRGB(0.192,0.192,0.192));//材质对环境光的反射率
		pMaterial->SetDiffuse(CRGB(0.508,0.508,0.508));//材质对环境光和漫反射光的反射率相等
		pMaterial->SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
		pMaterial->SetEmit(CRGB(0.2,0.2,0.2));//材质自身发散的颜色
		pMaterial->SetExp(50);//高光指数
		break;
	case 6://红宝石
		pMaterial->SetAmbient(CRGB(0.175,0.012,0.012));//材质对环境光的反射率
		pMaterial->SetDiffuse(CRGB(0.614,0.041,0.041));//材质对环境光和漫反射光的反射率相等
		pMaterial->SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
		pMaterial->SetEmit(CRGB(0.2,0.0,0.0));//材质自身发散的颜色
		pMaterial->SetExp(30);//高光指数
		break;
	case 7://绿宝石
		pMaterial->SetAmbient(CRGB(0.022,0.175,0.023));//材质对环境光的反射率
		pMaterial->SetDiffuse(CRGB(0.076,0.614,0.075));//材质对环境光和漫反射光的反射率相等
		pMaterial->SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
		pMaterial->SetEmit(CRGB(0.0,0.2,0.0));//材质自身发散的颜色
		pMaterial->SetExp(30);//高光指数
		break;
	case 8://左上
		pLight->Light[0].SetPosition(-800,800,800);//设置光源位置坐标
		break;
	case 9://左下
		pLight->Light[0].SetPosition(-800,-800,800);//设置光源位置坐标
		break;
	case 10://右上
		pLight->Light[0].SetPosition(800,800,800);//设置光源位置坐标
		break;
	case 11://右下
		pLight->Light[0].SetPosition(800,-800,800);//设置光源位置坐标
		break;
	default:
		break;
	}
	Invalidate(FALSE);
}

void CTestView::OnTimer(UINT nIDEvent)//定时器函数 
{
	// TODO: Add your message handler code here and/or call default
	CTestDoc* pDoc=GetDocument();
	if(((CMainFrame*)AfxGetMainWnd())->IsPlay)
	{
		Alpha=5;Beta=5;
		tran.RotateX(Alpha);
		tran.RotateY(Beta);
		Invalidate(FALSE);			
	}
	CView::OnTimer(nIDEvent);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景刷新 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();	
	// TODO: Add your specialized code here and/or call the base class
	SetTimer(1,100,NULL);
	ReadVertex();
	ReadFace();
	tran.SetMat(V,(N1-1)*N2+2);
	InitParameter();	
}




