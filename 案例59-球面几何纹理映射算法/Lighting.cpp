// Lighting.cpp: implementation of the CLighting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Lighting.h"
#include "math.h"
#define  PI  3.14159265
#define  MIN(a,b)  ((a<b)?(a):(b))
#define  MAX(a,b)  ((a>b)?(a):(b))
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLighting::CLighting()
{
	LightNum=1;
	Light=new CLight[LightNum];
	Ambient=CRGB(0.3,0.3,0.3);//环境光恒定不变
}

CLighting::~CLighting()
{
	if(Light)
	{
		delete []Light;
		Light=NULL;
	}
}
void CLighting::SetLightNumber(int lnum)
{
	if(Light)
	{
		delete []Light;
	}
	LightNum=lnum;
	Light=new CLight[lnum];
}

CLighting::CLighting(int lnum)
{
	LightNum=lnum;
	Light=new CLight[lnum];
	Ambient=CRGB(0.3,0.3,0.3);
}

CRGB CLighting::Lighting(CP3 ViewPoint,CP3 Point,CVector Normal,CMaterial *pMaterial)
{	
	CRGB LastC=pMaterial->M_Emit;//材质自身发散色为初始值	
	for(int i=0;i<LightNum;i++)//来自光源
	{	
		if(Light[i].L_OnOff)
		{		
			CRGB InitC;
			InitC.red=0,InitC.green=0,InitC.blue=0;
			CVector VL(Point,Light[i].L_Position);//VL为指向光源的矢量
			double d=VL.Mag();//d为光传播的距离，等于光矢量VL的模
			VL=VL.Normalize();//单位化光矢量
			CVector VN=Normal;
			VN=VN.Normalize();//单位化法矢量			
			//第1步，加入漫反射光
			double CosTheta=MAX(Dot(VL,VN),0);			
			InitC.red+=Light[i].L_Diffuse.red*pMaterial->M_Diffuse.red*CosTheta;
			InitC.green+=Light[i].L_Diffuse.green*pMaterial->M_Diffuse.green*CosTheta;
			InitC.blue+=Light[i].L_Diffuse.blue*pMaterial->M_Diffuse.blue*CosTheta;
			//第2步，加入镜面反射光
			CVector VS(Point,ViewPoint);//VS为视矢量
			VS=VS.Normalize();//单位化视矢量
			CVector VH=(VL+VS)/(VL+VS).Mag();//VH为平分矢量	
			double nHN=pow(MAX(Dot(VH,VN),0),pMaterial->M_n);
			InitC.red+=Light[i].L_Specular.red*pMaterial->M_Specular.red*nHN;
			InitC.green+=Light[i].L_Specular.green*pMaterial->M_Specular.green*nHN;
			InitC.blue+=Light[i].L_Specular.blue*pMaterial->M_Specular.blue*nHN;	
		//第3步，光强衰减
			double c0=Light[i].L_C0;//c0为常数衰减因子
			double c1=Light[i].L_C1;//c1线性衰减因子
			double c2=Light[i].L_C2;//c2二次衰减因子			
			double f=(1.0/(c0+c1*d+c2*d*d));//光强衰减函数
			f=MIN(1.0,f);		
			LastC+=InitC*f;		
		}
		else
		{
			LastC+=Point.c;//物体自身颜色
		}		
	}
	//第4步，加入环境光
	LastC+=Ambient*pMaterial->M_Ambient;
	//第5步，颜色归一到[0,1]区间
	LastC.Normalize();		
	//第6步，返回所计算顶点的光强颜色
	return LastC;
}	
