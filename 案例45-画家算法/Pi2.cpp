// Pi2.cpp: implementation of the CPi2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Pi2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPi2::CPi2()
{
	x=0.0;
	y=0;
	c=CRGB(0.0,0.0,0.0);
}

CPi2::~CPi2()
{

}

CPi2::CPi2(double x,int y)
{
	this->x=x;
	this->y=y;
}