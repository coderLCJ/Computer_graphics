// CLine.h: interface for the CCLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLINE_H__F98927DB_3AE3_4F19_AD21_E35A3900D86F__INCLUDED_)
#define AFX_CLINE_H__F98927DB_3AE3_4F19_AD21_E35A3900D86F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"P2.h"

class CCLine  
{
public:
	CCLine();
	virtual ~CCLine();
	void MoveTo(CDC *,CP2);//移动到指定位置
	void MoveTo(CDC *,double,double,CRGB);
	void LineTo(CDC *,CP2);//绘制直线，不含终点
	void LineTo(CDC *,double,double,CRGB);
	CRGB Interpolation(double,double,double,CRGB,CRGB);//线性插值
public:
	CP2 P0;//起点
	CP2 P1;//终点
};

#endif // !defined(AFX_CLINE_H__F98927DB_3AE3_4F19_AD21_E35A3900D86F__INCLUDED_)
