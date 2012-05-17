
#ifndef CRANDOMGEN_H
#define CRANDOMGEN_H


// #if _MSC_VER > 1000
// #pragma once
// #endif

#pragma once


#include "math.h"
#include "Stdafx.h"
/***************************************
* 随机函数类 功能 产生一个范围内的随机数
****************************************/
class CRandomGen  
{
public:
	//构造函数
	CRandomGen(void);
	//析构函数
	virtual ~CRandomGen(void);

	//在某一范围内产生随机数
	double NormalRandom(double miu, double sigma, double min, double max); 
	//平均随机数 保证完全无规律随机
	double AverageRandom(double min, double max);		
	//排列产生的随机数 确保无序性
	double Normal(double x, double miu, double sigma);
};

#endif 
