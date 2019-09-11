// T2.h: interface for the CT2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_T2_H__D2AA007F_2727_41E9_A00C_AC75B51F24AA__INCLUDED_)
#define AFX_T2_H__D2AA007F_2727_41E9_A00C_AC75B51F24AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CT2  
{
public:
	CT2();
	virtual ~CT2();
	CT2(double,double);
	friend CT2 operator +(const CT2 &,const CT2 &);//‘ÀÀ„∑˚÷ÿ‘ÿ
	friend CT2 operator -(const CT2 &,const CT2 &);
	friend CT2 operator *(const CT2 &,double);
	friend CT2 operator *(double,const CT2 &);
	friend CT2 operator /(const CT2 &,double);
	friend CT2 operator+=(CT2 &,CT2 &);
	friend CT2 operator-=(CT2 &,CT2 &);
	friend CT2 operator*=(CT2 &,double);
	friend CT2 operator/=(CT2 &,double);
public:
	double u;
	double v;
};

#endif // !defined(AFX_T2_H__D2AA007F_2727_41E9_A00C_AC75B51F24AA__INCLUDED_)
