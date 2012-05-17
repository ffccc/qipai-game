
#ifndef CRANDOMGEN_H
#define CRANDOMGEN_H


// #if _MSC_VER > 1000
// #pragma once
// #endif

#pragma once


#include "math.h"
#include "Stdafx.h"
/***************************************
* ��������� ���� ����һ����Χ�ڵ������
****************************************/
class CRandomGen  
{
public:
	//���캯��
	CRandomGen(void);
	//��������
	virtual ~CRandomGen(void);

	//��ĳһ��Χ�ڲ��������
	double NormalRandom(double miu, double sigma, double min, double max); 
	//ƽ������� ��֤��ȫ�޹������
	double AverageRandom(double min, double max);		
	//���в���������� ȷ��������
	double Normal(double x, double miu, double sigma);
};

#endif 
