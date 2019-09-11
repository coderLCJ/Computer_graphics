// Bucket.h: interface for the CBucket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CBUCKET_H__B36B2287_DDD5_4BE9_B57A_D359001CB0C5__INCLUDED_)
#define AFX_CBUCKET_H__B36B2287_DDD5_4BE9_B57A_D359001CB0C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AET.h"

class CBucket  
{
public:
	CBucket();
	virtual ~CBucket();
public:
	int     ScanLine;
	CAET    *pET;
	CBucket *pNext;
};

#endif // !defined(AFX_CBUCKET_H__B36B2287_DDD5_4BE9_B57A_D359001CB0C5__INCLUDED_)
