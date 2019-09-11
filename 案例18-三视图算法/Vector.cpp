// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Vector.h"
#include "math.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVector::CVector()
{
	x=0.0;
	y=0.0;
	z=0.0;
}

CVector::~CVector()
{

}

CVector::CVector(CP3 p)
{
	x=p.x;
	y=p.y;
	z=p.z;	
}

CVector::CVector(CP3 p1,CP3 p2)
{
	x=p2.x-p1.x;
	y=p2.y-p1.y;
	z=p2.z-p1.z;
}

CVector CVector::Unit()//单位矢量
{
	CVector vector;
	double product=sqrt(x*x+y*y+z*z);
	if(fabs(product)<1e-5)
		product=1.0;
	vector.x=x/product;
	vector.y=y/product;
	vector.z=z/product;
	return vector;
}

double CVector::Mold()//矢量的模
{
	double product=sqrt(x*x+y*y+z*z);
	return product;
}

CVector operator +(CVector &v1,CVector &v2)//矢量的和
{	
	CVector vector;
	vector.x=v1.x+v2.x;
	vector.y=v1.y+v2.y;
	vector.z=v1.z+v2.z;
	return vector;
}

CVector operator -(CVector &v1,CVector &v2)//矢量的差
{	
	CVector vector;
	vector.x=v1.x-v2.x;
	vector.y=v1.y-v2.y;
	vector.z=v1.z-v2.z;
	return vector;
}

CVector operator *(CVector &v,double k)//矢量和常量的积
{	
	CVector vector;
	vector.x=v.x*k;
	vector.y=v.y*k;
	vector.z=v.z*k;
	return vector;
}

CVector operator /(CVector &v,double k)//矢量数除
{
	if(fabs(k)<1e-6)
		k=1.0;
	CVector vector;
	vector.x=v.x/k;
	vector.y=v.y/k;
	vector.z=v.z/k;
	return vector;
}

CVector operator +=(CVector &v1,CVector &v2)//+=运算符重载
{
	v1.x=v1.x+v2.x;
	v1.y=v1.y+v2.y;
	v1.z=v1.z+v2.z;
	return v1;	
}

CVector operator -=(CVector &v1,CVector &v2)//-=运算符重载
{
	v1.x=v1.x-v2.x;
	v1.y=v1.y-v2.y;
	v1.z=v1.z-v2.z;
	return v1;	
}

CVector operator *=(CVector &v1,CVector &v2)//*=运算符重载
{
	v1.x=v1.x*v2.x;
	v1.y=v1.y*v2.y;
	v1.z=v1.z*v2.z;
	return v1;	
}

CVector operator /=(CVector &v1,double k)///=运算符重载
{
   v1.x=v1.x/k;
   v1.y=v1.y/k;
   v1.z=v1.z/k;
   return v1;
}

double Dot(CVector &v1,CVector &v2)//矢量的点积
{
	return(v1.x*v2.x+v1.y*v2.y+v1.z*v2.z);
}

CVector operator * (CVector &v1,CVector &v2)//矢量的叉积
{
	CVector vector;
	vector.x=v1.y*v2.z-v1.z*v2.y;
	vector.y=v1.z*v2.x-v1.x*v2.z;
	vector.z=v1.x*v2.y-v1.y*v2.x;
	return vector;
}

