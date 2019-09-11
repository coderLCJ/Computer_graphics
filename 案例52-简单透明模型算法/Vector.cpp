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

CVector::CVector()//z轴正向
{
	x=0.0;
	y=0.0;
	z=1.0;
}

CVector::~CVector()
{

}

CVector::CVector(const CP3 &p)
{
	x=p.x;
	y=p.y;
	z=p.z;	
}

CVector::CVector(const CPi3 &p)
{
	x=p.x;
	y=p.y;
	z=p.z;	
}

CVector::CVector(double x,double y,double z)
{
	this->x=x;
	this->y=y;
	this->z=z;	
}

CVector::CVector(const CP3 &p1,const CP3 &p2)
{
	x=p2.x-p1.x;
	y=p2.y-p1.y;
	z=p2.z-p1.z;
}

CVector::CVector(const CPi3 &p1,const CPi3 &p2)
{
	x=p2.x-p1.x;
	y=p2.y-p1.y;
	z=p2.z-p1.z;
}

double CVector::Mag()//矢量的模
{
	return sqrt(x*x+y*y+z*z);
}

CVector CVector::Normalize()//归一化到单位矢量
{
	CVector vector;
	double Mag=sqrt(x*x+y*y+z*z);
	if(fabs(Mag)<1e-6)
		Mag=1.0;
	vector.x=x/Mag;
	vector.y=y/Mag;
	vector.z=z/Mag;
	return vector;
}

CVector operator +(const CVector &v1,const CVector &v2)//矢量的和
{	
	CVector vector;
	vector.x=v1.x+v2.x;
	vector.y=v1.y+v2.y;
	vector.z=v1.z+v2.z;
	return vector;
}

CVector operator -(const CVector &v1,const CVector &v2)//矢量的差
{	
	CVector vector;
	vector.x=v1.x-v2.x;
	vector.y=v1.y-v2.y;
	vector.z=v1.z-v2.z;
	return vector;
}

CVector operator *(const CVector &v,double k)//矢量与常量的积
{	
	CVector vector;
	vector.x=v.x*k;
	vector.y=v.y*k;
	vector.z=v.z*k;
	return vector;
}

CVector operator *(double k,const CVector &v)//常量与矢量的积
{	
	CVector vector;
	vector.x=v.x*k;
	vector.y=v.y*k;
	vector.z=v.z*k;
	return vector;
}

CVector operator /(const CVector &v,double k)//矢量数除
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

double Dot(const CVector &v1,const CVector &v2)//矢量的点积
{
	return(v1.x*v2.x+v1.y*v2.y+v1.z*v2.z);
}

CVector Cross(const CVector &v1,const CVector &v2)//矢量的叉积
{
	CVector vector;
	vector.x=v1.y*v2.z-v1.z*v2.y;
	vector.y=v1.z*v2.x-v1.x*v2.z;
	vector.z=v1.x*v2.y-v1.y*v2.x;
	return vector;
}

