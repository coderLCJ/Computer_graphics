// StackNode.h: interface for the CStackNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACKNODE_H__17886C6E_A6AE_4F2B_AE18_2B30CB6C1D0F__INCLUDED_)
#define AFX_STACKNODE_H__17886C6E_A6AE_4F2B_AE18_2B30CB6C1D0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "P2.h"

class CStackNode  
{
public:
	CStackNode();
	virtual ~CStackNode();
	void Push(CP2 point);//»Î’ª
	void Pop(CP2 &point);//≥ˆ’ª

};

#endif // !defined(AFX_STACKNODE_H__17886C6E_A6AE_4F2B_AE18_2B30CB6C1D0F__INCLUDED_)
