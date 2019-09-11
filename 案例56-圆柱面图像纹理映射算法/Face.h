// Face.h: interface for the CFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACE_H__288D3AA0_BB35_4E92_9AE4_4939AE57DEC9__INCLUDED_)
#define AFX_FACE_H__288D3AA0_BB35_4E92_9AE4_4939AE57DEC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"

class CFace  
{
public:
	CFace();
	virtual ~CFace();
	void SetNum(int);//设置小面的顶点数
	void SetFaceNormal(CP3,CP3,CP3);//设置小面法矢量
public:
	int vN;          //小面的顶点数
	int *vI;         //小面的顶点索引
	CVector fNormal; //小面的法矢量
};


#endif // !defined(AFX_FACE_H__288D3AA0_BB35_4E92_9AE4_4939AE57DEC9__INCLUDED_)
