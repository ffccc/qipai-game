
#include "RandomGen.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

#define PI 3.1415926

/*
* 函数介绍：构造函数
* 输入参数：
* 输出参数：
* 返回值  ：
*/
CRandomGen::CRandomGen(void)
{
}

/*
* 函数介绍：析构函数
* 输入参数：
* 输出参数：
* 返回值  ：
*/
CRandomGen::~CRandomGen(void)
{
}

/*
* 函数介绍：无序性
* 输入参数：
* 输出参数：
* 返回值  ：
*/
double CRandomGen::Normal(double x, double miu, double sigma)
{
	return 1.0/(sqrt(2*3.1415926)*sigma)* exp(-1*(x-miu)*(x-miu)/(2*sigma*sigma));
}
/*
* 函数介绍：随机性
* 输入参数：
* 输出参数：
* 返回值  ：
*/
double CRandomGen::AverageRandom(double min, double max)
{
	int minInteger = (int)(min * 10000);
	int maxInteger = (int)(max * 10000);
	int randInteger = rand() * rand();
	int diffInteger = maxInteger - minInteger;
	int resultInteger = randInteger % diffInteger + minInteger;
	return resultInteger/10000.0;
}

/*
* 函数介绍：产生某一范围内的随机数
* 输入参数：
* 输出参数：
* 返回值  ：
*/
double CRandomGen::NormalRandom(double miu, double sigma, double min, double max)
{
	double x = 0;
	double dScope = 0;
	double y = 0;
	do
	{
		x = AverageRandom(min, max); 
		y = Normal(x, miu, sigma);
		dScope = AverageRandom(0, Normal(miu,miu,sigma));
	}while (dScope > y);
	return x;
}
