// Pi3.h: interface for the CPi3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PI3_H__1DC85C9B_C92E_4165_9851_4C61236BF305__INCLUDED_)
#define AFX_PI3_H__1DC85C9B_C92E_4165_9851_4C61236BF305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Pi2.h"

class CPi3:public CPi2  
{
public:
	CPi3();
	virtual ~CPi3();
	CPi3(double,int,double);
	double z;
};

#endif // !defined(AFX_PI3_H__1DC85C9B_C92E_4165_9851_4C61236BF305__INCLUDED_)
