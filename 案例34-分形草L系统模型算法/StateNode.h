// StateNode.h: interface for the CStateNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATENODE_H__6B4BD335_EA85_4C48_AF14_40457D8FF886__INCLUDED_)
#define AFX_STATENODE_H__6B4BD335_EA85_4C48_AF14_40457D8FF886__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStateNode  
{
public:
	CStateNode();
	virtual ~CStateNode();
	double x;
	double y;
	double alpha;
};

#endif // !defined(AFX_STATENODE_H__6B4BD335_EA85_4C48_AF14_40457D8FF886__INCLUDED_)
