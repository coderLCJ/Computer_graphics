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
	DrawGraph();
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
	P[0].x=50;  P[0].y=100;
	P[1].x=-150;P[1].y=300;
	P[2].x=-250;P[2].y=50;
	P[3].x=-150;P[3].y=-250;
	P[4].x=0;   P[4].y=-50;
	P[5].x=100; P[5].y=-250;
	P[6].x=500; P[6].y=150;
}

void CTestView::DrawGraph()//绘制图形
{
	CRect rect;                                         //定义客户区
	GetClientRect(&rect);                               //获得客户区的大小
	CDC *pDC=GetDC();                                   //定义设备上下文指针              
	pDC->SetMapMode(MM_ANISOTROPIC);                    //自定义坐标系
	pDC->SetWindowExt(rect.Width(),rect.Height());      //设置窗口比例
	pDC->SetViewportExt(rect.Width(),-rect.Height());   //设置视区比例，且x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//设置客户区中心为坐标系原点
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);  //矩形与客户区重合
	if(!bFill)
		DrawPolygon(pDC);//绘制多边形
	else
		FillPolygon(pDC);//填充多边形
	ReleaseDC(pDC);//释放DC
}

void CTestView::DrawPolygon(CDC *pDC)//绘制多边形边界
{
	CLine *line=new CLine;
	CP2 t;
	for(int i=0;i<7;i++)//绘制多边形
	{
		if(i==0)
		{
			line->MoveTo(pDC,P[i]);
			t=P[i];
		}
		else
		{
			line->LineTo(pDC,P[i]);
		}		
	}
	line->LineTo(pDC,t);//闭合多边形
	delete line;
}

void CTestView::FillPolygon(CDC *pDC)//填充多边形
{
	for(int i=0;i<7;i++)//转储顶点坐标，y坐标取为整数
	{
		P1[i].x=P[i].x;
		P1[i].y=Round(P[i].y);
		P1[i].c=CRGB(bRed/255.0,bGreen/255.0,bBlue/255.0);
	}
	CFill *fill=new CFill;      //动态分配内存 
	fill->SetPoint(P1,7);       //初始化Fill对象
	fill->CreateBucket();       //建立桶表
	fill->CreateEdge();         //建立边表
    fill->Gouraud(pDC);         //填充多边形
	delete fill;                //撤销内存
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	COLORREF GetClr=RGB(0,0,0);//调色板颜色
	CColorDialog ccd(GetClr,CC_SOLIDCOLOR);
	if(IDOK==ccd.DoModal())//调用颜色对话框选取填充色
		GetClr=ccd.GetColor();	
	else
		return;
	bRed=GetRValue(GetClr);  //获取红色分量
	bGreen=GetGValue(GetClr);//获取绿色分量
	bBlue=GetBValue(GetClr); //获取蓝色分量
	bFill=TRUE;
	Invalidate();	
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	bFill=FALSE;
	ReadPoint();
}
