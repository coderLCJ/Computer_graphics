// P2.cpp: implementation of the CP2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "P2.h"
#include "math.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CP2::CP2()
{
	x=0;
	y=0;
	w=1.0;
}

CP2::~CP2()
{

}

CP2::CP2(double x,double y)
{
	this->x=x;
	this->y=y;
	this->w=1.0;
}

CP2 operator +(const CP2 &p1,const CP2 &p2)//和
{	
	CP2 p;
	p.x=p1.x+p2.x;
	p.y=p1.y+p2.y;
	return p;
}

CP2 operator -(const CP2 &p1,const CP2 &p2)//差
{
	CP2 p;
	p.x=p1.x-p2.x;
	p.y=p1.y-p2.y;
	return p;
}

CP2 operator *(const CP2 &p,double k)//点和常量的积
{	
	return CP2(p.x*k,p.y*k);
}

CP2 operator *(double k,const CP2 &p)//点和常量的积
{	
	return CP2(p.x*k,p.y*k);
}

CP2 operator /(const CP2 &p1,double k)//数除
{
	if(fabs(k)<1e-6)
		k=1.0;
	CP2 p;
	p.x=p1.x/k;
	p.y=p1.y/k;
	return p;
}

CP2 operator+=(CP2 &p1,CP2 &p2)
{
	p1.x=p1.x+p2.x;
	p1.y=p1.y+p2.y;
	return p1;
}

CP2 operator-=(CP2 &p1,CP2 &p2)
{
	p1.x=p1.x-p2.x;
	p1.y=p1.y-p2.y;
	return p1;
}

CP2 operator*=(CP2 &p1,double k)
{
	p1.x=p1.x*k;
	p1.y=p1.y*k;
	return p1;
}

CP2 operator/=(CP2 &p1,double k)
{
	if(fabs(k)<1e-6)
		k=1.0;
	p1.x=p1.x/k;
	p1.y=p1.y/k;
	return p1;
}


