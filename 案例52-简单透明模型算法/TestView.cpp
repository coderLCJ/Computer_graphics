// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
#include "math.h"
#define Round(f) int(floor(f+0.5))//四舍五入宏定义
#define  PI 3.1415926
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
	R=1000.0,d=800.0,Phi=90.0,Theta=0.0;//视点位于正前方
	Near=200.0,Far=1200.0;//近剪切面与远剪切面		
	Alpha=0.0;Beta=0.0;
	LightNum=1;//光源个数
	pLight=new CLighting(LightNum);//一维光源动态数组
    pLight->Light[0].SetPosition(800,800,800);//设置第一个光源位置坐标
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0); //光源的漫反射颜色	
		pLight->Light[i].L_Specular=CRGB(1.0,1.0,1.0);//光源镜面高光颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.0000001;//线性衰减系数
		pLight->Light[i].L_C2=0.00000001;//二次衰减系数
		pLight->Light[i].L_OnOff=TRUE;//光源开启	
	}
	//球的材质
 	pMaterial=new CMaterial[2];//一维材质动态数组
	pMaterial[0].SetAmbient(CRGB(0.175,0.012,0.012));//材质对环境光光的反射率
	pMaterial[0].SetDiffuse(CRGB(0.614,0.041,0.041));//材质对漫反射光的反射率
	pMaterial[0].SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
	pMaterial[0].SetEmit(CRGB(0.2,0.0,0.0));//材质自身发散的颜色
	pMaterial[0].M_n=30.0;//高光指数
	//立方体的材质
	pMaterial[1].SetAmbient(CRGB(0.0,0.8,0.0));//材质对环境光光的反射率
	pMaterial[1].SetDiffuse(CRGB(0.0,0.8,0.0));//材质对环境光和漫反射光的反射率相等
	pMaterial[1].SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
	pMaterial[1].SetEmit(CRGB(0.0,0.2,0.0));//材质自身发散的颜色
	pMaterial[1].M_n=30.0;//高光指数
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
		delete []pMaterial;
		pMaterial=NULL;
	}
	if(SV!=NULL)
	{
		delete[] SV;
		SV=NULL;
	}
	for(int n=0;n<N1;n++)//注意撤销次序,先列后行,与设置相反
	{
		delete[] SF[n];
		SF[n]=NULL;
	}
	delete[] SF;
	SF=NULL;
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
void CTestView::ReadSphereVertex()//读入球面顶点坐标
{
	int gAlpha=4,gBeta=4;//面片夹角
	N1=180/gAlpha,N2=360/gBeta;//N1为纬度区域,N2为经度区域
	SV=new CP3[(N1-1)*N2+2];//SV为球的顶点
	//纬度方向除南北极点外有"N1－1"个点，"2"代表南北极两个点
	double gAlpha1,gBeta1,r=100;//r为球体半径
	//计算北极点坐标
	SV[0].x=0,SV[0].y=r,SV[0].z=0;
	//按行循环计算球体上的点坐标
	for(int i=0;i<N1-1;i++)
	{
		gAlpha1=(i+1)*gAlpha*PI/180;
		for(int j=0;j<N2;j++)
	    {
			gBeta1=j*gBeta*PI/180;		
			SV[i*N2+j+1].x=r*sin(gAlpha1)*sin(gBeta1);
			SV[i*N2+j+1].y=r*cos(gAlpha1);	
			SV[i*N2+j+1].z=r*sin(gAlpha1)*cos(gBeta1);
		}
	}
	//计算南极点坐标
	SV[(N1-1)*N2+1].x=0,SV[(N1-1)*N2+1].y=-r,SV[(N1-1)*N2+1].z=0;	
}

void CTestView::ReadSphereFace()//读入球的面表
{
	//设置二维动态数组
	SF=new CFace *[N1];//设置行
	for(int n=0;n<N1;n++)
		SF[n]=new CFace[N2];//设置列
	for(int j=0;j<N2;j++)//构造北极三角形面片
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;//面片的首尾连接
		int NorthIndex[3];//北极三角形面片索引号数组
		NorthIndex[0]=0;
		NorthIndex[1]=j+1;
		NorthIndex[2]=tempj+1;
		SF[0][j].SetNum(3);
		for(int k=0;k<SF[0][j].vN;k++)
			SF[0][j].vI[k]=NorthIndex[k];
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
			SF[i][j].SetNum(4);
			for(int k=0;k<SF[i][j].vN;k++)
				SF[i][j].vI[k]=BodyIndex[k];
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
		SF[N1-1][j].SetNum(3);	
		for(int k=0;k<SF[N1-1][j].vN;k++)
			SF[N1-1][j].vI[k]=SouthIndex[k];
	}
}

void CTestView::ReadCubeVertex()//读入立方体顶点坐标
{
	//顶点的三维坐标(x,y,z),立方体边长为2a,t为透明度
	double a=160;double t=0.7;
	CV[0].x=-a;CV[0].y=-a;CV[0].z=-a;CV[0].c.alpha=t;
	CV[1].x=+a;CV[1].y=-a;CV[1].z=-a;CV[1].c.alpha=t;
	CV[2].x=+a;CV[2].y=+a;CV[2].z=-a;CV[2].c.alpha=t;
	CV[3].x=-a;CV[3].y=+a;CV[3].z=-a;CV[3].c.alpha=t;
	CV[4].x=-a;CV[4].y=-a;CV[4].z=+a;CV[4].c.alpha=t;
	CV[5].x=+a;CV[5].y=-a;CV[5].z=+a;CV[5].c.alpha=t;
	CV[6].x=+a;CV[6].y=+a;CV[6].z=+a;CV[6].c.alpha=t;
	CV[7].x=-a;CV[7].y=+a;CV[7].z=+a;CV[7].c.alpha=t;
}

void CTestView::ReadCubeFace()//读入立方体面表
{
	//面的顶点数、面的顶点索引号与面的颜色
	CF[0].SetNum(4);CF[0].vI[0]=4;CF[0].vI[1]=5;CF[0].vI[2]=6;CF[0].vI[3]=7;
	CF[1].SetNum(4);CF[1].vI[0]=0;CF[1].vI[1]=3;CF[1].vI[2]=2;CF[1].vI[3]=1;
	CF[2].SetNum(4);CF[2].vI[0]=0;CF[2].vI[1]=4;CF[2].vI[2]=7;CF[2].vI[3]=3;
	CF[3].SetNum(4);CF[3].vI[0]=1;CF[3].vI[1]=2;CF[3].vI[2]=6;CF[3].vI[3]=5;
	CF[4].SetNum(4);CF[4].vI[0]=2;CF[4].vI[1]=3;CF[4].vI[2]=7;CF[4].vI[3]=6;
	CF[5].SetNum(4);CF[5].vI[0]=0;CF[5].vI[1]=1;CF[5].vI[2]=5;CF[5].vI[3]=4;
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
	pOldBitmap=MemDC.SelectObject(&NewBitmap);//将兼容位图选入MemDC 
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

void CTestView::DrawObject(CDC *pDC)//绘制物体
{
	CZBuffer *zbuf=new CZBuffer;//申请深度缓冲内存
	zbuf->InitDeepBuffer(800,800,1000,CRGB(0.0,0.0,0.0));//初始化深度和背景色
	DrawSphere(pDC,zbuf);
	DrawCube(pDC,zbuf);
	delete zbuf;//释放内存
}

void CTestView::DrawSphere(CDC *pDC,CZBuffer *zbuf)//绘制球面
{
	CPi3 Point3[3];//南北极顶点数组
	CVector Normal3[3];
	CPi3 Point4[4];//球体顶点数组
	CVector Normal4[4];
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			CVector ViewVector(SV[SF[i][j].vI[0]],ViewPoint);//面的视矢量
			ViewVector=ViewVector.Normalize();//单位化视矢量
			SF[i][j].SetFaceNormal(SV[SF[i][j].vI[0]],SV[SF[i][j].vI[1]],SV[SF[i][j].vI[2]]);
			SF[i][j].fNormal.Normalize();//单位化法矢量
			if(Dot(ViewVector,SF[i][j].fNormal)>=0)//背面剔除
			{
				if(3==SF[i][j].vN)
				{
					for(int m=0;m<SF[i][j].vN;m++)
					{
						PerProject(SV[SF[i][j].vI[m]]);
						Point3[m]=ScreenP;
						Normal3[m]=CVector(SV[SF[i][j].vI[m]]);
					}	
					zbuf->SetPoint(Point3,Normal3,3);//初始化
					zbuf->CreateBucket();//创建桶表
					zbuf->CreateEdge();//创建边表
					zbuf->Phong(pDC,ViewPoint,pLight,pMaterial,SPHERE);//颜色渐变填充三角形
					zbuf->ClearMemory();
				}
				else
				{
					for(int m=0;m<SF[i][j].vN;m++)
					{
						PerProject(SV[SF[i][j].vI[m]]);
						Point4[m]=ScreenP;
						Normal4[m]=CVector(SV[SF[i][j].vI[m]]);
					}
					zbuf->SetPoint(Point4,Normal4,4);//初始化
					zbuf->CreateBucket();//创建桶表
					zbuf->CreateEdge();//创建边表
					zbuf->Phong(pDC,ViewPoint,pLight,&pMaterial[0],SPHERE);//颜色渐变填充四边形
					zbuf->ClearMemory();
				}
			}
		}
	}
}

void CTestView::DrawCube(CDC* pDC,CZBuffer *zbuf)//绘制立方体表面
{
	CPi3 Point[4];//面的二维顶点数组
	CVector Normal4[4];//面的法矢量
	for(int nFace=0;nFace<6;nFace++)//面循环
	{
		for(int nVertex=0;nVertex<CF[nFace].vN;nVertex++)//顶点循环
		{
			PerProject(CV[CF[nFace].vI[nVertex]]);//透视投影
			Point[nVertex]=ScreenP;
			Normal4[nVertex]=CF[nFace].fNormal;
		}
		zbuf->SetPoint(Point,Normal4,4);//设置顶点
		zbuf->CreateBucket();//创建桶表
		zbuf->CreateEdge();//创建边表
		zbuf->Phong(pDC,ViewPoint,pLight,&pMaterial[1],CUBE);//颜色渐变填充四边形
		zbuf->ClearMemory();		
	}
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

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Alpha=5;Beta=5;
	tran.RotateX(Alpha);
	tran.RotateY(Beta);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景刷新 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
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
		pCmdUI->SetText("播放");
	}		
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadSphereVertex();
	ReadSphereFace();
	ReadCubeVertex();
	ReadCubeFace();
	tran.SetMat(CV,8);
	InitParameter();
}




