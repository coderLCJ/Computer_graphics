// LeftPortion.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "LeftPortion.h"
#include "TestDoc.h"//
#include "math.h"//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftPortion

IMPLEMENT_DYNCREATE(CLeftPortion, CFormView)

CLeftPortion::CLeftPortion()
	: CFormView(CLeftPortion::IDD)
{
	//{{AFX_DATA_INIT(CLeftPortion)
	m_Ambient = FALSE;
	m_Diffuse = FALSE;
	m_Ambient = FALSE;
	m_Specular = FALSE;
	//}}AFX_DATA_INIT
}

CLeftPortion::~CLeftPortion()
{
}

void CLeftPortion::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftPortion)
	DDX_Check(pDX, IDC_AMBIENT, m_Ambient);
	DDX_Check(pDX, IDC_DIFFUSE, m_Diffuse);
	DDX_Check(pDX, IDC_SPECULAR, m_Specular);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLeftPortion, CFormView)
	//{{AFX_MSG_MAP(CLeftPortion)
	ON_BN_CLICKED(IDC_AMBIENT, OnAmbient)
	ON_BN_CLICKED(IDC_DIFFUSE, OnDiffuse)
	ON_BN_CLICKED(IDC_SPECULAR, OnSpecular)
	ON_BN_CLICKED(IDC_GOLD, OnGold)
	ON_BN_CLICKED(IDC_SILVER, OnSilver)
	ON_BN_CLICKED(IDC_RUBY, OnRuby)
	ON_BN_CLICKED(IDC_BERYL, OnBeryl)
	ON_BN_CLICKED(IDC_LEFTTOP, OnLefttop)
	ON_BN_CLICKED(IDC_LEFTDOWN, OnLeftdown)
	ON_BN_CLICKED(IDC_RIGHTTOP, OnRighttop)
	ON_BN_CLICKED(IDC_RIGHTDOWN, OnRightdown)		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftPortion diagnostics

#ifdef _DEBUG
void CLeftPortion::AssertValid() const
{
	CFormView::AssertValid();
}

void CLeftPortion::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftPortion message handlers

void CLeftPortion::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	//设置左窗格控件的初始状态
	CheckRadioButton(IDC_GOLD,IDC_BERYL,IDC_RUBY);
	CheckRadioButton(IDC_LEFTTOP,IDC_RIGHTDOWN,IDC_RIGHTTOP);
	m_Ambient=TRUE;
	m_Diffuse=TRUE;
	m_Specular=TRUE;	
 	UpdateData(FALSE);
}

void CLeftPortion::OnAmbient()//环境光 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
 	pDoc->UpdateAllViews(NULL,1);	
}

void CLeftPortion::OnDiffuse()//漫反射光
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,2);	
}

void CLeftPortion::OnSpecular()//镜面反射光 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,3);
}

void CLeftPortion::OnGold()//“金”材质 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,4);
}

void CLeftPortion::OnSilver()//“银”材质 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,5);
}

void CLeftPortion::OnRuby()//“红宝石”材质  
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,6);
}

void CLeftPortion::OnBeryl()//“绿宝石”材质 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,7);
}

void CLeftPortion::OnLefttop()//光源位于“左上”
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,8);	
}

void CLeftPortion::OnLeftdown()//光源位于“左下” 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,9);
}

void CLeftPortion::OnRighttop()//光源位于“右上” 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,10);
}

void CLeftPortion::OnRightdown()//光源位于“右下” 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->UpdateAllViews(NULL,11);
}











