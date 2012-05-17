
#include "RandomGen.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

#define PI 3.1415926

/*
* �������ܣ����캯��
* ���������
* ���������
* ����ֵ  ��
*/
CRandomGen::CRandomGen(void)
{
}

/*
* �������ܣ���������
* ���������
* ���������
* ����ֵ  ��
*/
CRandomGen::~CRandomGen(void)
{
}

/*
* �������ܣ�������
* ���������
* ���������
* ����ֵ  ��
*/
double CRandomGen::Normal(double x, double miu, double sigma)
{
	return 1.0/(sqrt(2*3.1415926)*sigma)* exp(-1*(x-miu)*(x-miu)/(2*sigma*sigma));
}
/*
* �������ܣ������
* ���������
* ���������
* ����ֵ  ��
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
* �������ܣ�����ĳһ��Χ�ڵ������
* ���������
* ���������
* ����ֵ  ��
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
