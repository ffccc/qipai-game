#ifndef FLOWER_RES_HEAD_FILE
#define FLOWER_RES_HEAD_FILE

#pragma once
#include "PropertyModule.h"
///////////////////////////////////////////////////////////////////////////////////

class PROPERTY_MODULE_CLASS CFlowerRes : public IFlowerRes
{
	//��������
protected:
	CSkinImage						m_ImageFlower[FLOWER_COUNT];							//�ʻ�ͼƬ

	//��������
public:
	//���캯��
	CFlowerRes(void);
	//��������
	~CFlowerRes(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { if (IsValid()) delete this; return ; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CFlowerRes))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�ӿں���
public:
	//�滭�ʻ�
	virtual void __cdecl DrawFlower(CDC * pDC, int nXPos, int nYPos, UINT uFlowerID);
	//�滭�ʻ�
	virtual void __cdecl DrawGift(CDC * pDC, int nXPos, int nYPos, UINT uFlowerID);
};
///////////////////////////////////////////////////////////////////////////////////

#endif
