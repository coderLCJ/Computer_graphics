// Face.h: interface for the CFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_)
#define AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"

class CFace  
{
public:
	CFace();
	virtual ~CFace();
	void SetNum(int);//设置面的顶点数
	void SetFaceNormal(CP3,CP3,CP3);//设置面的法矢量
public:
	int vN;          //面的顶点数
	int *vI;         //面的顶点索引
	CVector fNormal; //面的法矢量
	CRGB fClr;       //面的颜色
};

#endif // !defined(AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_)
