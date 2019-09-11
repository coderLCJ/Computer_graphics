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
    pLight->Light[0].SetPosition(800,800,800);//设置第一个光源位置坐标
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0); //光源的漫反射颜色	
		pLight->Light[i].L_Specular=CRGB(0.8,0.8,0.8);//光源镜面高光颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.0001;//线性衰减系数
		pLight->Light[i].L_C2=0.00000001;//二次衰减系数
		pLight->Light[i].L_OnOff=TRUE;//光源开启	
	}
 	pMaterial=new CMaterial;//一维材质动态数组
	pMaterial->SetAmbient(CRGB(0.247,0.200,0.075));//材质对环境光光的反射率
	pMaterial->SetDiffuse(CRGB(0.6,0.0,0.0));//材质对漫反射光的反射率
	pMaterial->SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
	pMaterial->M_Emit=CRGB(0.2,0.0,0.0);//材质自身发散的颜色
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
		delete Imgx_Gradient[n];
		Imgx_Gradient[n]=NULL;
		delete Imgy_Gradient[n];
		Imgy_Gradient[n]=NULL;
	}
	delete []Image;
	Image=NULL;
	delete []Imgx_Gradient;
	Imgx_Gradient=NULL;
	delete []Imgy_Gradient;
	Imgy_Gradient=NULL;
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
	gAlpha=4,gBeta=8;//面片夹角
	int r=300;//r为球面半径
	N1=180/gAlpha,N2=360/gBeta;//N1为纬度区域,N2为经度区域
	V=new CP3[(N1-1)*N2+2];
	T=new CT2[(N1-1)*N2+2];
	double gAlpha1,gBeta1;
	//计算北极点坐标
	V[0].x=0,V[0].y=r,V[0].z=0;
	T[0].u=0;T[0].v=0;//闲置
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
			T[i*N2+j+1].u=gBeta1/(2*PI)*(bmp.bmWidth-1);
 			T[i*N2+j+1].v=(PI-gAlpha1)/PI*(bmp.bmHeight-1);
		}
	}
	//计算南极点坐标
	V[(N1-1)*N2+1].x=0,V[(N1-1)*N2+1].y=-r,V[(N1-1)*N2+1].z=0;
	T[(N1-1)*N2+1].u=0;T[(N1-1)*N2+1].v=0;//闲置
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

void CTestView::DrawObject(CDC *pDC)//绘制球面
{
	CZBuffer *zbuf=new CZBuffer;//申请内存
	zbuf->InitDeepBuffer(800,800,1000);//初始化深度缓冲器
	zbuf->ReadGradient(Imgx_Gradient,Imgy_Gradient);
	CPi3 Point3[3];//底面与顶面三角形顶点数组
	CT2 Texture3[3];//底面与顶面三角形纹理数组
	CVector Normal3[3];//底面与顶面三角形法矢量数组
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
				if(3==F[i][j].vN)//处理三角形面片
				{
					for(int m=0;m<F[i][j].vN;m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point3[m]=ScreenP;
						Normal3[m]=CVector(V[F[i][j].vI[m]]);
					}
					double tempi=i+1,tempj=j+1;//对三角形面片进行特殊处理
					Texture3[0].u=gBeta*(j)/360.0*(bmp.bmWidth-1);Texture3[0].v=(1.0-gAlpha*i/180.0)*(bmp.bmHeight-1);  
					Texture3[1].u=gBeta*(j)/360.0*(bmp.bmWidth-1);Texture3[1].v=(1.0-gAlpha*tempi/180.0)*(bmp.bmHeight-1);
					Texture3[2].u=gBeta*tempj/360.0*(bmp.bmWidth-1);  Texture3[2].v=(1.0-gAlpha*i/180.0)*(bmp.bmHeight-1);
					zbuf->SetPoint(Point3,Normal3,Texture3,3);//初始化
					zbuf->CreateBucket();//创建桶表
					zbuf->CreateEdge();//创建边表
					zbuf->Phong(pDC,ViewPoint,pLight,pMaterial);//填充三角形
					zbuf->ClearMemory();
				}
				else//处理四边形面片
				{
					for(int m=0;m<F[i][j].vN;m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point4[m]=ScreenP;
						Normal4[m]=CVector(V[F[i][j].vI[m]]);
						Texture4[m]=T[F[i][j].vI[m]];
					}
					if(N2-1==j)//消除图像纹理的接缝
					{
						Texture4[2].u=bmp.bmWidth-1;
						Texture4[3].u=bmp.bmWidth-1;
					}
					zbuf->SetPoint(Point4,Normal4,Texture4,4);//初始化
					zbuf->CreateBucket();//创建桶表
					zbuf->CreateEdge();//创建边表
					zbuf->Phong(pDC,ViewPoint,pLight,pMaterial);//填充四边形
					zbuf->ClearMemory();
				}				
			}
		}
	}
	delete zbuf;
}

void CTestView::ReadBumpMap()//读入凹凸纹理
{
    CBitmap NewBitmap;
	NewBitmap.LoadBitmap(IDB_BUMPTEXTURE);//导入DDB位图
	NewBitmap.GetBitmap(&bmp);//将CBitmap的信息保存到Bitmap结构体中
	int nbytesize=bmp.bmWidthBytes*bmp.bmHeight;
	im=new BYTE[nbytesize];
	NewBitmap.GetBitmapBits(nbytesize,(LPVOID)im);
	Image=new COLORREF*[bmp.bmHeight];
	Imgx_Gradient=new double*[bmp.bmHeight];
	Imgy_Gradient=new double*[bmp.bmHeight];
	for(int n1=0;n1<bmp.bmHeight;n1++)
	{
		Image[n1]=new COLORREF[bmp.bmWidth];
		Imgx_Gradient[n1]=new double[bmp.bmWidth];
		Imgy_Gradient[n1]=new double[bmp.bmWidth];
	}
	for(n1=0;n1<bmp.bmHeight;n1++)
	{
		for(int n2=0;n2<bmp.bmWidth;n2++)
		{
			int pos=n1*bmp.bmWidthBytes+4*n2;//颜色分量位置
			n1=bmp.bmHeight-1-n1;//位图从左下角向右上角绘制
			Image[n1][n2]=RGB(im[pos+2],im[pos+1],im[pos]);
		}
	}
	for(n1=0;n1<bmp.bmHeight;n1++)
	{
		for(int n2=0;n2<bmp.bmWidth;n2++)
		{
			int fontx,backx,fonty,backy;//一阶中心差分
			fontx=n1+1;backx=n1-1;
			fonty=n2+1;backy=n2-1;
			//检测图片的边界，防止越界
            if(backx<0)
				backx=0;
			if(backy<0)
				backy=0;
			if(fontx>bmp.bmHeight-1)
				fontx=bmp.bmHeight-1;
			if(fonty>bmp.bmWidth-1)
				fonty=bmp.bmWidth-1;	
			//分别得到每个点的x与y的偏移量
			Imgx_Gradient[n1][n2]=(GetRValue(Image[n1][fonty])-GetRValue(Image[n1][backy]));
			Imgy_Gradient[n1][n2]=(GetRValue(Image[fontx][n2])-GetRValue(Image[backx][n2]));
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
	Beta=-5;
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
		pCmdUI->SetText("开始");
	}
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadBumpMap();
	ReadVertex();
	ReadFace();
	tran.SetMat(V,(N1-1)*N2+2);
	tran.RotateY(280);
	InitParameter();
}




