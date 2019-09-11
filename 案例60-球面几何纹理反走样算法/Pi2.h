// Pi2.h: interface for the CPi2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PI2_H__B80DAA04_DDC9_4F35_8BC8_B3DDDE62F21D__INCLUDED_)
#define AFX_PI2_H__B80DAA04_DDC9_4F35_8BC8_B3DDDE62F21D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RGB.h"

class CPi2  
{
public:
	CPi2();
	virtual ~CPi2();
	CPi2(double,int);
public:
	double x;
	int    y;
	CRGB   c;
};

#endif // !defined(AFX_PI2_H__B80DAA04_DDC9_4F35_8BC8_B3DDDE62F21D__INCLUDED_)
