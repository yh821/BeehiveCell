#pragma once
#include "stdafx.h"

//蜂窝网格的定位方法 -- by hoodlum1980
//假设六边形的边长为a；


//两个方向的矩形定位的基本单位
#define unitx(a)	((a)*1.7320508)  //sqrt(3) * a
#define unity(a)	((a)*1.5)					//1.5 * a


//两个方向的矩形网格基本单位
double g_unitx;
double g_unity;
double g_MinDistance2; 	// (a*sqrt(3)/2)^2

//设置六边形的边长
void SetCellSize(int a)
{
	if(a>0)
	{
		g_unitx = unitx(a);
		g_unity = unity(a);
		
		//二分之根号3 边长的平方，如果距离比它还小，就必然捕获
		g_MinDistance2 = a*a*0.75;
	}
}


//求取两个点的距离平方
inline int distance2(int x1, int y1, int x2, int y2)
{
	return ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}


//输入鼠标按下的点坐标（x，y）
//返回被捕获六边形的中心坐标
void GetCell(int x, int y, int *lpCellX, int *lpCellY)
{
	//位于矩形网格边线上的三个CELL中心点
	POINT points[3];
	//当前距离
	int dist;
	int mindist= (int)(g_MinDistance2 * 100); //一个非常大的值
	int i, index;//index:被捕获的索引
	//计算出鼠标点位于哪一个矩形网格中
	int cx = (int)(x/g_unitx);
	int cy = (int)(y/g_unity);
	
	points[0].x = (int)(g_unitx * cx);
	points[1].x = (int)(g_unitx * (cx+0.5));
	points[2].x = (int)(g_unitx * (cx+1));
	//根据cy是否是偶数，决定三个点的纵坐标
	if(cy % 2 == 0)
	{
		//偶数时，三个点组成倒立三角
		points[0].y = points[2].y = (int)(g_unity * cy);
		points[1].y = (int)(g_unity * (cy+1));
	}	
	else
	{
		//奇数时，三个点组成正立三角
		points[0].y = points[2].y = (int)(g_unity * (cy+1));
		points[1].y = (int)(g_unity * cy);
	}
	
	//现在找出鼠标距离哪一个点最近
	for(i=0;i<3;i++)
	{
		//求出距离的平方
		dist = distance2(x,y, points[i].x, points[i].y);
		
		//如果已经肯定被捕获
		if(dist < g_MinDistance2)
		{
			index = i;
			break;
		}
		
		//更新最小距离值和索引
		if(dist < mindist)
		{
			mindist = dist;
			index = i;
		}
	}	
	
	//现在index 就是被捕获的结果
	*lpCellX = points[index].x;
	*lpCellY = points[index].y;	
}

//给出蜂窝CELL的中心点和边长a，填充Cell的六边形的六个端点

void GetCellPoints(int cellx, int celly, int a, POINT *lpPoints)
{
	if(lpPoints == NULL) return;
		
	lpPoints[0].x = cellx;
	lpPoints[0].y = celly - a;
	
	lpPoints[1].x = cellx + (int)(g_unitx*0.5);
	lpPoints[1].y = celly - a/2; 
	
	lpPoints[2].x = lpPoints[1].x;
	lpPoints[2].y = celly + a/2;
	
	lpPoints[3].x = cellx;
	lpPoints[3].y = celly + a;
	
	lpPoints[4].x = cellx - (int)(g_unitx*0.5);
	lpPoints[4].y = celly + a/2;
	
	lpPoints[5].x = lpPoints[4].x;
	lpPoints[5].y = celly - a/2;
}
