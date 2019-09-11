// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"
#define Round(d) int(floor(d+0.5))//四舍五入宏定义
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
	R=1000.0,d=800.0,Phi=70.0,Theta=0.0;//视点位于正前方
	Near=200.0,Far=1200.0;//近剪切面与远剪切面		
	Beta=0.0;//旋转角
	LightNum=1;//光源个数
	pLight=new CLighting(LightNum);//一维光源动态数组
    pLight->Light[0].SetPosition(800,800,800);//设置光源位置坐标
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0); //光源的漫反射颜色	
		pLight->Light[i].L_Specular=CRGB(1.0,1.0,1.0);//光源镜面高光颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.0000001;//线性衰减系数
		pLight->Light[i].L_C2=0.00000001;//二次衰减系数
		pLight->Light[i].L_OnOff=TRUE;//光源开启	
	}
	pMaterial=new CMaterial;//一维材质动态数组
	pMaterial->SetAmbient(CRGB(0.247,0.200,0.075));//材质对环境光光的反射率
	pMaterial->SetDiffuse(CRGB(0.752,0.606,0.226));//材质对漫反射光的反射率
	pMaterial->SetSpecular(CRGB(1.0,1.0,1.0));//材质对镜面反射光的反射率
	pMaterial->SetEmit(CRGB(0.05,0.05,0.002));//材质自身发散的颜色
	pMaterial->M_n=30.0;//高光指数	
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

void CTestView::ReadCubeVertex()//读入立方体顶点坐标
{
	//顶点的三维坐标(x,y,z),立方体边长为2a
	double a=160;	
	VCube[0].x=-a;VCube[0].y=-a;VCube[0].z=-a;
	VCube[1].x=+a;VCube[1].y=-a;VCube[1].z=-a;
	VCube[2].x=+a;VCube[2].y=+a;VCube[2].z=-a;
	VCube[3].x=-a;VCube[3].y=+a;VCube[3].z=-a;
	VCube[4].x=-a;VCube[4].y=-a;VCube[4].z=+a;
	VCube[5].x=+a;VCube[5].y=-a;VCube[5].z=+a;
	VCube[6].x=+a;VCube[6].y=+a;VCube[6].z=+a;
	VCube[7].x=-a;VCube[7].y=+a;VCube[7].z=+a;
}

void CTestView::ReadCubeFace()//读入立方体面表
{
	//面的顶点数、面的顶点索引号与面的颜色
	FCube[0].SetNum(4);FCube[0].vI[0]=4;FCube[0].vI[1]=5;FCube[0].vI[2]=6;FCube[0].vI[3]=7;
	FCube[1].SetNum(4);FCube[1].vI[0]=0;FCube[1].vI[1]=3;FCube[1].vI[2]=2;FCube[1].vI[3]=1;
	FCube[2].SetNum(4);FCube[2].vI[0]=0;FCube[2].vI[1]=4;FCube[2].vI[2]=7;FCube[2].vI[3]=3;
	FCube[3].SetNum(4);FCube[3].vI[0]=1;FCube[3].vI[1]=2;FCube[3].vI[2]=6;FCube[3].vI[3]=5;
	FCube[4].SetNum(4);FCube[4].vI[0]=2;FCube[4].vI[1]=3;FCube[4].vI[2]=7;FCube[4].vI[3]=6;
	FCube[5].SetNum(4);FCube[5].vI[0]=0;FCube[5].vI[1]=1;FCube[5].vI[2]=5;FCube[5].vI[3]=4;
}

void CTestView::ReadGroundVertex()//读入地面点表
{  
	int Length=800,Width=800,Depth=160;
	VGround[0].x=-Length; VGround[0].y=-Depth;VGround[0].z=-Width;VGround[0].c=CRGB(0.5,0.5,0.5);
    VGround[1].x=-Length; VGround[1].y=-Depth;VGround[1].z= Width;VGround[1].c=CRGB(0.5,0.5,0.5); 
	VGround[2].x= Length; VGround[2].y=-Depth;VGround[2].z= Width;VGround[2].c=CRGB(0.5,0.5,0.5); 
	VGround[3].x= Length; VGround[3].y=-Depth;VGround[3].z=-Width;VGround[3].c=CRGB(0.5,0.5,0.5); 
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
	zbuf->InitDeepBuffer(1500,1000,1000);//初始化深度缓冲器
	DrawGround(pDC);
	DrawCube(pDC,zbuf);	
	DrawShadow(pDC,zbuf);
	delete zbuf;//释放内存
}

void CTestView::DrawCube(CDC *pDC,CZBuffer *zbuffer)//绘制立方体
{
	CPi3 Point[4];//面的二维顶点数组
	for(int nFace=0;nFace<6;nFace++)//面循环
	{
		CVector ViewVector(VCube[FCube[nFace].vI[0]],ViewPoint);//面的视矢量
		ViewVector=ViewVector.Normalize();//单位化视矢量
		FCube[nFace].SetFaceNormal(VCube[FCube[nFace].vI[0]],VCube[FCube[nFace].vI[1]],VCube[FCube[nFace].vI[2]]);	
		FCube[nFace].fNormal.Normalize();//单位化法矢量
		if(Dot(ViewVector,FCube[nFace].fNormal)>=0)//背面剔除
		{
			for(int nVertex=0;nVertex<FCube[nFace].vN;nVertex++)//顶点循环
			{
				PerProject(VCube[FCube[nFace].vI[nVertex]]);//透视投影
				Point[nVertex]=ScreenP;
				Point[nVertex].c=pLight->Lighting(ViewPoint,VCube[FCube[nFace].vI[nVertex]],FCube[nFace].fNormal,pMaterial);
			}
			zbuffer->SetPoint(Point,4);//设置顶点
			zbuffer->CreateBucket();//创建桶表
			zbuffer->CreateEdge();//创建边表
			zbuffer->Gouraud(pDC);//填充四边形
			zbuffer->ClearMemory();				
		}
	}
}

void CTestView::DrawGround(CDC *pDC)//绘制地面
{
	CPi3 Point[4];//面的二维顶点数组
	for(int nVertex=0;nVertex<4;nVertex++)
	{
		PerProject(VGround[nVertex]);
		Point[nVertex]=ScreenP;
	}
	CBrush NewBrush;
	NewBrush.CreateSolidBrush(RGB(100,100,100));
	CBrush *pOldBrush=pDC->SelectObject(&NewBrush);	
	pDC->BeginPath();
	pDC->MoveTo(Round(Point[0].x),Point[0].y);//绘制左侧多边形
	for(int i=1;i<4;i++)
		pDC->LineTo(Round(Point[i].x),Point[i].y);
	pDC->LineTo(Round(Point[0].x),Point[0].y);
	pDC->EndPath();
	pDC->FillPath();//FillPath填充路径层
	pDC->SelectObject(pOldBrush);
	NewBrush.DeleteObject();
}

void CTestView::DrawShadow(CDC *pDC,CZBuffer *zbuffer)//绘制阴影
{
	CPi3 Point[4];//面的二维顶点数组
	for(int nFace=0;nFace<6;nFace++)//遍历表面
	{
		//光源做为视点
		CVector ViewVector(VCube[FCube[nFace].vI[0]],pLight->Light[0].L_Position);//面的视矢量
		ViewVector=ViewVector.Normalize();//单位化视矢量
		FCube[nFace].SetFaceNormal(VCube[FCube[nFace].vI[0]],VCube[FCube[nFace].vI[1]],VCube[FCube[nFace].vI[2]]);	
		FCube[nFace].fNormal.Normalize();//单位化法矢量
		if(Dot(ViewVector,FCube[nFace].fNormal)<0)//绘制阴影
		{
			for(int nVertex=0;nVertex<FCube[nFace].vN;nVertex++)//顶点循环
			{
				VCube[FCube[nFace].vI[nVertex]].c=CRGB(0.2,0.2,0.2);//阴影颜色
				PerProject(CalculateCrossPoint(pLight->Light[0].L_Position,VCube[FCube[nFace].vI[nVertex]]));//计算该背光面上的顶点和光线连线和地面的交点
				Point[nVertex]=ScreenP;
			}
			zbuffer->SetPoint(Point,4);//设置顶点
			zbuffer->CreateBucket();//创建桶表
			zbuffer->CreateEdge();//创建边表
			zbuffer->Gouraud(pDC);//颜色渐变填充四边形
			zbuffer->ClearMemory();
		}
	}
}

CP3 CTestView::CalculateCrossPoint(CP3 p0,CP3 p1)//计算光线和地面的交点（第一个参数是光源，第二个参数是物体顶点）
{
	CP3 p;
	double	A,B,C,D;//平面方程Ax+By+Cz＋D=0的系数
	CVector V01(VGround[0],VGround[1]),V02(VGround[0],VGround[2]);
	CVector VN=Cross(V01,V02);
	A=VN.x;B=VN.y;C=VN.z;
	D=-A*VGround[0].x-B*VGround[0].y-C*VGround[0].z;	
	double t;		//计算直线参数方程的公共系数t
	t=-(A*p0.x+B*p0.y+C*p0.z+D)/(A*(p1.x-p0.x)+B*(p1.y-p0.y)+C*(p1.z-p0.z));
	p.x=p0.x+t*(p1.x-p0.x);//代入参数方程计算交点坐标
	p.y=p0.y+t*(p1.y-p0.y);
	p.z=p0.z+t*(p1.z-p0.z);
	p.c=CRGB(p1.c.red,p1.c.green,p1.c.blue);
	return  p;
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(!bPlay)
	{
		switch(nChar)
		{
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
		pCmdUI->SetText("播放");
	}		
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadGroundVertex();
	ReadCubeVertex();
	ReadCubeFace();
	tran.SetMat(VCube,8);
	InitParameter();
}




