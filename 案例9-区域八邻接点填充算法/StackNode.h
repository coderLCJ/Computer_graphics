// StackNode.h: interface for the CStackNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACKNODE_H__EA341FD1_DF03_42A8_9A11_85A6C46D366B__INCLUDED_)
#define AFX_STACKNODE_H__EA341FD1_DF03_42A8_9A11_85A6C46D366B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "P2.h"

class CStackNode//Õ»½áµã  
{
public:
	CStackNode();
	virtual ~CStackNode();
public:
    CP2 PixelPoint;
	CStackNode *pNext;
};

#endif // !defined(AFX_STACKNODE_H__EA341FD1_DF03_42A8_9A11_85A6C46D366B__INCLUDED_)
