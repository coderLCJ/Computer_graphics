// P2.h: interface for the CP2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_P2_H__ACE02C95_06D7_4EB5_AEBB_A8AD9A00F81D__INCLUDED_)
#define AFX_P2_H__ACE02C95_06D7_4EB5_AEBB_A8AD9A00F81D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CP2  
{
public:
	CP2();
	virtual ~CP2();
public:
	double x;//直线段端点x坐标
	double y;//直线段端点y坐标
	UINT  rc;//直线段端点编码
};

  

#endif // !defined(AFX_P2_H__ACE02C95_06D7_4EB5_AEBB_A8AD9A00F81D__INCLUDED_)
