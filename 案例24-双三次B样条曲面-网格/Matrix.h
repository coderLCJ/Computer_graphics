// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__B272BEDF_8600_4B57_9053_C0278304BDD2__INCLUDED_)
#define AFX_MATRIX_H__B272BEDF_8600_4B57_9053_C0278304BDD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMatrix  
{
public:
	CMatrix();
	virtual ~CMatrix();
	//void SetMat(CP3 *,int);
	void Identity();//单位矩阵
	void MultiMatrix();//矩阵相乘
	void TransposeMatrix();//矩阵相乘
public:
	double M[4][4];
};

#endif // !defined(AFX_MATRIX_H__B272BEDF_8600_4B57_9053_C0278304BDD2__INCLUDED_)
