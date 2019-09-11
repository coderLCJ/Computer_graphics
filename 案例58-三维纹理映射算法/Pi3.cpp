// Pi3.cpp: implementation of the CPi3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Pi3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPi3::CPi3()
{
	z=0.0;
}

CPi3::~CPi3()
{

}

CPi3::CPi3(double x,int y,double z):CPi2(x,y)
{
	this->z=z;
}