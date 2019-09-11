// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
#define PI 3.1415926//PI的宏定义
#include "InputDlg.h"//对话框
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
	pDC=NULL;
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

void CTestView::Initial(int n)//文法模型
{	
	StackPushPos=0;
	NewRule=Rule[0];
	NewRuleTemp.Empty();
	int Length=NewRule.GetLength();
	for(int i=1;i<=n;i++)
	{		
		int Pos=0;
		for(int j=0;j<Length;j++)//规则替换
		{
			if(Axiom[1]==NewRule[j])
			{
				NewRuleTemp+=Rule[1];
				Pos=NewRuleTemp.GetLength()-1 ;
			}
			else if(Axiom[2]==NewRule[j])
			{
				NewRuleTemp+=Rule[2];
				Pos=NewRuleTemp.GetLength()-1 ;
			}
			else
			{
				NewRuleTemp+=NewRule[j];
				Pos++;
			}
		}
		NewRule=NewRuleTemp;
		NewRuleTemp.Empty();
		Length=NewRule.GetLength();
	}	
}
	
void CTestView::Peano(double theta,double d)//绘图规则
{
	if(NewRule.IsEmpty ())//字符串空返回
		return ;
	else 
	{
		CStateNode CurrentNode,NextNode;
  		CurrentNode.x=P0.x;
		CurrentNode.y=P0.y;
		CurrentNode.alpha=0;
		int Len=NewRule.GetLength();
		pDC->MoveTo(Round(CurrentNode.x),Round(CurrentNode.y));
		for(int i=0;i<Len;i++)
		{
			switch(NewRule[i])//访问字符串中的某个位置的字符
			{
				case 'F'://取出"F"字符的操作
		 			NextNode.x=CurrentNode.x+d*cos(CurrentNode.alpha);
					NextNode.y=CurrentNode.y+d*sin(CurrentNode.alpha);
					NextNode.alpha=CurrentNode.alpha;
					pDC->LineTo(Round(NextNode.x),Round(NextNode.y));
					CurrentNode=NextNode;
					break ;					
				case '+'://取出"+"字符的操作
					CurrentNode.alpha=CurrentNode.alpha+theta;
					break;
				case '-'://取出"-"字符的操作
					CurrentNode.alpha=CurrentNode.alpha-theta;
					break;
				default:
					break;
			}			
		}
	}
}			

void CTestView::OnDrawpic() 
{
	// TODO: Add your command handler code here
	CInputDlg dlg;
	int n;
	if(IDOK==dlg.DoModal())
	{
		n=dlg.m_n;	
	}
	else
		return;
	RedrawWindow();
	pDC=GetDC();
	CRect rect;
	GetClientRect(&rect);	
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);	
	P0.x=-rect.Width()/2.0+250;P0.y=rect.Height()/2.0-30;//起点坐标
	Axiom[1]="X";
    Axiom[2]="Y";
	Rule[0]="X";
	Rule[1]="-YF+XFX+FY-";
	Rule[2]="+XF-YFY-FX+";
	Initial(n);	
	double d,Theta;
	d=9;Theta=PI/2.0;
	Peano(Theta,d);
	ReleaseDC(pDC);
}
