// ALine.h: interface for the CALine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALINE_H__048BF17D_0463_44AD_BE54_377516381BD7__INCLUDED_)
#define AFX_ALINE_H__048BF17D_0463_44AD_BE54_377516381BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"P2.h"

class CALine  
{
public:
	CALine();
	virtual ~CALine();
	void MoveTo(CDC *,CP2);//移动到指定位置
	void MoveTo(CDC *,double,double);
	void MoveTo(CDC *,double,double,CRGB);
	void LineTo(CDC *,CP2);//绘制直线，不含终点
	void LineTo(CDC *,double,double);
	void LineTo(CDC *,double,double,CRGB);
public:
	CP2 P0;//起点
	CP2 P1;//终点
};

#endif // !defined(AFX_ALINE_H__048BF17D_0463_44AD_BE54_377516381BD7__INCLUDED_)
