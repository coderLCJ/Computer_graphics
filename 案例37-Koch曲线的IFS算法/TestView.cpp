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

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CRect rect;
	GetClientRect(&rect);	
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);	
	IFSCode(pDC);
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

void CTestView::IFSCode(CDC *pDC)
{
	P=CP2(100,100);
	Code[0][0]=0.333;Code[0][1]=0;     Code[0][2]=0;     Code[0][3]=0.333;Code[0][4]=0;    Code[0][5]=0;    Code[0][6]=0.25;
	Code[1][0]=0.167;Code[1][1]=-0.289;Code[1][2]=0.289; Code[1][3]=0.167;Code[1][4]=0.333;Code[1][5]=0;    Code[1][6]=0.25;
	Code[2][0]=0.167;Code[2][1]=0.289; Code[2][2]=-0.289;Code[2][3]=0.167;Code[2][4]=0.5;  Code[2][5]=0.289;Code[2][6]=0.25;
	Code[3][0]=0.333;Code[3][1]=0;     Code[3][2]=0;     Code[3][3]=0.333;Code[3][4]=0.667;Code[3][5]=0;    Code[3][6]=0.25;
	for(int i=0;i<100000;i++)//分形图的浓度
	{
		double R=double(rand())/RAND_MAX;//RAND_MAX随机数的最大值
		if(R<=Code[0][6]) 
		{
			a=Code[0][0];b=Code[0][1];c=Code[0][2];d=Code[0][3];e=Code[0][4];f=Code[0][5];
		}
		else if(R<=Code[0][6]+Code[1][6])
		{
			a=Code[1][0];b=Code[1][1];c=Code[1][2];d=Code[1][3];e=Code[1][4];f=Code[1][5];
		}
		else if(R<=Code[0][6]+Code[1][6]+Code[2][6])
		{
			a=Code[2][0];b=Code[2][1];c=Code[2][2];d=Code[2][3];e=Code[2][4];f=Code[2][5];
		}
		else 
		{
			a=Code[3][0];b=Code[3][1];c=Code[3][2];d=Code[3][3];e=Code[3][4];f=Code[3][5];
		}
		P1.x=a*P.x+b*P.y+e;//仿射变换
		P1.y=c*P.x+d*P.y+f;
		P=P1;
		double k=500;//调节系数
		pDC->SetPixelV((Round(4/3*k*P.x)-250),(Round(k*P.y-300)+200),RGB(P.x*500*R,R*100,P.y*500*R));
	}
}

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
}
