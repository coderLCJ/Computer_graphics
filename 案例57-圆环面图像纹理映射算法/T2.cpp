// T2.cpp: implementation of the CT2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "T2.h"
#include "math.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT2::CT2()
{
	u=0;
	v=0;
}

CT2::~CT2()
{

}

CT2::CT2(double u,double v)
{
	this->u=u;
	this->v=v;
}

CT2 operator +(const CT2 &t1,const CT2 &t2)//和
{	
	CT2 t;
	t.u=t1.u+t2.u;
	t.v=t1.v+t2.v;
	return t;
}

CT2 operator -(const CT2 &t1,const CT2 &t2)//差
{
	CT2 t;
	t.u=t1.u-t2.u;
	t.v=t1.v-t2.v;
	return t;
}

CT2 operator *(const CT2 &t,double k)//点和常量的积
{	
	return CT2(t.u*k,t.v*k);
}

CT2 operator *(double k,const CT2 &t)//点和常量的积
{	
	return CT2(t.u*k,t.v*k);
}

CT2 operator /(const CT2 &t1,double k)//数除
{
	if(fabs(k)<1e-6)
		k=1.0;
	CT2 t;
	t.u=t1.u/k;
	t.v=t1.v/k;
	return t;
}

CT2 operator+=(CT2 &t1,CT2 &t2)
{
	t1.u=t1.u+t2.u;
	t1.v=t1.v+t2.v;
	return t1;
}

CT2 operator-=(CT2 &t1,CT2 &t2)
{
	t1.u=t1.u-t2.u;
	t1.v=t1.v-t2.v;
	return t1;
}

CT2 operator*=(CT2 &t1,double k)
{
	t1.u=t1.u*k;
	t1.v=t1.v*k;
	return t1;
}

CT2 operator/=(CT2 &t1,double k)
{
	if(fabs(k)<1e-6)
		k=1.0;
	t1.u=t1.u/k;
	t1.v=t1.v/k;
	return t1;
}

