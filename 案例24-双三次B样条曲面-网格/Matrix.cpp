// Matrix.cpp: implementation of the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Matrix.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrix::CMatrix()
{

}

CMatrix::~CMatrix()
{

}

void CMatrix::Identity()//µ•Œªæÿ’Û
{
	M[0][0]=1.0;M[0][1]=0.0;M[0][2]=0.0;M[0][3]=0.0;
	M[1][0]=0.0;M[1][1]=1.0;M[1][2]=0.0;M[1][3]=0.0;
	M[2][0]=0.0;M[2][1]=0.0;M[2][2]=1.0;M[2][3]=0.0;
	M[3][0]=0.0;M[3][1]=0.0;M[3][2]=0.0;M[3][3]=1.0;
}

void CMatrix::MultiMatrix()//æÿ’Ûœ‡≥À
{
	/*CP3 *PNew=new CP3[num];	
	for(int i=0;i<num;i++)
	{
		PNew[i]=POld[i];
	}
	for(int j=0;j<num;j++)
	{
		POld[j].x=PNew[j].x*T[0][0]+PNew[j].y*T[1][0]+PNew[j].z*T[2][0]+PNew[j].w*T[3][0];
		POld[j].y=PNew[j].x*T[0][1]+PNew[j].y*T[1][1]+PNew[j].z*T[2][1]+PNew[j].w*T[3][1];
		POld[j].z=PNew[j].x*T[0][2]+PNew[j].y*T[1][2]+PNew[j].z*T[2][2]+PNew[j].w*T[3][2];
		POld[j].w=PNew[j].x*T[0][3]+PNew[j].y*T[1][3]+PNew[j].z*T[2][3]+PNew[j].w*T[3][3];
	}
	delete []PNew;*/
}