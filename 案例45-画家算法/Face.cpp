// Face.cpp: implementation of the CFace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Face.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFace::CFace()
{
	vI=NULL;
	fMaxDepth=1000.0; 
	fClr=CRGB(1.0,1.0,1.0);
}

CFace::CFace(const CFace &face)//拷贝构造函数
{
	vN=face.vN;
	vI=new int[face.vN];
	memcpy(vI,face.vI,sizeof(int)*vN);
	fMaxDepth=face.fMaxDepth;
	fClr=face.fClr;
}

void CFace::operator=(const CFace &face)//赋值运算符重载
{
	if(vI!=NULL)
	{
		delete []vI;
		vI=NULL;
	}
	vN=face.vN;
	vI=new int[vN];
	memcpy(vI,face.vI,sizeof(int)*vN);
	fMaxDepth=face.fMaxDepth;
	fClr=face.fClr;
}

CFace::~CFace()
{
	if(vI!=NULL)
	{
		delete []vI;
		vI=NULL;
	}
}

void CFace::SetNum(int n)
{
	vN=n;
	vI=new int[n];
}