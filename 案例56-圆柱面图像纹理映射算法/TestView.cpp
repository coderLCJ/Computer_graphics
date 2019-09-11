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
	for(int n=0;n<N2+2;n++)//注意撤销次序,先列后行,与设置相反
	{
		delete []F[n];
		F[n]=NULL;
	}
	delete []F;
	F=NULL;
	for(n=0;n<bmp.bmHeight;n++)//注意撤销次序,先列后行,与设置相反
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
	double r=144;//圆柱底面半径
	h=500;//圆柱的高
	cTheta=10;//周向夹角
	cNum=10;//纵向间距
	N1=360/cTheta;//N1周向网格数
	N2=Round(h/cNum);//N2纵向网格数
	V=new CP3[N1*(N2+1)+2];//顶点动态数组
	T=new CT2[N1*(N2+1)+2];//纹理动态数组
	N=new CVector[N1*(N2+1)+2];//法矢量动态数组
	double cTheta1,cNum1;
	V[0].x=0;V[0].y=0;V[0].z=0;//底面中心
	T[0].u=0;T[0].v=0;//闲置
	for(int i=0;i<N2+1;i++)//纵向
	{
		cNum1=i*cNum;
		for(int j=0;j<N1;j++)//周向
		{
			cTheta1=j*cTheta*PI/180;
			V[i*N1+j+1].x=r*cos(cTheta1);
			V[i*N1+j+1].y=cNum1;
			V[i*N1+j+1].z=r*sin(cTheta1);
			T[i*N1+j+1].u=(2*PI-cTheta1)/(2*PI)*(bmp.bmWidth-1);//u(0->1)
			T[i*N1+j+1].v=V[i*N1+j+1].y/h*(bmp.bmHeight-1);//v(0->1)
		}
	}
	V[N1*(N2+1)+1].x=0;V[N1*(N2+1)+1].y=h;V[N1*(N2+1)+1].z=0;//顶面中心	
	T[N1*(N2+1)+1].u=0;T[N1*(N2+1)+1].v=0;//闲置
}

void CTestView::CalNormal()//计算顶点法矢量
{
	for(int i=0;i<N2+1;i++)//周向
	{
		for(int j=0;j<N1;j++)//纵向
		{
			//计算顶点的平均法矢量
			int Beforei=i-1,Afteri=i+1;
			int Beforej=j-1,Afterj=j+1;
			if(0==i) continue;
			if(0==j) Beforej=N1-1;
			if(N2+1==Afteri) continue;
			if(N1==Afterj) Afterj=0;
			CVector vN0,vN1,AveN;//相邻2个面片的法矢量及平均法矢量	
			CVector vEdge01(V[Beforei*N1+Afterj+1],V[Beforei*N1+j+1]);
			CVector vEdge02(V[Beforei*N1+Afterj+1],V[i*N1+j+1]);
			vN0=Cross(vEdge01,vEdge02);
			CVector vEdge11(V[Beforei*N1+j+1],V[Beforei*N1+Beforej+1]);
			CVector vEdge12(V[Beforei*N1+j+1],V[i*N1+Beforej+1]);
			vN1=Cross(vEdge11,vEdge12);
			AveN=(vN0+vN1)/AveN.Mag();//顶点法矢量的平均值
			N[i*N1+j+1]=AveN;
		}
	}	
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
		if(N1==tempj) tempj=0;//面片的首尾连接
		int BottomIndex[3];//底部三角形面片索引号数组
		BottomIndex[0]=0;
		BottomIndex[1]=j+1;
		BottomIndex[2]=tempj+1;
		F[0][j].SetNum(3);
		for(int k=0;k<F[0][j].vN;k++)//面片中顶点索引号
			F[0][j].vI[k]=BottomIndex[k];
	}	
	for(int i=1;i<N2+1;i++)//构造圆柱体四边形面片
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
		if(N1==tempj) tempj=0;
		int TopIndex[3];//顶部三角形面片索引号数组
		TopIndex[0]=N1*i+1;
		TopIndex[1]=N1*(i-1)+tempj+1;
		TopIndex[2]=N1*(i-1)+j+1;
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

void CTestView::DrawObject(CDC *pDC)//绘制圆柱面
{
	CalNormal();
	CZBuffer *zbuf=new CZBuffer;//申请内存
	zbuf->InitDeepBuffer(800,800,1000);//初始化深度缓冲器
	CPi3 Point3[3];//底面与顶面三角形顶点数组
	CT2 Texture3[3];//底面与顶面三角形纹理数组
	CVector Normal3[3];//底面与顶面三角形法矢量数组
	CPi3 Point4[4];//侧面四边形顶点数组
	CT2 Texture4[4];//侧面四边形纹理数组
	CVector Normal4[4];//侧面四边形法矢量数组
	for(int i=0;i<N2+2;i++)
	{
		for(int j=0;j<N1;j++)
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
						Normal3[m]=F[i][j].fNormal;
					}
					double tempj=j+1;//对三角形面片进行特殊处理
					Texture3[0].u=cTheta*(j+0.5)/360.0;Texture3[0].v=0.0;  
				    Texture3[1].u=cTheta*(j+0.5)/360.0;Texture3[1].v=0.0;
				    Texture3[2].u=cTheta*tempj/360.0;  Texture3[2].v=0.0;
					zbuf->SetPoint(Point3,Normal3,Texture3,3);//初始化
					zbuf->CreateBucket();//创建桶表
					zbuf->CreateEdge();//创建边表
					zbuf->Phong(pDC,ViewPoint,pLight,pMaterial,Image);//填充三角形
					zbuf->ClearMemory();
				}
				else//处理四边形面片
				{
					for(int m=0;m<F[i][j].vN;m++)
					{
						PerProject(V[F[i][j].vI[m]]);
						Point4[m]=ScreenP;
						Normal4[m]=N[F[i][j].vI[m]];
						Texture4[m]=T[F[i][j].vI[m]];
					}
					if(N1-1==j)//消除图像纹理的接缝
					{
						Texture4[2].u=0.0;
						Texture4[3].u=0.0;
					}
					zbuf->SetPoint(Point4,Normal4,Texture4,4);//初始化
					zbuf->CreateBucket();//创建桶表
					zbuf->CreateEdge();//创建边表
					zbuf->Phong(pDC,ViewPoint,pLight,pMaterial,Image);//填充四边形
					zbuf->ClearMemory();
				}
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
	Beta=5;
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
	ReadImage();
	ReadVertex();
	ReadFace();
	tran.SetMat(V,N1*(N2+1)+2);
	tran.Translate(0,-h/2,0);
	InitParameter();
}




