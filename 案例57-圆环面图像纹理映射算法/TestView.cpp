// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define  PI 3.1415926
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
	Alpha=0.0,Beta=0.0;//旋转角
	LightNum=1;//光源个数
	pLight=new CLighting(LightNum);//一维光源动态数组
    pLight->Light[0].SetPosition(0,0,800);//设置第一个光源位置坐标
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(0.7,0.7,0.7); //光源的漫反射颜色	
		pLight->Light[i].L_Specular=CRGB(0.3,0.3,0.3);//光源镜面高光颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.0001;//线性衰减系数
		pLight->Light[i].L_C2=0.00000001;//二次衰减系数
		pLight->Light[i].L_OnOff=TRUE;//光源开启	
	}
 	pMaterial=new CMaterial;//一维材质动态数组
	pMaterial->SetAmbient(CRGB(0.247,0.200,0.075));//材质对环境光光的反射率
	pMaterial->SetDiffuse(CRGB(0.4,0.4,0.4));//材质对漫反射光的反射率
	pMaterial->SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
	pMaterial->M_Emit=CRGB(0.0,0.0,0.0);//材质自身发散的颜色
	pMaterial->M_n=20.0;//高光指数
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
		pLight=NULL;
	}
	if(V!=NULL)
	{
		delete []V;
		V=NULL;
	}
	if(T!=NULL)
	{
		delete []T;
		T=NULL;
	}
	if(N!=NULL)
	{
		delete []N;
		N=NULL;
	}
	for(int n=0;n<N1;n++)
	{
		delete[] F[n];
		F[n]=NULL;
	}
	delete []F;
	F=NULL;
	for(n=0;n<bmp.bmHeight;n++)
	{
		delete []Image[n];
		Image[n]=NULL;
	}
	delete []Image;
	Image=NULL;
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
	int tAlpha=10,tBeta=10;//等分角度
	int r1=200,r2=80;//圆环半径和环截面半径
	N1=360/tAlpha,N2=360/tBeta;//面片数量为N1×N2
	V=new CP3[N1*N2];//顶点动态数组
	T=new CT2[N1*N2];//纹理动态数组
	N=new CVector[N1*N2];//法矢量动态数组
	for(int i=0;i<N1;i++)
	{
		double tAlpha1=tAlpha*i*PI/180;
		for(int j=0;j<N2;j++)//顶点赋值
		{
			double tBeta1=tBeta*j*PI/180;
			V[i*N2+j].x=(r1+r2*sin(tBeta1))*sin(tAlpha1);
			V[i*N2+j].y=r2*cos(tBeta1);
			V[i*N2+j].z=(r1+r2*sin(tBeta1))*cos(tAlpha1);
			//计算顶点的纹理
			T[i*N2+j].u=(2*PI-tAlpha1)/(2*PI)*(bmp.bmWidth-1);
			T[i*N2+j].v=tBeta1/(2*PI)*(bmp.bmHeight-1);
		}
	}
}

void CTestView::CalNormal()//计算法矢量
{
	for(int i=0;i<N1;i++)//周向
	{
		for(int j=0;j<N2;j++)//纵向
		{
			//计算顶点的平均法矢量
			int Beforei=i-1,Afteri=i+1;
			int Beforej=j-1,Afterj=j+1;
			if(0==i) Beforei=N1-1;
			if(0==j) Beforej=N2-1;
			if(N1==Afteri) Afteri=0;
			if(N2==Afterj) Afterj=0;
			CVector vN0,vN1,vN2,vN3,AveN;//相邻四个面片的法矢量及平均法矢量	
			CVector vEdge01(V[Beforei*N2+Afterj],V[i*N2+Afterj]);
			CVector vEdge02(V[Beforei*N2+Afterj],V[i*N2+j]);
			vN0=Cross(vEdge01,vEdge02);
			CVector vEdge11(V[i*N2+Afterj],V[Afteri*N2+Afterj]);
			CVector vEdge12(V[i*N2+Afterj],V[Afteri*N2+j]);
			vN1=Cross(vEdge11,vEdge12);
			CVector vEdge21(V[i*N2+j],V[Afteri*N2+j]);
			CVector vEdge22(V[i*N2+j],V[Afteri*N2+Beforej]);
			vN2=Cross(vEdge21,vEdge22);
			CVector vEdge31(V[Beforei*N2+j],V[i*N2+j]);
			CVector vEdge32(V[Beforei*N2+j],V[i*N2+Beforej]);
			vN3=Cross(vEdge31,vEdge32);
			AveN=(vN0+vN1+vN2+vN3)/AveN.Mag();//顶点法矢量的平均值
			N[i*N2+j]=AveN;
		}
	}	
}

void CTestView::ReadFace()//面表
{
	F=new CFace *[N1];
	for(int n=0;n<N1;n++)
	{
		F[n]=new CFace[N2];
	}
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			int tempi=i+1;
			int tempj=j+1;
			if(N2==tempj) tempj=0;
			if(N1==tempi) tempi=0;
			F[i][j].SetNum(4);//面的边数
			F[i][j].vI[0]=i*N2+j;//建立面的边号
			F[i][j].vI[1]=i*N2+tempj;
			F[i][j].vI[2]=tempi*N2+tempj; 
			F[i][j].vI[3]=tempi*N2+j;
		}
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
	ViewPoint.x=R*k[6];
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CTestView::PerProject(CP3 P)//透视变换
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

void CTestView::DrawObject(CDC *pDC)//绘制圆环面
{
	CalNormal();
	CZBuffer *zbuf=new CZBuffer;//申请内存
	zbuf->InitDeepBuffer(800,800,1000);//初始化深度缓冲器
	CPi3 Point4[4];//侧面四边形顶点数组
	CT2 Texture4[4];//侧面四边形纹理数组
	CVector Normal4[4];//侧面四边形法矢量数组
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			CVector ViewVector(V[F[i][j].vI[0]],ViewPoint);//面的视矢量
			ViewVector=ViewVector.Normalize();//单位化视矢量
			F[i][j].SetFaceNormal(V[F[i][j].vI[0]],V[F[i][j].vI[1]],V[F[i][j].vI[2]]);//计算小面片法矢量
			F[i][j].fNormal.Normalize();//单位化法矢量
			if(Dot(ViewVector,F[i][j].fNormal)>=0)
			{
				for(int m=0;m<F[i][j].vN;m++)
				{
					PerProject(V[F[i][j].vI[m]]);
					Point4[m]=ScreenP;
					Normal4[m]=N[F[i][j].vI[m]];
					Texture4[m]=T[F[i][j].vI[m]];
				}
				if((N2-1)==j)//消除函数纹理的接缝
				{
					Texture4[1].v=bmp.bmHeight-1;
					Texture4[2].v=bmp.bmHeight-1;
				}
				if ((N1-1)==i)
				{
					Texture4[2].u=1.0;
					Texture4[3].u=1.0;
				}
				zbuf->SetPoint(Point4,Normal4,Texture4,4);//初始化
				zbuf->CreateBucket();//创建桶表
				zbuf->CreateEdge();//创建边表
				zbuf->Phong(pDC,ViewPoint,pLight,pMaterial,Image);//填充四边形
				zbuf->ClearMemory();
			}
		}
	}
	delete zbuf;
}

void CTestView::ReadImage()//读入纹理
{
    CBitmap NewBitmap;
	NewBitmap.LoadBitmap(IDB_TEXTURE);//调入DDB位图
	NewBitmap.GetBitmap(&bmp);//将CBitmap的信息保存到Bitmap结构体中
	int nbytesize=bmp.bmWidthBytes*bmp.bmHeight;
	im=new BYTE[nbytesize];
	NewBitmap.GetBitmapBits(nbytesize,(LPVOID)im);
	Image=new COLORREF*[bmp.bmHeight];
	for(int n1=0;n1<bmp.bmHeight;n1++)
		Image[n1]=new COLORREF[bmp.bmWidth];		
	for(n1=0;n1<bmp.bmHeight;n1++)
	{
		for(int n2=0;n2<bmp.bmWidth;n2++)
		{
			int pos=n1*bmp.bmWidthBytes+4*n2;//颜色分量位置
			n1=bmp.bmHeight-1-n1;//位图从左下角向右上角绘制
			Image[n1][n2]=RGB(im[pos+2],im[pos+1],im[pos]);
		}
	}
	delete []im;
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
	Alpha=5;
	tran.RotateX(Alpha);
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
		pCmdUI->SetText("开始");
	}
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadImage();
	ReadVertex();
	ReadFace();
	tran.SetMat(V,N1*N2);
	tran.RotateX(90);
	InitParameter();
}





