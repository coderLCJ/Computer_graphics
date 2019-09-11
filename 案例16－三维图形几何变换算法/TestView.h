// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
#define AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Line.h"//包含直线类
#include "Face.h"//包含表面类
#include "Transform.h"

class CTestView : public CView
{
protected: // create from serialization only
	CTestView();
	DECLARE_DYNCREATE(CTestView)

// Attributes
public:
	CTestDoc* GetDocument();

// Operations
public:
	void DoubleBuffer();//双缓冲
	void ReadPoint();//读入顶点表
	void ReadFace();//读入表面表
	void ObliqueProject(CP3);//斜等侧变换
	void DrawObject(CDC *);//绘制图形
	void DrawPolygon(CDC *,CLine *);//绘制多边形
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CP3 P[8];//点表
	CFace F[6];//面表
	CRect rect;//定义客户区
	CP2 ScreenP;//屏幕坐标系的二维坐标点
	CTransform trans;
	// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnDrawpic();
	afx_msg void OnLeft();
	afx_msg void OnRight();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnIncrease();
	afx_msg void OnDecrease();
	afx_msg void OnReset();
	afx_msg void OnBack();
	afx_msg void OnFront();
	afx_msg void OnRxaxis();
	afx_msg void OnRyaxis();
	afx_msg void OnRzaxis();
	afx_msg void OnRxoy();
	afx_msg void OnRyoz();
	afx_msg void OnRzox();
	afx_msg void OnSxdirectionplus();
	afx_msg void OnSydirectionplus();
	afx_msg void OnSzdirectionneg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestView.cpp
inline CTestDoc* CTestView::GetDocument()
   { return (CTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
