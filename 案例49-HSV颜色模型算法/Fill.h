// Fill.h: interface for the CFill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILL_H__EDD87BAB_FC56_479F_9769_F92E81FDBCAC__INCLUDED_)
#define AFX_FILL_H__EDD87BAB_FC56_479F_9769_F92E81FDBCAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Bucket.h"

class CFill  
{
public:
	CFill();
	virtual ~CFill();
	void SetPoint(CPi2 *p,int);//初始化
	void CreateBucket();//创建桶
	void CreateEdge();//边表
	void AddET(CAET *);//合并ET表
	void ETOrder();//ET表排序
	void Gouraud(CDC *);//填充多边形
    CRGB Interpolation(double,double,double,CRGB,CRGB);//线性插值
	void ClearMemory();//清理内存
	void DeleteAETChain(CAET* pAET);//删除边表
protected:
	int     PNum;//顶点个数
	CPi2    *P;//顶点坐标动态数组
	CAET    *pHeadE,*pCurrentE,*pEdge; //有效边表结点指针
	CBucket *pHeadB,*pCurrentB;        //桶表结点指针
};

#endif // !defined(AFX_FILL_H__EDD87BAB_FC56_479F_9769_F92E81FDBCAC__INCLUDED_)
