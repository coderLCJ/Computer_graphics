// ALine.cpp: implementation of the CALine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "ALine.h"
#include "math.h"
#define Round(d) int(floor(d+0.5))//四舍五入宏定义
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CALine::CALine()
{ 
}

CALine::~CALine()
{
	
}

void CALine::MoveTo(CDC *pDC,CP2 p0)
{	
	P0=p0;
}

void CALine::MoveTo(CDC *pDC,double x,double y)//重载函数
{
	MoveTo(pDC,CP2(x,y,CRGB(0.0,0.0,0.0)));
}

void CALine::MoveTo(CDC *pDC,double x,double y,CRGB c)
{	
	MoveTo(pDC,CP2(x,y,c));
}

void CALine::LineTo(CDC *pDC,CP2 p1)
{
	P1=p1;
	CP2 p,t;
	CRGB c0,c1;
	if(fabs(P0.x-P1.x)==0)//绘制垂线
	{
		if(P0.y>P1.y)//交换顶点,使得起始点低于终点顶点
		{
			t=P0;P0=P1;P1=t;
		}
		for(p=P0;p.y<P1.y;p.y++)
		{
			pDC->SetPixelV(Round(p.x),Round(p.y),RGB(p.c.red*255,p.c.green*255,p.c.blue*255));
		}
	}	
	else
	{
		double k,e;
		k=(P1.y-P0.y)/(P1.x-P0.x);
		if(k>1.0)//绘制k＞1
		{
			if(P0.y>P1.y)
			{
				t=P0;P0=P1;P1=t;
			}	  		
			for(p=P0,e=1/k;p.y<P1.y;p.y++)
			{
				c0=CRGB(e,e,e);
				c1=CRGB(1.0-e,1.0-e,1.0-e);
				pDC->SetPixelV(Round(p.x),Round(p.y),RGB(c0.red*255,c0.green*255,c0.blue*255));
				pDC->SetPixelV(Round(p.x+1),Round(p.y),RGB(c1.red*255,c1.green*255,c1.blue*255));
				e=e+1/k;
                if(e>=1.0)
				{
					p.x++;
					e--;
				}
			}
		}
		if(0.0<=k && k<=1.0)//绘制0≤k≤1
		{
			if(P0.x>P1.x)
			{
				t=P0;P0=P1;P1=t;
			}	  
			for(p=P0,e=k;p.x<P1.x;p.x++)
			{
				c0=CRGB(e,e,e);
				c1=CRGB(1.0-e,1.0-e,1.0-e);
				pDC->SetPixelV(Round(p.x),Round(p.y),RGB(c0.red*255,c0.green*255,c0.blue*255));
				pDC->SetPixelV(Round(p.x),Round(p.y+1),RGB(c1.red*255,c1.green*255,c1.blue*255));					
				e=e+k;
                if(e>=1.0)
				{
					p.y++;
					e--;
				}
			}
		}		
		if(k>=-1.0 && k<0.0)//绘制-1≤k＜0
		{
			if(P0.x>P1.x)
			{
				t=P0;P0=P1;P1=t;
			}	 
			for(p=P0,e=-k;p.x<P1.x;p.x++)
			{
				c0=CRGB(e,e,e);
				c1=CRGB(1.0-e,1.0-e,1.0-e);
				pDC->SetPixelV(Round(p.x),Round(p.y),RGB(c0.red*255,c0.green*255,c0.blue*255));
				pDC->SetPixelV(Round(p.x),Round(p.y-1),RGB(c1.red*255,c1.green*255,c1.blue*255));
				e=e-k;
                if(e>=1.0)
				{
					p.y--;
					e--;
				}
			}
		}
		if(k<-1.0)//绘制k＜-1 
		{
			if(P0.y<P1.y)
			{
				t=P0;P0=P1;P1=t;
			}	  
			for(p=P0,e=-1/k;p.y>P1.y;p.y--)
			{
				c0=CRGB(e,e,e);
				c1=CRGB(1.0-e,1.0-e,1.0-e);
				pDC->SetPixelV(Round(p.x),Round(p.y),RGB(c0.red*255,c0.green*255,c0.blue*255));
				pDC->SetPixelV(Round(p.x+1),Round(p.y),RGB(c1.red*255,c1.green*255,c1.blue*255));
				e=e-1/k;
                if(e>=1.0)
				{
					p.x++;
					e--;
				}
			}
		}
	}
	P0=p1;
}

void CALine::LineTo(CDC *pDC,double x,double y)//重载函数
{
	LineTo(pDC,CP2(x,y,CRGB(0.0,0.0,0.0)));
}

void CALine::LineTo(CDC *pDC,double x,double y,CRGB c)
{
	 LineTo(pDC,CP2(x,y,c));
}






