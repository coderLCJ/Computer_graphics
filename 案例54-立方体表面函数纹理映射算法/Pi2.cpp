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
	this->x=0.0;
	this->y=0;
	this->c=CRGB(1.0,1.0,1.0);
}

CPi2::~CPi2()
{

}

CPi2::CPi2(double x0,int y0)
{
	this->x=x0;
	this->y=y0;
}