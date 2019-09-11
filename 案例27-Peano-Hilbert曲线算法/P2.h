// P2.h: interface for the CP2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_P2_H__17ED81CB_4A2A_47B6_A74F_5A9CBDA538D2__INCLUDED_)
#define AFX_P2_H__17ED81CB_4A2A_47B6_A74F_5A9CBDA538D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CP2  
{
public:
	CP2();
	virtual ~CP2();
	CP2(double,double);
	friend CP2 operator +(const CP2 &,const CP2 &);//‘ÀÀ„∑˚÷ÿ‘ÿ
	friend CP2 operator -(const CP2 &,const CP2 &);
	friend CP2 operator *(const CP2 &,double);
	friend CP2 operator *(double,const CP2 &);
	friend CP2 operator /(const CP2 &,double);
public:
	double x;
	double y;
};

#endif // !defined(AFX_P2_H__17ED81CB_4A2A_47B6_A74F_5A9CBDA538D2__INCLUDED_)
